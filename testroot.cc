#include "ppmesh.hh"
int main(void)
{
    Ppmesh m("huge.ppm");
    VertexID id[5] = {1024, 1600, 25600, 16000, 256000};
    for (size_t i = 0; i < 5; i++)
    {
        std::cout<<id[i]<<" "<<m.rootId(id[i])<<std::endl;
    }
}
