//=============================================================================
//Written by Cheng Wei
//National University of Singapore
//10 Dec 2007
//=============================================================================


//== INCLUDES =================================================================

#ifdef _MSC_VER
#  pragma warning(disable: 4267 4311)
#endif

#define DEBUG(X) std::cerr<<"DEBUG: "<<(X)<<std::endl;

#include <cmath>
#include <iostream>
#include <fstream>
#include <bitset>
#include <list>
#include <string>
// --------------------
#include <OpenMesh/Core/IO/BinaryHelper.hh>
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Utils/Endian.hh>
// --------------------
#include "ppmesh.hh"
#include "gfmesh.hh"
#include "huffman.hh"
#include "bitstring.hh"
// --------------------
#include "logger.hh"

#define MAGIC_WORD "PPMESH"
#define MAX_NEIGHBORS 1000

//Quantize the difference.
static int quantize_d(double value, double max, double min, unsigned int quantize_bit = 12)
{
    unsigned int steps;
    static unsigned int step_array[17]=\
                                       {1,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192,16384,32768,65536};
    if (quantize_bit <= 16)
    {
        steps = step_array[quantize_bit];
    }
    else
    {
        steps = static_cast<unsigned int> (pow(2, quantize_bit));
    }
    int q_val = 0;
    q_val = static_cast<int>( value  * steps / (max - min));
    if (static_cast<unsigned int>(q_val) == steps) q_val --;
    return q_val;
}

//Dequantize the difference
static double de_quantize_d(int value, double max, double min, unsigned int quantize_bit = 12)
{
    unsigned int steps;
    static unsigned int step_array[17]=\
                                       {1,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192,16384,32768,65536};
    if (quantize_bit <= 16)
    {
        steps = step_array[quantize_bit];
    }
    else
    {
        steps = static_cast<unsigned int> (pow(2, quantize_bit));
    }
    double d_val = 0;
    d_val = value * 1.0 * (max - min) / steps;
    return d_val;
}

//== IMPLEMENTATION ==========================================================
Ppmesh::Ppmesh(bool needGfmesh)
        :n_base_vertices_(0), n_base_faces_(0), n_detail_vertices_(0),\
        n_max_vertices_(0), tree_bits_(0), levels_(0), \
        quantize_bits_(14), level0_(0), level1_(9),\
        x_min_(0), y_min_(0), z_min_(0), x_max_(0), y_max_(0), z_max_(0),\
        tree1Exist_(false), tree2Exist_(false), id_coder_(0), \
        geometry_coder1_(0), geometry_coder2_(0), waiting_(0), \
        needGfmesh_(needGfmesh), expected_level_(1), wait_(0)
{
    for (int i = 0; i < 32; i++)
    {
        n_[i] = 0;
    }
    //use '1' as the flag for leaves, which have no split to send.
    not_existed_flag_.push_back_bit(true);
    gfmesh_ = NULL;
}

Ppmesh::Ppmesh(int quantize_bits, bool needGfmesh)
        :n_base_vertices_(0), n_base_faces_(0), n_detail_vertices_(0),\
        n_max_vertices_(0), tree_bits_(0), levels_(0), \
        quantize_bits_(quantize_bits), level0_(0), level1_(9),\
        x_min_(0), y_min_(0), z_min_(0), x_max_(0), y_max_(0), z_max_(0),\
        tree1Exist_(false), tree2Exist_(false),id_coder_(0),\
        geometry_coder1_(0), geometry_coder2_(0), waiting_(0), \
        needGfmesh_(needGfmesh),expected_level_(1), wait_(0)
{
    for (int i = 0; i < 32; i++)
    {
        n_[i] = 0;
    }
    not_existed_flag_.push_back_bit(true);
    gfmesh_=NULL;
}

Ppmesh::Ppmesh(const char* filename,  bool needGfmesh, bool needDecode, bool readDetail, int quantize_bits)
        :n_base_vertices_(0), n_base_faces_(0), n_detail_vertices_(0),\
        n_max_vertices_(0), tree_bits_(0), levels_(0), \
        quantize_bits_(quantize_bits), level0_(0), level1_(9),\
        x_min_(0), y_min_(0), z_min_(0), x_max_(0), y_max_(0), z_max_(0),\
        tree1Exist_(false), tree2Exist_(false),id_coder_(0),\
        geometry_coder1_(0), geometry_coder2_(0), waiting_(0),\
        needGfmesh_(needGfmesh),expected_level_(1), wait_(0)
{
    for (int i = 0; i < 32; i++)
    {
        n_[i] = 0;
    }
    not_existed_flag_.push_back_bit(true);
    std::ifstream ifs(filename, std::ios::binary);
    gfmesh_ = NULL;
    //std::cerr<<"before entering read."<<std::endl;
    read(ifs, needDecode, readDetail);
    splits_.clear();
}

Ppmesh::~Ppmesh()
{
        if (id_coder_) delete id_coder_;
        if (geometry_coder1_) delete geometry_coder1_;
        if (geometry_coder2_) delete geometry_coder2_;
        if (gfmesh_) delete gfmesh_;
}

//----PUBLIC FUNTIONS-------------------------------------------------
void
Ppmesh::readPM(std::istream& ifs, bool readDetail)
{
    MyMesh::Point  p;
    unsigned int   i, i0, i1, i2;
    unsigned int   v1, vl, vr;
    char           c[10];

    bool swap = OpenMesh::Endian::local() != OpenMesh::Endian::LSB;
    bool is64bit = (sizeof(size_t) == 8);

    // read header
    ifs.read(c, 8);
    c[8] = '\0';
    if (std::string(c) != std::string("ProgMesh"))
    {
        throw WrongFileFormat();
    }

    size_t ignore;
    OpenMesh::IO::binary<size_t>::restore( ifs, n_base_vertices_,   swap );
    if (!is64bit)
    {
        OpenMesh::IO::binary<size_t>::restore(ifs, ignore, swap);
    }
    OpenMesh::IO::binary<size_t>::restore( ifs, n_base_faces_,      swap );
    if (!is64bit)
    {
        OpenMesh::IO::binary<size_t>::restore(ifs, ignore, swap);
    }
    OpenMesh::IO::binary<size_t>::restore( ifs, n_detail_vertices_, swap );
    if (!is64bit)
    {
        OpenMesh::IO::binary<size_t>::restore(ifs, ignore, swap);
    }

    n_max_vertices_    = n_base_vertices_ + n_detail_vertices_;


    tree_bits_ = (size_t)ceil(log(n_base_vertices_)/log(2.));
    //DEBUG("get tree_bits");
    //DEBUG(tree_bits_);
    unsigned int leading_one = 1<<tree_bits_;

    // load base mesh
    mesh_.clear();

    for (i=0; i<n_base_vertices_; ++i)
    {
        MyMesh::VertexHandle v;
        OpenMesh::IO::binary<MyMesh::Point>::restore( ifs, p, swap );
        v = mesh_.add_vertex(p);
        VertexID id = i + leading_one;
        mesh_.deref(v).id = id;
        mesh_.deref(v).level = 0;
        map_[id].v = v;
        vertex_front_.push_back(id);
        map_[id].pos_in_vertex_front = vertex_front_.size() - 1;
    }


    for (i=0; i<n_base_faces_; ++i)
    {
        OpenMesh::IO::binary<unsigned int>::restore( ifs, i0, swap);
        OpenMesh::IO::binary<unsigned int>::restore( ifs, i1, swap);
        OpenMesh::IO::binary<unsigned int>::restore( ifs, i2, swap);
        mesh_.add_face(mesh_.vertex_handle(i0),
                       mesh_.vertex_handle(i1),
                       mesh_.vertex_handle(i2));
    }

    if (needGfmesh_)
    {
        gfmesh_ = new Gfmesh(this);
        assert(gfmesh_);
    }

    if (!readDetail)
    {
        return;
    }
    // load progressive detail
    for (i=0; i<n_detail_vertices_; ++i)
    {
        OpenMesh::IO::binary<MyMesh::Point>::restore( ifs, p, swap );
        OpenMesh::IO::binary<unsigned int>::restore( ifs, v1, swap );
        OpenMesh::IO::binary<unsigned int>::restore( ifs, vl, swap );
        OpenMesh::IO::binary<unsigned int>::restore( ifs, vr, swap );

        PMInfo pminfo;
        pminfo.p0 = p;
        pminfo.v1 = MyMesh::VertexHandle(v1);
        pminfo.vl = MyMesh::VertexHandle(vl);
        pminfo.vr = MyMesh::VertexHandle(vr);
        pminfos_.push_back(pminfo);
    }
    pmiter_ = pminfos_.begin();

    // info
    std::cerr << mesh_.n_vertices() << " vertices, "
    << mesh_.n_edges()    << " edge, "
    << mesh_.n_faces()    << " faces, "
    << n_detail_vertices_ << " detail vertices\n";
    //encode_id();
    //DEBUG("id encoded.");
    //encode_geometry();
    //quantize();
}

void
Ppmesh::writePM(std::ostream& ofs, bool writeDetail, bool onlyBase)
{
    MyMesh::Point  p;
    unsigned int   v1, vl, vr;

    bool swap = OpenMesh::Endian::local() != OpenMesh::Endian::LSB;
    bool is64bit = (sizeof(size_t) == 8);
    size_t ignore = 0;

    // write header
    ofs.write("ProgMesh", 8);

    // write base mesh
    if (onlyBase)
    {
        coarsen(0);
        OpenMesh::IO::binary<size_t>::store( ofs, n_base_vertices_,   swap );
        if (!is64bit)
        {
            OpenMesh::IO::binary<size_t>::store(ofs, ignore, swap);
        }
        OpenMesh::IO::binary<size_t>::store( ofs, n_base_faces_,      swap );
        if (!is64bit)
        {
            OpenMesh::IO::binary<size_t>::store(ofs, ignore, swap);
        }
        OpenMesh::IO::binary<size_t>::store( ofs, n_detail_vertices_, swap );
        if (!is64bit)
        {
            OpenMesh::IO::binary<size_t>::store(ofs, ignore, swap);
        }
    }
    else //write current mesh
    {
        MyMesh::ConstVertexIter v_it(mesh_.vertices_begin());
        MyMesh::ConstVertexIter v_end(mesh_.vertices_end());
        size_t n_vertices = 0;
        size_t n_faces = 0;
        for  ( ; v_it != v_end; ++v_it)
        {
            n_vertices++;
        }
        MyMesh::ConstFaceIter f_it(mesh_.faces_begin());
        MyMesh::ConstFaceIter f_end(mesh_.faces_end());
        for ( ; f_it != f_end; ++f_it)
        {
            n_faces++;
        }
        OpenMesh::IO::binary<size_t>::store( ofs, n_vertices,   swap );
        if (!is64bit)
        {
            OpenMesh::IO::binary<size_t>::store(ofs, ignore, swap);
        }
        OpenMesh::IO::binary<size_t>::store( ofs, n_faces,      swap );
        if (!is64bit)
        {
            OpenMesh::IO::binary<size_t>::store(ofs, ignore, swap);
        }
        OpenMesh::IO::binary<size_t>::store( ofs, n_detail_vertices_, swap );
        if (!is64bit)
        {
            OpenMesh::IO::binary<size_t>::store(ofs, ignore, swap);
        }
    }
    MyMesh::ConstVertexIter v_it(mesh_.vertices_begin());
    MyMesh::ConstVertexIter v_end(mesh_.vertices_end());
    for  ( ; v_it != v_end; ++v_it)
    {
        p = mesh_.point(v_it);
        OpenMesh::IO::binary<MyMesh::Point>::store( ofs, p, swap );
    }

    MyMesh::ConstFaceIter f_it(mesh_.faces_begin());
    MyMesh::ConstFaceIter f_end(mesh_.faces_end());
    for ( ; f_it != f_end; ++f_it)
    {
        MyMesh::ConstFaceVertexIter fv_it(mesh_, f_it.handle());
        unsigned int v_idx1 = (fv_it.handle()).idx();
        unsigned int v_idx2 = ((++fv_it).handle()).idx();
        unsigned int v_idx3 = ((++fv_it).handle()).idx();
        OpenMesh::IO::binary<unsigned int>::store( ofs, v_idx1, swap);
        OpenMesh::IO::binary<unsigned int>::store( ofs, v_idx2, swap);
        OpenMesh::IO::binary<unsigned int>::store( ofs, v_idx3, swap);
    }


    if (writeDetail && onlyBase)
    {
        //save progressive detail
        for (unsigned int i=0; i<n_detail_vertices_; ++i)
        {
            p = pminfos_[i].p0;
            v1 = pminfos_[i].v1.idx();
            vl = pminfos_[i].vl.idx();
            vr = pminfos_[i].vr.idx();
            OpenMesh::IO::binary<MyMesh::Point>::store( ofs, p, swap );
            OpenMesh::IO::binary<unsigned int>::store( ofs, v1, swap );
            OpenMesh::IO::binary<unsigned int>::store( ofs, vl, swap );
            OpenMesh::IO::binary<unsigned int>::store( ofs, vr, swap );
        }
    }
    pmiter_ = pminfos_.begin();

    // info
    std::cerr << mesh_.n_vertices() << " vertices, "
    << mesh_.n_edges()    << " edge, "
    << mesh_.n_faces()    << " faces, "
    << n_detail_vertices_ << " detail vertices\n";
}

void
Ppmesh::writeOogl(std::ostream& ofs)
{
    MyMesh::Point  p;

    // write header
    ofs<<"OFF "<<mesh_.n_vertices()<<" "<<mesh_.n_faces()<<" "<<mesh_.n_edges()<<std::endl;
    MyMesh::ConstVertexIter v_it(mesh_.vertices_begin());
    MyMesh::ConstVertexIter v_end(mesh_.vertices_end());
    for  ( ; v_it != v_end; ++v_it)
    {
        p = mesh_.point(v_it);
        ofs<<p[0]<<" "<<p[1]<<" "<<p[2]<<std::endl;
    }

    MyMesh::ConstFaceIter f_it(mesh_.faces_begin());
    MyMesh::ConstFaceIter f_end(mesh_.faces_end());
    for ( ; f_it != f_end; ++f_it)
    {
        MyMesh::ConstFaceVertexIter fv_it(mesh_, f_it.handle());
        unsigned int v_idx1 = (fv_it.handle()).idx();
        unsigned int v_idx2 = ((++fv_it).handle()).idx();
        unsigned int v_idx3 = ((++fv_it).handle()).idx();
        ofs<<"3 "<<v_idx1<<" "<<v_idx2<<" "<<v_idx3<<std::endl;
    }
}


void
Ppmesh::refine(unsigned int _n)
{
    if (gfmesh_)
    {
        gfmesh_->mutex_.lock();
    }
    size_t n_vertices = mesh_.n_vertices();
    while (n_vertices < _n && pmiter_ != pminfos_.end())
    {
        pmiter_->v0 = mesh_.add_vertex(pmiter_->p0);
        mesh_.vertex_split(pmiter_->v0,pmiter_->v1,pmiter_->vl,pmiter_->vr);


        VertexID id1 = mesh_.deref(pmiter_->v1).id;
        VertexID id0 = (id1 << 1) + 1;
        id1 = id1 << 1;

        mesh_.deref(pmiter_->v0).id = id0;
        mesh_.deref(pmiter_->v1).id = id1;
        map_[id0].v = pmiter_->v0;
        map_[id1].v = pmiter_->v1;

        if (gfmesh_)
        {
            gfmesh_->vertex_split(pmiter_->v1.idx(), pmiter_->vl.idx(), pmiter_->vr.idx(), pmiter_->p0[0], pmiter_->p0[1], pmiter_->p0[2]);
        }
        ++pmiter_;
        ++n_vertices;
    }

    //comment out for efficiency.
    //mesh_.update_face_normals();
    //mesh_.update_vertex_normals();

    std::cerr << n_vertices << " vertices\n";
    if (gfmesh_)
    {
        gfmesh_->mutex_.unlock();
    }
}

void
Ppmesh::coarsen(unsigned int _n)
{
    size_t n_vertices = mesh_.n_vertices();

    while (n_vertices > _n && pmiter_ != pminfos_.begin())
    {
        --pmiter_;

        MyMesh::HalfedgeHandle hh =
            mesh_.find_halfedge(pmiter_->v0, pmiter_->v1);
        mesh_.deref(pmiter_->v1).id >>= 1;
        mesh_.collapse(hh);

        --n_vertices;
    }

    mesh_.garbage_collection();

    //mesh_.update_face_normals();
    //mesh_.update_vertex_normals();

    std::cerr << n_vertices << " vertices\n";
}

/*void Ppmesh::restore(std::vector<VertexID>& history)
{
    coarsen(0);
    //std::cerr<<"coarsened"<<std::endl;
    if (gfmesh_) delete gfmesh_;
    gfmesh_ = 0;
    gfmesh_ = new Gfmesh(*this);
    assert(gfmesh_);
    history_.clear();
    for (size_t i=0;i<history.size(); i++)
    {
        VertexID id = history[i];
        size_t pos = 0;
        decode(id, get_data(id), &pos);
    }
}*/

void Ppmesh::outputHistory(std::ostream& os)
{
    for (size_t i=0;i<history_.size();i++)
    {
        os<<history_[i]<<std::endl;
    }
}

void Ppmesh::restoreFromFile(std::istream& is)
{
    coarsen(0);
    if (gfmesh_) delete gfmesh_;
    gfmesh_ = 0;
    gfmesh_ = new Gfmesh(this);
    assert(gfmesh_);
    while (!is.eof())
    {
        VertexID id;
        is>>id;
        size_t pos = 0;
        decode(id, get_data(id), &pos);
    }
}

bool Ppmesh::decodeId(VertexID id, bool temp)
{
    size_t pos = 0;
    return decode(id, get_data(id), &pos, temp);
}

bool Ppmesh::decode(VertexID id, const BitString& data, size_t* p_pos, bool temp)
{
    //DEBUG("Decoding");
    //DEBUG(to_string(id));
    bool result = false;
    history_.push_back(id);
    if (id2level(id) >= minimum_depth_ && data.test(*p_pos))
    {
        (*p_pos)++;
        VertexID parent_id = (id >> 1);
        if (map_.find(parent_id) == map_.end() || map_[parent_id].isLeaf)
        {
            return false;
        }
        else
        {
            if (map_.find(id) != map_.end())
            {
                VsInfo& vsinfo = map_[id];
                size_t pos1 = vsinfo.pos_in_vertex_front;
                VertexID last_id = vertex_front_.back();
                vertex_front_[pos1] = last_id;
                map_[last_id].pos_in_vertex_front = pos1;
                vertex_front_.erase(--vertex_front_.end());
            }
            map_[id].isLeaf = true;
            return false;
        }
    }

    if (id2level(id) >= minimum_depth_)
    {
        (*p_pos)++;
    }


    std::vector<unsigned int> value_array;
    size_t number = 0;
    number = id_coder_->decode(data, value_array, p_pos, 2);
    assert(number == 2);
    unsigned int code_l = value_array[0];
    unsigned int code_r = value_array[1];

    //DEBUG("code decoded");

    int dx = 0;
    int dy = 0;
    int dz = 0;
    unsigned int level= id2level(id);
    if (level < level0_)
    {
        BitString bs1 = data.substr(*p_pos, 12);
        dx = static_cast<int>(bs1.to_ulong());
        (*p_pos) += 12;
        BitString bs2 = data.substr(*p_pos, 12);
        dy = static_cast<int>(bs2.to_ulong());
        (*p_pos) += 12;
        BitString bs3 = data.substr(*p_pos, 12);
        dz = static_cast<int>(bs3.to_ulong());
        (*p_pos) += 12;
    }
    else
    {
        std::vector<int> geometry_array;
        Huffman::HuffmanCoder<int> * coder = NULL;
        if (level < level1_)
        {
            coder = geometry_coder1_;
        }
        else
        {
            coder = geometry_coder2_;
        }
        //for debug
        number = 0;
        number = coder->decode(data, geometry_array, p_pos, 3);
        assert(number == 3);
        dx = geometry_array[0];
        dy = geometry_array[1];
        dz = geometry_array[2];
    }

    //DEBUG("geometry decoded");
    splitInfo* split = new splitInfo();
    //MyMesh::Point p1 = mesh_.point(vsinfo.v);
    //split->x1 = p1[0];
    //split->y1 = p1[1];
    //split->z1 = p1[2];
    //split->v = vsinfo.v;
    split->id = id;
    split->code_l = code_l;
    split->code_r = code_r;
    split->dx = dx;
    split->dy = dy;
    split->dz = dz;

    if (gfmesh_)
    {
        gfmesh_->mutex_.lock();
    }
    if (map_.find(id) == map_.end()) //vertex does not exist.
    {
        //std::cerr<<to_string(id)<<" no exist yet."<<std::endl;
        map_[id].isReceived = true;
        map_[id].isPicked   = true;
        map_[id>>1].waiting_list.push_back(split);
    }
    else
    {
        //store the split history
        result = splitVs(split, temp);
    }
    if (gfmesh_)
    {
        gfmesh_->mutex_.unlock();
    }
    return result;
}

void Ppmesh::reverse()
{
    --pmiter_;
    MyMesh::VertexHandle v1 = pmiter_->v1;
    MyMesh::VertexHandle v0 = pmiter_->v0;
    //std::cerr<<"v1: "<<v1<<" v0: "<<v0<<std::endl;

    MyMesh::HalfedgeHandle hh = mesh_.find_halfedge(v0, v1);
    mesh_.deref(v1).id >>= 1;
    mesh_.collapse(hh);
    mesh_.garbage_collection();
    pminfos_.erase(pminfos_.end());
    gfmesh_->collapse(v1.idx());
    //delete gfmesh_;
    //gfmesh_ = new Gfmesh(*this);
}



bool Ppmesh::splitVs(splitInfo* split, bool temp)
{
    PMInfo pminfo;
    pminfo.id = split->id;
    //std::cerr<<"split "<<split->id<<std::endl;
    //MyMesh::VertexHandle v1 = split->v;
    MyMesh::VertexHandle v1 = map_[split->id].v;
    //std::cerr<<mesh_.deref(v1).id<<" "<<split->id<<std::endl;
    assert(mesh_.deref(v1).id == split->id);
    //Logger log;

    MyMesh::Point p1 = mesh_.point(v1);
    double x1 = p1[0];
    double y1 = p1[1];
    double z1 = p1[2];
    //double x1 = split->x1;
    //double y1 = split->y1;
    //double z1 = split->z1;

    std::vector<VertexID> neighbors;
    pminfo.neighbor_number = one_ring_neighbor(v1, neighbors);

    unsigned int pos = 0;
    std::vector<VertexID> results;
    MyMesh::VertexHandle vl;
    MyMesh::VertexHandle vr;
    unsigned int         code_l = split->code_l;
    unsigned int         code_r = split->code_r;
    VertexID             id     = split->id;
    VertexID             id_l   = 0;
    VertexID             id_r   = 0;
    VertexID             o_id_l = 0;
    VertexID             o_id_r = 0;
    VsInfo&              vsinfo = map_[id];
    //try
    //{
    if (code_l != 0)
    {
        unsigned int code_remain = 1;
        pos = code2id(neighbors, code_l, results, &code_remain);
        if (results.size() > 1)
        {
            o_id_l = further_split(results, id, pos, LEFT, temp);
        }
        else
        {
            o_id_l = results[0];
        }
        assert(results.size() == 1);
        if (results.size() == 0)
        {
            throw DecodeError();
        }
        id_l = results[0];
        results.clear();
        vl = map_[id_l].v;
        vsinfo.id_l = o_id_l;
        vsinfo.code_remain_l = code_remain;
    }
    else
    {
        //it means no vl exist.
        //we just keep vl as the default value(-1).
        ;
    }

    if (code_r != 0)
    {
        unsigned int code_remain = 1;
        pos = code2id(neighbors, code_r, results, &code_remain);
        if (results.size() > 1)
        {
            o_id_r = further_split(results, id, pos, RIGHT, temp);
        }
        else
        {
            o_id_r = results[0];
        }
        assert(results.size() == 1);
        if (results.size() == 0)
        {
            throw DecodeError();
        }
        id_r = results[0];
        results.clear();
        vr = map_[id_r].v;
        vsinfo.id_r = o_id_r;
        vsinfo.code_remain_r = code_remain;

    }

    if (id_l == id_r)
    {
        //We have to delay the split since the two neighbors are currently one same vertex.
        if (!temp)
        {
            map_[id_r].waiting_list.push_back(split);
            to_be_split_.insert(id_r);
        }
        return false;
    }
    //}
    //catch(DecodeError& e)
    //{
    //    map_[neighbors[0]].waiting_list.push_back(split);
    //    return false;
    //}
    double x0 = x1 + de_quantize_d(split->dx, x_max_, x_min_, quantize_bits_);
    double y0 = y1 + de_quantize_d(split->dy, y_max_, y_min_, quantize_bits_);
    double z0 = z1 + de_quantize_d(split->dz, z_max_, z_min_, quantize_bits_);

    MyMesh::Point p0(x0, y0, z0);
    pminfo.p0 = p0;
    pminfo.v0 = mesh_.add_vertex(pminfo.p0);
    pminfo.v1 = v1;
    pminfo.vl = vl;
    pminfo.vr = vr;
    pminfos_.push_back(pminfo);
    pmiter_ = pminfos_.end();

    mesh_.vertex_split(pminfo.v0, pminfo.v1, pminfo.vl, pminfo.vr);

    delete split;
    split = 0;
    waiting_ ++;
    VertexID id0 = (id << 1) + 1;
    VertexID id1 = (id << 1);
    mesh_.deref(pminfo.v0).id = id0;
    mesh_.deref(pminfo.v1).id = id1;
    map_[id0].v = pminfo.v0;
    map_[id1].v = pminfo.v1;

    //update vertex_front_
    if (!temp)
    {
        vertex_front_.push_back(id0);
        map_[id0].pos_in_vertex_front = vertex_front_.size() - 1;
        size_t pos1 = vsinfo.pos_in_vertex_front;
        map_[id1].pos_in_vertex_front = pos1;
        vertex_front_[pos1] = id1;
    }

    if (gfmesh_)
    {
        //std::cerr<<"split in gfmesh."<<std::endl;
        gfmesh_->vertex_split(pminfo.v1.idx(), pminfo.vl.idx(), pminfo.vr.idx(), pminfo.p0[0], pminfo.p0[1], pminfo.p0[2]);
    }

    if (!temp)
    {
        //split the vertice splits in the waiting list
        for (size_t i = 0; i < vsinfo.waiting_list.size(); i++)
        {
            splitVs(vsinfo.waiting_list[i]);
        }
        vsinfo.waiting_list.clear();
    }
    return true;
}


void Ppmesh::read(std::istream& ifs, bool needDecode, bool readDetail)
{
    read_base_mesh(ifs);
    //DEBUG("base mesh read.");

    bool swap = OpenMesh::Endian::local() != OpenMesh::Endian::LSB;
    read_huffman_tree(ifs, id_tree_);


    OpenMesh::IO::binary<bool>::restore(ifs, tree1Exist_, swap);
    if (tree1Exist_)
    {
        read_huffman_tree(ifs, geometry_tree1_);

    }

    OpenMesh::IO::binary<bool>::restore(ifs, tree2Exist_, swap);
    if (tree2Exist_)
    {
        read_huffman_tree(ifs, geometry_tree2_);
    }
    //DEBUG("huffman_tree read.");
    if (readDetail)
    {
        read_vs_info(ifs, n_detail_vertices_);
    }

    if (needDecode)
    {
        id_coder_ = new Huffman::HuffmanCoder<unsigned int>(id_tree_);
        if (tree1Exist_)
        {
            geometry_coder1_ = new Huffman::HuffmanCoder<int>(geometry_tree1_);
        }
        if (tree2Exist_)
        {
            geometry_coder2_ = new Huffman::HuffmanCoder<int>(geometry_tree2_);
        }
    }
}

void Ppmesh::decodeAll()
{
    VertexID id = 0;

    while (!splits_.empty())
    {
        size_t  pos = 0;
        id =  *splits_.begin();
        splits_.erase(splits_.begin());
        decode(id, get_data(id), &pos);
    }
}

void Ppmesh::decode_to_level(unsigned int level)
{
    VertexID id = 0;
    //int count[20] = {0};
    while (!splits_.empty())
    {
        size_t pos = 0;
        id = *splits_.begin();
        splits_.erase(splits_.begin());
        //std::cerr<<"level "<<id2level(id)<<std::endl;
        if (id2level(id) < level)
        {
            decode(id, get_data(id), &pos);
        }
        //count[id2level(id)] ++;
    }
    //std::cerr<<"level 1: "<<count[1]<<std::endl;
    //std::cerr<<"level 2: "<<count[2]<<std::endl;
    //std::cerr<<"level 3: "<<count[3]<<std::endl;
    //std::cerr<<"level 4: "<<count[4]<<std::endl;
    //std::cerr<<"level 5: "<<count[5]<<std::endl;
    //std::cerr<<"level 6: "<<count[6]<<std::endl;
    //std::cerr<<"level 7: "<<count[7]<<std::endl;
    //std::cerr<<"level 8: "<<count[8]<<std::endl;
}




void Ppmesh::write(std::ostream& ofs, bool writeDetail)
{
    bool swap = OpenMesh::Endian::local() != OpenMesh::Endian::LSB;

    DEBUG("in write()");

    if (writeDetail)
    {
        encode_id();
        DEBUG("finished encoding id");
        encode_geometry();
        DEBUG("finished encoding geometry");
    }
    write_base_mesh(ofs);
    DEBUG("finished based mesh");

    write_huffman_tree(ofs, id_tree_);

    OpenMesh::IO::binary<bool>::store(ofs, tree1Exist_, swap);
    if (tree1Exist_)
    {
        write_huffman_tree(ofs, geometry_tree1_);
    }
    OpenMesh::IO::binary<bool>::store(ofs, tree2Exist_, swap);
    if (tree2Exist_)
    {
        write_huffman_tree(ofs, geometry_tree2_);
    }
    if (writeDetail)
    {
        write_vs_info(ofs);
    }
}

void Ppmesh::output_vertex_splits(std::vector<VertexID>& vertex_id_array)
{
    PMInfoConstIter pm_end = pminfos_.end();
    for (PMInfoConstIter pmiter = pminfos_.begin(); pmiter!= pm_end; ++pmiter)
    {
        vertex_id_array.push_back(pmiter->id);
    }
}



int Ppmesh::write_to_level(std::ostream& ofs, unsigned int level)
{

    bool swap = OpenMesh::Endian::local() != OpenMesh::Endian::LSB;
    encode_id();
    encode_geometry();
    write_base_mesh(ofs);
    write_huffman_tree(ofs, id_tree_);
    OpenMesh::IO::binary<bool>::store(ofs, tree1Exist_, swap);
    if (tree1Exist_)
    {
        write_huffman_tree(ofs, geometry_tree1_);
    }
    OpenMesh::IO::binary<bool>::store(ofs, tree2Exist_, swap);
    if (tree2Exist_)
    {
        write_huffman_tree(ofs, geometry_tree2_);
    }
    PMInfoConstIter pm_end = pminfos_.end();
    int count = 0;
    std::streampos curr = ofs.tellp();
    ofs.write((const char*)&count, sizeof(count));
    for (PMInfoConstIter pmiter = pminfos_.begin(); pmiter!= pm_end; ++pmiter)
    {
        if (id2level(pmiter->id) < level)
        {
            BitString& data = map_[pmiter->id].data;
            OpenMesh::IO::binary<VertexID>::store(ofs, pmiter->id, swap);
            OpenMesh::IO::binary<unsigned int>::store(ofs, data.size(), swap);
            data.write_binary(ofs);
            count ++;
        }
    }
    ofs.seekp(curr);
    ofs.write((const char*)&count, sizeof(count));
    return count;
}

/*void Ppmesh::read_compact(std::istream& ifs)
{
    bool swap = OpenMesh::Endian::local() != OpenMesh::Endian::LSB;

    read_base_mesh(ifs);
    read_huffman_tree(ifs, id_tree_);
    Huffman::HuffmanCoder<unsigned int> id_coder(id_tree_);
    Huffman::HuffmanCoder<int>* geometry_coder1 = NULL;
    Huffman::HuffmanCoder<int>* geometry_coder2 = NULL;

    OpenMesh::IO::binary<bool>::restore(ifs, tree1Exist_, swap);
    if (tree1Exist_)
    {
        read_huffman_tree(ifs, geometry_tree1_);
        geometry_coder1 = new Huffman::HuffmanCoder<int>(geometry_tree1_);
    }

    OpenMesh::IO::binary<bool>::restore(ifs, tree2Exist_, swap);
    if (tree2Exist_)
    {
        read_huffman_tree(ifs, geometry_tree2_);
        geometry_coder2 = new Huffman::HuffmanCoder<int>(geometry_tree2_);
    }

    //read vertex splits data (compressed.)
    size_t vs_number  = 0;
    while (vs_number < n_detail_vertices_)
    {
        unsigned int id  = 0;
        OpenMesh::IO::binary<unsigned int>::restore(ifs, id, swap);
        unsigned int level = id2level(id);

        BitString data;
        size_t string_pos  = 0;
        PMInfo  pminfo;
        size_t code_number = 0;
        std::vector<unsigned int> code_array;
        std::vector<int> geometry_array;
        while (code_number < 2)
        {
            data.read_binary(ifs, 8);
            code_number += id_coder.decode(data, code_array, &string_pos, 2 - code_number);
        }
        //unsigned int code_l = code_array[0];
        //unsigned int code_r = code_array[1];
        if (level < level0_)
        {
            while (string_pos + 36 > data.size())
            {
                data.read_binary(ifs, 8);
            }
            BitString bs1 = data.substr(string_pos, 12);
            string_pos += 12;
            BitString bs2 = data.substr(string_pos, 12);
            string_pos += 12;
            BitString bs3 = data.substr(string_pos, 12);
            string_pos += 12;
        }
        else
        {
            Huffman::HuffmanCoder<int> * coder = NULL;
            if (level < level1_)
            {
                coder = geometry_coder1;
            }
            else
            {
                coder = geometry_coder2;
            }
            if (string_pos == data.size())
            {
                data.read_binary(ifs,8);
            }
            code_number = 0;
            code_number = coder->decode(data, geometry_array, &string_pos, 3);
            while (code_number < 3)
            {
                data.read_binary(ifs, 8);
                code_number += coder->decode(data, geometry_array, &string_pos, 3-code_number);
            }
        }
        map_[id].data = data;
        vs_number++;
    }
    if (geometry_coder1)
    {
        delete geometry_coder1;
        geometry_coder1 = NULL;
    }
    if (geometry_coder2)
    {
        delete geometry_coder2;
        geometry_coder2 = NULL;
    }
}*/

/*void Ppmesh::write_compact(std::ostream& ofs)
{
    bool swap = OpenMesh::Endian::local() != OpenMesh::Endian::LSB;
    write_base_mesh(ofs);
    write_huffman_tree(ofs, id_tree_);
    OpenMesh::IO::binary<bool>::store(ofs, tree1Exist_, swap);
    if (tree1Exist_)
    {
        write_huffman_tree(ofs, geometry_tree1_);
    }
    OpenMesh::IO::binary<bool>::store(ofs, tree2Exist_, swap);
    if (tree2Exist_)
    {
        write_huffman_tree(ofs, geometry_tree2_);
    }
    write_vs_info_compact(ofs);
}*/



//-----------------------------------------------------------------------------
/*void Ppmesh::test_output(std::ostream& of)
{

    PMInfoIter pmiter = pminfos_.begin();
    PMInfoConstIter pm_end = pminfos_.end();
    for (; pmiter != pm_end; ++pmiter)
    {
        of<<"id "<<pmiter->id<<" level "<<id2level(pmiter->id)<<" neighbor number: "<<pmiter->neighbor_number<<" connectivity len: "<<pmiter->connectivity_len<<" geometry len:"<<pmiter->geometry_len<<" data "<<map_[pmiter->id].data<<std::endl;
        of<<"id "<<pmiter->id<<" id_l "<<map_[pmiter->id].id_l<<" id_r "<<map_[pmiter->id].id_r<<std::endl;
    }
    for (int i = 0; i<32;i++)
    {
        of <<i<<" "<<n_[i]<<std::endl;
    }
}*/

/*static unsigned int quantize_coord(double value, double max, double min, unsigned int quantize_bit = 12)
{
    unsigned int steps;
    if (quantize_bit == 12)
    {
        steps = 4096;
    }
    else
    {
        steps = static_cast<unsigned int> (pow(2, quantize_bit));
    }
    unsigned int q_val = 0;
    q_val = static_cast<unsigned int>( (value - min) * steps / (max - min));
    if (q_val == steps) q_val --;
    return q_val;
}



static double de_quantize_coord(unsigned int value, double max, double min, unsigned int quantize_bit = 12)
{
    unsigned int steps;
    if (quantize_bit == 12)
    {
        steps = 4096;
    }
    else
    {
        steps = static_cast<unsigned int> (pow(2, quantize_bit));
    }
    double d_val = 0;
    d_val = value * 1.0 / steps + min;
    return d_val;
}*/







size_t Ppmesh::one_ring_neighbor(const MyMesh::VertexHandle& v1, std::vector<VertexID>& neighbors) const
{
    size_t neighbor_number = 0;
    MyMesh::ConstVertexVertexIter vv_it(mesh_, v1);
    while (vv_it)
    {
        neighbors.push_back(vv_it->id);
        ++vv_it;
        neighbor_number++;
    }
    return neighbor_number;
}

VertexID Ppmesh::pick_one()
{
    VertexID id = 0;
    while (!to_be_split_.empty())
    {
        VertexID id = *to_be_split_.begin();
        to_be_split_.erase(to_be_split_.begin());
        if (!map_[id].isPicked)
        {
            map_[id].isPicked = true;
            break;
        }
    }
    return id;
}


size_t Ppmesh::pick(std::vector<VertexID>& id_array, size_t n)
{
    //static size_t pos = 0;
    size_t count = 0;
    while (!to_be_split_.empty() && count < n)
    {

        VertexID id =  *to_be_split_.begin();
        to_be_split_.erase(to_be_split_.begin());
        if (!map_[id].isPicked) //&& map_[id].isVisible)
        {
            id_array.push_back(id);
            count++;
            map_[id].isPicked = true;
        }
    }

    /*while(map_[vertex_front_[pos]].isLeaf)
    {
        pos++;
        if (pos >= n_max_vertices_)
        {
            return count;
        }
    }

    size_t i = pos;
    while (count < n)
    {
        while(i< vertex_front_.size() && (map_[vertex_front_[i]].isPicked ||\
                  map_[vertex_front_[i]].isReceived || !map_[vertex_front_[i]].isVisible || id2level(vertex_front_[i])>expected_level_))
        {
            //std::cerr<<map_[vertex_front_[i]].isVisible<<std::endl;
            i++;
            //std::cerr<<"skip "<<i<<std::endl;
        }
        if (i>=vertex_front_.size())
        {
            expected_level_ ++;
            //std::cerr<<expected_level_<<std::endl;
            if (expected_level_ >= 32)break;
            i=pos;
            continue;
        }
        id_array.push_back(vertex_front_[i]);
        map_[vertex_front_[i]].isPicked = true;
        i++;
        count++;
    }*/
    //std::cerr<<"return "<<count<<std::endl;
    return count;
}


//==============Private functions=========================================

void Ppmesh::quantize()
{
    refine(n_max_vertices_);
    MyMesh::VertexIter v_it(mesh_.vertices_begin());
    MyMesh::VertexIter v_end(mesh_.vertices_end());
    x_max_ = x_min_ = mesh_.point(v_it)[0];
    y_max_ = y_min_ = mesh_.point(v_it)[1];
    z_max_ = z_min_ = mesh_.point(v_it)[2];

    for (v_it=mesh_.vertices_begin(); v_it!= v_end; ++v_it)
    {
        double x = mesh_.point(v_it)[0];
        double y = mesh_.point(v_it)[1];
        double z = mesh_.point(v_it)[2];
        x_max_ = (x_max_ < x) ? x : x_max_;
        x_min_ = (x_min_ > x) ? x : x_min_;
        y_max_ = (y_max_ < y) ? y : y_max_;
        y_min_ = (y_min_ > y) ? y : y_min_;
        z_max_ = (z_max_ < z) ? z : z_max_;
        z_min_ = (z_min_ > z) ? z : z_min_;
    }
    long steps;
    long steps_array[13] = {1,2,4,8,16,32,64,128,256,512,1024,2048,4096};
    if (quantize_bits_ <= 13)
    {
        steps = steps_array[quantize_bits_ - 1];
    }
    else
    {
        steps =(long)pow(2, quantize_bits_ - 1);
    }
    double xstep = (x_max_ - x_min_) / steps;
    double ystep = (y_max_ - y_min_) / steps;
    double zstep = (z_max_ - z_min_) / steps;

    //for debug
    std::cerr<<steps<<" "<<xstep<<" "<<ystep<<" "<<zstep<<std::endl;
    std::cerr<<x_max_<<" "<<x_min_<<" "<<std::endl;
    std::cerr<<y_max_<<" "<<y_min_<<" "<<std::endl;
    std::cerr<<z_max_<<" "<<z_min_<<" "<<std::endl;
}

unsigned int
Ppmesh::id2code(const std::vector<VertexID>& id_array, VertexID id)const
{
    std::bitset<sizeof(VertexID) * 8> bs(id);
    std::string str(bs.to_string());
    std::string::size_type loc = str.find('1',0);
    if (loc==std::string::npos)
    {
        std::cerr<<"no 1 exists in the id."<<std::endl;
        return 0;
    }
    str = str.substr(loc+1);
    std::vector<VertexID>::const_iterator it;
    std::list<std::string> str_list;
    std::list<std::string>::iterator str_it;
    for (it = id_array.begin(); it != id_array.end(); ++it)
    {
        std::bitset<sizeof(VertexID) * 8> bs(*it);
        std::string bs_str = bs.to_string();
        loc = bs_str.find('1', 0);
        if (loc==std::string::npos)
        {
            std::cerr<<"no 1 exists in the id."<<std::endl;
            return 0;
        }
        str_list.push_back(bs_str.substr(loc+1));
    }

    unsigned int result = 1;
    for (std::string::size_type i = 0; i< str.size(); i++)
    {
        char base(str[i]);
        bool need_to_code(false);
        str_it = str_list.begin();
        std::list<std::string>::iterator it_temp;
        while (str_it != str_list.end())
        {
            it_temp = str_it;
            ++str_it;
            if ((*it_temp)[i] != base)
            {
                need_to_code = true;
                str_list.erase(it_temp);
            }
        }
        if (need_to_code)
        {
            result <<= 1;
            if (base == '1')
            {
                result++;
            }
        }
        if (str_list.size() == 1)
        {
            break;
        }
    }
    return result;
}

size_t Ppmesh::code2id(const std::vector<VertexID>&id_array, unsigned int code, std::vector<VertexID>& result_array, unsigned int* p_code_remain, size_t pos) const
{
    std::bitset<sizeof(unsigned int) * 8> bs(code);
    std::string str(bs.to_string());
    std::string::size_type loc = str.find('1',0);
    if (loc==std::string::npos)
    {
        std::cerr<<"no 1 exists in the id."<<bs<<std::endl;
        throw InvalidID();
    }
    str = str.substr(loc+1);

    //We devide the neighbors to two groups based on the value of
    //i-th bit, and then choose one group according to the input code.
    //If all neighbors go to one group, we just continue.
    size_t n  = 0;
    //to improve speed, we avoid using std::vector here
    //So we convert result_array to a real array
    
    const VertexID *real_array = &(id_array[0]);
    size_t real_size = id_array.size();
    assert(real_size <= MAX_NEIGHBORS);
    
    VertexID id_array_1[MAX_NEIGHBORS];
    VertexID id_array_2[MAX_NEIGHBORS];
    
    //result_array = id_array;
    size_t n1 = 0;
    size_t n2 = 0;
    for (; pos < sizeof(VertexID)*8; pos++)
    {
        n1 = 0;
        n2 = 0;
        //std::vector<VertexID> id_array_1;
        //std::vector<VertexID> id_array_2;
        //std::vector<VertexID>::const_iterator id_it;
        //for (id_it = result_array.begin(); id_it != result_array.end(); id_it++)
        for (size_t i = 0; i < real_size; i++)
        {
            //if (idIsSet(*id_it, pos))
            //DEBUG(real_array[i])
            if (idIsSet(real_array[i], pos))
            {
                //id_array_2.push_back(*id_it);
                id_array_2[n2] = real_array[i];
                n2++;
            }
            else
            {
                //id_array_1.push_back(*id_it);
                id_array_1[n1] = real_array[i];
                n1++;
            }
        }
        if (n1>0 && n2>0)
        {
            if (n == str.size())
            {
                pos--;
                break;
            }
            //DEBUG(str[n]);
            if (str[n] == '0')
            {
                //result_array = id_array_1;
                real_array = id_array_1;
                real_size = n1;
            }
            else
            {
                //result_array = id_array_2;
                real_array = id_array_2;
                real_size = n2;
            }
            n++;
        }
        //if (result_array.size() == 1)
        if (real_size == 1)
        {
            break;
        }
    }


    result_array.resize(real_size);
    if (real_size > n1 && real_size > n2)
    {
        assert(real_size == n1 + n2);
        for (size_t i = 0; i < n1; i++)
        {
            //result_array.push_back(id_array_1[i]);
            result_array[i] = id_array_1[i];
        }
        for (size_t i = 0; i < n2; i++)
        {
            //result_array.push_back(id_array_2[i]);
            result_array[n1+i] = id_array_2[i];
        }
    }
    else
    {
        for (size_t i = 0 ; i < real_size; i++)
        {
            //result_array.push_back(real_array[i]);
            result_array[i] = real_array[i];
        }
    }

    //push the rest code into code_remain. The last bit be put first.
    size_t i = str.size() - 1;
    while (n < str.size())
    {
        (*p_code_remain) <<= 1;
        if (str[i] == '1')
        {
            (*p_code_remain) +=  1;
        }
        n++;
        i--;
    }
    return pos;
}

VertexID Ppmesh::further_split(std::vector<VertexID>& neighbors, VertexID id, size_t pos,Side side, bool temp)
{

    //change the id to id string. (find the leading 1 and remove it)
    std::bitset<sizeof(unsigned int) * 8> bs(id);
    std::string str(bs.to_string());
    std::string::size_type loc = str.find('1',0);
    if (loc==std::string::npos)
    {
        std::cerr<<"no 1 exists in the id."<<bs<<std::endl;
        throw InvalidID();
    }
    str = str.substr(loc+1);


    VertexID o_parent = 0;
    while (neighbors.size() > 1)
    {
        //find the common parent of all the vertices in the neighbors list.
        size_t len = len_of_id(neighbors[0]) - pos - 1;
        VertexID mask = 0xffffffff << len;
        VertexID parent = (mask & neighbors[0]);
        parent >>= len;
        if (o_parent == 0) o_parent = parent;

        //according to the id_l or id_r of this parent, we collect the code to
        //guide the further split. (The code is in the reverse order.)
        VsInfo& vsinfo = map_[parent];
        VertexID  id_l = vsinfo.id_l;
        VertexID  id_r = vsinfo.id_r;
        unsigned int code = 1;
        unsigned int bit  = 0;
        VertexID     id_t = id;
        if (id_l > id_t)
        {
            while (id_l > id_t) id_l >>= 1;
        }
        else
        {
            while (id_l < id_t) id_t >>= 1;
        }
        if (id_l == id_t)
        {
            bit = (vsinfo.code_remain_l & 0x1);
            code <<= 1;
            if (side == LEFT)
            {
                code += bit;
            }
            else
            {
                code += (1-bit);
            }
            //vsinfo.id_l <<= 1;
            //vsinfo.id_l += bit;
            if (!temp)
            {
                vsinfo.code_remain_l >>= 1;
            }
        }
        else
        {
            id_t = id;
            if (id_r > id_t)
            {
                while (id_r > id_t) id_r >>=1;
            }
            else
            {
                while (id_r < id_t) id_t >>=1;
            }
            if (id_r == id_t)
            {
                bit = (vsinfo.code_remain_r & 0x1);
                code <<= 1;
                if (side == LEFT)
                {
                    code += (1-bit);
                }
                else
                {
                    code += bit;
                }
                //vsinfo.id_r <<= 1;
                //vsinfo.id_r += bit;
                if (!temp)
                {
                    vsinfo.code_remain_r >>= 1;
                }
            }
            else
            {
                std::cerr<<"id "<<id<<" id_l "<<id_l<<" id_r "<<id_r<<std::endl;
                std::cerr<<"decode error"<<std::endl;
                throw DecodeError();
            }
        }
        unsigned int remain = 0;
        pos = code2id(neighbors, code, neighbors, &remain, pos+1);
    }
    return o_parent;
}


unsigned int Ppmesh::id2level(VertexID id)const
{
    const VertexID mask = 0x80000000;
    size_t len = sizeof(VertexID)*8;
    while ((mask & id) == 0)
    {
        id <<= 1;
        len --;
    }
    if (len < tree_bits_)
    {
        std::cerr<<"Wrong id."<<std::endl;
        return 0;
    }
    return (len-tree_bits_);
}

void
Ppmesh::encode_id()
{
    coarsen(0);
    std::vector<unsigned int> code_array;
    std::vector<unsigned int>::const_iterator code_iter;
    PMInfoIter pmiter = pminfos_.begin();
    PMInfoConstIter pm_end = pminfos_.end();
    for (; pmiter != pm_end; ++pmiter)
    {
        pmiter->v0 = mesh_.add_vertex(pmiter->p0);
        pmiter->id = mesh_.deref(pmiter->v1).id;

        if (mesh_.deref(pmiter->v1).id > (2147483648U))
        {
            std::cerr<<"the id "<<mesh_.deref(pmiter->v1).id<<" is too long for vertex "<<pmiter->v1<<std::endl;
        }

        //find one-ring neighbors and save the indices as strings
        std::vector<VertexID> id_array;
        pmiter->neighbor_number = one_ring_neighbor(pmiter->v1, id_array);

        //encode the vl and vr
        VertexID     id_l = 0;
        VertexID     id_r = 0;
        unsigned int code_l = 0;
        unsigned int code_r = 0;
        if (pmiter->vl.is_valid())
        {
            id_l = mesh_.deref(pmiter->vl).id;
            code_l = id2code(id_array, id_l);
        }
        if (pmiter->vr.is_valid())
        {
            id_r = mesh_.deref(pmiter->vr).id;
            code_r = id2code(id_array, id_r);
        }

        code_array.push_back(code_l);
        code_array.push_back(code_r);

        mesh_.vertex_split(pmiter->v0,pmiter->v1,pmiter->vl,pmiter->vr);
        if (gfmesh_)
        {
            gfmesh_->vertex_split(pmiter_->v1.idx(), pmiter_->vl.idx(), pmiter_->vr.idx(), pmiter_->p0[0], pmiter_->p0[1], pmiter_->p0[2]);
        }
        mesh_.deref(pmiter->v0).id = (mesh_.deref(pmiter->v1).id << 1) + 1;
        mesh_.deref(pmiter->v1).id <<= 1;
        mesh_.deref(pmiter->v1).level ++;
        pmiter->level = mesh_.deref(pmiter->v0).level = mesh_.deref(pmiter->v1).level;
        //cannot change order here.
        map_[mesh_.deref(pmiter->v0).id].v = pmiter->v0;
        map_[mesh_.deref(pmiter->v1).id].v = pmiter->v1;
    }
    pmiter_ = pmiter;

    //find the minimum depth
    //
    MyMesh::ConstVertexIter v_it(mesh_.vertices_begin());
    MyMesh::ConstVertexIter v_end(mesh_.vertices_end());
    minimum_depth_ = v_it->level;
    for  ( ; v_it != v_end; ++v_it)
    {
        if (minimum_depth_ > v_it->level)
        {
            minimum_depth_ = v_it->level;
        }
    }



    //Compress by Huffman code.

    //use dynamic allocating just for save memory.
    Huffman::HuffmanCoder<unsigned int>* coder = new Huffman::HuffmanCoder<unsigned int>(code_array);
    size_t count = 0;
    avg_connectivity_len_ = 0;
    avg_connectivity_len3000_ = 0;
    avg_connectivity_len10000_ = 0;
    avg_connectivity_len50000_ = 0;
    avg_connectivity_len100000_ = 0;

    code_iter = code_array.begin();
    for (pmiter = pminfos_.begin(); pmiter != pm_end; ++pmiter)
    {
        BitString& data = map_[pmiter->id].data;
        if (id2level(pmiter->id) >= minimum_depth_)
        {
            data.push_back_bit(false); //0 to reprensent a valid split.
        }
        coder->encode(*code_iter, data);
        ++code_iter;
        coder->encode(*code_iter, data);
        ++code_iter;

        pmiter->connectivity_len = data.size();
        if (count < 3000)
        {
            avg_connectivity_len3000_ += pmiter->connectivity_len;
        }
        if (count < 10000)
        {
            avg_connectivity_len10000_ += pmiter->connectivity_len;
        }
        if (count < 50000)
        {
            avg_connectivity_len50000_ += pmiter->connectivity_len;
        }
        if (count < 100000)
        {
            avg_connectivity_len100000_ += pmiter->connectivity_len;
        }
        avg_connectivity_len_ += pmiter->connectivity_len;
        ++count;
    }
    avg_connectivity_len_ /= count;
    avg_connectivity_len3000_ /= 3000;
    avg_connectivity_len10000_ /= 10000;
    avg_connectivity_len50000_ /= 50000;
    avg_connectivity_len100000_ /= 100000;

    coder->output_tree(id_tree_);
    delete coder;
    coder = 0;
}

const BitString& Ppmesh::get_data(VertexID id) const
{
    MapConstIter it = map_.find(id);
    if (it != map_.end() && it->second.data.size() > 0)
    {
        return it->second.data;
    }
    else
    {
        return not_existed_flag_;
    }
}

//-----------------------------------------------------------------------------
//for geometry encoding.
//we use two huffman table, one for level0_ =< level < level1_,
//the other for level >= level1_.
//For levels < level0_, we do not encode it.
void
Ppmesh::encode_geometry()
{
    PMInfoIter pmiter = pminfos_.begin();
    PMInfoConstIter pm_end = pminfos_.end();

    std::vector<int> coords_array_0;
    std::vector<int> coords_array_1;
    std::vector<int> coords_array_2;

    refine(n_max_vertices_);
    quantize();
    for (pmiter = pminfos_.begin(); pmiter != pm_end; ++pmiter)
    {
        int dx, dy, dz;
        //unsigned int id = pmiter->id;
        MyMesh::Point p0 = mesh_.point(pmiter->v0);
        MyMesh::Point p1 = mesh_.point(pmiter->v1);
        dx = quantize_d(p0[0] - p1[0], x_max_, x_min_, quantize_bits_);
        dy = quantize_d(p0[1] - p1[1], y_max_, y_min_, quantize_bits_);
        dz = quantize_d(p0[2] - p1[2], z_max_, z_min_, quantize_bits_);
        unsigned int level = id2level(pmiter->id);

        //for debug
        //std::cerr<<id<<" "<<ppminfos_[id].id<<std::endl;
        //std::cerr<<ppminfos_[id].level<<std::endl;
        if (level < level0_)
        {
            coords_array_0.push_back(dx);
            coords_array_0.push_back(dy);
            coords_array_0.push_back(dz);
        }
        else if (level < level1_)
        {
            coords_array_1.push_back(dx);
            coords_array_1.push_back(dy);
            coords_array_1.push_back(dz);
        }
        else
        {
            coords_array_2.push_back(dx);
            coords_array_2.push_back(dy);
            coords_array_2.push_back(dz);
        }
        n_[level] ++;
    }

    Huffman::HuffmanCoder<int>* coder1 = NULL;
    try
    {
        coder1 = new Huffman::HuffmanCoder<int>(coords_array_1);
    }
    catch (Huffman::EmptyInput& e)
    {
        coder1 = NULL;
    }

    Huffman::HuffmanCoder<int>* coder2 = NULL;
    try
    {
        coder2 = new Huffman::HuffmanCoder<int>(coords_array_2);
    }
    catch (Huffman::EmptyInput& e)
    {
        coder2 = NULL;
    }


    avg_geometry_len_ = 0;
    avg_geometry_len3000_ = 0;
    avg_geometry_len10000_ = 0;
    avg_geometry_len50000_ = 0;
    avg_geometry_len100000_ = 0;
    int count = 0;

    std::vector<int>::iterator coords_it0(coords_array_0.begin());
    std::vector<int>::iterator coords_it1(coords_array_1.begin());
    std::vector<int>::iterator coords_it2(coords_array_2.begin());

    for (pmiter = pminfos_.begin(); pmiter != pm_end; ++pmiter)
    {
        pmiter->geometry_len = 0;
        int dx;
        int dy;
        int dz;
        unsigned int level = id2level(pmiter->id);
        BitString&   data  = map_[pmiter->id].data;
        if (level < level0_)
        {
            dx = *coords_it0;
            ++coords_it0;
            dy = *(coords_it0);
            ++coords_it0;
            dz = *(coords_it0);
            ++coords_it0;
            std::bitset<32> bitsx(dx);
            std::bitset<32> bitsy(dy);
            std::bitset<32> bitsz(dz);
            data.push_back(bitsx.to_string());
            data.push_back(bitsy.to_string());
            data.push_back(bitsz.to_string());
        }
        else if (level < level1_)
        {
            dx = *coords_it1;
            ++coords_it1;
            dy = *(coords_it1);
            ++coords_it1;
            dz = *(coords_it1);
            ++coords_it1;
            coder1->encode(dx, data);
            coder1->encode(dy, data);
            coder1->encode(dz, data);
        }
        else
        {
            dx = *coords_it2;
            ++coords_it2;
            dy = *(coords_it2);
            ++coords_it2;
            dz = *(coords_it2);
            ++coords_it2;
            coder2->encode(dx, data);
            coder2->encode(dy, data);
            coder2->encode(dz, data);
        }
        //DEBUG("encoding dx dy dz");
        //DEBUG(data);
        //DEBUG(dx);
        //DEBUG(dy);
        //DEBUG(dz);

        //for information
        pmiter->geometry_len = data.size()- pmiter->connectivity_len;
        if (count < 3000)
        {
            avg_geometry_len3000_ += pmiter->geometry_len;
        }
        if (count < 10000)
        {
            avg_geometry_len10000_ += pmiter->geometry_len;
        }
        if (count < 50000)
        {
            avg_geometry_len50000_ += pmiter->geometry_len;
        }
        if (count < 100000)
        {
            avg_geometry_len100000_ += pmiter->geometry_len;
        }
        avg_geometry_len_ += pmiter->geometry_len;
        ++count;
    }
    avg_geometry_len3000_ /= 3000;
    avg_geometry_len10000_ /= 10000;
    avg_geometry_len50000_ /= 50000;
    avg_geometry_len100000_ /= 100000;
    avg_geometry_len_ /= count;
    if (coder1)
    {
        coder1->output_tree(geometry_tree1_);
        tree1Exist_ = true;
        delete coder1;
        coder1 = 0;
    }
    else
    {
        tree1Exist_ = false;
    }
    if (coder2)
    {
        coder2->output_tree(geometry_tree2_);
        tree2Exist_ = true;
        delete coder2;
        coder2 = 0;
    }
    else
    {
        tree2Exist_ = false;
    }
}



void Ppmesh::write_base_mesh(std::ostream& ofs)
{
    bool swap = OpenMesh::Endian::local() != OpenMesh::Endian::LSB;

    //flag "PPM"
    std::cerr<<MAGIC_WORD<<std::endl;
    std::cerr<<"size "<<sizeof(MAGIC_WORD)-1<<std::endl;
    ofs.write(MAGIC_WORD, sizeof(MAGIC_WORD)-1);

    //write level0, level1, tree_bits_
    OpenMesh::IO::binary<unsigned int>::store(ofs, level0_, swap);
    OpenMesh::IO::binary<unsigned int>::store(ofs, level1_, swap);
    OpenMesh::IO::binary<unsigned int>::store(ofs, tree_bits_, swap);

    //write minimum depth
    OpenMesh::IO::binary<unsigned int>::store(ofs, minimum_depth_, swap);

    //write quantize_bits_, x_max, x_min, y_max, y_min, z_max, z_min
    OpenMesh::IO::binary<unsigned int>::store(ofs, quantize_bits_, swap);
    OpenMesh::IO::binary<double>::store(ofs, x_max_, swap);
    OpenMesh::IO::binary<double>::store(ofs, x_min_, swap);
    OpenMesh::IO::binary<double>::store(ofs, y_max_, swap);
    OpenMesh::IO::binary<double>::store(ofs, y_min_, swap);
    OpenMesh::IO::binary<double>::store(ofs, z_max_, swap);
    OpenMesh::IO::binary<double>::store(ofs, z_min_, swap);

    coarsen(0);
    //write base mesh
    writePM(ofs, false);
}

void Ppmesh::read_base_mesh(std::istream& ifs)
{
    bool swap = OpenMesh::Endian::local() != OpenMesh::Endian::LSB;
    //flag
    char buffer[255];
    ifs.read(buffer, sizeof(MAGIC_WORD)-1);	//check a key word at the beginning of the file. Here "PPMESH"
    buffer[sizeof(MAGIC_WORD)-1] = '\0';
    if (std::string(buffer) != MAGIC_WORD)
    {
        throw WrongFileFormat();
    }

    //read level0, level1, tree_bits_
    OpenMesh::IO::binary<unsigned int>::restore(ifs, level0_, swap);
    OpenMesh::IO::binary<unsigned int>::restore(ifs, level1_, swap);
    OpenMesh::IO::binary<unsigned int>::restore(ifs, tree_bits_, swap);

    //read minimum_depth_
    OpenMesh::IO::binary<unsigned int>::restore(ifs, minimum_depth_, swap);

    //write quantize_bits_, x_max, x_min, y_max, y_min, z_max, z_min
    OpenMesh::IO::binary<unsigned int>::restore(ifs, quantize_bits_, swap);
    OpenMesh::IO::binary<double>::restore(ifs, x_max_, swap);
    OpenMesh::IO::binary<double>::restore(ifs, x_min_, swap);
    OpenMesh::IO::binary<double>::restore(ifs, y_max_, swap);
    OpenMesh::IO::binary<double>::restore(ifs, y_min_, swap);
    OpenMesh::IO::binary<double>::restore(ifs, z_max_, swap);
    OpenMesh::IO::binary<double>::restore(ifs, z_min_, swap);

    readPM(ifs, false);
}

template <typename T>
void Ppmesh::write_huffman_tree(std::ostream& ofs, const Huffman::DecodeTree<T>& tree) const
{
    bool swap = OpenMesh::Endian::local() != OpenMesh::Endian::LSB;
    unsigned int tree_str_len = (unsigned int)tree.tree_str.size();
    OpenMesh::IO::binary<unsigned int>::store(ofs, tree_str_len, swap);
    tree.tree_str.write_binary(ofs);

    unsigned int value_array_len = (unsigned int)tree.value_array.size();
    OpenMesh::IO::binary<unsigned int>::store(ofs, value_array_len, swap);
    for (size_t i=0; i< value_array_len; ++i)
    {
        OpenMesh::IO::binary<T>::store(ofs, tree.value_array[i], swap);
    }
}

template <typename T>
void Ppmesh::read_huffman_tree(std::istream& ifs, Huffman::DecodeTree<T>& tree)
{
    bool swap = OpenMesh::Endian::local() != OpenMesh::Endian::LSB;

    unsigned int tree_str_len;
    OpenMesh::IO::binary<unsigned int>::restore(ifs, tree_str_len, swap);
    tree.tree_str.read_binary(ifs, tree_str_len);

    unsigned int value_array_len;
    OpenMesh::IO::binary<unsigned int>::restore(ifs, value_array_len, swap);
    for (size_t i = 0; i<value_array_len; ++i)
    {
        T value;
        OpenMesh::IO::binary<T>::restore(ifs, value, swap);
        tree.value_array.push_back(value);
    }
}

void Ppmesh::write_vs_info(std::ostream& ofs)
{
    bool swap = OpenMesh::Endian::local() != OpenMesh::Endian::LSB;
    PMInfoConstIter pm_end = pminfos_.end();
    for (PMInfoConstIter pmiter = pminfos_.begin(); pmiter!= pm_end; ++pmiter)
    {
        BitString& data = map_[pmiter->id].data;
        OpenMesh::IO::binary<VertexID>::store(ofs, pmiter->id, swap);
        OpenMesh::IO::binary<unsigned int>::store(ofs, data.size(), swap);
        data.write_binary(ofs);
    }
}



void Ppmesh::write_vs_info_compact(std::ostream& ofs)
{
    bool swap = OpenMesh::Endian::local() != OpenMesh::Endian::LSB;
    PMInfoConstIter pm_end = pminfos_.end();
    for (PMInfoConstIter pmiter = pminfos_.begin(); pmiter!= pm_end; ++pmiter)
    {
        OpenMesh::IO::binary<VertexID>::store(ofs, pmiter->id, swap);
        map_[pmiter->id].data.write_binary(ofs);
    }
}

void Ppmesh::read_vs_info(std::istream& ifs, size_t n)
{
    bool swap = OpenMesh::Endian::local() != OpenMesh::Endian::LSB;
    for (size_t i = 0; i< n; i++)
    {
        BitString data;
        VertexID  id;
        unsigned int len;
        OpenMesh::IO::binary<VertexID>::restore(ifs, id, swap);
        OpenMesh::IO::binary<unsigned int>::restore(ifs, len, swap);
        data.read_binary(ifs, len);
        VsInfo& vsinfo = map_[id];
        vsinfo.data = data;
        splits_.push_back(id);
    }
}


bool Ppmesh::self_check()
{
    bool correct = true;
    MyMesh::ConstVertexIter v_it(mesh_.vertices_begin());
    MyMesh::ConstVertexIter v_end(mesh_.vertices_end());
    for (; v_it != v_end; ++v_it)
    {
        if (map_[v_it->id].v != v_it.handle())
        {
            correct = false;
            break;
        }
    }

    return correct;
}

template <typename T>
void Ppmesh::output_array(const std::vector<T>& array)
{
    for (size_t i = 0; i< array.size(); i++)
    {
        DEBUG(array[i]);
    }
}

const Coordinate *Ppmesh::vertex_array () const
{
    if (gfmesh_ == 0)return 0;
    return gfmesh_->vertex_array();
}

const Index* Ppmesh::face_array()  const
{
    if (gfmesh_== 0) return 0;
    return gfmesh_->face_array();
}

//=============================================================================
