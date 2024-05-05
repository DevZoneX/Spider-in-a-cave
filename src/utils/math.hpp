#ifndef MATH_HPP
#define MATH_HPP

#include "cgp/cgp.hpp"
#include "../environment.hpp"

using namespace cgp;

namespace math{
    enum cube_face{
        UP_FACE,DOWN_FACE,LEFT_FACE,RIGHT_FACE,FRONT_FACE,BACK_FACE
    };

    struct plane{
        float a,b,c,d;
        vec3 point0;
        plane(vec3 point, vec3 normal);
        plane(vec3 point, vec3 tengente1, vec3 tengente2);
        bool evaluate(vec3 point);
    };
    struct parallelogram: public plane{
        vec3 axis1;
        vec3 axis2;
        parallelogram(vec3 point, vec3 _axis1, vec3 _axis2);
        bool evaluate(vec3 point);
    };
    struct line{
        vec3 director;
        vec3 start;
        line(vec3 _start,vec3 _director);
        bool evaluate(vec3 point);
    };
    struct segment: public line{
        segment(vec3 _start,vec3 _director): line::line(_start,_director){}
        bool evaluate(vec3 point);
    };

    float determinant2D(vec2 vect1, vec2 vect2);
    float determinant3D(vec3 vect1, vec3 vect2, vec3 vect3);

    

    bool plane_line_intersection(plane plane, line line, vec3 &intersection);
    bool plane_line_intersection(plane plane, line line);
    bool parallelogram_segment_intersection(parallelogram parallelogram, segment segment, vec3 &intersection);
    bool parallelogram_segment_intersection(parallelogram parallelogram, segment segment);

    
    void draw(segment segment,environment_structure environment);
    void draw(parallelogram parallelogram,environment_structure environment);
}

#endif // MATH_HPP
