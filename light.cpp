#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include "light.h"


// Dessinne la Skybox à chaque boucle
void drawSkybox(GLuint* textureCielID, float lenght) { 

    float couleurActuelle[4];
    glGetFloatv(GL_CURRENT_COLOR, couleurActuelle);
    float l = lenght/2; // Taille du cube
    glEnable(GL_TEXTURE_2D);
   
    
    glColor3f(120, 120, 120);
        
    glBindTexture(GL_TEXTURE_2D, textureCielID[5]);
    glBegin(GL_QUADS);
        glTexCoord2f(1.,1.); glVertex3f(-l , l, -l);
        glTexCoord2f(1.,0.); glVertex3f(-l , l, l);
        glTexCoord2f(0.,0.); glVertex3f(l , l, l);
        glTexCoord2f(0.,1.); glVertex3f(l , l, -l);
    glEnd();

   /* ================ 4 | FLOOR ================ */ 
    glBindTexture(GL_TEXTURE_2D, textureCielID[4]);
    glBegin(GL_QUADS);
        
        glTexCoord2f(1.,0.); glVertex3f(-l , -l, -l);
        glTexCoord2f(0.,0.); glVertex3f(l , -l, -l);
        glTexCoord2f(0.,1.); glVertex3f(l , -l, l);
        glTexCoord2f(1.,1.);glVertex3f(-l , -l, l);
    glEnd();
    /* ================ 0 | Back ================ */    
    glBindTexture(GL_TEXTURE_2D, textureCielID[0]);
    glBegin(GL_QUADS);
        glTexCoord2f(1.,1.); glVertex3f(l , -l, l);
        glTexCoord2f(1.,0.); glVertex3f(l , l, l);
        glTexCoord2f(0.,0.); glVertex3f(-l , l, l);
        glTexCoord2f(0.,1.); glVertex3f(-l , -l, l);
    glEnd();
    

    /* ================ 1 | Front ================ */    
    glBindTexture(GL_TEXTURE_2D, textureCielID[1]);
    glBegin(GL_QUADS);
        
        glTexCoord2f(1.,1.); glVertex3f(-l , -l, -l);
        glTexCoord2f(1.,0.); glVertex3f(-l , l, -l);
        glTexCoord2f(0.,0.); glVertex3f(l , l, -l);
        glTexCoord2f(0.,1.); glVertex3f(l , -l, -l);
    glEnd();

    
   /* ================ 2 | Left ================ */
    glBindTexture(GL_TEXTURE_2D, textureCielID[2]);
    glBegin(GL_QUADS);
        glTexCoord2f(1.,1.); glVertex3f(l , -l, -l);
        glTexCoord2f(1.,0.); glVertex3f(l , l, -l);
        glTexCoord2f(0.,0.); glVertex3f(l , l, l);
        glTexCoord2f(0.,1.); glVertex3f(l , -l, l);
    glEnd();



   /* ================ 3 | Right ================ */ 
    glBindTexture(GL_TEXTURE_2D, textureCielID[3]);
    glBegin(GL_QUADS);
        glTexCoord2f(1.,1.);  glVertex3f(-l,-l,l);
        glTexCoord2f(1.,0.); glVertex3f(-l,l,l);
        glTexCoord2f(0.,0.);glVertex3f(-l,l,-l);
        glTexCoord2f(0.,1.); glVertex3f(-l,-l,-l);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);   
}
