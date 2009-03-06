//=============================================================================
//Written by Cheng Wei
//National University of Singapore
//10 Dec 2007
//=============================================================================


#ifndef __PPMESH_HH__
#define __PPMESH_HH__


//== INCLUDES =================================================================

#include <OpenMesh/Core/Mesh/Types/TriMesh_ArrayKernelT.hh>
#include <OpenMesh/Core/Attributes/Attributes.hh>
#include <Poco/Mutex.h>
#include <vector>
#include <string>
#include <list>
#include <iostream>
#include <fstream>
#include <bitset>
#include <set>
#include <map>
#include <queue>
#include <GL/glut.h>
#include <Poco/Thread.h>
#include <Poco/ThreadPool.h>

#include "bitstring.hh"
#include "huffman.hh"
#include "vertexid.hh"
#include "commonType.hh"
class Gfmesh;
//== CLASS DEFINITION =========================================================

using namespace OpenMesh;
using namespace OpenMesh::Attributes;

struct MyTraits : public OpenMesh::DefaultTraits
{
    VertexAttributes  ( OpenMesh::Attributes::Normal       |
                        OpenMesh::Attributes::Status       );
    EdgeAttributes    ( OpenMesh::Attributes::Status       );
    HalfedgeAttributes( OpenMesh::Attributes::PrevHalfedge );
    FaceAttributes    ( OpenMesh::Attributes::Normal       |
                        OpenMesh::Attributes::Status       );
    VertexTraits
    {
public:
        //the id to represent the position in the binary trees.
        unsigned int id;
        unsigned int level;
        //unsigned int s_area;
    };
    FaceTraits
    {
public:
        unsigned int s_area;
    };

};


typedef OpenMesh::TriMesh_ArrayKernelT<MyTraits>  MyMesh;

enum Side {LEFT, RIGHT};

//exceptions
class WrongFileFormat{};

/**
 * the class to represent progressive mesh. It is base on OpenMesh currently.
 * We plan to develop a new presentation in the future.
 */
class Ppmesh
{
    /**
     * The information of a vertex split.
     */
    struct PMInfo
    {
        VertexID             id;
        MyMesh::Point        p0;
        MyMesh::VertexHandle v0, v1, vl, vr;
        unsigned int         level;

        //for stat information only
        unsigned int         neighbor_number;
        unsigned int         connectivity_len;
        unsigned int         geometry_len;
        PMInfo()
                :id(0), p0(0,0,0), v0(0), v1(0), vl(0), vr(0),level(0),\
                neighbor_number(0), connectivity_len(0), geometry_len(0)
        {
            ;
        }
    };
    typedef std::vector<PMInfo>                 PMInfoContainer;
    typedef PMInfoContainer::iterator           PMInfoIter;
    typedef PMInfoContainer::const_iterator     PMInfoConstIter;
public:

    //exceptions.
    class InvalidID {};
    class DecodeError {};
    class NoDecoder {};

    //== CONSTRUCTORS ======================
    /**
     * The constructor. If needGfmesh is true, the a GPU friendly mesh
     * is created. If the ppmesh is not used to render (as on server side),
     * needGfmesh can be set as false to save memory. The default quantize_bit is
     * 14 bit.
     */
    Ppmesh(bool needGfmesh = false);

    /**
     * similar to above, but the quantize_bit can be set.
     */
    Ppmesh(int quantize_bit, bool needGfmesh = false);

    /**
     * Ppmesh is directly read from a .ppm file.
     * If decoding vertex splits is not needed (as in server side),
     * the needDecode can be set as false.
     * If the detail vertex splits are not available 
     * (as in client side when reading base mesh),
     * the needDetail needs to set as false.
     */
    Ppmesh(const char* _filename, bool needGfmesh = false, bool needDecode = true, bool readDetail = true, int quantize_bit=14);

    //== DESTRUCTOR =======================
    /**
     * the destructor.
     */
    virtual ~Ppmesh();

    //== PUBLIC FUNCTIONS==================
    /**
     * read content from a .pm file (OpenMesh file).
     * The ppmesh needs to be empty.
     */
    void    readPM(std::istream& ifs, bool readDetail = true);

    /**
     * output a OpenMesh file (.pm).
     */
    void    writePM(std::ostream& ofs, bool writeDetail = true, bool onlyBase = true);

    /**
     * output a Oogl file.
     */
    void    writeOogl(std::ostream& ofs);


    /** 
     * refine the mesh to n vertex splits.
     */
    void    refine(unsigned int n);

    /**
     * coarsen the mesh to n vertex splits.
     */
    void    coarsen(unsigned int n);

    /**
     * read from a .ppm file (Ppmesh file).
     */
    void    read(std::istream& ifs, bool needDecode=true, bool readDetail=true);

    /**
     * write to a .ppm file.
     */
    void    write(std::ostream& ofs, bool writeDetail = true);

    /**
     *write to a .ppm file (only to given level).
     */
    int     write_to_level(std::ostream& ofs, unsigned int level);

    /**
     * decode all the vertex splits read from a .ppm file.
     * need to be call immediatelly after read().
     */
    void    decodeAll(void);

    /**
     * decode all the vertex splits to the given level.
     * need to be call immediatelly after read().
     */
    void    decode_to_level(unsigned int level);

    /**
     * output all the vertex splits ID. 
     */
    void    output_vertex_splits(std::vector<VertexID>& vertex_id_array);



    //write in compact format. Decoding needed in reading. For test.
    //void    read_compact(std::istream& ifs);
    //void    write_compact(std::ostream& ofs);
    
    //write in ascii format. mainly for debug.
    //void    test_output(std::ostream& of);

    /**
     * output the one ring neighbors of a given vertex.
     */
    size_t  one_ring_neighbor(const MyMesh::VertexHandle& v1, std::vector<VertexID>& neighbors) const;

    /**
     * output all the vertices and faces in the ppmesh to vertex_array and face_array.
     */
    template <typename T1, typename T2>
    void    output_arrays(std::vector<T1>& vertex_array, std::vector<T2>& face_array) const
    {
        MyMesh::ConstVertexIter v_it(mesh_.vertices_begin());
        MyMesh::ConstVertexIter v_end(mesh_.vertices_end());
        for (; v_it != v_end; ++v_it)
        {
            MyMesh::Point p = mesh_.point(v_it);
            vertex_array.push_back(static_cast<T1>(p[0]));
            vertex_array.push_back(static_cast<T1>(p[1]));
            vertex_array.push_back(static_cast<T1>(p[2]));
        }

        MyMesh::ConstFaceIter f_it(mesh_.faces_begin());
        MyMesh::ConstFaceIter f_end(mesh_.faces_end());
        for ( ; f_it != f_end; ++f_it)
        {
            MyMesh::ConstFaceVertexIter fv_it(mesh_, f_it.handle());
            unsigned int v_idx1 = (fv_it.handle()).idx();
            unsigned int v_idx2 = ((++fv_it).handle()).idx();
            unsigned int v_idx3 = ((++fv_it).handle()).idx();
            face_array.push_back(static_cast<T2>(v_idx1));
            face_array.push_back(static_cast<T2>(v_idx2));
            face_array.push_back(static_cast<T2>(v_idx3));
        }
        return;
    }

    /**
     * output the faces having given vertex to face_array.
     */
    template <typename T>
    void vertex_faces(long vertex_index, std::vector<T>& face_array) const
    {
        MyMesh::VertexHandle v(vertex_index);
        MyMesh::ConstVertexFaceIter vf_it(mesh_, v);
        while (vf_it)
        {
            face_array.push_back(static_cast<T>(vf_it.handle().idx()));
            ++vf_it;
        }
        return;
    }

    //unsafe version, for performance
    template <typename T>
    size_t vertex_faces(long vertex_index, T* face_array) const
    {
        size_t index = 0;
        MyMesh::VertexHandle v(vertex_index);
        MyMesh::ConstVertexFaceIter vf_it(mesh_, v);
        while (vf_it)
        {
            //face_array.push_back(static_cast<T>(vf_it.handle().idx()));
            face_array[index++] = static_cast<T>(vf_it.handle().idx());
            ++vf_it;
        }
        return index;
    }
    /**
     * output the vertices within a given face to give vertex_array.
     */
    template <typename T>
    void face_vertices(long face_index, std::vector<T>& vertex_array) const
    {
        MyMesh::FaceHandle f(face_index);
        MyMesh::ConstFaceVertexIter fv_it(mesh_, f);
        while (fv_it)
        {
            vertex_array.push_back(static_cast<T>(fv_it.handle().idx()));
            ++fv_it;
        }
        return;
    }

    //unsafe version, for performance
    template <typename T>
    size_t face_vertices(long face_index, T* vertex_array) const
    {
        size_t index = 0;
        MyMesh::FaceHandle f(face_index);
        MyMesh::ConstFaceVertexIter fv_it(mesh_, f);
        while (fv_it)
        {
            vertex_array[index++] = static_cast<T>(fv_it.handle().idx());
            ++fv_it;
        }
        return index;
    }
    /**
     * output all the vertices connected to a given vertex. Similar to one-ring neighbor
     * but with vertex index as the input.
     */
    template <typename T>
    void vertex_vertices(long vertex_index, std::vector<T>& vertex_array) const
    {
        MyMesh::VertexHandle v(vertex_index);
        MyMesh::ConstVertexVertexIter vv_it(mesh_, v);
        while (vv_it)
        {
            vertex_array.push_back(static_cast<T>(vv_it.handle().idx()));
            ++vv_it;
        }
        return;
    }


    //unsafe version, for performance
    template <typename T>
    size_t vertex_vertices(long vertex_index, T* vertex_array) const
    {
        size_t index = 0;
        MyMesh::VertexHandle v(vertex_index);
        MyMesh::ConstVertexVertexIter vv_it(mesh_, v);
        while (vv_it)
        {
            vertex_array[index++] = static_cast<T>(vv_it.handle().idx());
            ++vv_it;
        }
        return index;
    }
    //self test, for debug
    bool    self_check(void);

    /**
     * pick a vertex in the vertex front to be split.
     */
    VertexID pick_one(void);

    /**
     * pick a given number of vertices to be split.
     */
    virtual size_t  pick(std::vector<VertexID>& id_array, size_t n);

    /**
     * to decode a vertex. if temp = true. then we just update the 
     * companying gfmesh. In most cases, please let the temp be false.
     * It is used in a ppmesh with all the data available. In streaming,
     * please use the decode().
     */
    bool    decodeId(VertexID id, bool temp = false);

    /**
     * For temporary decoding, use this to reverse. In most cases,
     * it is not useful.
     */
    void    reverse(void);

    /**
     * to decode a vertex id from given position of a given bitstring, 
     * In most cases, please let temp be false.
     * Return ture if this vertex split can be done. return false
     * if this vertex split has to wait for parents. It will be done
     * when the parent vertex split is done without further request.
     */
    bool    decode(VertexID id, const BitString& data, size_t* p_pos, bool temp = false);

    
    /**
     * output the split history to a file. It is used for later replay.
     */
    void outputHistory(std::ostream& os);

    /**
     * Restore the mesh from a recorded split history.
     * The ppmesh needs to be empty before calling this function.
     */
    void restoreFromFile(std::istream& is);

    //get functions
    size_t  n_base_vertices(void) const
    {
        return n_base_vertices_;
    }
    size_t  n_detail_vertices(void) const
    {
        return n_detail_vertices_;
    }
    size_t  n_max_vertices(void) const
    {
        return n_max_vertices_;
    }
    unsigned int tree_bits(void) const
    {
        return tree_bits_;
    }
    size_t  n_split(void) const
    {
        return waiting_;
    }
    size_t  n_vertices(void) const
    {
        return mesh_.n_vertices();
    }

    /**
     * get vertex split data from the given ID. It is used for the server to search
     * for vertex split data.
     */
    const BitString& get_data(VertexID id) const;

    /**
     * return the current vertex front of this mesh.
     */
    const std::vector<VertexID>& vertex_front(void) const
    {
        return vertex_front_;
    }

    /**
     * return the current pending vertex splits.
     */
    const std::list<VertexID>& splits(void) const
    {
        return splits_;
    }


    size_t    wait(void) const
    {
        return wait_;
    }


    //set functions
    void   set_received(VertexID id)
    {
        map_[id].isReceived = true;
    }
    void   set_visible(VertexID  id)
    {
        map_[id].isVisible  = true;
    }
    void   set_invisible(VertexID id)
    {
        map_[id].isVisible  = false;
    }
    void   set_unpicked(VertexID  id)
    {
        map_[id].isPicked   = false;
    }
    void   set_picked(VertexID    id)
    {
        map_[id].isPicked   = true;
    }
    bool   isPicked(VertexID      id)
    {
        return map_[id].isPicked;
    }
    void   setWait(size_t wait)
    {
        wait_ = wait;
    }


    //other functions.
    /**
     * convert from vertex index to vertex ID.
     */
    VertexID  index2id(unsigned int index) const
    {
        MyMesh::VertexHandle vh(index);
        return mesh_.deref(vh).id;
    }

    /**
     * convert from vertex ID to vertex handle.
     */
    MyMesh::VertexHandle& id2handle(VertexID id)
    {
        return map_[id].v;
    }

    /**
     * convert from vertex ID to vertex index.
     */
    unsigned int id2index(VertexID id)
    {
        return map_[id].v.idx();
    }

    /**
     * get the coordinates of a given vertex ID.
     */
    void getCoord(VertexID id, float* p_x, float* p_y, float* p_z)
    {
        MyMesh::Point p = mesh_.point(map_[id].v);
        *p_x = p[0];
        *p_y = p[1];
        *p_z = p[2];
    }
    
    /**
     * convert vertex ID to level number.
     */
    unsigned int    id2level(VertexID id) const;

    /**
     * to find the root ID of a vertex ID.
     */
    VertexID rootId(VertexID id) const
    {
        size_t len = len_of_id(id);
        VertexID rootId = id;
        while(len > tree_bits_)
        {
            rootId >>=1;
            len--;
        }
        return rootId;
    }

    /**
     * to check whether an ID is of a leaf.
     */
    bool idIsLeaf(VertexID id)
    {
        return map_[id].isLeaf;
    }

    void      resetExpectedLevel(void)
    {
        expected_level_ = 1;
    }
    
    //for embedded gfmesh
    /**
     * return the vertex array of embedded gfmesh.
     */
    const Coordinate* vertex_array (void) const;

    /**
     * return the face array of embedded gfmesh.
     */
    const Index*      face_array(void)    const;

    /**
     * return the pointer to embedded gfmesh.
     */
    Gfmesh* gfmesh(void) const
    {
        return gfmesh_;
    }




    //for stat only
    unsigned int      avg_connectivity_len_;
    unsigned int      avg_connectivity_len3000_;
    unsigned int      avg_connectivity_len10000_;
    unsigned int      avg_connectivity_len50000_;
    unsigned int      avg_connectivity_len100000_;
    unsigned int      avg_geometry_len_;
    unsigned int      avg_geometry_len3000_;
    unsigned int      avg_geometry_len10000_;
    unsigned int      avg_geometry_len50000_;
    unsigned int      avg_geometry_len100000_;
private:
    //== PRIVATE FUNTIONS =============================
    unsigned int code_remain (VertexID id)
    {
        return map_[id].code_remain_l + map_[id].code_remain_r;
    }
    void            quantize();
    void            encode_id();
    void            encode_geometry();
    unsigned int    id2code(const std::vector<VertexID>& id_array, VertexID id) const;
    size_t          code2id(const std::vector<VertexID>& id_array, unsigned int code, std::vector<VertexID>& result_array, unsigned int* p_id_code, size_t pos = 0) const;
    //unsigned int    id2index(VertexID id) const;

    void            write_base_mesh(std::ostream& ofs);

    template <typename T>
    void            write_huffman_tree(std::ostream& ofs, const Huffman::DecodeTree<T>& tree) const;

    void            write_vs_info(std::ostream& ofs);
    void            write_vs_info_compact(std::ostream& ofs);//need to be decoded. for test only.

    void            read_base_mesh(std::istream& ifs);

    template <typename T>
    void            read_huffman_tree(std::istream& ifs, Huffman::DecodeTree<T>& tree);

    void            read_vs_info(std::istream& ifs, size_t n);
    VertexID          further_split(std::vector<VertexID>& neighbors, VertexID id, size_t pos, Side side, bool temp = false);


private:
    //for debug
    template <typename T>
    void            output_array(const std::vector<T>& array);

    struct splitInfo
    {
        MyMesh::VertexHandle        v;
        VertexID                   id;
        unsigned int           code_l;
        unsigned int           code_r;
        int               dx;
        int               dy;
        int               dz;
        double             x1;
        double             y1;
        double             z1;
    };

    bool          splitVs(splitInfo* split, bool temp = false);

    struct VsInfo
    {
        MyMesh::VertexHandle       v;
        BitString               data;
        VertexID                id_l;
        VertexID                id_r;
        unsigned int            code_remain_l;
        unsigned int            code_remain_r;
        size_t                  pminfo_index;
        std::vector<splitInfo*> waiting_list;
        //std::vector<BitString>  data_waiting_list;
        size_t                  pos_in_vertex_front;
        bool                    isPicked;
        bool                    isReceived;
        bool                    isVisible;
        bool                    isLeaf;
        VsInfo()
                :id_l(0), id_r(0), code_remain_l(1), code_remain_r(1), \
                isPicked(false), isReceived(false), isVisible(true), isLeaf(false)
        {
            ;
        }
        ~VsInfo()
        {
            for (size_t i = 0; i<waiting_list.size(); i++)
            {
                if (waiting_list[i]) delete waiting_list[i];
                waiting_list[i] = 0;
            }
        }
    };

    typedef std::map<VertexID, VsInfo>          Map;
    typedef Map::iterator                       MapIter;
    typedef Map::const_iterator                 MapConstIter;

    MyMesh            mesh_;
    PMInfoContainer   pminfos_;
    PMInfoIter        pmiter_;
    Map               map_;
    //=== PPMESH INFORMATIONS =========================================

    size_t            n_base_vertices_, n_base_faces_, n_detail_vertices_;
    size_t            n_max_vertices_;
    unsigned int      tree_bits_;
    unsigned int      levels_;
    unsigned int      minimum_depth_;
    unsigned int      quantize_bits_;
    unsigned int      level0_;
    unsigned int      level1_;
    double            x_min_;
    double            y_min_;
    double            z_min_;
    double            x_max_;
    double            y_max_;
    double            z_max_;


    Huffman::DecodeTree<unsigned int>   id_tree_;
    bool                                tree1Exist_;
    bool                                tree2Exist_;
    Huffman::DecodeTree<int>            geometry_tree1_;
    Huffman::DecodeTree<int>            geometry_tree2_;

    Huffman::HuffmanCoder<unsigned int>*  id_coder_;
    Huffman::HuffmanCoder<int>         *  geometry_coder1_;
    Huffman::HuffmanCoder<int>         *  geometry_coder2_;
    BitString                             not_existed_flag_;


    //the vertices in current mesh.
    std::vector<VertexID>               vertex_front_;

    //the vertices has high priority to be split.
    std::set<VertexID>                 to_be_split_;

    //the vertex splits in initial order.
    std::list<VertexID>                splits_;

    //for information only.
    size_t                              n_[32];
    size_t                              waiting_;


    Gfmesh*                             gfmesh_;
    bool                                needGfmesh_;
    unsigned int                        expected_level_;

    std::vector<VertexID>             history_;
    int                               wait_;
};



//=============================================================================
#endif // _PPMESH_HH defined
//=============================================================================

