#ifndef JERRY_H
#define JERRY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "Defs.h"

typedef struct Planet_t {
    char *name; //UNIQUE
    float x, y, z;
} Planet;

typedef struct Origin_t {
    char *name;
    struct Planet_t *planet; // pointer to the Origin planet
} Origin;

typedef struct PhysicalCharacteristics_t {
    char *name;
    float value;
} PC;

typedef struct Jerry_t {
    char *ID; // unique
    int happy; // between 0 and 100
    Origin *origin;
    PC **PCArray;// pointer to array of pointers
    int PCcount;
} Jerry;

Jerry *Create_Jerry(char *id, int happy, Origin *origin);

Planet *Create_Planet(char *name, float x, float y, float z);

PC *Create_PC(char *name, float value);

Origin *Create_Origin(char *name, Planet *planet);

bool PCExist(Jerry *jerry, char *Pcname);

status Add_PC(Jerry *jerry, PC *newPC);

status Delete_PC(Jerry *jerry, char *Pcname);

status JerryToString(Jerry *jerry);

status PlanetToString(Planet *planet);

status DeletePlanet(Planet *planet);

status DeleteOrigin(Origin *origin);

status DeleteJerry(Jerry *jerry);

bool isEqualJerry(Jerry *jerry1,Jerry *jerry2);

//Jerry *copyJerry(Jerry *jerry);

//sub-function: check and return true if one of the pointer arguments passed is NUll.
//bool memoryProblem(int num, ...);

#endif