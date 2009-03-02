
#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/DatagramSocket.h"
#include "Poco/Net/SocketStream.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/StreamCopier.h"
#include "Poco/Path.h"
#include "Poco/Exception.h"
#include <iostream>
#include "ppmesh.hh"
#include "bitstring.hh"
#include "vertexid.hh"
#include "preceiver.hh"
#include "packetid.hh"

#define RTT 0.4

using Poco::Net::StreamSocket;
using Poco::Net::DatagramSocket;
using Poco::Net::SocketStream;
using Poco::Net::SocketAddress;
using Poco::StreamCopier;
using Poco::Path;
using Poco::Exception;

const long wait_mseconds = 8000;
const long wait_seconds  = 0;
const double alpha = 1. / 8.;
int window_size = 100;
int current_window = 0;
long data_rate = 125000;

void PReceiver::run(void)
{
    try
    {
        DatagramSocket udp_sock;
        SocketAddress udpsa(ip_addr_, udp_port_);
        udp_sock.connect(udpsa);
        //udp_sock.setReceiveTimeout(Poco::Timespan(wait_seconds, wait_mseconds));
        char buffer[4096];

        
        while (1)
        {
            BitString bs;
            PacketID p_id = 0;
            while (current_window < window_size && p_id == 0) 
            {
                try
                {
                    p_id = pq_.pop();
                }
                catch (Exception& exp)
                {
                    exit(1);
                }
                
             	if( p_id > 0)
            	{
            		logger_.log(REQ, p_id, sizeof(p_id));
            		udp_sock.sendBytes(&p_id, sizeof(p_id));
            		current_window++;
            		p_id = 0;
           		
            	}
            }//end sender while loop
            	
            try
            {
                int len = udp_sock.receiveBytes(buffer, sizeof(buffer));
                render_.receivedBytes_+= len;
                PacketID id = readPacketID(buffer);
                logger_.log(RESP, id, len-sizeof(id));

                current_window--;
                average_len = len * (1-alpha) + average_len * alpha;
                window_size = ( data_rate * RTT )/average_len;
                
                bs.read_binary(buffer+sizeof(id));
                std::vector<VertexID> v_id_array;
                packetID_to_vertexID_array(id, v_id_array, 4);
                size_t pos = 0;
                size_t j   = 0;
                //logger_.log("before decode");
                while (pos < bs.size())
                {
                    //std::cerr<<"to decode "<<v_id_array[j]<<" "<<pos<<std::endl;
                    mesh_.decode(v_id_array[j], bs, &pos);
                    //std::cerr<<v_id_array[j]<<" "<<pos<<" "<<bs.size()<<std::endl;
                    //mesh_.set_received(id_array[j]);
                    j++;
                }
                //logger_.log("after decode");
                //std::cerr<<"decoded "<<j<<std::endl;
                //if (n<number)
                //{
                //    break;
                //}
            }
            catch (Exception& exc)
            {
                std::cerr<<exc.displayText()<<std::endl;
            }
        }
        sock_.shutdownSend();
        //std::ofstream ofs("out.pm", std::ios::binary);
        //mesh_.writePM(ofs);
    }
    catch (Exception& exc)
    {
        std::cerr<<exc.displayText()<<std::endl;
    }
    return;
}
