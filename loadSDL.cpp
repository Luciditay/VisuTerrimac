#include "SDL2/SDL.h"
#include <iostream>
#include "loadSDL.h"
#include "GL/glu.h"
#include "SDL2/SDL_image.h"

void loadSDL(){
    if (SDL_Init(SDL_INIT_VIDEO)<0){
        std::cout << "Cant load SDL_VIDEO"<<std::endl;
        SDL_Quit();
        exit(EXIT_FAILURE);
    }
}

SDL_Window* createWindow(int width, int height){
    SDL_Window* window = SDL_CreateWindow(
        "",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        width, height,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

        if(NULL == window) 
        {
            const char* error = SDL_GetError();
            fprintf(
                stderr,
                "Erreur lors de la creation de la fenetre : %s\n", error);

            SDL_Quit();
            exit(EXIT_FAILURE);
        }
        return window;
}

SDL_GLContext createContext(SDL_Window* window)
{
     SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

            SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

        SDL_GLContext context = SDL_GL_CreateContext(window);
    
        if(NULL == context) 
        {
            const char* error = SDL_GetError();
            fprintf(
                stderr,
                "Erreur lors de la creation du contexte OpenGL : %s\n", error);

            SDL_DestroyWindow(window);
            SDL_Quit();
            exit(EXIT_FAILURE);
        }
        return context;
}

void onWindowResized(unsigned int width, unsigned int height, float zFar, float zNear, float fov)
{ 
    float aspectRatio = width / (float) height;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fov, aspectRatio , zNear, zFar);
}

SDL_Surface* loadTexture(std::string texturePath){
    SDL_Surface* texture = IMG_Load(texturePath.c_str());

    if (!texture){
        std::cout << "Problème de loading de la texture" << std::endl;
        SDL_FreeSurface(texture);
        exit(EXIT_FAILURE);
    }
    return texture;
}

//On passe la texture à la carte graphique ==> cf TD4
GLuint textureCarteGraphique(SDL_Surface* texture, int* IDTexture){
    GLuint idTexture = *IDTexture;

    GLenum format;
    switch(texture->format->BytesPerPixel) {
        case 1:
            format = GL_RED;
            break;
        case 3:
            format = GL_RGB;
            break;
        case 4:
            format = GL_RGBA;
            break;
        default:
            fprintf(stderr, "Format des pixels de l'image non supporte.\n");
            exit(EXIT_FAILURE);
    }

    glGenTextures(1, &idTexture);
    glBindTexture(GL_TEXTURE_2D, idTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGBA8, 
        texture->w, texture->h, 0,
        format, GL_UNSIGNED_BYTE, texture->pixels
    );
    glBindTexture(GL_TEXTURE_2D, 0);
    *IDTexture++;
    return idTexture;
}

