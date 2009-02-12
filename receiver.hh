#include "Poco/Runnable.h"
#include "Poco/Net/StreamSocket.h"
#include "ppmesh.hh"
#include "visiblepq.hh"
#include "render.hh"

using Poco::Net::StreamSocket;
class Receiver:public Poco::Runnable
{
public:
    Receiver(Ppmesh& ppmesh, VisiblePQ& pq, Render& render, \
             const std::string& ip_addr, Poco::UInt16 udp_port, StreamSocket& sock)
            :mesh_(ppmesh), pq_(pq), render_(render), ip_addr_(ip_addr), udp_port_(udp_port), sock_(sock)
    {
        ;
    }

    virtual ~Receiver(void)
    {
        ;
    }

    virtual void run(void);
private:
    Ppmesh& mesh_;
    VisiblePQ& pq_;
    Render& render_;
    const std::string& ip_addr_;
    Poco::UInt16 udp_port_;
    StreamSocket& sock_;
};


