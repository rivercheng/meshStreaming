#include <string>
#include <iostream>
#include "packetid.hh"

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cerr<<"Usage: "<<argv[0]<<" <input>"<<std::endl;
        exit(1);
    }

    std::ifstream ifs(argv[1], std::ios::binary);
    if (!ifs)
    {
        std::cerr<<"Cannot open "<<argv[1]<<std::endl;
        exit(1);
    }

    unsigned int size;
    ifs.read((char*)&size, sizeof(size));
    std::cout<<size<<" packets"<<std::endl;

    for (size_t i = 0; i<size; i++)
    {
        PacketID p_id = read_id_binary(ifs);
        BitString bs;
        bs.read_binary(ifs);
        std::cout<<p_id<<" "<<bs.size()/8 + sizeof(p_id)<<std::endl;
    }

    return 0;
}

