#include "vertexid.hh"
#include "visiblepq.hh"
#include "Poco/RunnableAdapter.h"
#include "Poco/Mutex.h"


VisiblePQ::VisiblePQ(Ppmesh* ppmesh, Gfmesh* gfmesh)
        :toContinue_(true), isStrict_(true), \
        pixels_(0), size_(0) //ppmesh_(ppmesh), gfmesh_(gfmesh)
{
    in_queue_ = new PQ();
    out_queue_ = new PQ();
    to_run_ = new Poco::RunnableAdapter<VisiblePQ>(*this, &VisiblePQ::run);
    ppmesh_ = ppmesh;
    gfmesh_ = gfmesh;
}

void VisiblePQ::stat_screen_area()
{
    Gfmesh* gfmesh = gfmesh_;
    //clear weight
    gfmesh->clear_weight();
    for (size_t i = 0; i < size_; i+=3)
    {
        unsigned char color_r = pixels_[i];
        unsigned char color_g = pixels_[i+1];
        unsigned char color_b = pixels_[i+2];
        Index seq_no = color_r * 65536 + color_g*256 + color_b;
        if (seq_no != 0)
        {
            gfmesh->increment_face_weight(seq_no-1);
        }
        if (!toContinue_) return;
        //std::cerr<<(int)color_r<<" "<<(int)color_g<<" "<<(int)color_b<<" "<<seq_no<<std::endl;
    }
    for (size_t i = 0; i < gfmesh->face_number(); i++)
    {
        gfmesh->add_vertex_weight_in_face(i);
        if (!toContinue_) return;
    }
    //for debug
    /*
    for (size_t i = 0; i<gfmesh->face_number(); i++)
    {
        std::cerr<<i<<" "<<gfmesh->face_weight(i)<<std::endl;
    }*/
    /*
    for (size_t i = 0; i<gfmesh->vertex_number(); i++)
    {
        if (gfmesh->vertex_weight(i) != 0)
        std::cerr<<i<<" "<<ppmesh_->index2id(i)<<" "<<gfmesh->vertex_weight(i)\
           <<" "<<ppmesh_->idIsLeaf(ppmesh_->index2id(i))<<std::endl;
    }
    */
}

void VisiblePQ::update(unsigned char* pixels, size_t size)
{

    //toContinue_=false;
    stoped_.lock();
    pixels_ = pixels;
    size_ = size;
    toContinue_=true;
    if (in_queue_) delete in_queue_;
    in_queue_ = new PQ();
    //weights.reset();
    //std::cerr<<thread_pool_.available()<<" "<<thread_pool_.allocated()<<std::endl;
    //thread_pool_.start(*to_run_);
    run();
}

size_t VisiblePQ::pick(std::vector<VertexID>& id_array, size_t number)
{
    size_t n = 0;
    n = ppmesh_->pick(id_array, number);
    //std::cerr<<"from to-be-split "<<n<<std::endl;
    while (n<number)
    {
        VertexID id = pop();
        if (id == 0)
        {
            //std::cerr<<"see 0. end. "<<std::endl;
            break;
        }
        if (ppmesh_->isPicked(id))
        {
            //std::cerr<<id<<" picked"<<std::endl;
            if (ppmesh_->idIsLeaf(id))
            {
                //std::cerr<<id<<" is leaf."<<std::endl;
            }
            continue;
        }
        id_array.push_back(id);
        ppmesh_->set_picked(id);
        n++;
    }
    return n;
}

VertexID VisiblePQ::pop()
{
    VertexID top = 0;
    if (!out_queue_->empty())
    {
        top = out_queue_->top();
        assert(top!=0);
        out_queue_->pop();
    }
    return top;
}

void VisiblePQ::run()
{
    stat_screen_area();
    //gfmesh_->reset_color();
    std::vector<VertexID>::const_iterator it(ppmesh_->vertex_front().begin());
    std::vector<VertexID>::const_iterator end(ppmesh_->vertex_front().end());
    //std::cerr<<"vertex front size "<<ppmesh_->vertex_front().size()<<std::endl;
    for (; it != end; ++it)
    {
        if (!toContinue_) break;
        if (ppmesh_->idIsLeaf(*it)) continue;
        if (ppmesh_->isPicked(*it)) continue;
        /*if (ppmesh_->index2id(ppmesh_->id2index(*it)) != *it)
        {
            std::cerr<<*it<<" unmatch "<<ppmesh_->index2id(ppmesh_->id2index(*it))<<std::endl;
            
        }
        std::cerr<<"front"<<*it<<" "<<ppmesh_->id2index(*it)<<std::endl;*/
        if (isStrict_)
        {
            if (gfmesh_->vertex_weight(ppmesh_->id2index(*it)) == 0) continue;
        }
        //std::cerr<<gfmesh_->vertex_weight(ppmesh_->id2index(*it))<<std::endl;
        //gfmesh_->set_color(ppmesh_->id2index(*it), 0., 0., 1.);
        push(*it);
    }
    PQ* temp = out_queue_;
    out_queue_ = in_queue_;
    in_queue_  = temp;
    //std::cerr<<"out queue size "<<out_queue_->size()<<std::endl;
    stoped_.unlock();
}















