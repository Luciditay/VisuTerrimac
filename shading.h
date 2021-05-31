#ifndef SHADING_H
#define SHADING_H

#include "colors.h"

typedef struct Material {
    ColorRGB diffuse;
    ColorRGB specular;
    float shininess;
} Material;

Material createMaterial(ColorRGB diffuse, ColorRGB specular, float shininess);

#endif