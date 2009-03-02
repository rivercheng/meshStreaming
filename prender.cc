#define GL_GLEXT_PROTOTYPES
#include <GL/glut.h>
#include <GL/glext.h>
#include <Poco/Mutex.h>
#include <sstream>

#include "prender.hh"
#include "ppmesh.hh"
#include "gfmesh.hh"
#include "pvisiblepq.hh"

const size_t FRAME_BUFFER_SIZE = 1024*768*3;
static PRender* render_ = NULL;
static unsigned char pixels_[FRAME_BUFFER_SIZE] = {0};
static double modelview_matrix_[16];
static float  view_pos_[3];
static int    recheck_interval_ = 1;
static int    counter_  = 0;


inline void log_view_parameter()
{
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview_matrix_);
    view_pos_[0] =  render_->view_x_;
    view_pos_[1] =  render_->view_y_;
    view_pos_[2] =  render_->view_z_;
    render_->logger_.log2_->log(modelview_matrix_, view_pos_);
}

inline void check_visibility()
{
    Gfmesh* gfmesh_ = render_->ppmesh_->gfmesh();
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
    if (render_->check_visibility_)
    {
        gfmesh_->reset_visibility(false);
    }
    render_->visible_pq_->update(pixels_, render_->width_*render_->height_*3);
    glEnable(GL_LIGHTING);
    glEnable(GL_DITHER);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
}


void draw_surface_with_arrays()
{
    static int output_counter = 1;
    Gfmesh* gfmesh_ = render_->ppmesh_->gfmesh();
    Poco::ScopedLock<Poco::Mutex> lock(gfmesh_->mutex_);

    //glEnableClientState(GL_VERTEX_ARRAY);
    //glVertexPointer(3, GL_DOUBLE, 0, gfmesh_->vertex_array());
    //glBindBuffer(GL_ARRAY_BUFFER, render_->bufferObjects_[0]);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(double)*gfmesh_->vertex_number()*3, gfmesh_->vertex_array(), GL_DYNAMIC_DRAW);
    //glVertexPointer(3, GL_DOUBLE, 0, 0);
    if (render_->check_visibility_ && render_->visible_pq_ != NULL)
    {
        recheck_interval_ = 1;
        counter_ = 0;
        //TODO render_->logger_.log("check visibility");
        gfmesh_->reset_visibility(true);
        log_view_parameter();
        check_visibility();
        render_->setCheck(false);
    }
    if (render_->recheck_visibility_ && render_->visible_pq_ != NULL)
    {
        //TODO render_->logger_.log("recheck visibility");
        check_visibility();
        render_->setRecheck(false);
    }
    if (render_->smooth_ || render_->interpolated_)
    {
        glEnableClientState(GL_NORMAL_ARRAY);
        glNormalPointer(GL_FLOAT, 0, gfmesh_->vertex_normal_array());
        glDrawElements(GL_TRIANGLES, 3*gfmesh_->face_number(),GL_UNSIGNED_INT, gfmesh_->face_array());
    }
    else
    {
        glDisableClientState(GL_NORMAL_ARRAY);
        glBegin(GL_TRIANGLES);
        assert(gfmesh_->face_number()*3 == gfmesh_->face_array_size());
        for (size_t i=0; i< gfmesh_->face_number(); i++)
        {
            if (gfmesh_->is_visible(i)) //only render visible triangles
            {
                glNormal3f(gfmesh_->face_normal_array()[3*i], gfmesh_->face_normal_array()[3*i+1], gfmesh_->face_normal_array()[3*i+2]);
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
        glReadPixels(0,0,render_->width_,render_->height_,GL_RGB, GL_UNSIGNED_BYTE, pixels_);

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

void PRender::render_reset()
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
    mouse_last_x_ = 0;
    mouse_last_y_ = 0;
    smooth_ = false;
    interpolated_ = false;
    perspective_  = true;
    outline_      = false;
    fill_         = true;
    check_visibility_ = true;
    ppmesh_->resetExpectedLevel();
}

void keyboard(unsigned char key, int x, int y)
{
    size_t n = render_->ppmesh_->n_vertices();
    std::ofstream ofs;
    switch (key)
    {
    case 27:
        if (render_->visible_pq_)
        {
            render_->visible_pq_->quit();
        }
        render_->logger_.log(QUIT);
        sleep(1);
        //Poco::join(receiver);
        exit(0);
    case 'R':
    case 'r':
        render_->render_reset();
        render_->logger_.log(RESET);
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
        render_->ppmesh_->resetExpectedLevel();
        render_->logger_.log(SCALE);
        break;
    case 'M':
    case 'm':
        render_->smooth_ = !(render_->smooth_);
        render_->logger_.log(SMOOTH);
        break;
    case 'I':
    case 'i':
        render_->interpolated_ = !(render_->interpolated_);
        render_->logger_.log(INTRAPOLATE);
        break;
    case 'F':
    case 'f':
        render_->fill_   = !(render_->fill_);
        render_->logger_.log(FILL);
        break;
    case 'L':
    case 'l':
        render_->outline_ = !(render_->outline_);
        render_->logger_.log(OUTLINE);
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
        render_->ppmesh_->resetExpectedLevel();
        render_->logger_.log(PERSPECTIVE);
        break;
    case 'B':
    case 'b':
        render_->ppmesh_->refine(2*n);
        render_->logger_.log(REFINE);
        break;
    case 'W':
    case 'w':
        ofs.open("out.oogl");
        render_->ppmesh_->writeOogl(ofs);
        ofs.close();
        render_->logger_.log(WRITE);
        break;
    case 'O':
    case 'o':
        render_->to_output_ = true;
        render_->logger_.log(OUTPUT);
        break;
    case 'C':
    case 'c':
        if (render_->visible_pq_)
        {
            render_->visible_pq_->setStrict(false);
        }
        render_->logger_.log(CULL);
        break;
    case 'V':
    case 'v':
        ofs.open("view_points", std::ios::app);
        ofs<<render_->dx_<<" "<<render_->dy_<<" "<<render_->dz_<<" "\
        <<render_->angle_x_<<" "<<render_->angle_y_<<" "<<\
        render_->angle_z_<<" "<<render_->scale_<<std::endl;
        ofs.close();
        render_->logger_.log(VIEW);
        break;
    case '+':
        render_->ppmesh_->refine(n + n/50);
        render_->logger_.log(REFINE);
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
            render_->angle_y_ -= 10;
            render_->logger_.log(REVOLVE_CLOCKWISE);
            break;
        case GLUT_KEY_RIGHT:
            render_->angle_y_ += 10;
            render_->logger_.log(REVOLVE_ANTICLOCKWISE);
            break;
        case GLUT_KEY_UP:
            render_->dz_ += 0.1 * render_->bounding_length_;
            render_->logger_.log(ZOOM_IN);
            break;
        case GLUT_KEY_DOWN:
            render_->dz_ -= 0.1 * render_->bounding_length_;
            render_->logger_.log(ZOOM_OUT);
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
            render_->logger_.log(MOVE_LEFT);
            break;
        case GLUT_KEY_RIGHT:
            render_->dx_ += 0.1*render_->bounding_length_;
            render_->logger_.log(MOVE_RIGHT);
            break;
        case GLUT_KEY_UP:
            render_->dy_ += 0.1*render_->bounding_length_;
            render_->logger_.log(MOVE_UP);
            break;
        case GLUT_KEY_DOWN:
            render_->dy_ -= 0.1*render_->bounding_length_;
            render_->logger_.log(MOVE_DOWN);
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
            render_->angle_z_ += 10;
            render_->logger_.log(ROTATE_CLOCKWISE);
            break;
        case GLUT_KEY_RIGHT:
            render_->angle_z_ -= 10;
            render_->logger_.log(ROTATE_ANTICLOCKWISE);
            break;
        case GLUT_KEY_UP:
            render_->angle_x_ -= 10;
            render_->logger_.log(TILT_FORWARD);
            break;
        case GLUT_KEY_DOWN:
            render_->angle_x_ += 10;
            render_->logger_.log(TILT_BACKWARD);
            break;
        }
    }
    render_->setCheck();
    render_->ppmesh_->resetExpectedLevel();
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
    if (state == GLUT_DOWN)
    {
        render_->ppmesh_->gfmesh()->reset_visibility(true);
        render_->mouse_button_ = button;
        render_->mouse_previous_x_ = x;
        render_->mouse_previous_y_ = y;
        render_->mouse_last_x_ = x;
        render_->mouse_last_y_ = y;
        
        if (render_->visible_pq_)
        {
            render_->visible_pq_->stop();
        }
    }
    else if (state == GLUT_UP)
    {
    	//log the rotation or the zoom done by the user on the button up 
    	if (render_->mouse_button_ == GLUT_LEFT_BUTTON)
    	{
    		double rotation_x = (120 * (x - render_->mouse_last_x_) / render_->width_);
    		double rotation_y = (120 * (y - render_->mouse_last_y_) / render_->height_);
    		render_->logger_.log(MOUSE_ROTATION,rotation_x,rotation_y);
    		
    	}
    	else if (render_->mouse_button_ == GLUT_RIGHT_BUTTON)
    	{
    		double zoom =  (y - render_->mouse_last_y_)/render_->height_;
    		render_->logger_.log(MOUSE_ZOOM,zoom);
    	}
    
        render_->mouse_button_ = 0;
        render_->mouse_previous_x_ = 0;
        render_->mouse_previous_y_ = 0;
        render_->mouse_last_x_ = 0;
        render_->mouse_last_y_ = 0;
        render_->setCheck();
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
        render_->setRecheck();
        recheck_interval_ *= 2;
        glutPostRedisplay();
    }
    else
    {
        counter_++;
        if (render_->ppmesh_->gfmesh()->updated())
        {
            //std::cerr<<"gfmesh updated."<<std::endl;
            glutPostRedisplay();
        }
    }
    glutTimerFunc(1000/value, timer, value);
}


PRender::PRender(int& argc, char* argv[], const char* name, Ppmesh* ppmesh, PVisiblePQ* visible_pq, int framerate, Logger& logger)
        :receivedBytes_(0), sentBytes_(0),\
        visible_pq_(visible_pq), ppmesh_(ppmesh), view_angle_(45), \
        min_distance_(0.01), max_distance_(5000),\
        view_x_(0), view_y_(0), view_z_(0), dx_(0), dy_(0), dz_(0), angle_x_(0), \
        angle_y_(0), angle_z_(0), scale_(1), bounding_length_(1), mouse_button_(0),\
        mouse_previous_x_(0), mouse_previous_y_(0),mouse_last_x_(0),mouse_last_y_(0) ,width_(500), height_(500), \
        smooth_(false), interpolated_(false), perspective_(true), \
        outline_(false), fill_(true), check_visibility_(true), recheck_visibility_(false),\
        to_output_(false), isStopped_(false), logger_(logger)
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
    min_distance_ = 0.01 * view_z_;
    max_distance_ = 100 * view_z_;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(name);

    glutDisplayFunc(disp);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutTimerFunc(1000/framerate, timer, framerate);

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
    render_ = this;
    std::cerr<<view_x_<<" "<<view_y_<<" "<<view_z_<<" "<<std::endl;
    std::cerr<<bounding_length_<<std::endl;
    
    //glGenBuffers(1, bufferObjects_);
    //glBindBuffer(GL_ARRAY_BUFFER, bufferObjects_[0]);
    //glBindBuffer(GL_ARRAY_BUFFER, bufferObjects_[1]);
    //glBufferData(GL_ARRAY_BUFFER, 100000000, ppmesh_->gfmesh()->vertex_array(), GL_DYNAMIC_DRAW);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_DOUBLE, 0, ppmesh_->gfmesh()->vertex_array());
    //glVertexPointer(3, GL_FLOAT, 0, 0);
}

void PRender::enterMainLoop()
{
    glutMainLoop();
}
