#ifndef __LOGGER_H__
#define __LOGGER_H__
#include <sys/time.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <Poco/Mutex.h>
#include "vertexid.hh"
#include "packetid.hh"

#define LOCK Poco::ScopedLock<Poco::Mutex> lock(mutex_)

#define SEPERATOR "."
#define EXT ".txt"

#define USER_TRACE "behavior"
#define PAKT_TRACE "packet"
#define TRANSFORM_TRACE "matrix"

//trace formats for packet trace 
#define BASE 'B' //request for a base mesh by client 
#define REQ  'S' //request for a node by the client 
#define RESP 'R' //response by the server for a client request

//trace formats for user actiosn 
#define QUIT "QUIT"
#define RESET "RESET"
#define SCALE "SCALE"
#define SMOOTH "SMOOTH"
#define INTRAPOLATE "INTRAPOLATE"
#define FILL "FILL"
#define OUTLINE "OUTLINE"
#define PERSPECTIVE "PERSPECTIVE"
#define REFINE "REFINE"
#define WRITE "WRITE"
#define OUTPUT "OUTPUT"
#define CULL "CULL"
#define VIEW "VIEW"
#define REVOLVE_CLOCKWISE "REVOLVE_CLOCKWISE"
#define REVOLVE_ANTICLOCKWISE "REVOLVE_ANTICLOCKWISE"
#define ZOOM_IN "ZOOM_IN"
#define ZOOM_OUT "ZOOM_OUT"
#define MOVE_LEFT "MOVE_LEFT"
#define MOVE_RIGHT "MOVE_RIGHT"
#define MOVE_UP "MOVE_UP"
#define MOVE_DOWN "MOVE_DOWN"
#define ROTATE_CLOCKWISE "ROTATE_CLOCKWISE"
#define ROTATE_ANTICLOCKWISE "ROTATE_ANTICLOCKWISE"
#define TILT_FORWARD "TILT_FORWARD"
#define TILT_BACKWARD "TILT_BACKWARD" 
#define MOUSE_ROTATION "MOUSE_ROTATION" 
#define MOUSE_ZOOM "MOUSE_ZOOM"

/**
* MOUSE_ROTATION is in degrees 
* MOUSE_ZOOM relative zoom relative to the bounding box
*/



/**
 * a class for logging. 
 */
class Logger
{
public:
    /**
     * default constructor.
     */
    Logger(void)
            :needRelease_(false)
             //needRelease_ means if any allocated data need to be released when destructing this object.
    {
        p_os_ = &std::cout;
        gettimeofday(&begin_, 0);
        *p_os_<<begin_.tv_sec<<" "<<begin_.tv_usec<<" BEGIN"<<std::endl;
    }


    /**
     * constructor from a C string as the filename.
     */
    Logger(const char* filename)
            :needRelease_(true)
    {
        p_os_ = new std::ofstream(filename);
        gettimeofday(&begin_, 0);
        *p_os_<<begin_.tv_sec<<" "<<begin_.tv_usec<<" BEGIN"<<std::endl;
    }

    /**
     * constructor from a ostream.
     */
    Logger(std::ostream os)
            :needRelease_(false)
    {
        p_os_ = &os;
        gettimeofday(&begin_,0);
        *p_os_<<begin_.tv_sec<<" "<<begin_.tv_usec<<" BEGIN"<<std::endl;
    }

    /**
     * constructor for file name using model name and type of file
     */
    Logger(const std::string model , const std::string user ,const std::string type)
            :needRelease_(true)
    {

	std::string filename = genTraceFileName(model,user,type);
        p_os_ = new std::ofstream(filename.c_str());
        gettimeofday(&begin_, 0);
        *p_os_<<begin_.tv_sec<<" "<<begin_.tv_usec<<" BEGIN"<<std::endl;
    }

    /**
     * destructor.
     */
    ~Logger()
    {
        *p_os_<<std::endl;
        if (needRelease_)
        {
            delete p_os_;
        }
    }

    /**
     * log a message.
     */
    inline void log(const char* message)
    {
        LOCK;
        *p_os_<<curr_time()<<" "<<message<<std::endl;
    }

    /**
     * log a specific information about the transmission.
     */
    inline void log(const char direction, const char type, PacketID packet_no, int size)
    {
        LOCK;
        *p_os_<<curr_time()<<" "<<direction<<" "<<type<<" "<<packet_no<<" "<<size<<std::endl;
    }
    
    /**
     * log type, packet id, and size of pkt
     */
    inline void log(const char type, PacketID packet_no, int size)
    {
        LOCK;
        *p_os_<<curr_time()<<" "<<type<<" "<<packet_no<<" "<<size<<std::endl;
    }


    /**
     * log a specific message (C string).
     */
    inline void log(const char direction, const char type, const char* message)
    {
        LOCK;
        *p_os_<<curr_time()<<" "<<direction<<" "<<type<<" "<<message<<std::endl;
    }

    /**
     * log type and message
     */
    inline void log( const char type, const std::string message)
    {
        LOCK;
        *p_os_<<curr_time()<<" "<<type<<" "<<message<<std::endl;
    }
    
    /**
     * log mouse zoom parameters 
     */
    inline void log( const std::string param1 , const double param2)
    {
        LOCK;
        *p_os_<<curr_time()<<" "<<param1<<" "<<param2<<std::endl;
    }
    
    
    /**
     * log mouse rotation parameters 
     */
    inline void log( const std::string param1 , const double param2,const double param3)
    {
        LOCK;
        *p_os_<<curr_time()<<" "<<param1<<" "<<param2<<" "<<param3<<std::endl;
    }
    

    /**
     * log a specific message (STL string).
     */
    inline void log(const char direction, const char type, const std::string& message)
    {
        LOCK;
        *p_os_<<curr_time()<<" "<<direction<<" "<<type<<" "<<message<<std::endl;
    }

    /**
     * log a message (STL string).
     */
    inline void log(const std::string& message)
    {
        LOCK;
        *p_os_<<curr_time()<<" "<<message<<std::endl;
    }

    /**
     * log a projection matrix.
     */
    inline void log(double modelview_matrix_[16], float view_pos_[3])
    {
        LOCK;
        *p_os_<<curr_time()<<" Matrix: ";
        *p_os_<<modelview_matrix_[0]<<" ";
        *p_os_<<modelview_matrix_[1]<<" ";
        *p_os_<<modelview_matrix_[2]<<" ";
        *p_os_<<modelview_matrix_[3]<<" ";
        *p_os_<<modelview_matrix_[4]<<" ";
        *p_os_<<modelview_matrix_[5]<<" ";
        *p_os_<<modelview_matrix_[6]<<" ";
        *p_os_<<modelview_matrix_[7]<<" ";
        *p_os_<<modelview_matrix_[8]<<" ";
        *p_os_<<modelview_matrix_[9]<<" ";
        *p_os_<<modelview_matrix_[10]<<" ";
        *p_os_<<modelview_matrix_[11]<<" ";
        *p_os_<<modelview_matrix_[12]<<" ";
        *p_os_<<modelview_matrix_[13]<<" ";
        *p_os_<<modelview_matrix_[14]<<" ";
        *p_os_<<modelview_matrix_[15]<<" ";
        *p_os_<<"View "<<view_pos_[0]<<" "<<view_pos_[1]<<" "<<view_pos_[2]<<std::endl;
    }

    /**
     * return unique filename based on model and type names
     */
    inline std::string genTraceFileName(std::string model_name,std::string user_name,std::string type)
    {
	time_t t = time(0);
	struct tm* lt = localtime(&t);
	char time_str[20];
	std::sprintf(time_str, "%04d.%02d.%02d.%02d.%02d.%02d",
	lt->tm_year + 1900, lt->tm_mon + 1, lt->tm_mday,
	lt->tm_hour, lt->tm_min, lt->tm_sec);

	std::string timestring = time_str;

	std::string filename = user_name + SEPERATOR + model_name + SEPERATOR + type + SEPERATOR + timestring + EXT;
	return filename;

    }

    Logger *log2_;
    
private:
    
    std::ostream* p_os_;
    struct timeval begin_;
    bool  needRelease_;
    Poco::Mutex  mutex_;
    inline const std::string curr_time()
    {
        struct timeval tp;
        std::stringstream curr;
        gettimeofday(&tp, 0);
        time_t diff_s = tp.tv_sec - begin_.tv_sec;
        suseconds_t diff_us = tp.tv_usec - begin_.tv_usec;
        if (diff_us < 0)
        {
            diff_us += 1000000;
            diff_s  -= 1;
        }
        //curr<<tp.tv_sec<<" "<<tp.tv_usec<<" ";
        curr<<diff_s<<" "<<diff_us;
        return curr.str();
    }
};
#endif
