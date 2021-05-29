#ifndef TERRAIN_H
#define TERRAIN_H

#include "GL/glu.h"

struct Triangle;
struct Point3D;
struct Node;

void drawCamera(Triangle* triangle);
void drawTriangleTextureSup(Point3D pHG, Point3D pHD, Point3D pBG, GLuint idTexture, float zMax, float zMin);
void drawTriangleTextureInf(Point3D pBG, Point3D pBD, Point3D pHD, GLuint idTexture, float zMax, float zMin);
void drawTerrain(Node* quadtree);
void drawTerrainFil(Node* quadTree);
void drawTerrainTexture(Node *quadTree, GLuint idTexture, Triangle* triangleCamera, float zMax, float zMin);

#endif