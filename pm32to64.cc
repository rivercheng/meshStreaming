#include <cstdlib>
#include <iostream>
#include <fstream>
//#include "ppmesh.hh"
#include <OpenMesh/Core/IO/BinaryHelper.hh>
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Utils/Endian.hh>
int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        std::cerr<<"Usage "<<argv[0]<<" <input file> <output file>"<<std::endl;
        exit(1);
    }
    char c[10];

    std::ifstream ifs(argv[1], std::ios::binary);
    std::ofstream ofs(argv[2], std::ios::binary);
    bool swap = OpenMesh::Endian::local() != OpenMesh::Endian::LSB;
    ifs.read(c,8);
    c[8] = '\0';
    if (std::string(c) != std::string("ProgMesh"))
    {
        std::cerr<<"Wrong Format"<<std::endl;
    }
    ofs.write(c,8);
    size_t n_base_vertices_;
    size_t n_base_faces_;
    size_t n_detail_vertices_;
    
    OpenMesh::IO::binary<size_t>::restore(ifs, n_base_vertices_, swap);
    OpenMesh::IO::binary<size_t>::restore( ifs, n_base_faces_,      swap );
    OpenMesh::IO::binary<size_t>::restore( ifs, n_detail_vertices_, swap );
    
    size_t ignore = 0;
    OpenMesh::IO::binary<size_t>::store( ofs, n_base_vertices_,   swap );
    OpenMesh::IO::binary<size_t>::store(ofs, ignore, swap);
    OpenMesh::IO::binary<size_t>::store( ofs, n_base_faces_,      swap );
    OpenMesh::IO::binary<size_t>::store(ofs, ignore, swap);
    OpenMesh::IO::binary<size_t>::store( ofs, n_detail_vertices_, swap );
    OpenMesh::IO::binary<size_t>::store(ofs, ignore, swap);

    char buffer[4096];
    while (!ifs.eof())
    {
       ifs.read(buffer, 4096);
       ofs.write(buffer, 4096);
    }
}




    
