OBJDIR = obj/
SRCDIR = src/

CC = g++
CFLAGS = -Wall -g -O2
LDFLAGS = -lSDL2 -lGLU -lGL -lm -lSDL2_image
SRC = $(wildcard *.cpp) #Fichier source (.cpp) 
OBJ = $(SRC:.cpp=.o) #Pour chaque fichier source (ie .cpp), on crée un fichier .o du même nom

all: projet

projet: $(OBJ) #Projet : Tout les fichiers objets du projet
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

main.o : quadtree.h getFile.h terrain.h loadSDL.h camera.h geometry.h light.h colors.h shading.h

%.o: %.cpp 
	$(CC) -c $< -o $@ $(CFLAGS) 

clean:
	rm -rf *.o  