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

#include <string>
#include <iostream>
#include "ppmesh.hh"
#include "vertexid.hh"

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
    PpmeshServerConnection(const StreamSocket& s, UInt16 port):
            TCPServerConnection(s),
            port_(port)

    {
    }

    ~PpmeshServerConnection()
    {
        delete ppmesh_;
    }

    void run()
    {
        Application& app = Application::instance();
        app.logger().information("Request from " + this->socket().peerAddress().toString());
        try
        {

            SocketAddress addr("localhost", port_);
            DatagramSocket sock(addr);
            long wait_seconds =  3000;
            long wait_mseconds = 0;
            sock.setReceiveTimeout(Poco::Timespan(wait_seconds, wait_mseconds));

            SocketStream str(socket());
            std::string filename;
            str>>filename;
            filename.append(".ppm");
            std::cerr<<filename<<std::endl;
            ppmesh_ = new Ppmesh(filename.c_str(), false, false, true);
            socket().sendBytes(&port_, sizeof(port_));
            ppmesh_->write(str, false);
            str<<std::endl;

            char buffer[4096];
            while (1)
            {
                int len = sock.receiveFrom(buffer, sizeof(buffer), addr);
                std::cerr<<"received "<<len<<std::endl;
                BitString bs;
                bs.read_binary(buffer);
                //std::cerr<<bs<<std::endl;
                std::vector<VertexID> id_array;
                id_decode(bs, ppmesh_->tree_bits(), id_array);
                /*for (size_t i = 0; i<id_array.size(); i++)
                {
                    std::cerr<<id_array[i]<<std::endl;
                }*/

                BitString outbs;
                std::vector<VertexID>::const_iterator it = id_array.begin();
                std::vector<VertexID>::const_iterator end = id_array.end();
                for (; it!=end; ++it)
                {
                    outbs.push_back(ppmesh_->get_data(*it));
                }
                outbs.write_binary(buffer, true);
                if (outbs.size() /8 * 8 == outbs.size())
                {
                    len = outbs.size() /8;
                }
                else
                {
                    len = outbs.size()/8 + 1;
                }
                len += 2; //the length header
                sock.sendTo(buffer, len, addr);
                std::cerr<<"send "<<len<<std::endl;
                //std::cerr<<outbs<<std::endl;
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
    Ppmesh* ppmesh_;
    UInt16  port_;
};


class PpmeshServerConnectionFactory: public TCPServerConnectionFactory
            // A factory for TimeServerConnection.
{
public:
    PpmeshServerConnectionFactory(UInt16 port):
            port_(port)
    {
    }

    TCPServerConnection* createConnection(const StreamSocket& socket)
    {
        port_++;
        if (port_ <1025) port_ = 1025;
        return new PpmeshServerConnection(socket, port_);
    }

private:
    std::string _format;
    UInt16 port_;
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
        if (_helpRequested)
        {
            displayHelp();
        }
        else
        {
            // get parameters from configuration file
            unsigned short port = (unsigned short) config().getInt("PpmeshServer.port", 10020);
            std::string format(config().getString("TimeServer.format", DateTimeFormat::ISO8601_FORMAT));

            // set-up a server socket
            ServerSocket svs(port);
            // set-up a TCPServer instance
            TCPServer srv(new PpmeshServerConnectionFactory(1025), svs);
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


