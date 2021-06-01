#include "geometry.h"
#include "math.h"
#include "quadtree.h"
#include "iostream"
#include "GL/glu.h"

Point2D add2DVect(Point2D p1, Point2D p2)
{
    Point2D add = {
        p1.x + p2.x,
        p1.y + p2.y,
    };
    return add;
}

Point2D sub2DVect(Point2D p1, Point2D p2)
{
    return {p2.x - p1.x,
            p2.y - p1.y};
}

Point3D sub3DVect(Point3D p1, Point3D p2)
{
    return {p2.x - p1.x,
            p2.y - p1.y,
            p2.z - p1.z
        };
}

Point2D scale2DVect(Point2D p1, float scalaire)
{
    return {
        p1.x *scalaire,
        p1.y *scalaire};
}

Point3D add3DVect(Point3D p1, Point3D p2)
{
    Point3D add = {
        p1.x + p2.x,
        p1.y + p2.y,
        p1.z + p2.z};
    return add;
}

float determinant(Point2D Vect1, Point2D Vect2)
{
    return Vect1.x * Vect2.y - Vect1.y * Vect2.x;
}

// Projecte un point en 3d sur le plan OXY
Point2D projection2D(Point3D p1)
{
    return {p1.x, p1.y};
}

float norme2D(Point2D p1)
{
    return sqrt(p1.x * p1.x + p1.y * p1.y);
}

float norme3D(float x, float y, float z)
{
    return sqrt(x * x + y * y + z * z);
}

void afficherTriangle(Triangle *triangle)
{
    glBegin(GL_LINE_LOOP);
    glColor3f(1, 0, 1);
    glVertex3f(triangle->p1.x, triangle->p1.y, 0);
    glVertex3f(triangle->p2.x, triangle->p2.y, 0);
    glVertex3f(triangle->p3.x, triangle->p3.y, 0);
    glEnd();

    // afficherPoint2D(triangle->p1);
    // afficherPoint2D(triangle->p2);
    // afficherPoint2D(triangle->p3);
}

