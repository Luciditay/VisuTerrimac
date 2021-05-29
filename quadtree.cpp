#include "quadtree.h"
#include "stdio.h"
#include "stdlib.h"
#include "iostream"
using namespace std;

//Remplit un Node avec quatre points 
void remplirNode(Node* node, Point3D pointHG, Point3D pointHD, Point3D pointBD, Point3D pointBG){
    node->pointHG=pointHG; //On remplit en partant du coin hautGauche du node et on tourne dans le sens horaire
    node->pointHD=pointHD; // Pour les coins
    node->pointBG=pointBG;
    node->pointBD=pointBD;
}

void afficherPoint3D(Point3D point) {
    cout << "{"<<point.x << " "<< point.y << " " <<point.z<< "}";
}

void afficherPoint2D(Point2D point) {
    cout << "{"<<point.x << " "<< point.y << "}"<<endl;
}

void afficherNode(Node* node){
    cout << "HG :"; afficherPoint3D(node->pointHG);
    cout << "HD :"; afficherPoint3D(node->pointHD);
    cout << "BG :";afficherPoint3D(node->pointBG);
    cout<< "BD :";afficherPoint3D(node->pointBD);
    cout<<endl;
}

Point3D milieuSegment(Point3D p1, Point3D p2, float** image){
    Point3D intersection;
    intersection.x = (int) (p1.x + p2.x)/2;
    intersection.y = (int) (p1.y + p2.y)/2;
    intersection.z = image[(int) intersection.y][(int) intersection.x]; //Image ==> Height map
    return intersection;
}

bool estIdentique(Point3D p1, Point3D p2){
    if (p1.x==p2.x && p1.y==p2.y){
        return true;
    }
    return false;
}

//Pour une explication sur le Quad Tree voir quadTree.h
void initialiserQuadTree(Node* quadTree, float** image, int xSize, int ySize){
    xSize--; ySize--;
    Point3D CoinHG = {0, 0, image[0][0]}; // On initialise le quadtree avec les quatres coins de l'image
    Point3D CoinHD = {xSize, 0, image[0][xSize]};
    Point3D CoinBG = {0, ySize, image[ySize][0]};
    Point3D CoinBD = {xSize, ySize, image[ySize][xSize]};  
    remplirNode(quadTree, CoinHG, CoinHD, CoinBD, CoinBG);
    int depth=0; //Pas encore utilisé
    construireQuadTree(quadTree, image, depth);
}

//Cette fonction marche "en théorie" ==>J'ai les bons nodes. Néanmoins l'affichage de la height map est weird jsp pourquoi
void construireQuadTree(Node* quadTree, float** image, int depth){ //NodeHG ==> en haut à gauche, NodeHD ==> En haut à droite, NodeBG ==> Bas Gauche, Node BD ==> Bas Droite
    
    if (estIdentique(quadTree->pointHG, quadTree->pointHD)){ //Les 4 fonctions suivantes servent pr des cas particuliers, compliqué à expliquer par commentaire
        if (quadTree->pointHG.x==0){ //Si HG/HD sont au bord gauche 
            quadTree->pointHD.x++; //On décale d'un cran vers la droite
            quadTree->pointHD.z=image[(int) quadTree->pointHD.y][(int) quadTree->pointHD.x];
        }
        else{
            quadTree->pointHG.x--;
            quadTree->pointHG.z=image[(int) quadTree->pointHG.y][(int) quadTree->pointHG.x];
        }
    }

    if (estIdentique(quadTree->pointHD, quadTree->pointBD)){
        if (quadTree->pointHD.y==0){ //Si BG/HD sont au bord haut 
            quadTree->pointBD.y++; //On décale BD d'un cran vers le bas
            quadTree->pointBD.z=image[(int) quadTree->pointBD.y][(int) quadTree->pointBD.x];
        }
        else{
            quadTree->pointHD.y--; //Sinon on décale HD d'un cran vers le haut
            quadTree->pointHD.z=image[(int) quadTree->pointHD.y][(int) quadTree->pointHD.x];
        }
    }

   if (estIdentique(quadTree->pointBD, quadTree->pointBG)){
        if (quadTree->pointBG.x==0){ //Si BG/HD sont au bord haut 
            quadTree->pointBD.x++; //On décale d'un cran vers le bas
            quadTree->pointBD.z=image[(int) quadTree->pointBD.y][(int) quadTree->pointBD.x];
        }
        else{
            quadTree->pointBG.x--;
            quadTree->pointBG.z=image[(int) quadTree->pointBG.y][(int) quadTree->pointBG.x];
        }
    }

    if (estIdentique(quadTree->pointBG, quadTree->pointHG)){
        if (quadTree->pointHG.y==0){ //Si BG/HD sont au bord haut 
            quadTree->pointBG.y++; //On décale d'un cran vers le bas
            quadTree->pointBG.z=image[(int) quadTree->pointBG.y][(int) quadTree->pointBG.x];
        }
        else{
            quadTree->pointHG.y--;
            quadTree->pointHG.z=image[(int) quadTree->pointHG.y][(int) quadTree->pointHG.x];
        }
    }    
    
    // afficherNode(quadTree);
    // cout <<" fin AfficherNode"<<endl;
    
    if (formeUnSquare(quadTree->pointHG, quadTree->pointHD, quadTree->pointBD, quadTree->pointBG)){ //Si 
        //afficherNode(quadTree);
        quadTree->nodeHG=nullptr;
        quadTree->nodeHD=nullptr;
        quadTree->nodeBD=nullptr;
        quadTree->nodeBG=nullptr;
        
        return ;
    }

    else{    
    quadTree->nodeHG = (Node*) malloc(sizeof(Node));
    quadTree->nodeHD = (Node*) malloc(sizeof(Node));
    quadTree->nodeBG = (Node*) malloc(sizeof(Node));
    quadTree->nodeBD = (Node*) malloc(sizeof(Node));

    Point3D milieuHGHD = milieuSegment(quadTree->pointHG, quadTree->pointHD, image); //centre ligne horizontale supérieure du QuadTree
    Point3D milieuBGBD = milieuSegment(quadTree->pointBG, quadTree->pointBD, image);// centre ligne horizontale inférieure du QuadTree    
    Point3D milieuHGBG = milieuSegment(quadTree->pointHG, quadTree->pointBG, image); //centre ligne verticale gauche 
    Point3D milieuHDBD = milieuSegment(quadTree->pointHD, quadTree->pointBD, image); //centre ligne verticale droite

    Point3D centreQuadTree = milieuSegment(milieuHGHD, milieuBGBD, image); //Point au centre du quadTree (au centre du carré formé par HG/HD/BG/BD)
    
    remplirNode(quadTree->nodeHG, quadTree->pointHG, milieuHGHD, centreQuadTree, milieuHGBG); //On remplit en partant du coin en haut à gauche du node puis le coin HD/BD/BG
    construireQuadTree(quadTree->nodeHG, image, depth++); //On appelle récursivement la fonction sur chacun des fils de l'arbre
   
    remplirNode(quadTree->nodeHD, milieuHGHD, quadTree->pointHD, milieuHDBD, centreQuadTree);
    construireQuadTree(quadTree->nodeHD, image, depth++);
    
    remplirNode(quadTree->nodeBD, centreQuadTree, milieuHDBD, quadTree->pointBD, milieuBGBD);
    construireQuadTree(quadTree->nodeBD, image, depth++);
    
    remplirNode(quadTree->nodeBG, milieuHGBG, centreQuadTree, milieuBGBD, quadTree->pointBG);
    construireQuadTree(quadTree->nodeBG, image, depth++);    
    }
}

bool formeUnSquare(Point3D pHG, Point3D pHD, Point3D pBD, Point3D pBG){ //Checke si les 4 pts forment un carré côte à côte
    if (pHG.x+1==pHD.x && pHG.y==pHD.y){                                                 // Si c'est le cas, c'est que l'on a 4 pts cote à cote donc ==> c'est une feuille
        if (pHD.y+1==pBD.y && pHD.x==pBD.x){
            if (pBD.x-1==pBG.x && pBD.y==pBG.y){
                if (pBG.y-1==pHG.y && pBG.x==pHG.x){
                    return true;
                }
            }
        }
    }                                                              
    return false;                                                          
}

bool isLeaf(Node* node){
    return node->nodeHG==nullptr && node->nodeHD==nullptr && node->nodeBG==nullptr && node->nodeBD==nullptr;// Aucun node ==> On ne peut pas aller plus loin dans l'arbre
}

//Affiche toute les feuilles de l'arbre (fonction servant à tester le quad)
void parcourirQuad(Node* quadTree){
    if (isLeaf(quadTree)){
        afficherNode(quadTree);
    }
    else{
        if (quadTree->nodeHG != nullptr)
            parcourirQuad(quadTree->nodeHG);
        if (quadTree->nodeHD != nullptr)
            parcourirQuad(quadTree->nodeHD);
        if (quadTree->nodeBG != nullptr)
            parcourirQuad(quadTree->nodeBG);
        if (quadTree->nodeBD != nullptr)
            parcourirQuad(quadTree->nodeBD);
    }
}

