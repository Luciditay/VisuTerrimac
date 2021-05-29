#include "getFile.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

//Transforme un tableau de dimension width*height en matrice de dimension[width][height]
void tabToMatrice(float* tab, float** matrice, int width, int height){
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            matrice[i][j] = tab[i * (width) + j];
            //cout << (int) matrice[i][j] << " ";
        }
        //cout << endl;
    }
}
//Charge tout un tas de paramètres depuis fichierParams (fichier parametres.txt)
void loadParams(const string fichierParams, string* heightMapFile, int *width, int *height, int *zMin, int *zMax, float *zNear, float *zFar, float *fov)
{
    ifstream file(fichierParams);

    if(!file){
    cerr << "Impossible d'ouvrir le fichier " << endl;
    exit(EXIT_FAILURE);
    }

    int i = 0, valueChiffre1;
    float valueChiffre2;
    string name, value;

    while (file >> name)
    {
        file >> value;
        valueChiffre1 = atoi(value.c_str());
        valueChiffre2 = atof(value.c_str());

        if (i == 0)
        { //Si name=="name"
            *heightMapFile = value;
            cout << "nomFichier :" << *heightMapFile << endl;
        }

        if (i == 1)
        { //Si name==x_size
            *width = valueChiffre1;
            cout << "width " << *width << endl;
        }

        if (i == 2)
        {
            *height = valueChiffre1;
            cout << "height " << *height << endl;
        }

        if (i == 3)
        {
            *zMin = valueChiffre1;
            cout << "zMin " << *zMin << endl;
        }

        if (i == 4)
        {
            *zMax = valueChiffre1;
            cout << "zMax " << *zMax << endl;
        }

        if (i == 5)
        {
            *zNear = valueChiffre2;
            cout << "zNear " << *zNear << endl;
        }

        if (i == 6)
        {
            *zFar = valueChiffre2;
            cout << "zFar " << *zFar << endl;
        }

        if (i == 7)
        {
            *fov = valueChiffre2;
            cout << "fov " << *fov << endl;
        }
        i++;
    }
}

//Convertit une valeur entre istart/istop vers ostart et ostop 
float mapValue(float value, float istart, float istop, float ostart, float ostop) { 
    return ostart + (ostop - ostart) * ((value - istart) / (istop - istart));
}

//Charge une heightMap depuis heightMapPath dans un tableau imageTab à UNE dimension, qu'on convertira ensuite en matrice
//Oui c'est un peu foireux et ca peut s'améliorer, mais à la fin on a dans la structure Image, une matrice de pixels contenant la height Map
void loadHeightMap(string heightMapPath, float imageTab[], int dimTableau, int zMin, int zMax, Image* image)
{
    ifstream heightMap(heightMapPath.c_str()); // Fichier ouvert en lecture
    string osef;

    getline(heightMap, osef); //On squizze la première ligne (donnant le format du fichier)
    getline(heightMap, osef); // puis la seconde (#created by gimp pour nos fichiers)
    heightMap >> image->w; // On stocke les dimensions originales de l'Image dans la structure IMAGE (présent dans getFile.h)
    heightMap >> image->h;
    float couleurMax;

    if (heightMap) // Si le fichier a bien été chargé
    {
        heightMap >> couleurMax; //Première valeur ==> Intensité maximale
        int i = 0, zValue, height;
        while (i < dimTableau) //On get tous les caractères du fichier jusqu'à arriver au bout du fichier (EOF)
        {
            heightMap >> zValue; //Conversion de la hauteur selon l'intensité du pixel entre 0 et 1 (pas sur de la formule)
            imageTab[i] = mapValue(zValue, 0, couleurMax, zMin, zMax);
            i++;
        }
        heightMap.close();
    }

    else
    {
        cout << heightMapPath << endl;
        cout << "Problème d'ouverture du fichier " << heightMapPath << endl;
        heightMap.close();
        exit(EXIT_FAILURE);
    }
}