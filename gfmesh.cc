#include "ppmesh.hh"
#include "gfmesh.hh"
#include <cmath>


Gfmesh::Gfmesh()
        :updated_(false), ppmesh_(0)
{
    ;
}

Gfmesh::Gfmesh(Ppmesh* ppmesh)
        :updated_(false),ppmesh_(ppmesh)
{
    assert(ppmesh_);
    ppmesh_->output_arrays(vertex_array_, face_array_);
    for (size_t i = 0; i< face_number(); i++)
    {
        face_normal_array_.push_back(0);
        face_normal_array_.push_back(0);
        face_normal_array_.push_back(0);
        face_normal(i);
        face_weight_array_.push_back(0);
        face_visibility_array_.push_back(true);
    }
    for (size_t i = 0; i< vertex_number(); i++)
    {
        vertex_normal_array_.push_back(0);
        vertex_normal_array_.push_back(0);
        vertex_normal_array_.push_back(0);
        vertex_normal(i);
        vertex_weight_array_.push_back(0);
        //color1_array_.push_back(1.);
        //color2_array_.push_back(1.);
        //color3_array_.push_back(1.);
    }
    updated_ = true;
}

Gfmesh::~Gfmesh()
{
    ;
}

void Gfmesh::face_normal(Index face_index)
{
    if (face_index >= static_cast<Index>(face_array_.size() / 3))
    {
        //std::cerr<<face_index<<">="<<face_array_.size()/3<<std::endl;
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
}

void Gfmesh::vertex_normal(Index vertex_index)
{
    std::vector<Index> faces;
    ppmesh_->vertex_faces(vertex_index, faces);
    assert(faces.size() > 0);
    double v_x = 0;
    double v_y = 0;
    double v_z = 0;
    //vertex normal is the sum of face normal without normalization of neighbor faces.
    for (size_t i = 0; i < faces.size(); i++)
    {
        v_x += (static_cast<double>(face_normal_array_[3*faces[i]]));
        v_y += (static_cast<double>(face_normal_array_[3*faces[i]+1]));
        v_z += (static_cast<double>(face_normal_array_[3*faces[i]+2]));
    }

    vertex_normal_array_[3*vertex_index]   = static_cast<Normalf>(v_x);
    vertex_normal_array_[3*vertex_index+1] = static_cast<Normalf>(v_y);
    vertex_normal_array_[3*vertex_index+2] = static_cast<Normalf>(v_z);
    return;
}

void Gfmesh::collapse(Index v1)
{
    Index v0 = vertex_array_.size() / 3 - 1;
    //remove the vertex v0
    vertex_array_.resize(vertex_array_.size() - 3);
    vertex_normal_array_.resize(vertex_array_.size() - 3);
    vertex_weight_array_.resize(vertex_weight_array_.size() -1);
    //color1_array_.resize(color1_array_.size()-1);
    //color2_array_.resize(color2_array_.size()-1);
    //color3_array_.resize(color3_array_.size()-1);

    //remove the faces including v0
    bool find = false;
    while (1)
    {
        size_t len = face_array_.size();
        for (size_t i = 0; i< 3; i++)
        {
            if (face_array_[len - i] == v0)
            {
                find = true;
                break;
            }
        }
        if (find)
        {
            face_array_.resize(len - 3);
            face_normal_array_.resize(len -3);
            face_visibility_array_.resize(face_visibility_array_.size() -1);
            face_weight_array_.resize(face_weight_array_.size() -1);
        }
        else
        {
            break;
        }
    }

    //update the normal of faces and vertices
    std::vector<Index> faces;
    ppmesh_->vertex_faces(v1, faces);
    for (size_t i = 0; i< faces.size(); i++)
    {
        Index face_index = faces[i];
        //need to update vertices here.
        for (size_t j=0; j< 3; j++)
        {
            if (face_array_[face_index*3 + j] == v0)
            {
                face_array_[face_index*3 + j] = v1;
            }
        }
        face_normal(face_index);
    }

    std::vector<Index> vertices;
    ppmesh_->vertex_vertices(v1, vertices);
    for (size_t i = 0; i< vertices.size(); i++)
    {
        vertex_normal(vertices[i]);
    }

    //std::cerr<<"vertex normal updated."<<std::endl;
    /*
    splits_.push_back(v0);
    splits_.push_back(v1);
    splits_.push_back(vl);
    splits_.push_back(vr);
    splits_.push_back(ppmesh_->index2id(v0));
    splits_.push_back(ppmesh_->index2id(v1));
    splits_.push_back(ppmesh_->index2id(vl));
    splits_.push_back(ppmesh_->index2id(vr));
    */
    updated_ = true;
}

void Gfmesh::vertex_split(Index v1, Index vl, Index vr, Coordinate x0, Coordinate y0, Coordinate z0)
{
    Index v0 = vertex_array_.size() / 3;
    vertex_array_.push_back(x0);
    vertex_array_.push_back(y0);
    vertex_array_.push_back(z0);
    vertex_normal_array_.push_back(0);
    vertex_normal_array_.push_back(0);
    vertex_normal_array_.push_back(0);
    vertex_weight_array_.push_back(0);
    //color1_array_.push_back(1);
    //color2_array_.push_back(1);
    //color3_array_.push_back(1);

    std::vector<Index> faces;
    ppmesh_->vertex_faces(v0, faces);
    size_t old_size = face_array_.size();
    for (size_t i = 0; i< faces.size(); i++)
    {
        Index face_index = faces[i];
        std::vector<Index> vertex_array;
        //set all the neighbors visibile since they are changed.
        if (face_index < face_visibility_array_.size())
        {
            face_visibility_array_[face_index] = true;
        }
        ppmesh_->face_vertices(face_index, vertex_array);
        if (face_index*3 == static_cast<Index>(old_size))
        {
            if (old_size == face_array_.size())
            {
                face_array_.push_back(vertex_array[0]);
                face_array_.push_back(vertex_array[1]);
                face_array_.push_back(vertex_array[2]);
                face_normal_array_.push_back(0);
                face_normal_array_.push_back(0);
                face_normal_array_.push_back(0);
                face_weight_array_.push_back(0);
                face_visibility_array_.push_back(true);
            }
            else
            {
                assert(face_array_.size() == old_size + 3);
                face_array_[face_index*3]   = vertex_array[0];
                face_array_[face_index*3+1] = vertex_array[1];
                face_array_[face_index*3+2] = vertex_array[2];
            }
        }
        else if (face_index*3 == static_cast<Index>(old_size + 3))
        {
            if (old_size == face_array_.size())
            {
                face_array_.push_back(0);
                face_array_.push_back(0);
                face_array_.push_back(0);
                face_normal_array_.push_back(0);
                face_normal_array_.push_back(0);
                face_normal_array_.push_back(0);
                face_weight_array_.push_back(0);
                face_visibility_array_.push_back(true);
            }
            else
            {
                assert(face_array_.size() == old_size+3);
            }
            face_array_.push_back(vertex_array[0]);
            face_array_.push_back(vertex_array[1]);
            face_array_.push_back(vertex_array[2]);
            face_normal_array_.push_back(0);
            face_normal_array_.push_back(0);
            face_normal_array_.push_back(0);
            face_weight_array_.push_back(0);
            face_visibility_array_.push_back(true);
        }
        else
        {
            face_array_[face_index*3]   = vertex_array[0];
            face_array_[face_index*3+1] = vertex_array[1];
            face_array_[face_index*3+2] = vertex_array[2];
        }
        face_normal(face_index);
    }

    vertex_normal(v0);
    std::vector<Index> vertices;
    ppmesh_->vertex_vertices(v0, vertices);

    //set the v1's neighbors are visible. since they may be covered before and now are visible.
    std::vector<Index> faces2;
    ppmesh_->vertex_faces(v1, faces2);
    for (size_t i = 0; i< faces2.size(); i++)
    {
        Index face_index = faces2[i];
        face_visibility_array_[face_index] = true;
    }
    for (size_t i = 0; i< vertices.size(); i++)
    {
        vertex_normal(vertices[i]);
    }
    /*
    splits_.push_back(v0);
    splits_.push_back(v1);
    splits_.push_back(vl);
    splits_.push_back(vr);
    splits_.push_back(ppmesh_->index2id(v0));
    splits_.push_back(ppmesh_->index2id(v1));
    splits_.push_back(ppmesh_->index2id(vl));
    splits_.push_back(ppmesh_->index2id(vr));
    */
    updated_ = true;
}

void Gfmesh::output(std::ostream& os) const
{
    os<<"GFMESH"<<" "<<vertex_number()<<" "<<face_number()<<std::endl;
    size_t vn = vertex_number();
    size_t fn = face_number();
    for (size_t i = 0; i< vn; i++)
    {
        os<<vertex_array()[3*i]<<" "<<vertex_array_[3*i+1]<<" "<<vertex_array_[3*i+2]<<" "<<vertex_normal_array_[3*i]<<" "<<vertex_normal_array_[3*i+1]<<" "<<vertex_normal_array_[3*i+2]<<std::endl;
    }

    for (size_t i = 0; i< fn; i++)
    {
        os<<face_array()[3*i]<<" "<<face_array_[3*i+1]<<" "<<face_array_[3*i+2]<<" "<<face_normal_array_[3*i]<<" "<<face_normal_array_[3*i+1]<<" "<<face_normal_array_[3*i+2]<<std::endl;
    }
}

