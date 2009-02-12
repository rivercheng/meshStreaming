
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
#include "receiver.hh"


using Poco::Net::StreamSocket;
using Poco::Net::DatagramSocket;
using Poco::Net::SocketStream;
using Poco::Net::SocketAddress;
using Poco::StreamCopier;
using Poco::Path;
using Poco::Exception;



void Receiver::run(void)
{
    const size_t number = 300;
    try
    {
        DatagramSocket udp_sock;
        SocketAddress udpsa(ip_addr_, udp_port_);
        udp_sock.connect(udpsa);
        char buffer[4096];
        while (1)
        {
            if (mesh_.wait() > 0)
            {
                usleep(mesh_.wait());
            }
            std::vector<VertexID> id_array;
            size_t n = pq_.pick(id_array, number);
            //size_t n = mesh_.pick(id_array,number);
            //if (n==0) break;
            if (n==0) continue;
            //for debug
            //for (size_t i = 0; i< id_array.size(); i++)
            //{
            //std::cerr<<id_array[i]<<std::endl;
            //}
            BitString bs;
            id_encode(id_array, mesh_.tree_bits(), bs);
            id_array.clear();
            id_decode(bs, mesh_.tree_bits(), id_array);
            //for (size_t i = 0; i< id_array.size(); i++)
            //{
            //    std::cerr<<id_array[i]<<std::endl;
            //}
            //std::cerr<<bs<<std::endl;
            bs.write_binary(buffer, true);
            int len = 0;

            if (bs.size() /8 * 8 == bs.size())
            {
                len = bs.size()/8;
            }
            else
            {
                len = bs.size()/8 + 1;
            }
            len += 2;
            render_.sentBytes_+= len;
            //std::cerr<<len<<std::endl;
            udp_sock.sendBytes(buffer, len);
            len = udp_sock.receiveBytes(buffer, sizeof(buffer));
            std::cerr<<"received "<<len<<std::endl;
            render_.receivedBytes_+= len;
            bs.clear();
            bs.read_binary(buffer);
            //std::cerr<<bs<<std::endl;

            size_t pos = 0;
            size_t j   = 0;
            while (pos < bs.size())
            {
                //std::cerr<<"to decode "<<id_array[j]<<" "<<pos<<std::endl;
                mesh_.decode(id_array[j], bs, &pos);
                //std::cerr<<id_array[j]<<" "<<pos<<" "<<bs.size()<<std::endl;
                //mesh_.set_received(id_array[j]);
                j++;
            }
            std::cerr<<"decoded "<<j<<std::endl;
            //if (n<number)
            //{
            //    break;
            //}
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
