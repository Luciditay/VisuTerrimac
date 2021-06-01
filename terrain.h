#ifndef TERRAIN_H
#define TERRAIN_H

#include "GL/glu.h"

struct Triangle;
struct Point3D;
struct Node;

void drawCamera(Triangle* triangle);
void drawObjet(GLuint idTexture, float x, float y, float** pixels, float pan, float zObjet, float zMin);
void drawTriangleTextureSup(Point3D pHG, Point3D pHD, Point3D pBG, GLuint idTexture, float zMax, float zMin);
void drawTriangleTextureInf(Point3D pBG, Point3D pBD, Point3D pHD, GLuint idTexture, float zMax, float zMin);

void drawQuadTreeFil(Node* quadtree, bool frustum);
void drawQuadTreeTexture(Node* quadtree, GLuint idtexture, bool frustum);

void drawTerrainFil(Node *quadTree, Triangle* triangleCamera, bool frustum);
void drawTerrainTexture(Node *quadTree, GLuint idTexture, Triangle* triangleCamera, float zMax, float zMin, bool frustum);


#endif