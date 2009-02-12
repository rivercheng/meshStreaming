#include "ppmesh.hh"
//#include "huffman.hh"
#include <iostream>
#include <fstream>
#include "logger.hh"
#include "prender.hh"

int main(int argc, char** argv)
{
    char* config = NULL;
    if (argc < 2 || argc > 4)
    {
        std::cerr<<"Usage "<<argv[0]<<" <pm file> [vertex splits to apply] [view_config file]"<<std::endl;
        exit(1);
    }
    Ppmesh *p_ppmesh = new Ppmesh(true);
    Ppmesh& ppmesh = *p_ppmesh;
    std::ifstream fin(argv[1], std::ios::binary);
    ppmesh.readPM(fin);
    if (argc == 2 || atoi(argv[2]) < 0)
    {
        ppmesh.refine(ppmesh.n_max_vertices());
    }
    else
    {
        ppmesh.refine(ppmesh.n_base_vertices()+atoi(argv[2]));
    }
    Logger logger;
    PRender render(argc, argv, "happy", &ppmesh, 0, 15, logger);
    if (argc == 4)
    {
        config = argv[3];
        std::ifstream ifs(config);
        if (ifs)
        {
            double dx, dy, dz, angle_x, angle_y, angle_z, scale;
            ifs>>dx>>dy>>dz>>angle_x>>angle_y>>angle_z>>scale;
            render.setView(dx, dy, dz, angle_x, angle_y, angle_z, scale);
        }
    }
    render.enterMainLoop();
    delete p_ppmesh;
    return 0;
}







