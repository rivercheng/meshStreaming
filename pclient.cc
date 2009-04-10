#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/SocketStream.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Path.h"
#include "Poco/Exception.h"
#include "Poco/Thread.h"
#include <OpenMesh/Core/Utils/Endian.hh>
#include <iostream>
#include "ppmesh.hh"
#include "bitstring.hh"
#include "vertexid.hh"
#include "prender.hh"
#include "preceiver.hh"
#include "pvisiblepq.hh"
#include "logger.hh"

using Poco::Net::StreamSocket;
using Poco::Net::SocketStream;
using Poco::Net::SocketAddress;
using Poco::Path;
using Poco::Exception;
using Poco::Thread;

int main(int argc, char** argv)
{
    std::string config("view_config");
    std::string user("DEFAULT");
    int rtt= 200; //in ms
    long data_rate = 125000; //bytes/s
    
    if (argc < 4 ||argc > 9)
    {
        Path p(argv[0]);
        std::cout << "usage: " << p.getBaseName() << " <address> <port> <prefix>[view_config_file][user][RTT][data_rate]" << std::endl;
        std::cout << "       download and display prefix.ppm from the server." << std::endl;
	std::cout << "       RTT should be given in ms." << std::endl;
	std::cout << "       data_rate should be given in xxx bytes/s." << std::endl;
        return 1;
    }
    
    if (argc == 5)
    {
        config = argv[4];
    }
    
    if (argc >= 8)
    {
        config = argv[4];
        user = argv[5];
        rtt = atoi(argv[6]);
        data_rate = atol(argv[7]);
    }

    PRender::Record *record = 0;
    if (argc == 9)
    {
        record = new PRender::Record;
        std::ifstream ifs(argv[8]);
        std::string str;
        while(!ifs.eof())
        {
            std::getline(ifs, str);
            std::stringstream sstr(str);
            int sec;
            int microsec;
            std::string action;
            sstr>>sec>>microsec>>action;
            std::cerr<<str<<std::endl;
            long t = sec*1000000 + microsec;
            if (action == "BEGIN")
            {
                continue;
            }
            else if(action == "ZOOM_IN")
            {
                record->push_back(PRender::Action(t, PRender::SPECIAL, PRender::NONE, PRender::UP));
            }
            else if(action == "ZOOM_OUT")
            {
                record->push_back(PRender::Action(t, PRender::SPECIAL, PRender::NONE, PRender::DOWN));
            }
            else if(action == "REVOLVE_CLOCKWISE")
            {
                record->push_back(PRender::Action(t, PRender::SPECIAL, PRender::NONE, PRender::LEFT));
            }
            else if(action == "REVOLVE_ANTICLOCKWISE")
            {
                record->push_back(PRender::Action(t, PRender::SPECIAL, PRender::NONE, PRender::RIGHT));
            }
            else if(action == "MOVE_LEFT")
            {
                record->push_back(PRender::Action(t, PRender::SPECIAL, PRender::ALT, PRender::LEFT));
            }
            else if(action == "MOVE_RIGHT")
            {
                record->push_back(PRender::Action(t, PRender::SPECIAL, PRender::ALT, PRender::RIGHT));
            }
            else if(action == "MOVE_UP")
            {
                record->push_back(PRender::Action(t, PRender::SPECIAL, PRender::ALT, PRender::UP));
            }
            else if(action == "MOVE_DOWN")
            {
                record->push_back(PRender::Action(t, PRender::SPECIAL, PRender::ALT, PRender::DOWN));
            }
            else if(action == "ROTATE_CLOCKWISE")
            {
                record->push_back(PRender::Action(t, PRender::SPECIAL, PRender::CTRL, PRender::LEFT));
            }
            else if(action == "ROTATE_ANTICLOCKWISE")
            {
                record->push_back(PRender::Action(t, PRender::SPECIAL, PRender::CTRL, PRender::RIGHT));
            }
            else if(action == "TILT_FORWARD")
            {
                record->push_back(PRender::Action(t, PRender::SPECIAL, PRender::CTRL, PRender::UP));
            }
            else if(action == "TILT_BACKWARD")
            {
                record->push_back(PRender::Action(t, PRender::SPECIAL, PRender::CTRL, PRender::DOWN));
            }
            else if(action == "RESET")
            {
                record->push_back(PRender::Action(t, PRender::NORMAL, PRender::NONE, PRender::R));
            }
            else if(action == "QUIT")
            {
                record->push_back(PRender::Action(t, PRender::NORMAL, PRender::NONE, PRender::Q));
            }
            else
            {
                std::cerr<<"Unkown action in record file."<<std::endl;
                continue;
            }
        }
    }
    std::string ip_addr(argv[1]);
    std::string port(argv[2]);
    std::string prefix(argv[3]);
    
    Logger logusr(prefix,user,USER_TRACE);
    Logger logpkt(prefix,user,PAKT_TRACE);
    Logger logtran(prefix,user,TRANSFORM_TRACE);
    logusr.log2_ = &logtran;
    
    Logger logger;
    try
    {
        SocketAddress sa(ip_addr, port);
        StreamSocket sock(sa);
        SocketStream str(sock);

        logger.log(BASE, prefix);
        str << prefix << std::endl;
        Poco::UInt16 udp_port;
        sock.receiveBytes(&udp_port, sizeof(udp_port));
        std::cerr<< "pclient -- UDP port received: "<<udp_port<<std::endl;
        Ppmesh mesh(true);
        mesh.read(str, true, false);				// read the poor progressive mesh
        std::cerr<<"mesh readed."<<std::endl;
        std::cerr<<mesh.n_detail_vertices()<<std::endl;//get a number of vertex
        //initial decoding here
        int count = 0;
        str.read((char*)&count, sizeof(count));//no swap is considered.
        std::cerr<<"count "<<count<<std::endl;
        int i = 0;
        for (i = 0; i<count; i++)
        {
            BitString data;
            VertexID id;
            unsigned int len;
            size_t   p_pos = 0;
            str.read((char* )&id, sizeof(id));
            str.read((char* )&len, sizeof(len));
            //std::cerr<<"id "<<id<<" len "<<len<<std::endl;
            data.read_binary(str, len);
            //std::cerr<<"to decode "<<id<<" with "<<data<<std::endl;
            mesh.decode(id, data,&p_pos);
            //std::cerr<<i<<std::endl;
        }
        str.close();  					//reconstitution of the poor progressive mesh complete
        //======================
        PVisiblePQ visible_pq(&mesh, mesh.gfmesh(), logger);
        PRender render(argc, argv, "happy", &mesh, &visible_pq, 8, logusr, record);

        std::ifstream ifs(config.c_str());
        if (ifs)
        {
            double dx, dy, dz, angle_x, angle_y, angle_z, scale;
            ifs>>dx>>dy>>dz>>angle_x>>angle_y>>angle_z>>scale;
            render.setView(dx,dy,dz,angle_x,angle_y,angle_z,scale);
        }

        PReceiver   receiver(mesh, visible_pq, render, ip_addr, udp_port, sock, logpkt);
	receiver.rtt = rtt;
	receiver.data_rate = data_rate;
        Thread      receiver_thread(std::string("receiver"));
        receiver_thread.start(receiver);
        render.enterMainLoop();
    }
    catch (Exception& exc)
    {
        std::cerr << exc.displayText() << std::endl;
        return 1;
    }
    return 0;
}
