#include <string>
#include <set>
#include <vector>
#include <iostream>
#include <fstream>
#include "ppmesh.hh"
#include "vertexid.hh"
#include "bitstring.hh"
#include "packetid.hh"

int main(int argc, char** argv)
{
    if (argc < 2 || argc > 3)
    {
        std::cout<<"Usage: "<<argv[0]<<" <prefix>"<<std::endl;
        exit(1);
    }

    unsigned int level = 4;
    if (argc == 3)
    {
        level = (unsigned int)atoi(argv[2]);
    }


    std::string     inputFile(argv[1]);
    inputFile.append(".pm");
    std::ifstream   ifs(inputFile.c_str(), std::ios::binary);
    Ppmesh          ppmesh(14);
    ppmesh.readPM(ifs);
    std::cout<<inputFile<<" is read."<<std::endl;


    std::string     baseFile(argv[1]);
    std::string     packetFile(argv[1]);
    baseFile.append(".base");
    packetFile.append(".packet");
    std::ofstream   basefs(baseFile.c_str(),std::ios::binary);
    std::ofstream   packetfs(packetFile.c_str(),std::ios::binary);

    //write the base mesh + information
    //ppmesh.write(basefs, false);
    //ppmesh.write(packetfs);
    ppmesh.write_to_level(basefs, 5);


    //begin the packetization

    //generate the packet ID set
    std::vector<VertexID> v_id_array;
    std::set<PacketID> p_id_set;
    ppmesh.output_vertex_splits(v_id_array);
    for (size_t i=0; i<v_id_array.size(); i++)
    {
        p_id_set.insert(v_id_to_p_id(v_id_array[i], ppmesh));
    }
    v_id_array.clear();

    unsigned int size = p_id_set.size();
    packetfs.write((const char*)&size, sizeof(size));
    std::cerr<<"size written."<<std::endl;

    std::set<PacketID>::const_iterator end = p_id_set.end();
    std::set<PacketID>::const_iterator iter= p_id_set.begin();
    for (; iter != end; ++iter)
    {
        std::vector<VertexID> v_array;
        BitString packet_data;
        PacketID p_id = *iter;
        std::cerr<<to_string(p_id)<<std::endl;
        write_id_binary(packetfs, p_id);
        packetID_to_vertexID_array(p_id, v_array, 4);
        std::cerr<<v_array.size()<<std::endl;
        for (size_t i = 0; i<v_array.size(); i++)
        {
            packet_data.push_back(ppmesh.get_data(v_array[i]));
        }
        packet_data.write_binary(packetfs, true);
        v_array.clear();
    }
}



