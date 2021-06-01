#ifndef GETFILE_H
#define GETFILE_H

#include <string>

struct Image
{
    float **pixels;
    int w;
    int h;
};


void tabToMatrice(float* tab, float** matrice, int width, int height);
void loadParams(const std::string fichierParams, std::string* heightMapFile, int *width, int *height, int *zMin, int *zMax, float *zNear, float *zFar, float *fov);
void loadHeightMap(std::string heightMapPath, float* imageTab, int largeurImage, int zMin, int zMax, Image* image);
float mapValue(float value, float istart, float istop, float ostart, float ostop);

#endif 