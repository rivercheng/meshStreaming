#include <iostream>
#include <fstream>
#include "ppmesh.hh"

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cerr<<" Usage: "<<argv[0]<<" <file name prefix>"<<std::endl;
        exit(1);
    }
    std::string input_filename(argv[1]);
    input_filename.append(".ppm");
    std::ifstream fin(input_filename.c_str(), std::ios::binary);
    if (!fin)
    {
        std::cerr<<" Cannot open "<<input_filename<<std::endl;
        exit(1);
    }
    std::string output_filename(argv[1]);
    output_filename.append(".pm");
    std::ofstream fout(output_filename.c_str(), std::ios::binary);
    Ppmesh ppmesh;
    ppmesh.read(fin);
    std::cout<<"ppmesh readed"<<std::endl;
    ppmesh.decodeAll();
    std::cout<<"ppmesh decoded"<<std::endl;
    ppmesh.writePM(fout, true);
    fin.close();
    fout.close();
}



