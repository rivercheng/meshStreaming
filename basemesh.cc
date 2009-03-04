#include "basemesh.hh"
#include <cmath>


Basemesh::Basemesh()
{
    ;
}

Basemesh::Basemesh(Ppmesh* ppmesh)
{
    assert(ppmesh);
    
    ppmesh->output_arrays(vertex_array_, face_array_);
    for (size_t i = 0; i< face_number(); i++)
    {
        //face_normal_array_.push_back(0);
        //face_normal_array_.push_back(0);
        //face_normal_array_.push_back(0);
        //face_normal(i);
        face_weight_array_.push_back(0);
        face_visibility_array_.push_back(true);
    }
    for (size_t i = 0; i< vertex_number(); i++)
    {
        vertex_weight_array_.push_back(0);
        vertex_round_.push_back(0);
    }
}

Gfmesh::~Gfmesh()
{
    ;
}

/*void Gfmesh::face_normal(Index face_index)
{
    if (face_index >= static_cast<Index>(face_array_.size() / 3))
    {
        throw InvalidFaceIndex();
    }
    Index v1 = face_array_[3*face_index];
    Index v2 = face_array_[3*face_index+1];
    Index v3 = face_array_[3*face_index+2];

    Coordinate x1 = vertex_array_[3*v1];
    Coordinate x2 = vertex_array_[3*v2];
    Coordinate x3 = vertex_array_[3*v3];

    Coordinate y1 = vertex_array_[3*v1+1];
    Coordinate y2 = vertex_array_[3*v2+1];
    Coordinate y3 = vertex_array_[3*v3+1];

    Coordinate z1 = vertex_array_[3*v1+2];
    Coordinate z2 = vertex_array_[3*v2+2];
    Coordinate z3 = vertex_array_[3*v3+2];

    // vector (v2 - v1) = a1*i + a2*j + a3*k
    double a1 = static_cast<double>(x2) - static_cast<double>(x1);
    double a2 = static_cast<double>(y2) - static_cast<double>(y1);
    double a3 = static_cast<double>(z2) - static_cast<double>(z1);

    // vector (v3 - v1) = b1*i + b2*j + b3*k
    double b1 = static_cast<double>(x3) - static_cast<double>(x1);
    double b2 = static_cast<double>(y3) - static_cast<double>(y1);
    double b3 = static_cast<double>(z3) - static_cast<double>(z1);

    //normal = (v2-v1) x (v3-v1) = (a2*b3 - a3*b2)i
    //                             (a3*b1 - a1*b3)j
    //                             (a1*b2 - a2*b1)k
    Normalf v_x = static_cast<Normalf>(a2*b3 - a3*b2);
    Normalf v_y = static_cast<Normalf>(a3*b1 - a1*b3);
    Normalf v_z = static_cast<Normalf>(a1*b2 - a2*b1);


    face_normal_array_[3*face_index] = v_x;
    face_normal_array_[3*face_index+1] = v_y;
    face_normal_array_[3*face_index+2] = v_z;
    return;
}*/

void generateNeighbors(int rounds)
{
    for (size_t i=0; i<face_number(); i++)
    {
        Index v1 = vertex1_in_face(i);
        Index v2 = vertex2_in_face(i);
        Index v3 = vertex3_in_face(i);
        vertex_neighbors_[v1].insert(v2);
        vertex_neighbors_[v1].insert(v3);
        vertex_neighbors_[v2].insert(v1);
        vertex_neighbors_[v2].insert(v3);
        vertex_neighbors_[v3].insert(v1);
        vertex_neighbors_[v3].insert(v2);
    }
    for (int round = 1; round < rounds; round++)
    {
        for (size_t i = 0; i < vertex_number; i++)
        {
            std::vector<Index> neighbors;
            std::set<Index>::iterator it = vertex_neighbors_[i].begin();
            std::set<Index>::iterator end = vertex_neighbors_[i].end();
            reset_visited();
            vertex_visited_[i] = true;
            for (; it != end; ++it)
            {
                std::set<Index>::iterator it2 = vertex_neighbors_[*it].begin();
                std::set<Index>::iterator end2 = vertex_neighbors_[*it].end();
                for (; it2 != end2; ++it2)
                {
                    if (!vertex_visited_[*it2])
                    {
                        neighbors.push_back(*it2);
                        vertex_visited_[*it2] = true;
                    }
                }
            }
            std::vector<Index>::iterator it3 = neighbors.begin();
            std::vector<Index>::iterator end3 = neighbors.end();
            for (; it3 != end3; ++it3)
            {
                vertex_neighbors_[i].insert(*it3);
            }
        }
    }
}


