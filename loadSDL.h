#ifndef LOADSDL_H
#define LOADSDL_H
#include "SDL2/SDL.h"
#include "GL/glu.h"

void loadSDL();
SDL_Window* createWindow(int width, int height);
SDL_GLContext createContext(SDL_Window* window);
void onWindowResized(unsigned int width, unsigned int height, float zFar, float zNear, float fov);
SDL_Surface* loadTexture(std::string texturePath);
GLuint textureCarteGraphique(SDL_Surface* texture, int* IDTexture);

#endif