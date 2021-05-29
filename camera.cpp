#include "camera.h"
#include "math.h"
#include "quadtree.h"

Point2D add2DVect(Point2D p1, Point2D p2){
    Point2D add = {
        p1.x + p2.x,
        p1.y + p2.y,
    };
    return add;
}

Point2D scale2DVect(Point2D p1, float scalaire){
    return {
        p1.x/scalaire,
        p1.y/scalaire
    };
}

// Projecte un point en 3d sur le plan OXY
Point2D projection2D(Point3D p1){
    return {p1.x, p1.y};
}

float norme2D(Point2D p1){
    return sqrt(p1.x*p1.x + p1.y * p1.y);
}

float norme3D(float x, float y, float z){
    return sqrt(x*x + y*y + z*z);
}

//On calcule le vecteur lookAt de la camera et vecteurUP (cf sujet), coordonnees cylindriques
// Problème de conversion maybe ?
//La camera est l'origine du repère
// Ca sert à rien pour l'instant 
void vecteurCamera(Pointf3D pointRegard, Pointf3D* vecteurUP, Pointf3D* lookAt){
    float ro = norme3D(pointRegard.x, pointRegard.y, pointRegard.z);
    float theta = acos(pointRegard.z/ro);
    float phi = atan(pointRegard.y/pointRegard.x);

    lookAt->x = cos(theta)*sin(phi);
    lookAt->y = sin(theta)*sin(phi);
    lookAt->z = cos(theta);

    //Obtenu en calculant le vecteur normal au plan formé par les vecteurs (LookAt et Left)
    
    vecteurUP->x = tan(theta + M_PI/2); //Par résolution de l'équation {Left*normal = 0, LookAT*normal = 0} 
    vecteurUP->y = 1;
    vecteurUP->z = tan(theta)*cos(theta)*tan(phi) - sin(theta)*tan(phi); 
}

//Vive la Trigo !
Triangle* triangleCamera(Point3D camPos, Point3D vectCamera, float fov, float zFar){
    Point2D projection2DVectCamera = projection2D(vectCamera); //==> (a, b)
    Point2D projection2DcamPos = projection2D(camPos); 
    Point2D normaleVectCamera = {-vectCamera.y, vectCamera.x}; //==> (-b, a)

    float scalaire = (tan(fov/2)*zFar)/(norme2D(normaleVectCamera));
    Point2D normalScalaire = scale2DVect(normaleVectCamera, scalaire);
    Point2D normalScalaire2 = scale2DVect(normaleVectCamera, -scalaire);
    
    Point2D transition = add2DVect(projection2DcamPos, projection2DVectCamera);

    //On a un triangle formant le champ de vue de la camera
    Triangle* triangleCamera;
    triangleCamera->p1 = projection2DcamPos;
    triangleCamera->p2 = add2DVect(transition, normalScalaire);
    triangleCamera->p3 = add2DVect(transition, normalScalaire2);
    return triangleCamera;
}

//Reste à calculer les intersections entre les quad et la camera