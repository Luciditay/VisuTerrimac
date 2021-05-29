#include "terrain.h"
#include "SDL2/SDL.h"
#include "GL/glu.h"
#include "quadtree.h"
#include <iostream>

void drawVertex(Point3D p1)
{ 
    //glColor3f(0 / 100.f, 0 / 100.f, p1.z /100.f);
    glVertex3f(p1.x, p1.y, p1.z);
}

//Dessine un triangle HGHDBG (la diagonale "supérieure" du carré formé par HGHDBGBD)
void drawTriangleTextureSup(Point3D pHG, Point3D pHD, Point3D pBG, GLuint idTexture){
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
void drawTriangleTextureInf(Point3D pBG, Point3D pBD, Point3D pHD, GLuint idTexture){
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

void drawTerrainTexture(Node *quadTree, GLuint idTexture){
    if (isLeaf(quadTree))
    {
        //afficherNode(quadTree);
        drawTriangleTextureSup(quadTree->pointHG, quadTree->pointHD, quadTree->pointBG, idTexture);
        drawTriangleTextureInf(quadTree->pointBG, quadTree->pointBD, quadTree->pointHD, idTexture);        
    }

    else
    {
        if (quadTree->nodeHG != nullptr)
            drawTerrainTexture(quadTree->nodeHG, idTexture);        
        if (quadTree->nodeHD != nullptr)
            drawTerrainTexture(quadTree->nodeHD, idTexture);  
        if (quadTree->nodeBG != nullptr)
            drawTerrainTexture(quadTree->nodeBG, idTexture);  
        if (quadTree->nodeBD != nullptr)
            drawTerrainTexture(quadTree->nodeBD, idTexture);  
    }
}

//Fonction qui dessine un terrain sans texture : non utilisé 
void drawTerrain(Node *quadTree)
{
    if (isLeaf(quadTree))
    {
       //afficherNode(quadTree);
       glColor3f(0 / 100.f, 1, 1);

        glBegin(GL_TRIANGLES);
        drawVertex(quadTree->pointHG);
        drawVertex(quadTree->pointHD);
        drawVertex(quadTree->pointBG);
        glColor3f(1, 1, 0);
        drawVertex(quadTree->pointBG);
        drawVertex(quadTree->pointBD);        
        drawVertex(quadTree->pointHD);
        glEnd();
        
    }

    else
    {
        if (quadTree->nodeHG != nullptr)
            drawTerrain(quadTree->nodeHG);        
        if (quadTree->nodeHD != nullptr)
            drawTerrain(quadTree->nodeHD);
        if (quadTree->nodeBG != nullptr)
            drawTerrain(quadTree->nodeBG);
        if (quadTree->nodeBD != nullptr)
            drawTerrain(quadTree->nodeBD);
    }
}

void drawTerrainFil(Node* quadTree){
    if (isLeaf(quadTree)){
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
            drawTerrainFil(quadTree->nodeHG);        
        if (quadTree->nodeHD != nullptr)
            drawTerrainFil(quadTree->nodeHD);
        if (quadTree->nodeBG != nullptr)
            drawTerrainFil(quadTree->nodeBG);
        if (quadTree->nodeBD != nullptr)
            drawTerrainFil(quadTree->nodeBD);
    }
}