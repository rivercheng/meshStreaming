#ifndef __VIEW_H__
#define __VIEW_H__
#include "ppmesh.hh"
#include "vertexid.hh"
struct Image
{
    size_t size_;
    unsigned char* array_;
    int max_;
    Image(size_t size, int max = 255)
    {
        size_  = size;
        array_ = new unsigned char[size];
        max_   = max;
    }
    ~Image()
    {
        delete[] array_;
        array_ = 0;
    }
};

class View
{
    std::istream& fstream_;
    Ppmesh* ppmesh_;
    double  dx_;
    double  dy_;
    double  dz_;
    double  angle_x_;
    double  angle_y_;
    double  angle_z_;
    double  scale_;
    int     width_;
    int     height_;
    int     size_;
    std::map<VertexID, int>  screenArea_;
    std::vector<VertexID> history_;
    Image* final_image_;
    Image* initial_image_;
public:
    //View(int width, int height);
    View(std::istream& ppmesh, std::istream& config, std::istream& history, int width, int height);
    View(std::istream& ppmesh, std::istream& is, int width, int height);
    ~View(void);
    void save(std::ostream& os);
    void display(void);
    void disp(void);
    void reverse(void);
    bool test(VertexID id);
    void initiateView(void);
    void restore(void);
    void availableSplits(std::vector<VertexID>& split);
    int screenArea(VertexID id);
    Image* finalImage(void);
    Image* initialImage(void);
    Image* splitImage(VertexID id);
    Image* generateImage();
    void   readHistory(std::istream& is);
    static double psnr(unsigned char* image1, unsigned char* image2, int size, int max);
    static double psnr (Image* image1, Image* image2);
    inline double dx() const
    {
        return dx_;
    }
    inline double dy() const
    {
        return dy_;
    }
    inline double dz() const
    {
        return dz_;
    }
    inline double angle_x() const
    {
        return angle_x_;
    }
    inline double angle_y() const
    {
        return angle_y_;
    }
    inline double angle_z() const
    {
        return angle_z_;
    }
    inline double scale()  const
    {
        return scale_;
    }
    inline Ppmesh* ppmesh() const
    {
        return ppmesh_;
    }
};
#endif
