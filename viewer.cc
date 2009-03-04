#include "gfpmesh.hh"
#include "viewRender.hh"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>

int main(int argc, char*argv[])
{
    if (argc < 2)
    {
        std::cerr<<"Usage "<<argv[0]<<" <off file>"<<std::endl;
        exit(1);
    }
    std::ifstream ifs(argv[1]);
    if (!ifs)
    {
        std::cerr<<"Cannot open file "<<argv[1]<<std::endl;
        exit(1);
    }
    char buffer[255];
    std::string token;
    
    ifs.getline(buffer, 255);
    std::istringstream istr(buffer);
    istr>>token;
    if (token != "OFF")
    {
        std::cerr<<argv[1]<<" is not a valid OFF file."<<std::endl;
        exit(1);
    }

    ifs.getline(buffer,255);
    std::istringstream istr2(buffer);
    int vertex_number=0;
    int face_number=0; 
    int edge_number=0;
    
    istr2>>vertex_number>>face_number>>edge_number;
    std::cout<<"vertex number: "<<vertex_number<<" face_number: "<<face_number<<std::endl;

    Gfpmesh gfmesh;
    
    for (int i = 0; i < vertex_number; i++)
    {
        ifs.getline(buffer, 255);
        std::istringstream istr(buffer);
        double x=0;
        double y=0;
        double z=0;
        istr>>x>>y>>z;
        gfmesh.add_vertex(x, y, z);
    }
    
    for (int i = 0; i < face_number; i++)
    {
        ifs.getline(buffer, 255);
        std::istringstream istr(buffer);
        int edges=0;
        int a=0;
        int b=0;
        int c=0;
        istr>>edges>>a>>b>>c;
        gfmesh.add_face(a, b, c);
    }

    for (int i = 0; i < face_number; i++)
    {
        gfmesh.face_normal(i);
    }

    /*for (int i = 0; i < vertex_number; i++)
    {
        gfmesh.vertex_normal(i);
    }*/
    ViewRender render(argc, argv, argv[1],&gfmesh); 
    render.enterMainLoop();
    
}


   

    



    


