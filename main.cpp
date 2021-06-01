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
#include "geometry.h"
#include "light.h"

using namespace std;

int nbreArbres = 10;

bool drawFil = false;      //True ==> Dessiner en mode fil de fer
bool drawQuadtree = false; //Dessiner le quadTree
bool frustum=false; //On applique le frustum ou non
bool affichageCamera = false;

bool modeCameraFixe = false; //Camera a 1 mètre du sol

float distanceSol = 1.;

static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;

int main(int argc, char const *argv[])
{
    const string fichierParams = (string)argv[1]; //Fichiers parametre.timac renommé pour tester
    const string texturePath1 = "texture/sable.jpg";
    const string textureArbrePath = "texture/palmier.png";

    srand(time(NULL));

    string heightMapFile;
    int xSize, ySize, zMin, zMax;
    float zNear, zFar, fov;

    loadParams(fichierParams, &heightMapFile, &xSize, &ySize, &zMin, &zMax, &zNear, &zFar, &fov);

    int xArbre[nbreArbres];
    int yArbre[nbreArbres];

    for (int i = 0; i < nbreArbres; i++)
    {
        xArbre[i] = rand() % xSize + 20;
        yArbre[i] = rand() % ySize + 20;
    }

    float imageTab[xSize * ySize];
    Image *image = (Image *)malloc(sizeof(Image));

    image->pixels = new float *[xSize];
    for (int i = 0; i < xSize; i++)
    {
        image->pixels[i] = new float[ySize];
    }

    loadHeightMap(heightMapFile, imageTab, xSize * ySize, zMin, zMax, image);
    tabToMatrice(imageTab, image->pixels, xSize, ySize);
   
    Node quadTree;
    initialiserQuadTree(&quadTree, image->pixels, xSize, ySize);

    loadSDL();
    int IDText = 0;
    SDL_Window *window = createWindow(800, 600);
    SDL_GLContext context = createContext(window);


    // Chargement des textures et de la skyBox

    SDL_Surface *texture1 = loadTexture(texturePath1);
    GLuint idTexture = textureCarteGraphique(texture1, &IDText);

    SDL_Surface *textureArbre = loadTexture(textureArbrePath);
    GLuint idTextArbre = textureCarteGraphique(textureArbre, &IDText);

    const char *images_paths[6] = {
        "./Skybox/left.jpg",
        "./Skybox/right.jpg",
        "./Skybox/top.jpg",
        "./Skybox/bottom.jpg",
        "./Skybox/front.jpg",
        "./Skybox/back.jpg",
    };

    GLuint idTextureCiel[6];
    SDL_Surface* FaceCube;

    for (int i = 0; i < 6; ++i)
    {
        FaceCube = loadTexture(images_paths[i]);
        idTextureCiel[i] = textureCarteGraphique(FaceCube, &IDText);
    }

    /* FIN PARTIE SKYBOX */

    onWindowResized(800, 600, zFar, zNear, fov);

    Point3D camPos = {(float) xSize, (float) ySize,(float) zMax};    //Position de la camera
    Point3D lookAtDirection = {10, 10, -0.8}; //Vecteur directeur (dans quelle direction regarde la camera)
    Point3D vectCamera;
    Triangle *champCamera = (Triangle *)malloc(sizeof(Triangle));
    float zTerrain;

    int loop = 1;
    while (loop)
    {
        glClearColor(1, 1, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDisable(GL_LIGHTING);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glEnable(GL_DEPTH_TEST);
            glDepthMask(GL_FALSE);
            drawSkybox(idTextureCiel, 5);
            glDepthMask(GL_TRUE);
         glDisable(GL_DEPTH_TEST);

        if (modeCameraFixe){
                zTerrain = image->pixels[(int) camPos.y %ySize][(int) camPos.x%xSize] + 10;
            
            camPos = {camPos.x, camPos.y, zTerrain};
            afficherPoint3D(camPos);
            cout <<endl;
        }

        vectCamera = add3DVect(camPos, lookAtDirection);
        champCamera = triangleCamera(sub3DVect({(float)xSize / 2, (float)ySize / 2}, camPos), lookAtDirection, fov, zFar);
        gluLookAt(camPos.x, camPos.y, camPos.z, vectCamera.x, vectCamera.y, vectCamera.z, 0, 0, 1);

        glTranslatef(image->w / 2.f, image->h / 2.f, 0);
        glScalef((float)xSize / image->w, (float)ySize / image->h, 1.);

        if (drawFil)
        {
            if (drawQuadtree)
                drawQuadTreeFil(&quadTree, frustum);
            else
                drawTerrainFil(&quadTree, champCamera, frustum);
        }

        else
        {
            if (drawQuadtree)
                drawQuadTreeTexture(&quadTree, idTexture, frustum);
            else
                drawTerrainTexture(&quadTree, idTexture, champCamera, zMax, zMin, frustum);
        }

        for (int i = 0; i < nbreArbres; i++)
        {
            if (frustum){
                if (isInTriangle(champCamera, {(float)xArbre[i], (float)yArbre[i]}))
                    drawObjet(idTextArbre, xArbre[i], yArbre[i], image->pixels, 0, 5, zMin);
            }
            else{
                drawObjet(idTextArbre, xArbre[i], yArbre[i], image->pixels, 0, 5, zMin);
            }
            
        }

       if (affichageCamera)
            afficherTriangle(champCamera);

        Uint32 startTime = SDL_GetTicks();

        Uint32 elapsedTime = SDL_GetTicks() - startTime;
        /* Si trop peu de temps s'est ecoule, on met en pause le programme */
        if (elapsedTime < FRAMERATE_MILLISECONDS)
        {
            SDL_Delay(FRAMERATE_MILLISECONDS - elapsedTime);
        }

        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            switch (e.type)
            {

            case SDL_QUIT:
                loop = 0;
                break;
            /* Clic souris */
            case SDL_MOUSEBUTTONUP:
                //printf("clic en (%d, %d)\n", e.button.x, e.button.y);
                break;

            /* Touche clavier */
            case SDL_KEYDOWN:

                /* Check the SDLKey values and move change the coords */
                switch (e.key.keysym.sym)
                {
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
                    lookAtDirection.x += 0.3;
                    break;

                case SDLK_l:
                    lookAtDirection.x -= 0.3;
                    break;

                case SDLK_LEFT:
                    lookAtDirection.y -= 0.3;
                    break;

                case SDLK_RIGHT:
                    lookAtDirection.y += 0.3;
                    break;

                case SDLK_UP:
                    lookAtDirection.z += 0.3;
                    break;

                case SDLK_DOWN:
                    lookAtDirection.z -= 0.3;
                    break;

                case SDLK_f:
                    drawFil = !drawFil; //Active/Désactive le mode draw fil en appuyant sur f
                    break;

                case SDLK_g:
                    frustum=!frustum;
                    break;

                case SDLK_i:
                    modeCameraFixe=!modeCameraFixe;
                    break;

                case SDLK_c:
                    affichageCamera=!affichageCamera;
                    break;

                case SDLK_n:
                    drawQuadtree = !drawQuadtree;
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
    glDeleteTextures(1, &idTexture);
    SDL_Quit();

    return EXIT_SUCCESS;
}
