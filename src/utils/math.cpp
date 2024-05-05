#include "math.hpp"


// using namespace math;
math::plane::plane(vec3 point, vec3 normal)
{
    normal = normal / norm(normal);
    a = normal.x;
    b = normal.y;
    c = normal.z;
    d = a * point.x + b * point.y + c * point.z;
    d = -d;
    point0 = point;
}
math::plane::plane(vec3 point, vec3 tengente1, vec3 tengente2)
{
    vec3 normal = cgp::cross(tengente1, tengente2);
    normal = normal / norm(normal);
    if (normal.x == 0 && normal.y == 0 && normal.z == 0)
    {
        std::cout << "Plane Undefined Error" << std::endl;
        return;
    }
    a = normal.x;
    b = normal.y;
    c = normal.z;
    d = a * point.x + b * point.y + c * point.z;
    d = -d;
    point0 = point;
}
bool math::plane::evaluate(vec3 point)
{
    return cgp::abs(a * point.x + b * point.y + c * point.z + d) <= 1e-06;
}
math::parallelogram::parallelogram(vec3 point, vec3 _axis1, vec3 _axis2) : math::plane::plane(point, _axis1, _axis2)
{
    axis1 = _axis1;
    axis2 = _axis2;
}
bool math::parallelogram::evaluate(vec3 point)
{
    vec3 to = point - point0;
    if (!plane::evaluate(point))
    {
        return false;
    }
    float l1 = cgp::dot(axis1, to) / pow(norm(axis1), 2);
    float l2 = cgp::dot(axis2, to) / pow(norm(axis2), 2);
    if (0 <= l1 && l1 <= 1 && 0 <= l2 && l2 <= 1)
    {
        return true;
    }
    return false;
}
math::line::line(vec3 _start, vec3 _director)
{
    start = _start;
    director = _director;
}
bool math::line::evaluate(vec3 point)
{
    if (cgp::abs((point.x - start.x) * director.y - (point.y - start.y) * director.x) >= 5 * 1e-04)
    {
        return false;
    }
    if (cgp::abs((point.x - start.x) * director.z - (point.z - start.z) * director.x) >= 5 * 1e-04)
    {
        return false;
    }
    if (cgp::abs((point.y - start.y) * director.z - (point.z - start.z) * director.y) >= 5 * 1e-04)
    {
        return false;
    }
    return true;
}
bool math::segment::evaluate(vec3 point)
{
    if (!line::evaluate(point))
    {
        return false;
    }
    vec3 to = point - start;
    float l1 = cgp::dot(director, to) / pow(norm(director), 2);
    if (0 <= l1 && l1 <= 1)
    {
        return true;
    }
    return false;
}

float math::determinant2D(vec2 vect1, vec2 vect2)
{
    return vect1.x * vect2.y - vect1.y * vect2.x;
}
float math::determinant3D(vec3 vect1, vec3 vect2, vec3 vect3)
{
    float det = 0;
    det += vect1.x * math::determinant2D({vect2.y, vect2.z}, {vect3.y, vect3.z});
    det += -vect1.y * math::determinant2D({vect2.x, vect2.z}, {vect3.x, vect3.z});
    det += vect1.z * math::determinant2D({vect2.x, vect2.y}, {vect3.x, vect3.y});
    return det;
}

bool math::plane_line_intersection(plane plane, line line, vec3 &intersection)
{
    vec3 normal = {plane.a, plane.b, plane.c};
    float dot = cgp::dot(normal, line.director);
    if (dot == 0)
    {
        return false;
    } // Note that the line could be in the plane, in which case we, in the collision contexte, don't consider it a collision
    dot = cgp::dot((plane.point0 - line.start), normal) / dot;
    intersection = line.start + dot * line.director;
    return true;
}
bool math::plane_line_intersection(plane plane, line line)
{
    vec3 dummy;
    return math::plane_line_intersection(plane, line, dummy);
}
bool math::parallelogram_segment_intersection(math::parallelogram parallelogram, math::segment segment, vec3 &intersection)
{
    vec3 temp;
    if (!plane_line_intersection(parallelogram, segment, temp))
    {
        return false;
    }
    intersection = temp;
    if (!segment.evaluate(temp))
    {
        return false;
    }
    if (!parallelogram.evaluate(temp))
    {
        return false;
    }
    intersection = temp;
    return true;
}
bool math::parallelogram_segment_intersection(math::parallelogram parallelogram, math::segment segment)
{
    vec3 dummy;
    return math::parallelogram_segment_intersection(parallelogram, segment, dummy);
}

static cgp::curve_drawable math_draw_curve;
static bool curve_initialized = false;
void initialize_curve()
{
    if (!curve_initialized)
    {
        numarray<vec3> positions;
        positions.resize(2);
        math_draw_curve.initialize_data_on_gpu(positions);
        curve_initialized = true;
    }
}
void math::draw(segment segment, environment_structure environment)
{
    initialize_curve();
    numarray<vec3> positions;
    math_draw_curve.color = {0.6, 0.2, 0.1};
    positions.resize(2);
    positions[0] = segment.start;
    positions[1] = segment.start + segment.director;
    math_draw_curve.vbo_position.update(positions);
    cgp::draw(math_draw_curve, environment);
}
void math::draw(parallelogram parallelogram, environment_structure environment)
{
    initialize_curve();
    numarray<vec3> positions;
    math_draw_curve.color = {0.2, 0.5, 0.3};
    positions.resize(2);
    positions[0] = parallelogram.point0;
    positions[1] = parallelogram.point0 + parallelogram.axis1;
    math_draw_curve.vbo_position.update(positions);
    cgp::draw(math_draw_curve, environment);
    positions[1] = parallelogram.point0 + parallelogram.axis2;
    math_draw_curve.vbo_position.update(positions);
    cgp::draw(math_draw_curve, environment);

    positions[0] = parallelogram.point0 + parallelogram.axis1 + parallelogram.axis2;
    positions[1] = parallelogram.point0 + parallelogram.axis1;
    math_draw_curve.vbo_position.update(positions);
    cgp::draw(math_draw_curve, environment);
    positions[1] = parallelogram.point0 + parallelogram.axis2;
    math_draw_curve.vbo_position.update(positions);
    cgp::draw(math_draw_curve, environment);
}