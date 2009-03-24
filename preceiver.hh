#include "Poco/Runnable.h"
#include "Poco/Net/StreamSocket.h"
#include "ppmesh.hh"
#include "pvisiblepq.hh"
#include "prender.hh"
#include "logger.hh"

using Poco::Net::StreamSocket;
class PReceiver:public Poco::Runnable
{
public:
    PReceiver(Ppmesh& ppmesh, PVisiblePQ& pq, PRender& render, \
              const std::string& ip_addr, Poco::UInt16 udp_port, StreamSocket& sock, Logger& logger)
            :mesh_(ppmesh), pq_(pq), render_(render), ip_addr_(ip_addr), udp_port_(udp_port), sock_(sock), logger_(logger), average_len(1000)
    {
        ;
    }

    virtual ~PReceiver(void)
    {
        ;
    }

    virtual void run(void);
    
    int rtt;
    long data_rate;
private:
    Ppmesh& mesh_;
    PVisiblePQ& pq_;
    PRender& render_;
    const std::string& ip_addr_;
    Poco::UInt16 udp_port_;
    StreamSocket& sock_;
    Logger& logger_;
    double average_len;
    
};


