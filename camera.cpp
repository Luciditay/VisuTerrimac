#include "math.h"
#include "quadtree.h"
#include "geometry.h"
#include "camera.h"

Point3D directionCamera(float theta, float phi){
    return {cos(theta)*sin(phi),
            sin(theta)*sin(phi),
            cos(theta) // Theta ==> Angle du z donc haut en bas
    };
}



    //Obtenu en calculant le vecteur normal au plan formé par les vecteurs (LookAt et Left)

//     vecteurUP->x = tan(theta + M_PI/2); //Par résolution de l'équation {Left*normal = 0, LookAT*normal = 0} 
//     vecteurUP->y = 1;
//     vecteurUP->z = tan(theta)*cos(theta)*tan(phi) - sin(theta)*tan(phi); 
// }
// float ro = norme(pointRegard.x, pointRegard.y, pointRegard.z);
//     float theta = acos(pointRegard.z/ro);
//     float phi = atan(pointRegard.y/pointRegard.x);