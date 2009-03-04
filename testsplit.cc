#include <GL/glut.h>
#include <iostream>
#include <fstream>
#include "view.hh"
#include "vertexid.hh"
#include "gfpmesh.hh"

Ppmesh* ppmesh = 0;
View*   view   = 0;
double view_angle_ = 45;
double min_distance_ = 0.01;
double max_distance_ = 100;
double view_x_ = 0;
double view_y_ = 0;
double view_z_ = 0;
double bounding_length_ = 0;
bool   initiated_ = false;
bool   displayed_ = false;
bool   reshaped_  = false;

void disp()
{
    ppmesh = view->ppmesh();
    Gfpmesh* gfmesh = ppmesh->gfmesh();
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glShadeModel(GL_FLAT);
    glPushMatrix();
    glTranslated(view->dx(), view->dy(), view->dz());
    glRotated(view->angle_x(), 1.0, 0.0, 0.0);
    glRotated(view->angle_y(), 0.0, 1.0, 0.0);
    glRotated(view->angle_z(), 0.0, 0.0, 1.0);
    glScaled(view->scale(), view->scale(), view->scale());
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
    displayed_ = true;
}

void reshape(int w, int h)
{
    if (h==0) h=1;
    if (w==0) w=1;
    double ratio = 1.0 * w / h;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, w, h);
    gluPerspective(view_angle_, ratio, min_distance_, max_distance_);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(view_x_, view_y_, view_z_, view_x_, view_y_, view_z_-2*bounding_length_, 0.0, 1.0, 0.0);
    reshaped_ = true;
    return;
}

void idle()
{
    if (!displayed_ || !reshaped_)
    {
        return;
    }

    view->initiateView();
    std::cerr<<"initiated"<<std::endl;
    double initPsnr = view->psnr(view->initialImage(), view->finalImage());
    std::cerr << initPsnr <<std::endl;
    std::vector<VertexID> splits;
    VertexID id = 0;
    do
    {
        std::cout<<"enter the ID to split"<<std::endl;
        std::cin>>id;
        Image* splitImage = view->splitImage(id);
        if (splitImage)
        {
            double psnr1  = view->psnr(view->initialImage(), splitImage);
            double psnr2  = view->psnr(view->finalImage(), splitImage) - initPsnr;
            std::cout<<id<<" "<<view->ppmesh()->id2level(id)<<" "<<view->screenArea(id)<<" "<<psnr1<<" "<<psnr2<<std::endl;
        }
        else
        {
            std::cout<<"cannot be split"<<std::endl;
        }
        delete splitImage;
    }
    while (id != 0);
    exit(0);
}

int main(int argc, char** argv)
{
    if (argc != 3)
    {
        std::cerr<<"Usage "<<argv[0]<<" <mesh file> <configFile>"<<std::endl;
        exit(1);
    }
    const char* meshFile = argv[1];
    const char* configFile = argv[2];
    int   width    = 500;
    int   height   = 720;
    std::ifstream fmesh(meshFile, std::ios::binary);
    if (!fmesh)
    {
        std::cerr << "cannot open " + std::string(meshFile) <<std::endl;
        exit(1);
    }

    std::ifstream config(configFile, std::ios::binary);
    if (!config)
    {
        std::cerr << "cannot open " + std::string(configFile) <<std::endl;
        exit(1);
    }
    view = new View(fmesh, config, width, height);
    ppmesh = view->ppmesh();
    Gfpmesh* gfmesh = ppmesh->gfmesh();

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

    view_x_ = center_x;
    view_y_ = center_y;
    view_z_ = center_z + 2*max_length;

    std::cerr<<view_x_<<" "<<view_y_<<" "<<view_z_<<std::endl;
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH);
    glutInitWindowSize(width, height);
    glutInitWindowPosition(20, 20);
    glutCreateWindow("test");

    glutDisplayFunc(disp);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);
    glClearColor(0., 0., 0., 0.);
    GLfloat mat_specular[] = {0.3, 0.3, 0.3, 0.0};
    GLfloat mat_shininess[] = {100};
    GLfloat light_position[] = {0.0, 0.0, 1.0, 0.0};
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glEnable (GL_DEPTH_TEST);
    glEnable (GL_CULL_FACE);
    glEnable (GL_LIGHTING);
    glEnable (GL_LIGHT0);
    glEnable (GL_NORMALIZE);

    glutMainLoop();
}








