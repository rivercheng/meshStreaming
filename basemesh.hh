//=============================================================================
//Written by Cheng Wei
//National University of Singapore
//10 Dec 2007
//=============================================================================


#ifndef __BASEMESH_HH__
#define __BASEMESH_HH__


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
#include <cstring>
#include <GL/glut.h>
#include <Poco/Thread.h>
#include <Poco/ThreadPool.h>

#include "bitstring.hh"
#include "huffman.hh"
#include "vertexid.hh"
#include "commonType.hh"

#define INLINE inline
class Ppmesh;

//exceptions
class InvalidFaceIndex{};

/**
 * The class to provide GPU friendly representation of a mesh.
 * It includes vertex array, face array, vertex normal array, 
 * and face normal array. To fasilitate the estimation of importance,
 * it also includes vertex weight array and face weight array.
 * There are other arrays: face visibility array and color arrays.
 * They are used in the demo.
 */
class Basemesh
{
public:
    const static int RESERVE_SIZE = 10000000;//assume at least space for RESERVER_SIZE vertices are allocated.
    //const static int RESERVE_SIZE = 10;//assume at least space for RESERVER_SIZE vertices are allocated.
    const static int MAX_VERTEX_FACE = 10000;
    const static int MAX_FACE_VERTEX = 10;
    const static int MAX_VERTEX_VERTEX = 10000;
    /**
     * default construction method.
     */
    Basemesh(void);

    /**
     * construction method with ppmesh as parameter.
     */
    Basemesh(Ppmesh* ppmesh);

    /**
     * the destruction method.
     */
    ~Basemesh(void);

    /**
     * add a new vertex into the gfmesh.
     */
    void add_vertex     (Coordinate x, Coordinate y, Coordinate z)
    {
        vertex_array_.push_back(x);
        vertex_array_.push_back(y);
        vertex_array_.push_back(z);
        vertex_weight_array_.push_back(0);
        vertex_visited_.push_back(false);
    }

    /**
     * add a new face into the gfmesh.
     */
    void add_face  (Index a, Index b, Index c)
    {
        face_array_.push_back(a);
        face_array_.push_back(b);
        face_array_.push_back(c);
        //face_normal_array_.push_back(0);
        //face_normal_array_.push_back(0);
        //face_normal_array_.push_back(0);
        face_weight_array_.push_back(0);
        face_visibility_array_.push_back(true);
    }

    
    INLINE const std::vector<Coordinate>& vertex_array(void) const
    {
        return vertex_array_;
    }

    INLINE Index vertex1_in_face(Index faceIndex) const
    {
        assert(faceIndex < face_array_.size());
        return face_array_[faceIndex*3];
    }

    INLINE Index vertex2_in_face(Index faceIndex) const
    {
        assert(faceIndex < face_array_.size());
        return face_array_[faceIndex*3+1];
    }


    INLINE Index vertex3_in_face(Index faceIndex) const
    {
        assert(faceIndex < face_array_.size());
        return face_array_[faceIndex*3+2];
    }
    
    INLINE const std::vector<Index>&  face_array     (void)   const
    {
        return face_array_;
    }

    INLINE size_t vertex_number    (void) const
    {
        return vertex_array_.size()/3;
    }

    /**
     * return the face number.
     */
    INLINE size_t face_number      (void) const
    {
        return face_array_.size()/3;
    }

    /**
     * return the face weight.
     */
    INLINE unsigned int face_weight (Index index) const
    {
        return face_weight_array_[index];
    }

    /**
     * return the vertex weight.
     */
    INLINE unsigned int vertex_weight(Index index) const
    {
        return vertex_weight_array_[index];
    }

    /**
     * set the face weight.
     */
    INLINE void   set_face_weight  (Index index, unsigned int weight)
    {
        face_weight_array_[index] = weight;
    }

    /**
     * add weight to a given face.
     */
    INLINE void   add_face_weight (Index index, unsigned int weight)
    {
        face_weight_array_[index] += weight;
    }

    /**
     * add face weight by one.
     */
    INLINE void increment_face_weight(Index index)
    {
        face_weight_array_[index] ++;
    }


    /**
     * set vertex weight.
     */
    INLINE void   set_vertex_weight (Index index, unsigned int weight)
    {
        vertex_weight_array_[index] = weight;
    }

    /**
     * add vertex weight to a vertex.
     */
    INLINE void   add_vertex_weight (Index index, unsigned int weight)
    {
        vertex_weight_array_[index] += weight;
    }

    /**
     * add the face weight to all its three vertices.
     */
    INLINE void   add_vertex_weight_in_face(Index f_index)
    {
        Index v1 = face_array_[3*f_index];
        Index v2 = face_array_[3*f_index+1];
        Index v3 = face_array_[3*f_index+2];
        unsigned int weight = face_weight_array_[f_index];
        add_vertex_weight(v1, weight);
        add_vertex_weight(v2, weight);
        add_vertex_weight(v3, weight);
    }


    /**
     * clear all the face weight and vertex weight.
     */
    INLINE void clear_weight(void)
    {
        memset(&face_weight_array_[0], 0, face_weight_array_.size()*sizeof(unsigned int));
        memset(&vertex_weight_array_[0], 0, vertex_weight_array_.size()*sizeof(unsigned int));
        
        /*for (size_t i = 0; i< face_weight_array_.size(); i++)
        {
            face_weight_array_[i] = 0;
        }
        for (size_t i = 0; i<vertex_weight_array_.size(); i++)
        {
            vertex_weight_array_[i] = 0;
        }*/
    }

    /**
     * check if a face is visible.
     */
    bool is_visible(Index f_index) const
    {
        return face_visibility_array_[f_index];
    }

    /**
     * set visibility of a face to the given value
     */
    void set_visibility(Index f_index, bool value)
    {
        face_visibility_array_[f_index] = value;
    }

    /**
     * reset all the face to invisible.
     */
    void reset_visibility(bool value)
    {
        for (size_t i = 0; i<face_visibility_array_.size(); i++)
        {
            face_visibility_array_[i] = value;
        }
    }

    void reset_visited(void)
    {
        for (size_t i=0; i<vertex_visited_.size(); i++)
        {
            vertex_visited_[i] = false;
        }
    }

    void  face_normal   (Index face_index);
private:
    void generateNeighbors (int rounds);
private:
    std::vector<Coordinate>    vertex_array_;
    std::vector< std::set<Index> > vertex_neighbors_;
    std::vector<Index>         face_array_;
    std::vector<bool>           vertex_visited_;
    //std::vector<Normalf>       face_normal_array_;
    std::vector<unsigned int>  vertex_weight_array_;
    std::vector<unsigned int>  face_weight_array_;
    std::vector<bool>          face_visibility_array_;
};
#endif
