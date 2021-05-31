#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include "light.h"


// Dessinne la Skybox à chaque boucle
void drawSkybox(float cam_yaw, float cam_pitch) { 

    float t = 1.0; // Taille du cube
    GLuint cubemap_text_ID;
    glGenTextures(1, &cubemap_text_ID);
    glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, cubemap_text_ID);
    
    glPushMatrix();
        //Camera
        glLoadIdentity();
        glRotatef( cam_pitch, 1.0, 0.0, 0.0 );
        glRotatef( cam_yaw, 0.0, 1.0, 0.0 );    
        
        
        // Rendu
        glBegin(GL_TRIANGLE_STRIP); // Face X Négatif        
            glTexCoord3f(-t,-t,-t); glVertex3f(-t,-t,-t);     
            glTexCoord3f(-t,t,-t); glVertex3f(-t,t,-t);
            glTexCoord3f(-t,-t,t); glVertex3f(-t,-t,t);
            glTexCoord3f(-t,t,t); glVertex3f(-t,t,t);
        glEnd();
        
        glBegin(GL_TRIANGLE_STRIP); // Face X Positif
            glTexCoord3f(t, -t,-t); glVertex3f(t,-t,-t);
            glTexCoord3f(t,-t,t); glVertex3f(t,-t,t);
            glTexCoord3f(t,t,-t); glVertex3f(t,t,-t); 
            glTexCoord3f(t,t,t); glVertex3f(t,t,t);     
        glEnd();
        
        glBegin(GL_TRIANGLE_STRIP); // Face Y Négatif    
            glTexCoord3f(-t,-t,-t); glVertex3f(-t,-t,-t);
            glTexCoord3f(-t,-t,t); glVertex3f(-t,-t,t);
            glTexCoord3f(t, -t,-t); glVertex3f(t,-t,-t);
            glTexCoord3f(t,-t,t); glVertex3f(t,-t,t);
        glEnd();
        
        glBegin(GL_TRIANGLE_STRIP); // Face Y Positif        
            glTexCoord3f(-t,t,-t); glVertex3f(-t,t,-t);
            glTexCoord3f(t,t,-t); glVertex3f(t,t,-t); 
            glTexCoord3f(-t,t,t); glVertex3f(-t,t,t);
            glTexCoord3f(t,t,t); glVertex3f(t,t,t);     
        glEnd();
        
        glBegin(GL_TRIANGLE_STRIP); // Face Z Négatif        
            glTexCoord3f(-t,-t,-t); glVertex3f(-t,-t,-t);
            glTexCoord3f(t, -t,-t); glVertex3f(t,-t,-t);
            glTexCoord3f(-t,t,-t); glVertex3f(-t,t,-t);
            glTexCoord3f(t,t,-t); glVertex3f(t,t,-t); 
        glEnd();
        
        glBegin(GL_TRIANGLE_STRIP); // Face Z Positif    
            glTexCoord3f(-t,-t,t); glVertex3f(-t,-t,t);
            glTexCoord3f(-t,t,t); glVertex3f(-t,t,t);
            glTexCoord3f(t,-t,t); glVertex3f(t,-t,t);
            glTexCoord3f(t,t,t); glVertex3f(t,t,t);     
        glEnd();
    glPopMatrix();
}



Light createLight(Point3D position, ColorRGB color) {
    Light l;
    l.position = position;
    l.color = color;
    return l;
}