#ifndef CAMERA_H
#define CAMERA_H

struct Pointf3D{
    float x;
    float y;
    float z;
};

typedef struct Point2D{
    float x;
    float y;
};

struct Triangle{
    Point2D p1;
    Point2D p2;
    Point2D p3;
};

Point2D add2DVect(Point2D p1, Point2D p2);

Point2D scale2DVect(Point2D p1, float scalaire);

float norme2D(float x, float y);

float norme3D(float x, float y, float z);

void vecteurCamera(Pointf3D pointRegard, Pointf3D* vecteurUP, Pointf3D* lookAt);

Triangle* triangleCamera(Pointf3D camPos, Pointf3D vectCamera, float fov, float zFar);

#endif