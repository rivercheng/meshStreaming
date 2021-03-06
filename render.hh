#ifndef __RENDER_HH__
#define __RENDER_HH__
#include "ppmesh.hh"
#include "visiblepq.hh"

class NoBaseMesh{};


class Render
{
    friend void update_view_parameter(void);
    friend void stat_screen_area(void);
    friend void disp(void);
    friend void draw_surface_with_arrays(void);
    friend void reshape(int w, int h);
    friend void keyboard(unsigned char key, int x, int y);
    friend void special(int key, int x, int y);
    friend void mouse(int button, int state, int x, int y);
    friend void motion(int x, int y);
    friend void timer(int value);
    friend inline void check_visibility();

public:
    Render(int& argc, char* argv[], const char* name, Ppmesh* ppmesh, VisiblePQ* visible_pq, int framerate);
    void enterMainLoop();

    void setSmooth(bool value)
    {
        smooth_ = value;
    }
    void setInterPolated(bool value)
    {
        interpolated_ = value;
    }
    void setPerspective(bool value)
    {
        perspective_ = value;
    }
    void setOutline(bool value)
    {
        outline_ = value;
    }
    void setFill(bool value)
    {
        fill_ = value;
    }
    void setCheck(bool value = true)
    {
        check_visibility_ = value;
    }
    void setColor(bool value = true)
    {
        display_color_ = value;
    }
    bool showColor(void)
    {
        return display_color_;
    }
    size_t receivedBytes_;
    size_t sentBytes_;
    VisiblePQ* visible_pq_;
    void setView(double dx, double dy, double dz, double angle_x,\
                 double angle_y, double angle_z, double scale)
    {
        dx_ = dx;
        dy_ = dy;
        dz_ = dz;
        angle_x_ = angle_x;
        angle_z_ = angle_z;
        angle_y_ = angle_y;
        scale_   = scale;
    }

private:
    void render_reset(void);



private:
    Ppmesh* ppmesh_;
    double view_angle_;
    double left_distance_;
    double right_distance_;
    double top_distance_;
    double bottom_distance_;
    double min_distance_;
    double max_distance_;
    double view_x_;
    double view_y_;
    double view_z_;
    double dx_;
    double dy_;
    double dz_;
    double angle_x_;
    double angle_y_;
    double angle_z_;
    double scale_;
    double bounding_length_;
    int mouse_button_;
    int mouse_previous_x_;
    int mouse_previous_y_;
    int width_;
    int height_;
    bool smooth_;
    bool interpolated_;
    bool perspective_;
    bool outline_;
    bool fill_;
    bool check_visibility_;
    bool to_output_;
    bool display_color_;
};


#endif
