#include "SDL2/SDL.h"
#include "GL/glu.h"
#include "stdlib.h"
#include "stdio.h"
#include "iostream"
#include "quadtree.h"
#include "getFile.h"
#include <fstream>
#include <string>
#include "terrain.h"
#include "loadSDL.h"
#include "camera.h"

using namespace std;


bool drawFil = false; //True ==> Dessiner en mode fil de fer

static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;

int main(int argc, char const *argv[])
{
    const string fichierParams = "parameters"; //Fichiers parametre.timac renommé pour tester 
    const string texturePath1 = "texture/texture_grass_freepik.jpg";
    const string texturePath2 = "texture/texture_Bleu_Chelou.jpg";
    const string texturePath3 = "texture/texture_beige.jpg";
    
    //Pour changer la texture renommer la texture désiré en texturePath1
    string heightMapFile;
    int xSize, ySize, zMin, zMax;
    float zNear, zFar, fov;
    
    loadParams(fichierParams, &heightMapFile, &xSize, &ySize, &zMin, &zMax, &zNear, &zFar, &fov);
    
    float imageTab[xSize * ySize];
    Image* image = (Image*) malloc(sizeof(Image));

    image->pixels = new float *[xSize];
    for (int i = 0; i < xSize; i++)
    {
        image->pixels[i] = new float[ySize];
    }

    loadHeightMap(heightMapFile, imageTab, xSize * ySize, zMin, zMax, image);
    tabToMatrice(imageTab, image->pixels, xSize, ySize);  
    Node quadTree;
    initialiserQuadTree(&quadTree, image->pixels, xSize, ySize);
    //parcourirQuad(&quadTree);
    loadSDL();
    int IDText = 0;
    SDL_Window* window = createWindow(800, 600);
    SDL_GLContext context=createContext(window);

    SDL_Surface* texture1 = loadTexture(texturePath1);
    GLuint idTexture = textureCarteGraphique(texture1, &IDText);

    //Pour loader plsuieurs textures : later !
    // GLuint idTexture = textureCarteGraphique(texture2, &IDText);
    
    // SDL_Surface* texture3 = loadTexture(texturePath3);
    // GLuint idTexture = textureCarteGraphique(texture3, &IDText);
    
    
    onWindowResized(800, 600, zFar, zNear, fov);

    Point3D camPos = {xSize, ySize, zMax}; //Position de la camera
    Pointf3D lookAtDirection = {10, 10, -0.8}; //Vecteur directeur (dans quelle direction regarde la camera)

    cout << image->w << image->h<<endl;

    int loop=1;
    while(loop){
        glClearColor(1, 1, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        glDisable(GL_LIGHTING);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(camPos.x, camPos.y, camPos.z, camPos.x + lookAtDirection.x, camPos.y + lookAtDirection.y, camPos.z + lookAtDirection.z, 0, 0, 1);
        glScalef((float) xSize / image->w, (float) ySize / image->h, 1.);
        // cout << (float) width/image->w<<endl <<endl;
        // cout << (float) height / image->h<<endl;
        glTranslatef(image->w / 2.f, image->h / 2.f, 0);
        
         
        if (drawFil)
            drawTerrainFil(&quadTree);

        else 
            drawTerrainTexture(&quadTree, idTexture);

        Uint32 startTime = SDL_GetTicks();

        Uint32 elapsedTime = SDL_GetTicks() - startTime;
        /* Si trop peu de temps s'est ecoule, on met en pause le programme */
        if(elapsedTime < FRAMERATE_MILLISECONDS) 
        {
            SDL_Delay(FRAMERATE_MILLISECONDS - elapsedTime);
        }

        SDL_Event e;
        while(SDL_PollEvent(&e)) 
        {           
            switch(e.type) 
            {

                case SDL_QUIT:
                    loop=0;
                    break;
                /* Clic souris */
                case SDL_MOUSEBUTTONUP:
                    //printf("clic en (%d, %d)\n", e.button.x, e.button.y);
                    break;
                
                /* Touche clavier */
                case SDL_KEYDOWN:                    
                    //printf("touche pressee (code = %d)\n", e.key.keysym.sym);

                    /* Check the SDLKey values and move change the coords */
                    switch(e.key.keysym.sym ){
                        //Gestion Camera (position de la camera et changement du vecteur directeur de la camera)
                        case SDLK_z:
                            camPos.x++;
                            break;
                        case SDLK_q:
                            camPos.y++;
                            break;
                        case SDLK_s:
                            camPos.x--;
                            break;
                        case SDLK_d:
                            camPos.y--;
                            break;

                        case SDLK_SPACE:
                            camPos.z++;
                            break;

                        case SDLK_x:
                            camPos.z--;
                            break;

                        case SDLK_m:
                            lookAtDirection.x+=0.3;
                            break;

                        case SDLK_l:
                            lookAtDirection.x-=0.3;
                            break;

                        case SDLK_LEFT:
                            lookAtDirection.y+=0.3;
                            break;

                        case SDLK_RIGHT:
                            lookAtDirection.y-=0.3;
                            break;

                        case SDLK_UP:
                            lookAtDirection.z+=0.3;
                            break;

                        case SDLK_DOWN:
                            lookAtDirection.z-=0.3;
                            break;
                        
                        case SDLK_f:
                            drawFil=!drawFil; //Active/Désactive le mode draw fil en appuyant sur f
                            break;
                    }
                    break;
                
                case SDL_WINDOWEVENT:
                    switch (e.window.event) 
                    {
                        /* Redimensionnement fenetre */
                        case SDL_WINDOWEVENT_RESIZED:
                            onWindowResized(e.window.data1, e.window.data2, zFar, zNear, fov);                
                            break;

                        default:
                            break; 
                    }
                    break;

                default:
                    break;
            }
        }

        SDL_GL_SwapWindow(window);
     }

    /* Liberation des ressources associees a la SDL */
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_FreeSurface(texture1);
    glDeleteTextures(1, &idTexture);
    SDL_Quit();
    
    return EXIT_SUCCESS;
}
