//=============================================================================
//Written by Cheng Wei
//National University of Singapore
//10 Dec 2007
//=============================================================================


#ifndef __GFMESH_HH__
#define __GFMESH_HH__


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
class Gfmesh
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
    Gfmesh(void);

    /**
     * construction method with ppmesh as parameter.
     */
    Gfmesh(Ppmesh* ppmesh);

    /**
     * the destruction method.
     */
    ~Gfmesh(void);

    /**
     * add a new vertex into the gfmesh.
     */
    void add_vertex     (Coordinate x, Coordinate y, Coordinate z)
    {
        vertex_array_.push_back(x);
        vertex_array_.push_back(y);
        vertex_array_.push_back(z);
        vertex_normal_array_.push_back(0);
        vertex_normal_array_.push_back(0);
        vertex_normal_array_.push_back(0);
        vertex_weight_array_.push_back(0);
    }

    /**
     * add a new face into the gfmesh.
     */
    void add_face       (Index a, Index b, Index c)
    {
        face_array_.push_back(a);
        face_array_.push_back(b);
        face_array_.push_back(c);
        face_normal_array_.push_back(0);
        face_normal_array_.push_back(0);
        face_normal_array_.push_back(0);
        face_weight_array_.push_back(0);
        face_visibility_array_.push_back(true);
    }

    
    /**
     * to split vertex v1 to v1 and (x0, y0, z0) with the cut neighbors vl and vr. 
     * This is the half-edge collapse, which is used currently.
     */
    void vertex_split   (Index v1, Index vl, Index vr, Coordinate x0, Coordinate y0, Coordinate z0);

    /**
     *to split vertex v1 to (x0, y0, z0) and (x1, y1, z1). Not used now.
     */
    //void vertex_split   (Index v1, Index vl, Index vr, Coordinate x0, Coordinate y0, Coordinate z0, Coordinate x1, Coordinate y1, Coordinate z1);

    /**
     * to collapse the newest vertex into vertex v1.
     * It is not a general collapse method since it can only collapse the newest edge.
     */
    void collapse       (Index v1);

    
    /**
     *Output gfmesh to ASCII file. 
     */
    void output         (std::ostream& os) const;

    /**
     * Return the address of the vertex array. We use STL vector, but OpenGL needs
     * a C array. With this function, we return the C array address of the
     * content of a vector.
     * Here, we assume the items in STL vector
     * is allocated together (it is often this case in current STL implementations).
     */
    INLINE const Coordinate* vertex_array   (void) const
    {
        return (Coordinate *)&vertex_array_[0];
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
    /**
     * Return the address of face array.
     */
    INLINE const Index*     face_array     (void)   const
    {
        return &face_array_[0];
    }

    /**
     * Return the address of vertex normal array.
     */
    INLINE const Normalf*    vertex_normal_array(void) const
    {
        return &vertex_normal_array_[0];
    }

    /**
     * return the address of face normal array.
     */
    INLINE const Normalf*    face_normal_array(void)   const
    {
        return &face_normal_array_[0];
    }

    /**
     * return the vertex number.
     */
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
     * return the size of vertex array.
     */
    INLINE size_t vertex_array_size(void) const
    {
        return vertex_array_.size();
    }

    /**
     * return the size of face array.
     */
    INLINE size_t face_array_size  (void) const
    {
        return face_array_.size();
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

    /**
     * set the color of a face with R, G, B value.
     */
    /*void set_color(int i, float value1, float value2, float value3)
    {
        color1_array_[i] = value1;
        color2_array_[i] = value2;
        color3_array_[i] = value3;
    }*/

    /**
     * reset all the face color to black.
     */
    /*void reset_color(void)
    {
        for (size_t i = 0; i<color1_array_.size(); i++)
        {
            color1_array_[i] = 1.;
            color2_array_[i] = 1.;
            color3_array_[i] = 1.;
        }
    }*/

    /**
     * to see if this gfmesh is updated.
     * It is used in rendering part to avoid refreshing
     * the same gfmesh.
     */
    INLINE bool   updated          (void) const
    {
        return updated_;
    }

    /**
     * reset the updated flag to false
     */
    void reset_updated  (void)
    {
        updated_ = false;
    };
    
    /**
     * the mutex used to synchronize.
     */
    Poco::Mutex         mutex_;
    //std::vector<unsigned int>  splits_;
    //std::vector<float>         color1_array_;
    //std::vector<float>         color2_array_;
    //std::vector<float>         color3_array_;
    //to calculate the normal of the given face
    void  face_normal   (Index face_index);
    //to calculate the normal of the given vertex
    void  vertex_normal (Index vertex_index);

    void clear_vertex_array(void)
    {
        for (size_t i=0; i<vertex_array_.size(); i++)
        {
            vertex_array_[i] = 0;
        }
    }
private:
    void refine         (size_t n);
    void coarsen        (size_t n);

private:
    std::vector<Coordinate>    vertex_array_;
    std::vector<Index>         face_array_;
    std::vector<Normalf>       vertex_normal_array_;
    std::vector<Normalf>       face_normal_array_;
    std::vector<unsigned int>  vertex_weight_array_;
    std::vector<unsigned int>  face_weight_array_;
    std::vector<bool>          face_visibility_array_;
    bool                updated_;
    Ppmesh*             ppmesh_;
};
#endif
