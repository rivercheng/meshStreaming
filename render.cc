#include <GL/glut.h>
#include <Poco/Mutex.h>

#include "render.hh"
#include "ppmesh.hh"
#include "visiblepq.hh"
#include <ctime>

const size_t FRAME_BUFFER_SIZE = 1024*768*3;
static Render* render_ = NULL;
static unsigned char pixels_[FRAME_BUFFER_SIZE] = {0};
static int mainWindow_;
static int recheck_interval_ = 1;
static int counter_ = 0;



void draw_surface_with_arrays()
{
    static int output_counter = 1;
    Gfmesh* gfmesh_ = render_->ppmesh_->gfmesh();
    Poco::ScopedLock<Poco::Mutex> lock(gfmesh_->mutex_);

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_DOUBLE, 0, gfmesh_->vertex_array());
    if (render_->check_visibility_ && render_->visible_pq_ != NULL)
    {
        glDisable(GL_LIGHTING);
        glDisable(GL_DITHER);
        unsigned char color_r = 0;
        unsigned char color_g = 0;
        unsigned char color_b = 1;
        for (size_t i=0; i<gfmesh_->face_number(); i++)
        {
            //std::cerr<<(int)color_r<<" "<<(int)color_g<<" "<<(int)color_b<<std::endl;
            if (gfmesh_->is_visible(i))
            {
                glColor3ub(color_r, color_g, color_b);
                glBegin(GL_TRIANGLES);
                glArrayElement(gfmesh_->face_array()[3*i]);
                glArrayElement(gfmesh_->face_array()[3*i+1]);
                glArrayElement(gfmesh_->face_array()[3*i+2]);
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
        glReadPixels(0,0,render_->width_, render_->height_, GL_RGB, GL_UNSIGNED_BYTE, pixels_);
        render_->visible_pq_->update(pixels_, render_->width_*render_->height_*3);
        glEnable(GL_LIGHTING);
        glEnable(GL_DITHER);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        render_->setCheck(false);
    }
    if (render_->smooth_ || render_->interpolated_)
    {
        glEnableClientState(GL_NORMAL_ARRAY);
        glNormalPointer(GL_DOUBLE, 0, gfmesh_->vertex_normal_array());
        glDrawElements(GL_TRIANGLES, 3*gfmesh_->face_number(),GL_UNSIGNED_INT, gfmesh_->face_array());
    }
    else
    {
        glDisableClientState(GL_NORMAL_ARRAY);
        glBegin(GL_TRIANGLES);
        assert(gfmesh_->face_number()*3 == gfmesh_->face_array_size());
        for (size_t i=0; i< gfmesh_->face_number(); i++)
        {
            //std::cerr<<i<<std::endl;
            //std::cerr<<gfmesh_->face_array_size()<<std::endl;
            //std::cerr<<gfmesh_->face_number()<<std::endl;
            //std::cout<<gfmesh_->is_visible(i)<<std::endl;
            if (gfmesh_->is_visible(i))
            {
                glNormal3d(gfmesh_->face_normal_array()[3*i], gfmesh_->face_normal_array()[3*i+1], gfmesh_->face_normal_array()[3*i+2]);
                glArrayElement(gfmesh_->face_array()[3*i]);
                glArrayElement(gfmesh_->face_array()[3*i+1]);
                glArrayElement(gfmesh_->face_array()[3*i+2]);
            }
        }
        glEnd();
    }
    gfmesh_->reset_updated();
    if (render_->to_output_)
    {
        //output pgm file
        /*glReadPixels(0,0,render_->width_,render_->height_,GL_LUMINANCE, GL_UNSIGNED_BYTE, pixels_);
        std::ostringstream filename_str;
        filename_str<<"output"<<output_counter<<".pgm";
        std::ofstream ofs(filename_str.str().c_str(), std::ios::binary);
        ofs<<"P5 "<<render_->width_<<" "<<render_->height_<<" 255"<<std::endl;
        ofs.write((char*)pixels_, render_->width_*render_->height_);
        ofs.close();*/

        glReadPixels(0,0,render_->width_,render_->height_,GL_RGB, GL_UNSIGNED_BYTE, pixels_);

        //output ppm file
        /*std::ostringstream filename_str2;
        filename_str2<<"output"<<output_counter<<".ppm";
        std::ofstream ofs2(filename_str2.str().c_str(), std::ios::binary);
        ofs2<<"P6 "<<render_->width_<<" "<<render_->height_<<" 255"<<std::endl;
        ofs2.write((char*)pixels_, render_->width_*render_->height_*3);
        ofs2.close();*/

        //output oogl file
        std::ostringstream filename_str3;
        filename_str3<<"output"<<output_counter<<".oogl";
        std::ofstream ofs3(filename_str3.str().c_str());
        render_->ppmesh_->writeOogl(ofs3);
        ofs3.close();

        //output another file contains Y
        std::ostringstream filename_str4;
        filename_str4<<"output"<<output_counter<<".pgm";
        std::ofstream ofs4(filename_str4.str().c_str(), std::ios::binary);
        ofs4<<"P5 "<<render_->width_<<" "<<render_->height_<<" 255"<<std::endl;
        unsigned char* ptr = pixels_;
        for (size_t i = 0; i< size_t(render_->width_*render_->height_); i++)
        {
            unsigned char r = *(ptr++);
            unsigned char g = *(ptr++);
            unsigned char b = *(ptr++);
            unsigned char y = (unsigned char)(0.299*r + 0.587*g + 0.114*b);
            ofs4.write((char*)&y, 1);
        }
        ofs4.close();
        std::ofstream ofs5("record", std::ios::app);
        ofs5<<output_counter<<" "<<render_->sentBytes_<<" "<<render_->receivedBytes_<<" "<<gfmesh_->vertex_number()<<std::endl;

        output_counter++;
        render_->to_output_ = false;
    }

    //gfmesh_->mutex_.unlock();
    //std::cerr<<"unlocked by render"<<std::endl;
}


void disp()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    if (render_->smooth_)
    {
        glShadeModel(GL_SMOOTH);
    }
    else
    {
        glShadeModel(GL_FLAT);
    }

    glPushMatrix();
    glTranslated(render_->dx_, render_->dy_, render_->dz_);
    glRotated(render_->angle_x_, 1.0, 0.0, 0.0);
    glRotated(render_->angle_y_, 0.0, 1.0, 0.0);
    glRotated(render_->angle_z_, 0.0, 0.0, 1.0);
    glScaled (render_->scale_, render_->scale_, render_->scale_);
    if (render_->fill_)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        draw_surface_with_arrays();
    }
    if (render_->outline_)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        draw_surface_with_arrays();
    }
    glutSwapBuffers();
    glPopMatrix();
}


void reshape(int w, int h)
{
    if (h==0) h = 1;
    if (w==0) w = 1;
    render_->width_ = w;
    render_->height_= h;
    double ratio = 1.0 * w / h;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0 , w , h);

    if (render_->perspective_)
    {
        gluPerspective(render_->view_angle_, ratio, render_->min_distance_, render_->max_distance_);
    }
    else
    {
        glOrtho(render_->left_distance_, render_->right_distance_, render_->bottom_distance_, render_->top_distance_, render_->min_distance_, render_->max_distance_);
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(render_->view_x_, render_->view_y_, render_->view_z_, render_->view_x_, render_->view_y_, render_->view_z_-2*render_->bounding_length_, 0.0, 1.0, 0.0);
    return;
}
void Render::render_reset()
{
    dx_ = 0;
    dy_ = 0;
    dz_ = 0;
    angle_x_ = 0;
    angle_y_ = 0;
    angle_z_ = 0;
    scale_   = 1;
    mouse_button_ = 0;
    mouse_previous_x_ = 0;
    mouse_previous_y_ = 0;
    smooth_ = false;
    interpolated_ = false;
    perspective_  = true;
    outline_      = false;
    fill_         = true;
    check_visibility_ = true;
    ppmesh_->gfmesh()->reset_visibility(true);
    ppmesh_->resetExpectedLevel();
}

int name(void)
{
    return time(0);
}

void keyboard(unsigned char key, int x, int y)
{
    size_t n = render_->ppmesh_->n_vertices();
    std::ofstream ofs;
    std::ostringstream oss;
    std::string filename;
    switch (key)
    {
    case 27:
        exit(0);
    case 'R':
    case 'r':
        render_->render_reset();
        break;
    case 'S':
    case 's':
        if (glutGetModifiers() == GLUT_ACTIVE_SHIFT)
        {
            render_->scale_ *= 0.5;
        }
        else
        {
            render_->scale_ *= 2;
        }
        if (render_->visible_pq_)
        {
            render_->visible_pq_->stop();
        }
        render_->setCheck();
        render_->ppmesh_->gfmesh()->reset_visibility(true);
        render_->ppmesh_->resetExpectedLevel();
        break;
    case 'M':
    case 'm':
        render_->smooth_ = !(render_->smooth_);
        break;
    case 'I':
    case 'i':
        render_->interpolated_ = !(render_->interpolated_);
        break;
    case 'F':
    case 'f':
        render_->fill_   = !(render_->fill_);
        break;
    case 'L':
    case 'l':
        render_->outline_ = !(render_->outline_);
        break;
    case 'P':
    case 'p':
        render_->perspective_ = !(render_->perspective_);
        reshape(render_->width_, render_->height_);
        if (render_->visible_pq_)
        {
            render_->visible_pq_->stop();
        }
        render_->setCheck();
        render_->ppmesh_->gfmesh()->reset_visibility(true);
        render_->ppmesh_->resetExpectedLevel();
        break;
    case 'B':
    case 'b':
        render_->ppmesh_->refine(2*n);
        break;
        /*case 'C':
        case 'c':
            render_->ppmesh_->coarsen(n/2);
            break;*/
    case 'W':
    case 'w':
        ofs.open("out.oogl");
        render_->ppmesh_->writeOogl(ofs);
        ofs.close();
        break;
    case 'O':
    case 'o':
        render_->to_output_ = true;
        break;
    case 'C':
    case 'c':
        if (render_->visible_pq_)
        {
            render_->visible_pq_->setStrict(false);
        }
        break;
    case 'V':
    case 'v':
        oss << name();
        filename = std::string("view.")+std::string(oss.str());
        ofs.open(filename.c_str());
        ofs<<render_->dx_<<" "<<render_->dy_<<" "<<render_->dz_<<" "\
        <<render_->angle_x_<<" "<<render_->angle_y_<<" "<<\
        render_->angle_z_<<" "<<render_->scale_<<std::endl;
        ofs.close();
        break;
    case '+':
        //render_->ppmesh_->refine(n + n/50);
        if (render_->ppmesh_->wait() == 1) break;
        render_->ppmesh_->setWait(render_->ppmesh_->wait() / 2);
        std::cerr<<"reduce interval to "<<render_->ppmesh_->wait()<<std::endl;
        break;
    case '-':
        if (render_->ppmesh_->wait() == 16000000u) break;
        render_->ppmesh_->setWait(render_->ppmesh_->wait() * 2);
        std::cerr<<"add interval to "<<render_->ppmesh_->wait()<<std::endl;
        break;
    case 'E':
    case 'e':
        render_->display_color_ = ! (render_->display_color_);
        break;
    case 'H':
    case 'h':
        oss << name();
        filename = std::string("config.")+std::string(oss.str());
        ofs.open(filename.c_str());
        ofs<<render_->dx_<<" "<<render_->dy_<<" "<<render_->dz_<<" "\
        <<render_->angle_x_<<" "<<render_->angle_y_<<" "<<\
        render_->angle_z_<<" "<<render_->scale_<<std::endl;
        render_->ppmesh_->outputHistory(ofs);
        ofs.close();
        break;
    default:
        break;
    }
    glutPostRedisplay();
}

void special(int key, int x, int y)
{
    int state = glutGetModifiers();
    if (state != GLUT_ACTIVE_ALT && state != GLUT_ACTIVE_CTRL)
    {
        switch (key)
        {
        case GLUT_KEY_LEFT:
            render_->angle_y_ -= 5;
            break;
        case GLUT_KEY_RIGHT:
            render_->angle_y_ += 5;
            break;
        case GLUT_KEY_UP:
            render_->dz_ += 0.1 * render_->bounding_length_;
            break;
        case GLUT_KEY_DOWN:
            render_->dz_ -= 0.1 * render_->bounding_length_;
            break;
        default:
            break;
        }
    }
    else if (state == GLUT_ACTIVE_ALT)
    {
        switch (key)
        {
        case GLUT_KEY_LEFT:
            render_->dx_ -= 0.1*render_->bounding_length_;
            break;
        case GLUT_KEY_RIGHT:
            render_->dx_ += 0.1*render_->bounding_length_;
            break;
        case GLUT_KEY_UP:
            render_->dy_ += 0.1*render_->bounding_length_;
            break;
        case GLUT_KEY_DOWN:
            render_->dy_ -= 0.1*render_->bounding_length_;
            break;
        default:
            break;
        }
    }
    else
    {
        switch (key)
        {
        case GLUT_KEY_LEFT:
            render_->angle_z_ += 5;
            break;
        case GLUT_KEY_RIGHT:
            render_->angle_z_ -= 5;
            break;
        case GLUT_KEY_UP:
            render_->angle_x_ -= 5;
            break;
        case GLUT_KEY_DOWN:
            render_->angle_x_ += 5;
            break;
        }
    }
    render_->setCheck();
    render_->ppmesh_->resetExpectedLevel();
    render_->ppmesh_->gfmesh()->reset_visibility(true);
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
    if (state == GLUT_DOWN)
    {
        render_->mouse_button_ = button;
        render_->mouse_previous_x_ = x;
        render_->mouse_previous_y_ = y;
        if (render_->visible_pq_)
        {
            render_->visible_pq_->stop();
        }
    }
    else if (state == GLUT_UP)
    {
        render_->mouse_button_ = 0;
        render_->mouse_previous_x_ = 0;
        render_->mouse_previous_y_ = 0;
        render_->setCheck();
        render_->ppmesh_->gfmesh()->reset_visibility(true);
    }
}

void motion(int x, int y)
{
    if (render_->mouse_button_ == GLUT_LEFT_BUTTON)
    {
        render_->angle_y_ += (120 * (x - render_->mouse_previous_x_) / render_->width_);
        render_->angle_x_ += (120 * (y - render_->mouse_previous_y_) / render_->height_);
        render_->mouse_previous_x_ = x;
        render_->mouse_previous_y_ = y;
    }
    else if (render_->mouse_button_ == GLUT_RIGHT_BUTTON)
    {
        render_->dz_ += (render_->bounding_length_ * (y - render_->mouse_previous_y_)/render_->height_);
        render_->mouse_previous_y_ = y;
    }
    render_->ppmesh_->resetExpectedLevel();
    glutPostRedisplay();
}

void timer(int value)
{
    //static int counter_record = 0;
    /*if (counter_record == value)
    {
        counter_record = 0;
        render_->to_output_ = true;
    }
    else
    {
        counter_record++;
    }*/
    if (counter_ == (value * recheck_interval_ / 8))
    {
        counter_ = 0;
        render_->setCheck();
        glutPostRedisplay();
    }
    else
    {
        counter_++;
        if (render_->ppmesh_->gfmesh()->updated())
        {
            glutPostRedisplay();
        }
    }
    glutTimerFunc(1000/value, timer, value);
}

void initWindow(void)
{
    glClearColor(0., 0., 0., 0.);

    GLfloat mat_specular[] = {0.3, 0.3, 0.3, 0.0};
    GLfloat mat_shininess[]  = {100};
    GLfloat light_position[] = {0.0, 0.0, 1.0, 0.0};
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
}

Render::Render(int& argc, char* argv[], const char* name, Ppmesh* ppmesh, VisiblePQ* visible_pq, int framerate)
        :receivedBytes_(0), sentBytes_(0),\
        visible_pq_(visible_pq), ppmesh_(ppmesh), view_angle_(45), \
        min_distance_(0.01), max_distance_(100),\
        view_x_(0), view_y_(0), view_z_(0), dx_(0), dy_(0), dz_(0), angle_x_(0), \
        angle_y_(0), angle_z_(0), scale_(1), bounding_length_(1), mouse_button_(0),\
        mouse_previous_x_(0), mouse_previous_y_(0), width_(500), height_(500), \
        smooth_(false), interpolated_(false), perspective_(true), \
        outline_(false), fill_(true), check_visibility_(true), to_output_(false), display_color_(true)
{
    Gfmesh* gfmesh = ppmesh->gfmesh();

    Coordinate center_x = 0;
    Coordinate center_y = 0;
    Coordinate center_z = 0;
    Coordinate min_x = 0;
    Coordinate max_x = 0;
    Coordinate min_y = 0;
    Coordinate max_y = 0;
    Coordinate min_z = 0;
    Coordinate max_z = 0;
    Coordinate max_length = 0;

    center_x = min_x = max_x = gfmesh->vertex_array()[0];
    center_y = min_y = max_y = gfmesh->vertex_array()[1];
    center_z = min_z = max_z = gfmesh->vertex_array()[2];
    for (size_t i=1; i < gfmesh->vertex_number(); i++)
    {
        //std::cerr<<gfmesh->vertex_number()<<" ";
        //std::cerr<<i<<std::endl;
        //std::cerr<<gfmesh->vertex_array_size()<<std::endl;
        Coordinate x = gfmesh->vertex_array()[3*i];
        Coordinate y = gfmesh->vertex_array()[3*i+1];
        Coordinate z = gfmesh->vertex_array()[3*i+2];

        min_x = min_x > x ? x : min_x;
        max_x = max_x < x ? x : max_x;

        min_y = min_y > y ? y : min_y;
        max_y = max_y < y ? y : max_y;

        min_z = min_z > z ? z : min_z;
        max_z = max_z < z ? z : max_z;
    }
    center_x = (max_x + min_x)/2;
    center_y = (max_y + min_y)/2;
    center_z = (max_z + min_z)/2;

    max_length = max_x - min_x;
    if (max_y - min_y > max_length)
    {
        max_length = max_y - min_y;
    }
    if (max_z - min_z > max_length)
    {
        max_length = max_z - max_z;
    }
    bounding_length_ = max_length;

    double factor = 2;
    left_distance_ = right_distance_ = top_distance_ = bottom_distance_ = factor*max_length*0.5;
    view_x_ = center_x;
    view_y_ = center_y;
    view_z_ = center_z + 2*max_length;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH);
    glutInitWindowSize(500, 720);
    glutInitWindowPosition(20, 20);
    mainWindow_ = glutCreateWindow(name);

    glutDisplayFunc(disp);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutTimerFunc(1000/framerate, timer, framerate);
    initWindow();
    glClearColor(0., 0., 0., 0.);

    render_ = this;
    std::cerr<<view_x_<<" "<<view_y_<<" "<<view_z_<<" "<<std::endl;
    std::cerr<<bounding_length_<<std::endl;
}


void Render::enterMainLoop()
{
    glutMainLoop();
}
