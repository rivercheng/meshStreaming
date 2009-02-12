#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/SocketStream.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Path.h"
#include "Poco/Exception.h"
#include "Poco/Thread.h"
#include <iostream>
#include "ppmesh.hh"
#include "bitstring.hh"
#include "vertexid.hh"
#include "render.hh"
#include "receiver.hh"

using Poco::Net::StreamSocket;
using Poco::Net::SocketStream;
using Poco::Net::SocketAddress;
using Poco::Path;
using Poco::Exception;
using Poco::Thread;


int main(int argc, char** argv)
{
    std::string config("view_config");
    if (argc < 4 ||argc > 5)
    {
        Path p(argv[0]);
        std::cout << "usage: " << p.getBaseName() << " <address> <port> <prefix>[view_config_file]" << std::endl;
        std::cout << "       download and display prefix.ppm from the server." << std::endl;
        return 1;
    }
    if (argc == 5)
    {
        config = argv[4];
    }

    std::string ip_addr(argv[1]);
    std::string port(argv[2]);
    std::string prefix(argv[3]);
    try
    {
        SocketAddress sa(ip_addr, port);
        StreamSocket sock(sa);
        SocketStream str(sock);

        str << prefix << std::endl;
        Poco::UInt16 udp_port;
        sock.receiveBytes(&udp_port, sizeof(udp_port));
        std::cerr<<udp_port<<std::endl;
        Ppmesh mesh(true);
        mesh.read(str, true, false);
        std::cerr<<"mesh readed."<<std::endl;
        std::cerr<<mesh.n_detail_vertices()<<std::endl;
        //Render render(argc, argv, prefix.c_str(), &mesh, 60);
        VisiblePQ visible_pq(&mesh, mesh.gfmesh());
        Render render(argc, argv, "happy", &mesh, &visible_pq, 15);
        std::ifstream ifs(config.c_str());
        if (ifs)
        {
            double dx, dy, dz, angle_x, angle_y, angle_z, scale;
            ifs>>dx>>dy>>dz>>angle_x>>angle_y>>angle_z>>scale;
            render.setView(dx,dy,dz,angle_x,angle_y,angle_z,scale);
        }
        Receiver receiver(mesh, visible_pq, render, ip_addr, udp_port, sock);
        Thread   receiver_thread(std::string("receiver"));
        mesh.setWait(8000000u);
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
