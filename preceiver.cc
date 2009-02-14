
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


using Poco::Net::StreamSocket;
using Poco::Net::DatagramSocket;
using Poco::Net::SocketStream;
using Poco::Net::SocketAddress;
using Poco::StreamCopier;
using Poco::Path;
using Poco::Exception;

const long wait_mseconds = 8000;
const long wait_seconds  = 0;

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
            PacketID p_id = 0;
            while (p_id == 0)
            {
                try
                {
                    p_id = pq_.pop();
                }
                catch (Exception& exp)
                {
                    exit(1);
                }
                usleep(1000);
            }
            BitString bs;
            logger_.log(REQ, p_id, sizeof(p_id));
            udp_sock.sendBytes(&p_id, sizeof(p_id));
            try
            {
                int len = udp_sock.receiveBytes(buffer, sizeof(buffer));
                //std::cerr<<"received "<<len<<std::endl;
                render_.receivedBytes_+= len;
                PacketID id = readPacketID(buffer);
                logger_.log(RESP, id, len-sizeof(id));
                bs.read_binary(buffer+sizeof(id));
                std::vector<VertexID> v_id_array;
                packetID_to_vertexID_array(id, v_id_array, 4);
                size_t pos = 0;
                size_t j   = 0;
                while (pos < bs.size())
                {
                    //std::cerr<<"to decode "<<id_array[j]<<" "<<pos<<std::endl;
                    mesh_.decode(v_id_array[j], bs, &pos);
                    //std::cerr<<id_array[j]<<" "<<pos<<" "<<bs.size()<<std::endl;
                    //mesh_.set_received(id_array[j]);
                    j++;
                }
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
