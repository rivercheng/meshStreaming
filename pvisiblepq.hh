#ifndef __PVISIBLE_PQ_H__
#define __PVISIBLE_PQ_H__
#include <queue>
#include <GL/glut.h>
#include <Poco/Thread.h>
#include <Poco/ThreadPool.h>
#include <Poco/RunnableAdapter.h>
#include "ppmesh.hh"
#include "packetid.hh"
#include "logger.hh"


//static Ppmesh* ppmesh_ = NULL;
//static Gfmesh* gfmesh_  = NULL;
/**
 * The class similar to VisiblePQ but for chunk based transmission. We sort the chunks instead of vertices.
 */
class PVisiblePQ
{
public:
    /**
     * constructor from points to ppmesh and gfmesh. A logger is needed for logging.
     */
    PVisiblePQ(Ppmesh* ppmesh, Gfmesh* gfmesh, Logger& logger);

    /**
     * destructor.
     */
    ~PVisiblePQ(void);

    /**
     * update the contribution of each chunk.
     */
    void update(unsigned char* pixels, size_t size);

    /**
     * popup a packet ID.
     */
    PacketID pop(void);

    /**
     * stop calculating since the view-point is changed again.
     */
    void stop(void);

    /**
     * Set value of strict_. If it is true then the chunk with 0 contribution will never be requested.
     */
    void setStrict(bool value)
    {
        isStrict_ = value;
    }

    /**
     * a function to quit the render.
     */
    void quit(void);
private:
    void push(PacketID id, int area);
    void run(void);
    void stat_screen_area(void);


    bool toContinue_;
    bool isStrict_;
    Logger& logger_;
    bool toQuit_;
    unsigned char* pixels_;
    size_t  size_;

    //weight 1 -> pid_lists[0]
    //weight 2 -> pid_lists[1]
    //weight 3-4 -> pid_lists[2]
    //weight 5-8 -> pid_lists[3]
    //weight 9-16 -> pid_lists[4]
    //weight 17-32 -> pid_lists[5]
    //weight 33-64 -> pid_lists[6]
    //weight 65-128 -> pid_lists[7]
    //weight 129-256 -> pid_lists[8]
    //weight 257-inf -> pid_lists[9]
    std::list<PacketID> pid_lists[10];
    std::set<PacketID> p_id_set_;
    std::map<PacketID, bool> p_id_sent_;
    Poco::FastMutex stoped_;
    Poco::RunnableAdapter<PVisiblePQ> *to_run_;
    Poco::ThreadPool thread_pool_;
};
#endif
