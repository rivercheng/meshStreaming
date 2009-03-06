#ifndef __VISIBLE_PQ_H__
#define __VISIBLE_PQ_H__
#include <queue>
#include <GL/glut.h>
#include <Poco/Thread.h>
#include <Poco/ThreadPool.h>
#include <Poco/RunnableAdapter.h>
#include "ppmesh.hh"
#include "gfmesh.hh"
class UnknownToken{};

static Ppmesh* ppmesh_ = NULL;
static Gfmesh* gfmesh_  = NULL;
class CompareArea : std::binary_function< VertexID, VertexID, bool>
{
public:
    bool operator()(VertexID id1, VertexID id2) const
    {
        return gfmesh_->vertex_weight(ppmesh_->id2index(id1)) < gfmesh_->vertex_weight(ppmesh_->id2index(id2));
        //return ppmesh_->id2level(id1) > ppmesh_->id2level(id2);
    }
};




typedef std::priority_queue<VertexID, std::vector<VertexID>, CompareArea>   PQ;

/**
 * the class to support sorting according to visual importance.
 */
class VisiblePQ
{
public:
    /**
     * Constructor from pointers to ppmesh and gfmesh.
     */
    VisiblePQ(Ppmesh* ppmesh, Gfmesh* gfmesh);

    /**
     * destructor.
     */
    ~VisiblePQ(void)
    {
        if (in_queue_) delete in_queue_;
        if (out_queue_) delete out_queue_;
        delete to_run_;
    }

    /**
     * Update the visual contribution according to the content of frame buffer.
     * The content of frame buffer exists in pixels and the size indicates the 
     * size of frame buffer.
     */
    void update(unsigned char* pixels, size_t size);

    /**
     * pop up the ID of vertex has the highest contribution.
     */
    VertexID pop(void);

    /**
     * to stop the calculating the visual contribution. If user change view-point quickly, the later change will make the previous calculation meaningless. Thus, we provide this funtion to stop the previous calculation.
     */
    void stop(void)
    {
        toContinue_ = false;
    }

    /**
     *Choose given number of vertex IDs according to the descending order of contribution.
     */
    size_t pick(std::vector<VertexID>& id_array, size_t number);

    /**
     * set the parameter strict. If strict is true, the vertex has contribution of 0 will not be requested. Otherwise, they are requested eventually.
     */
    void setStrict(bool value)
    {
        isStrict_ = value;
    }
private:
    inline void push(VertexID id)
    {
        in_queue_->push(id);
    }
    void run(void);
    void swap(void)
    {
        PQ* temp = in_queue_;
        in_queue_=out_queue_;
        \
        out_queue_ = temp;
    }
    void stat_screen_area(void);


    bool toContinue_;
    bool isStrict_;
    unsigned char* pixels_;
    size_t  size_;

    PQ* in_queue_;
    PQ* out_queue_;
    Poco::FastMutex stoped_;
    Poco::ThreadPool thread_pool_;
    Poco::RunnableAdapter<VisiblePQ>* to_run_;
};
#endif
