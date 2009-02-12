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
    input_filename.append(".pm");
    std::ifstream fin(input_filename.c_str(), std::ios::binary);
    if (!fin)
    {
        std::cerr<<" Cannot open "<<input_filename<<std::endl;
        exit(1);
    }
    std::string output_filename(argv[1]);
    output_filename.append(".ppm");
    std::ofstream fout(output_filename.c_str(), std::ios::binary);
    Ppmesh ppmesh;
    ppmesh.readPM(fin);
    ppmesh.write(fout);
    std::cout<<"average connectivity length: "<<ppmesh.avg_connectivity_len_<<" average geometry length: "<<ppmesh.avg_geometry_len_<<std::endl;
    std::cout<<"average connectivity length of first 3000: "<<ppmesh.avg_connectivity_len3000_<<" average geometry length of first 3000: "<<ppmesh.avg_geometry_len3000_<<std::endl;
    std::cout<<"average connectivity length of first 10000: "<<ppmesh.avg_connectivity_len10000_<<" average geometry length of first 10000: "<<ppmesh.avg_geometry_len10000_<<std::endl;
    std::cout<<"average connectivity length of first 50000: "<<ppmesh.avg_connectivity_len50000_<<" average geometry length of first 50000: "<<ppmesh.avg_geometry_len50000_<<std::endl;
    std::cout<<"average connectivity length of first 100000: "<<ppmesh.avg_connectivity_len100000_<<" average geometry length of first 100000: "<<ppmesh.avg_geometry_len100000_<<std::endl;
    fin.close();
    fout.close();
}

