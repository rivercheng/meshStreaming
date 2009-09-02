
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
#include <sys/time.h>

//#define RTT 0.4

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
//int window_size = 1;
int current_window = 0;
//long data_rate = 125000;

void PReceiver::run(void)
{
    try
    {
        DatagramSocket udp_sock;
	    udp_sock.setBlocking(false);
        SocketAddress udpsa(ip_addr_, udp_port_);
        udp_sock.connect(udpsa);
        //udp_sock.setReceiveTimeout(Poco::Timespan(wait_seconds, wait_mseconds));
        char buffer[4096];
	//std::cout<<"we started "<<std::endl;
        
        while (1)
        {
            BitString bs;
            PacketID p_id = 0;
	    //int current_send =0;
            //while (current_send == 0 && current_window < window_size && p_id == 0) 
            
            if (current_window < window_size)
            {
                try
                {
                    p_id = pq_.pop();
                }
                catch (Exception& exp)
                {
                    exit(1);
                }
                //std::cout<<"we are in the while "<<std::endl;
             	if( p_id > 0)
            	{
            		logger_.log(REQ, p_id, sizeof(p_id));
            		udp_sock.sendBytes(&p_id, sizeof(p_id));
            		current_window++;
            		p_id = 0;
			//std::cout<<"we sent 1"<<std::endl;
                    	//usleep(5000);
            	}
            }//end sender while loop
            //	std::cout<<"we are out of the while "<<std::endl;
	    //usleep(5000);
            try
            {
		        struct timeval begin_;
		        struct timeval end_;
	            gettimeofday(&begin_, 0);
                
                int len = udp_sock.receiveBytes(buffer, sizeof(buffer));
//		std::cout<<"we rec 1"<<std::endl;
		if(len >0)
		        {
			   // std::cout<<"len > 0"<<std::endl;
		            render_.receivedBytes_+= len;
		            PacketID id = readPacketID(buffer);
		            logger_.log(RESP, id, len-sizeof(id));

		            current_window--;
		            average_len = len * (1-alpha) + average_len * alpha;
		            window_size = ( data_rate * rtt ) / (1000 *average_len);
                    	    //std::cout<<"current_window"<<current_window<<std::endl;
			    //std::cout<<"wnidow size "<<window_size<<std::endl;
                
		            bs.read_binary(buffer+sizeof(id));
		            std::vector<VertexID> v_id_array;
		            packetID_to_vertexID_array(id, v_id_array, 4);
		            size_t pos = 0;
		            size_t j   = 0;
		            while (pos < bs.size())
		            {
		            //std::cerr<<"to decode "<<v_id_array[j]<<" "<<pos<<std::endl;
		                mesh_.decode(v_id_array[j], bs, &pos);
		            //std::cerr<<v_id_array[j]<<" "<<pos<<" "<<bs.size()<<std::endl;
		            //mesh_.set_received(id_array[j]);
		                j++;
		            }
		        //if (n<number)
		        //{
		        //    break;
		        //}


		        }

          	gettimeofday(&end_, 0);
                long diff = (end_.tv_sec - begin_.tv_sec ) * 1000000 + end_.tv_usec -  begin_.tv_usec ;
                ///std::cout<<"Decoder time "<<diff <<std::endl;
		if (diff < 5000){
			usleep(5000-diff);	
		}

            }
            catch (Exception& exc)
            {
                //std::cerr<<exc.displayText()<<std::endl;
                usleep(5000);
            }
        }
        sock_.shutdownSend();
    }
    catch (Exception& exc)
    {
        std::cerr<<exc.displayText()<<std::endl;
    }
    return;
}
