#include "math.h"
#include "quadtree.h"
#include "geometry.h"
#include "camera.h"

bool isInTriangle(Triangle *triangle, Point2D p1)
{
    Point2D Vect1 = sub2DVect(p1, triangle->p1); //On calcule le vecteur posCameraPoint
    Point2D Vect2 = sub2DVect(p1, triangle->p2);
    Point2D Vect3 = sub2DVect(p1, triangle->p3);

    Point2D tVect1 = sub2DVect(triangle->p1, triangle->p2);
    Point2D tVect2 = sub2DVect(triangle->p2, triangle->p3);
    Point2D tVect3 = sub2DVect(triangle->p3, triangle->p1);

    return determinant(tVect1, Vect1) > 0 && determinant(tVect2, Vect2) > 0 && determinant(tVect3, Vect3) > 0;
}

bool intersectionSegment(Point2D pA, Point2D pB, Point2D pC, Point2D pD)
{
    Point2D VecteurAB = add2DVect(pB, scale2DVect(pA, -1)); // AB = B - A
    Point2D VecteurCD = add2DVect(pD, scale2DVect(pC, -1));
    float diviseur = (VecteurAB.x * VecteurCD.y - VecteurAB.y * VecteurCD.x);
    float paramAB, paramCD;

    if (diviseur != 0)
    { //Droite non parallèle
        paramCD = (VecteurAB.x * pA.y - VecteurAB.x * pC.y - VecteurAB.y * pA.x + VecteurAB.y * pC.x) / diviseur;

        paramAB = (VecteurCD.x * pA.y - VecteurCD.x * pC.y - VecteurCD.y * pA.x + VecteurCD.y * pC.x) / diviseur;
    }
    return (1 > paramAB && paramAB > 0 && 1 > paramCD && paramCD > 0);
    //Param AB et paramCD entre 0 et 1 exclus ==> Les deux segments se croisent
    // Résolution d'un système d'équation paramètrique de deux droites 2D
}

// On calcule les intersections de chaque côté du triangle avec chaque côté du carré, ce qui donne
// 3*4 tests


bool intersectionCameraNode(Triangle *triangle, Node *node)
{
    Point2D pHG = projection2D(node->pointHG);
    Point2D pHD = projection2D(node->pointHD);
    Point2D pBG = projection2D(node->pointBG);
    Point2D pBD = projection2D(node->pointBD);

    bool intersec = isInTriangle(triangle, pHG) ||
                    isInTriangle(triangle, pHD) ||
                    isInTriangle(triangle, pBG) ||
                    isInTriangle(triangle, pBD);

    bool a = intersectionSegment(triangle->p1, triangle->p2, pHG, pHD); //Intersec carré, premier côté du triangle
    bool b = intersectionSegment(triangle->p1, triangle->p2, pHD, pBD);
    bool c = intersectionSegment(triangle->p1, triangle->p2, pBD, pBG);
    bool d = intersectionSegment(triangle->p1, triangle->p2, pBG, pHG);

    bool e = intersectionSegment(triangle->p1, triangle->p3, pHG, pHD); //Intersec carré, deuxieme côté du triangle
    bool f = intersectionSegment(triangle->p1, triangle->p3, pHD, pBD);
    bool g = intersectionSegment(triangle->p1, triangle->p3, pBD, pBG);
    bool h = intersectionSegment(triangle->p1, triangle->p3, pBG, pHG);

    bool i = intersectionSegment(triangle->p2, triangle->p3, pHG, pHD); //Intersec carré, troisieme côté du triangle
    bool j = intersectionSegment(triangle->p2, triangle->p3, pHD, pBD);
    bool k = intersectionSegment(triangle->p2, triangle->p3, pBD, pBG);
    bool l = intersectionSegment(triangle->p2, triangle->p3, pBG, pHG);

    return (intersec || a || b || c || d || e || f || g || h || i || j || k || l);
}

Triangle *triangleCamera(Point3D camPos, Point3D lookAtVector, float fov, float zFar)
{

    Point2D projection2DVectCamera = projection2D(lookAtVector); //==> (a, b)
    Point2D projection2DcamPos = projection2D(camPos);
    Point2D normaleVectCamera = {-lookAtVector.y, lookAtVector.x}; //==> (-b, a)

    fov = M_PI*fov/180;
    float scalaire = (tan(fov / 2) * zFar) / (norme2D(normaleVectCamera));
    Point2D normalScalaire = scale2DVect(normaleVectCamera, scalaire);

    Point2D normalScalaire2 = scale2DVect(normaleVectCamera, -scalaire);
    
    Point2D LookNormalise = scale2DVect(projection2DVectCamera, zFar/(norme2D(projection2DVectCamera)));
    Point2D transition = add2DVect(projection2DcamPos, LookNormalise);

    //On a un triangle formant le champ de vue de la camera
    Triangle *triangleCamera = (Triangle *)malloc(sizeof(Triangle));
    triangleCamera->p1 = projection2DcamPos;
    triangleCamera->p2 = add2DVect(transition, normalScalaire);
    triangleCamera->p3 = add2DVect(transition, normalScalaire2);
    return triangleCamera;
}

