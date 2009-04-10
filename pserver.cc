/*
 * written by Cheng Wei 2007
 * National University of Singapore.
 * chengwe2@comp.nus.edu.sg
 */
#include "Poco/Net/TCPServer.h"
#include "Poco/Net/TCPServerConnection.h"
#include "Poco/Net/TCPServerConnectionFactory.h"
#include "Poco/Net/TCPServerParams.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/DatagramSocket.h"
#include "Poco/Net/SocketStream.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Timestamp.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/DateTimeFormat.h"
#include "Poco/Exception.h"
#include "Poco/Util/ServerApplication.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/HelpFormatter.h"
#include "Poco/Thread.h"

#include <string>
#include <cstring>
#include <iostream>
#include "ppmesh.hh"
#include "vertexid.hh"
#include "packetid.hh"


using Poco::Net::ServerSocket;
using Poco::Net::StreamSocket;
using Poco::Net::SocketStream;
using Poco::Net::SocketAddress;
using Poco::Net::DatagramSocket;
using Poco::Net::TCPServerConnection;
using Poco::Net::TCPServerConnectionFactory;
using Poco::Net::TCPServer;
using Poco::Timestamp;
using Poco::UInt16;
using Poco::DateTimeFormatter;
using Poco::DateTimeFormat;
using Poco::Util::ServerApplication;
using Poco::Util::Application;
using Poco::Util::Option;
using Poco::Util::OptionSet;
using Poco::Util::HelpFormatter;


class PpmeshServerConnection: public TCPServerConnection
            // This class handles all client connections.
            //
            // A string with the current date and time is sent back to the client.
{
public:
    PpmeshServerConnection(const StreamSocket& svs):
            TCPServerConnection(svs)
    {
    }

    ~PpmeshServerConnection()
    {
        //delete ppmesh_;
    }

    void run()
    {
    	std::cout<<" DEMO VERSION "<<std::endl;
        Application& app = Application::instance();
        app.logger().information("Request from " + this->socket().peerAddress().toString());
        try
        {

            //SocketAddress addr("localhost", port_);
            SocketAddress addr;
            DatagramSocket sock;
            sock.bind(addr);
            addr = sock.address();
            std::cout<<" UDP port : "<<addr.port()<<std::endl;
            port_ = addr.port();
            long wait_seconds =  1200;
            long wait_mseconds = 0;
            sock.setReceiveTimeout(Poco::Timespan(wait_seconds, wait_mseconds));

            SocketStream str(socket());
            std::string base_filename;
            str>>base_filename;
            std::string packet_filename(base_filename);
            base_filename.append(".base");
            packet_filename.append(".packet");


            //read the packet file to std::map
            std::ifstream packetfs(packet_filename.c_str(), std::ios::binary);
            unsigned int size;
            packetfs.read((char *)&size, sizeof(size));
            std::map<PacketID, BitString>  data_map;
            std::map<PacketID, BitString>::const_iterator  data_iter;
            std::map<PacketID, BitString>::const_iterator  data_end = data_map.end();
            for (size_t i = 0; i<size; i++)
            {
                PacketID p_id = read_id_binary(packetfs);
                BitString  bs;
                bs.read_binary(packetfs);
                data_map[p_id] = bs;
            }

            std::cerr<<base_filename<<std::endl; 		
            std::ifstream ifs(base_filename.c_str(), std::ios::binary);
            socket().sendBytes(&port_, sizeof(port_));
            str<<ifs.rdbuf()<<std::flush;	//send the base mesh
            socket().close();
            
            char buffer[4096];
            BitString empty_bs;
            for (size_t i = 0; i< 240; i++)
            {
                empty_bs.push_back_bit(true);
            }
            
            int debug = 0; //TO DELETE - FOR DEMO
            
            while (1)
            {
            	 debug ++;
                int len = sock.receiveFrom(buffer, sizeof(buffer), addr);
                //std::cerr<<"received "<<len<<" bytes"<<std::endl;
                PacketID p_id;
                memcpy(&p_id, buffer, sizeof(p_id));
                //std::cerr<<"p_id "<<to_string(p_id)<<std::endl;

                data_iter = data_map.find(p_id);
                writePacketID(p_id, buffer);
                if (data_iter == data_end)
                {
                    len = empty_bs.write_binary(buffer+sizeof(p_id), true);
                    //std::cerr<<"Packet not found. Empty Response. Tree leaves reached."<<std::endl;
                }
                else
                {
                    len = (data_iter->second).write_binary(buffer+sizeof(p_id), true);
                    //std::cerr<<data_iter->second<<std::endl;
                }         
                sock.sendTo(buffer, len + sizeof(p_id), addr);

            }
        }
        catch (Poco::Exception& exc)
        {
            app.logger().log(exc);
        }
        catch (WrongFileFormat& e)
        {
            UInt16 port = 0;
            socket().sendBytes(&port, sizeof(port));
        }
    }

private:
    std::string _format;
    UInt16  port_;
};


class PpmeshServerConnectionFactory: public TCPServerConnectionFactory
            // A factory for TimeServerConnection.
{
public:
    PpmeshServerConnectionFactory()
    {
    }

    TCPServerConnection* createConnection(const StreamSocket& socket)
    {
        return new PpmeshServerConnection(socket);
    }

private:
    std::string _format;
};


class PpmeshServer: public Poco::Util::ServerApplication
            // The main application class.
            //
            // This class handles command-line arguments and
            // configuration files.
            // Start the PpmeshServer executable with the help
            // option (/help on Windows, --help on Unix) for
            // the available command line options.
            //
            // To use the sample configuration file (TimeServer.properties),
            // copy the file to the directory where the TimeServer executable
            // resides. If you start the debug version of the TimeServer
            // (TimeServerd[.exe]), you must also create a copy of the configuration
            // file named TimeServerd.properties. In the configuration file, you
            // can specify the port on which the server is listening (default
            // 9911) and the format of the date/time string sent back to the client.
            //
            // To test the TimeServer you can use any telnet client (telnet localhost 9911).
{
public:
    PpmeshServer(): _helpRequested(false)
    {
    }

    ~PpmeshServer()
    {
    }

protected:
    void initialize(Application& self)
    {
        loadConfiguration(); // load default configuration files, if present
        ServerApplication::initialize(self);
    }

    void uninitialize()
    {
        ServerApplication::uninitialize();
    }

    void defineOptions(OptionSet& options)
    {
        ServerApplication::defineOptions(options);

        options.addOption(
            Option("help", "h", "display help information on command line arguments")
            .required(false)
            .repeatable(false));
    }

    void handleOption(const std::string& name, const std::string& value)
    {
        ServerApplication::handleOption(name, value);

        if (name == "help")
            _helpRequested = true;
    }

    void displayHelp()
    {
        HelpFormatter helpFormatter(options());
        helpFormatter.setCommand(commandName());
        helpFormatter.setUsage("OPTIONS");
        helpFormatter.setHeader("A server application that serves the current date and time.");
        helpFormatter.format(std::cout);
    }

    int main(const std::vector<std::string>& args)
    {
	if((int)args.size()!= 1)
	{
		std::cout<<" Usage : <TCP port> "<<std::endl;
		return 1;
	}

        if (_helpRequested)
        {
            displayHelp();
        }
        else
        {
            // get parameters from configuration file
            //unsigned short port = (unsigned short) config().getInt("PpmeshServer.port", 10020);
	  	  unsigned short port = atoi(args[0].c_str());
		  std::cout<<" TCP port: "<<args[0].c_str()<<std::endl;
            std::string format(config().getString("TimeServer.format", DateTimeFormat::ISO8601_FORMAT));

            // set-up a server socket
            ServerSocket svs(port);
            // set-up a TCPServer instance
            TCPServer srv(new PpmeshServerConnectionFactory(),svs);
            // start the TCPServer
            srv.start();
            // wait for CTRL-C or kill
            waitForTerminationRequest();
            // Stop the TCPServer
            srv.stop();
        }
        return Application::EXIT_OK;
    }

private:
    bool _helpRequested;
};


int main(int argc, char** argv)
{
    PpmeshServer app;
    return app.run(argc, argv);
}


