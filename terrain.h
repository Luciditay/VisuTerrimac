#ifndef TERRAIN_H
#define TERRAIN_H

#include "GL/glu.h"

struct Point3D;
struct Node;
void drawTriangleTextureSup(Point3D pHG, Point3D pHD, Point3D pBG, GLuint idTexture);
void drawTriangleTextureInf(Point3D pBG, Point3D pBD, Point3D pHD, GLuint idTexture);
void drawTerrain(Node* quadtree);
void drawTerrainFil(Node* quadTree);
void drawTerrainTexture(Node *quadTree, GLuint idTexture);

#endif