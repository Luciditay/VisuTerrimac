#ifndef LIGHT_H
#define LIGHT_H

#include <GL/glu.h>
#include "shading.h"
#include "geometry.h"

typedef struct Ray {
    Point3D origin;
    Vector3D direction;
} Ray;

typedef struct Intersection {
    Point3D position;
    Material material;
    Vector3D normal;
} Intersection;

typedef struct Light {
    Point3D position;
    ColorRGB color;
} Light;


void drawSkybox(float cam_yaw, float cam_pitch, GLuint cubemap_text_ID);

Light createLight(Point3D position, ColorRGB color);

#endif 