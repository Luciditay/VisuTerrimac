#include "shading.h"

Material createMaterial(ColorRGB diffuse, ColorRGB specular, float shininess)
{
    Material material;
    material.diffuse = diffuse;
    material.specular = specular;
    material.shininess = shininess;
    return material;
}