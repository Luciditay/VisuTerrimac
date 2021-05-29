#ifndef QUADTREE_H
#define QUADTREE_H

#include "geometry.h"
#include "quadtree.h"

struct Point2D;
struct Node;

struct Point3D {
    float x;
    float y;
    float z;
};
 
typedef struct Node// Each node contains 4 points forming the border of the square it covers. If it is a leaf ==> the four nodes will be set to NULL, 
               // and the four point will be 4 pixels                         
{
    Point3D pointHG;  // *-----*
    Point3D pointHD;  // |     |
    Point3D pointBG;  // *-----* ==> In the node related to this area, we store the coordonnates of the four points represented by "*"
    Point3D pointBD;

    Node* nodeHG = nullptr;
    Node* nodeHD = nullptr;
    Node* nodeBG = nullptr;
    Node* nodeBD = nullptr;
} Node; // ==> One quadTree : The node, whose 4 points will be the corner of the heigt map

Point3D milieuSegment(Point3D p1, Point3D p2, float** image);

void afficherNode(Node* node);
void afficherPoint3D(Point3D point);
void afficherPoint2D(Point2D p1); 

void remplirNode(Node* node, Point3D pointHG, Point3D pointHD, Point3D pointBG, Point3D pointBD);
void initialiserQuadTree(Node* quadTree, float** image, int longueur, int largeur);
void construireQuadTree(Node* quadTree, float** image, int depth);
bool formeUnSquare(Point3D pHG, Point3D pHD, Point3D pBD, Point3D pBG);
bool isLeaf(Node* node);
void parcourirQuad(Node* node);

#endif