#include "terrain.h"
#include "SDL2/SDL.h"
#include "GL/glu.h"
#include "quadtree.h"
#include <iostream>
#include "geometry.h"
#include "camera.h"

float max(float v1, float v2)
{
    if (v1 > v2)
        return v1;
    return v2;
}

float maxValue(float v1, float v2, float v3)
{
    return max(max(v1, v2), v3); //On compare v1 à v2 puis le résultat à v3
}

void drawObjet(GLuint idTexture, float x, float y, float **pixels, float pan, float zObjet, float zMin)
{
    glColor3f(1, 1, 1);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND); 
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA); 
    glBindTexture(GL_TEXTURE_2D, idTexture);

    //glColor3f(1, 0, 1);
    int zPoint = (int)pixels[(int)x][(int)y];

    zObjet+=zMin-0.5;

    glPushMatrix();
    glTranslatef(x, y, zObjet);
    glRotatef(pan/M_PI*180,0.f,1.0f,0.f);

    // glColor3f(diffuse1,diffuse1,diffuse1);
    glBegin(GL_QUADS);

    glTexCoord2f(0, 1);
    glVertex3f(-zObjet, 0, -zObjet);

    glTexCoord2f(1, 1);
    glVertex3f(zObjet, 0, -zObjet);

    glTexCoord2f(1, 0);
    glVertex3f(zObjet, 0, zObjet);

    glTexCoord2f(0, 0);
    glVertex3f(-zObjet, 0, zObjet);

    glEnd();

    glPopMatrix();

     glBindTexture(GL_TEXTURE_2D, 0);

    glDisable(GL_TEXTURE_2D);
}

void drawVertex(Point3D p1)
{
    //glColor3f(0 / 100.f, 0 / 100.f, p1.z /100.f);
    glVertex3f(p1.x, p1.y, p1.z);
}

void drawVertex2D(Point2D p1)
{
    glVertex2f(p1.x, p1.y);
}

//Dessine un triangle HGHDBG (la diagonale "supérieure" du carré formé par HGHDBGBD)
void drawTriangleTextureSup(Point3D pHG, Point3D pHD, Point3D pBG, GLuint idTexture, float zMax, float zMin)
{
    float couleur = maxValue(pHG.z, pHD.z, pBG.z);
    glColor3f((zMax - zMin) / couleur, (zMax - zMin) / couleur, 0);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, idTexture);
    glBegin(GL_TRIANGLES);
        glTexCoord2f(0., 0.);
        drawVertex(pHG);

        glTexCoord2f(1., 0.);
        drawVertex(pHD);

        glTexCoord2f(0., 1.);
        drawVertex(pBG);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
}

//Dessine un triangle BGBDHD (la diagonale "inférieure du carré")
void drawTriangleTextureInf(Point3D pBG, Point3D pBD, Point3D pHD, GLuint idTexture, float zMax, float zMin)
{
    float couleur = maxValue(pBD.z, pHD.z, pBG.z);
    glColor3f((zMax - zMin) / couleur, (zMax - zMin) / couleur, 0);
    //std::cout << couleur/(zMax - zMin)<<std::endl;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, idTexture);
    glBegin(GL_TRIANGLES);

        glTexCoord2f(0., 1.);
        drawVertex(pBG);

        glTexCoord2f(1., 1.);
        drawVertex(pBD);

        glTexCoord2f(1., 0.);
        drawVertex(pHD);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
}

void drawCamera(Triangle *triangle)
{
    glColor3f(1, 0, 1);
    glBegin(GL_TRIANGLES);
    drawVertex2D(triangle->p1);
    drawVertex2D(triangle->p2);
    drawVertex2D(triangle->p3);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
}

void drawTerrainTexture(Node *quadTree, GLuint idTexture, Triangle *triangleCamera, float zMax, float zMin)
{
    if (intersectionCameraNode(triangleCamera, quadTree))
    {
        if (isLeaf(quadTree))
        {

            //afficherNode(quadTree);
            drawTriangleTextureSup(quadTree->pointHG, quadTree->pointHD, quadTree->pointBG, idTexture, zMax, zMin);
            drawTriangleTextureInf(quadTree->pointBG, quadTree->pointBD, quadTree->pointHD, idTexture, zMax, zMin);
        }

        else
        {
            if (quadTree->nodeHG != nullptr)
                drawTerrainTexture(quadTree->nodeHG, idTexture, triangleCamera, zMax, zMin);
            if (quadTree->nodeHD != nullptr)
                drawTerrainTexture(quadTree->nodeHD, idTexture, triangleCamera, zMax, zMin);
            if (quadTree->nodeBG != nullptr)
                drawTerrainTexture(quadTree->nodeBG, idTexture, triangleCamera, zMax, zMin);
            if (quadTree->nodeBD != nullptr)
                drawTerrainTexture(quadTree->nodeBD, idTexture, triangleCamera, zMax, zMin);
        }
    }
}

void drawQuadTreeTexture(Node *quadtree, GLuint idtexture) {}

//Fonction qui dessine un terrain sans texture : non utilisé

void drawQuadTreeFil(Node *quadTree)
{
    // if (intersectionCameraNode(quadTree))
    // {
    if (isLeaf(quadTree))
        glColor3f(0, 1, 1);

    else
        glColor3f(1, 0, 1);

    glBegin(GL_QUADS);
    drawVertex(quadTree->pointHG);
    drawVertex(quadTree->pointHD);
    drawVertex(quadTree->pointBG);
    drawVertex(quadTree->pointBD);
    glEnd();

    if (quadTree->nodeHG != nullptr)
        drawQuadTreeFil(quadTree->nodeHG);
    if (quadTree->nodeHD != nullptr)
        drawQuadTreeFil(quadTree->nodeHD);
    if (quadTree->nodeBD != nullptr)
        drawQuadTreeFil(quadTree->nodeBD);
    if (quadTree->nodeBG != nullptr)
        drawQuadTreeFil(quadTree->nodeBG);
    // }
}

void drawTerrainFil(Node *quadTree, Triangle *triangleCamera)
{
    //if (intersectionCameraNode(triangleCamera, quadTree))
    //{
        if (isLeaf(quadTree))
        {
            glColor3f(0 / 100.f, 1, 1);
            glBegin(GL_LINES);
            drawVertex(quadTree->pointHG);
            drawVertex(quadTree->pointHD);
            drawVertex(quadTree->pointBG);
            drawVertex(quadTree->pointHG);
            glEnd();
            glColor3f(1, 1, 0);
            glBegin(GL_LINES);
            drawVertex(quadTree->pointBG);
            drawVertex(quadTree->pointBD);
            drawVertex(quadTree->pointHD);
            drawVertex(quadTree->pointBG);
            glEnd();
        }
        else
        {
            if (quadTree->nodeHG != nullptr)
                drawTerrainFil(quadTree->nodeHG, triangleCamera);
            if (quadTree->nodeHD != nullptr)
                drawTerrainFil(quadTree->nodeHD, triangleCamera);
            if (quadTree->nodeBG != nullptr)
                drawTerrainFil(quadTree->nodeBG, triangleCamera);
            if (quadTree->nodeBD != nullptr)
                drawTerrainFil(quadTree->nodeBD, triangleCamera);
        }
  //  }
}