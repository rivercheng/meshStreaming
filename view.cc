#include <GL/glut.h>
#include "view.hh"
#include "gfpmesh.hh"
const size_t SIZE = 1024*768*3;
unsigned char pixels_[SIZE] = {0};
void View::disp()
{
    Gfpmesh* gfmesh = ppmesh_->gfmesh();
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glShadeModel(GL_FLAT);
    glPushMatrix();
    glTranslated(dx_, dy_, dz_);
    glRotated(angle_x(), 1.0, 0.0, 0.0);
    glRotated(angle_y(), 0.0, 1.0, 0.0);
    glRotated(angle_z(), 0.0, 0.0, 1.0);
    glScaled(scale(), scale(), scale());
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_DOUBLE, 0, gfmesh->vertex_array());
    glBegin(GL_TRIANGLES);
    assert(gfmesh->face_number()*3 == gfmesh->face_array_size());
    for (size_t i = 0; i< gfmesh->face_number(); i++)
    {
        glNormal3d(gfmesh->face_normal_array()[3*i],gfmesh->face_normal_array()[3*i+1], gfmesh->face_normal_array()[3*i+2]);
        glArrayElement(gfmesh->face_array()[3*i]);
        glArrayElement(gfmesh->face_array()[3*i+1]);
        glArrayElement(gfmesh->face_array()[3*i+2]);
    }
    glEnd();
    glutSwapBuffers();
    glPopMatrix();
}

/*View::View(int width, int height):
    fstream_(0), ppmesh_(0), dx_(0), dy_(0), dz_(0), \
        angle_x_(0), angle_y_(0), angle_z_(0), scale_(0),\
    width_(width), height_(height), size_(width*height)
{
    ppmesh_ = new Ppmesh(true);
}*/

View::View(std::istream& fmesh, std::istream& config, std::istream& history, int width, int height)
        :fstream_(fmesh), ppmesh_(0), dx_(0), dy_(0), dz_(0), \
        angle_x_(0), angle_y_(0), angle_z_(0), scale_(0),\
        width_(width), height_(height), size_(width*height)
{
    ppmesh_ = new Ppmesh(true);
    assert(ppmesh_);
    ppmesh_->read(fmesh, true, true);
    config>>dx_>>dy_>>dz_>>angle_x_>>angle_y_>>angle_z_>>scale_;
    readHistory(history);
}

View::View(std::istream& fmesh, std::istream& is, int width, int height)
        :fstream_(fmesh),ppmesh_(0), dx_(0), dy_(0), dz_(0), \
        angle_x_(0), angle_y_(0), angle_z_(0), scale_(0), \
        width_(width), height_(height), size_(width*height)
{
    ppmesh_ = new Ppmesh(true);
    ppmesh_->read(fmesh, true, true);
    is>>dx_>>dy_>>dz_>>angle_x_>>angle_y_>>angle_z_>>scale_;
    readHistory(is);
}

View::~View()
{
    if (ppmesh_) delete ppmesh_;
    ppmesh_ = 0;
    if (final_image_) delete final_image_;
    final_image_ = 0;
    if (initial_image_) delete initial_image_;
    initial_image_ = 0;
}

void View::readHistory(std::istream& is)
{
    history_.clear();
    VertexID id;
    while (is >> id)
    {
        if (id != 0)
        {
            history_.push_back(id);
        }
    }
}

void View::restore()
{
    if (ppmesh_) delete ppmesh_;
    ppmesh_ = 0;
    ppmesh_ = new Ppmesh(true);
    assert(ppmesh_);
    fstream_.seekg(0, std::ios_base::beg);
    ppmesh_->read(fstream_, true, true);
    for (size_t i=0; i<history_.size(); i++)
    {
        ppmesh_->decodeId(history_[i]);
    }
}

void View::reverse()
{
    ppmesh_->reverse();
}

bool View::test(VertexID id)
{
    return ppmesh_->decodeId(id, true);
}


void View::initiateView()
{
    ppmesh_->decodeAll();
    final_image_ = generateImage();
    restore();
    initial_image_ = generateImage();
    Gfpmesh* gfmesh = ppmesh_->gfmesh();



    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glDisable(GL_LIGHTING);
    glDisable(GL_DITHER);
    unsigned char color_r = 0;
    unsigned char color_g = 0;
    unsigned char color_b = 0;
    for (size_t i=0; i<gfmesh->face_number(); i++)
    {
        if (gfmesh->is_visible(i))
        {
            glColor3ub(color_r, color_g, color_b);
            glBegin(GL_TRIANGLES);
            glArrayElement(gfmesh->face_array()[3*i]);
            glArrayElement(gfmesh->face_array()[3*i+1]);
            glArrayElement(gfmesh->face_array()[3*i+2]);
            glEnd();
        }
        if (color_b == 255)
        {
            color_b = 0;
            if (color_g == 255)
            {
                color_g = 0;
                if (color_r == 255)
                {
                    std::cerr<<"overflow!"<<std::endl;
                }
                else
                {
                    color_r ++;
                }
            }
            else
            {
                color_g ++;
            }
        }
        else
        {
            color_b++;
        }
    }
    glReadBuffer(GL_BACK);
    glReadPixels(0,0,width_, height_, GL_RGB, GL_UNSIGNED_BYTE, pixels_);
    //glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glEnable(GL_LIGHTING);
    glEnable(GL_DITHER);
    //std::cerr<<"pixels read"<<std::endl;

    gfmesh->clear_weight();
    for (int i = 0; i < size_; i+=3)
    {
        unsigned char color_r = pixels_[i];
        unsigned char color_g = pixels_[i+1];
        unsigned char color_b = pixels_[i+2];
        Index seq_no = color_r * 65536 + color_g*256 + color_b;
        if (seq_no != 0)
        {
            gfmesh->increment_face_weight(seq_no-1);
        }
    }
    for (size_t i = 0; i < gfmesh->face_number(); i++)
    {
        gfmesh->add_vertex_weight_in_face(i);
    }

    for (size_t i = 0; i < gfmesh->vertex_number(); i++)
    {
        screenArea_[ppmesh_->index2id(i)] = gfmesh->vertex_weight(i);
    }

}



void View::save(std::ostream& os)
{
    os<<dx_<<dy_<<dz_<<angle_x_<<angle_y_<<angle_z_<<scale_<<std::endl;
    ppmesh_->outputHistory(os);
}

void View::display()
{
    //VisiblePQ visible_pq(ppmesh_, ppmesh_->gfmesh());
    //Render render(0, 0, "view", ppmesh_, &visible_pq, 0, 1);
    //render.setView(dx_, dy_, dz_, angle_x_, angle_y_, angle_z_, scale_);
    //render.enterMainLoop();
}

void View::availableSplits(std::vector<VertexID>& split)
{

    split.clear();
    //VertexID id = 0;
    std::vector<VertexID>::const_iterator it(ppmesh_->vertex_front().begin());
    std::vector<VertexID>::const_iterator end(ppmesh_->vertex_front().end());
    for (; it != end; ++ it)
    {
        if (ppmesh_->idIsLeaf(*it)) continue;
        if (ppmesh_->gfmesh()->vertex_weight(ppmesh_->id2index(*it)) == 0)continue;
        split.push_back(*it);
    }
}

int View::screenArea(VertexID id)
{
    return screenArea_[id];
}

Image* View::finalImage()
{
    return final_image_;
}

Image* View::initialImage()
{
    return initial_image_;
}

Image* View::splitImage(VertexID id)
{
    if (test(id))
    {
        Image* image = generateImage();
        reverse();
        return image;
    }
    else
    {
        return 0;
    }
}

Image* View::generateImage()
{
    size_t size = size_*3;
    Image* image = new Image(size);
    disp();
    usleep(1000);
    glReadBuffer(GL_FRONT);
    glReadPixels(0, 0, width_, height_, GL_RGB, GL_UNSIGNED_BYTE, image->array_);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    return image;
}

double get_mse(unsigned char* f1, unsigned char* f2, size_t n)
{
    double sum_of_square = 0;
    size_t i = 0;
    int count = 0;
    for (i = 0; i<n ; i+=3)
    {
        double y1 = 0.299*f1[i] + 0.587*f1[i+1] + 0.144*f1[i+2];
        double y2 = 0.299*f2[i] + 0.587*f2[i+1] + 0.144*f2[i+2];
        double diff = y1 - y2;
        if (diff != 0)
        {
            count++;
            //printf("%d %e\n", count, diff);
        }
        sum_of_square += (diff*diff);
    }
    //printf("sum: %e\n", sum_of_square);
    //printf("total %ld, different in %d\n", n, count);
    return sum_of_square * 3 / n;
}

double get_psnr(double mse, double max)
{
    double rmse = sqrt(mse);
    return 20*log10(max / rmse);
}

double View::psnr(unsigned char* image1, unsigned char* image2, int size, int max)
{
    double mse = get_mse(image1, image2, size);
    return get_psnr(mse, max);
}

double View::psnr(Image* image1, Image* image2)
{
    return psnr(image1->array_, image2->array_, image1->size_, image1->max_);
}













