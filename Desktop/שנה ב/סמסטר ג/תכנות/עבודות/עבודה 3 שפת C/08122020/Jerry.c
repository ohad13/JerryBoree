#include "Jerry.h"

//sub-function: check and return true if one of the pointer arguments passed is NUll.
//bool memoryProblem(int num, ...) {
//    va_list argv;
//    va_start(argv, num);
//    int i;
//    for (i = 0; i < num; ++i) {
//        void *pointer = va_arg(argv, void *);
//        if (pointer == NULL) {
//            printf("Memory Problem \n");
//            return true;
//        }
//    }
//    va_end(argv);
//    return false;
//}

Jerry *Create_Jerry(char *id, int happy, Origin *origin) {
    if (!id || !origin)
        return NULL;
    Jerry *jerry = (Jerry *) malloc(sizeof(Jerry));
    if (jerry == NULL) { //check if there was an error when alloc
        printf("Memory Problem \n");
        return NULL;
    }
    jerry->ID = (char *) malloc(sizeof(char) * (strlen(id) + 1));
    if (jerry->ID == NULL) { //check if there was an error when alloc
        printf("Memory Problem \n");
        return NULL;
    }
    strcpy(jerry->ID, id);
    jerry->happy = happy;
    jerry->origin = origin;
    jerry->PCArray = NULL;
    jerry->PCcount = 0;
    return jerry;
}

Planet *Create_Planet(char *name, float x, float y, float z) {
    if (!name)
        return NULL;
    Planet *planet = (Planet *) malloc(sizeof(Planet));
    if (planet == NULL) { //check if there was an error when alloc
        printf("Memory Problem \n");
        return NULL;
    }
    planet->name = (char *) malloc(sizeof(char) * (strlen(name) + 1));
    if (planet->name == NULL) { //check if there was an error when alloc
        printf("Memory Problem \n");
        return NULL;
    }
    strcpy(planet->name, name);
    planet->x = x;
    planet->y = y;
    planet->z = z;
    return planet;
}

PC *Create_PC(char *name, float value) {
    if (!name)
        return NULL;
    PC *pc;
    pc = (PC *) malloc(sizeof(PC));
    if (!pc) { //check if there was an error when alloc
        printf("Memory Problem \n");
        return NULL;
    }
    pc->name = (char *) malloc(sizeof(char) * (strlen(name) + 1));
    if (!pc->name) { //check if there was an error when alloc
        printf("Memory Problem \n");
        return NULL;
    }
    strcpy(pc->name, name);
    pc->value = value;
    return pc;
}

Origin *Create_Origin(char *name, Planet *planet) {
    if (!name || !planet)
        return NULL;
    Origin *origin = (Origin *) malloc(sizeof(Origin));
    if (!origin) { //check if there was an error when alloc
        printf("Memory Problem \n");
        return NULL;
    }
    origin->name = (char *) malloc(sizeof(char) * (strlen(name) + 1));
    if (!origin->name) { //check if there was an error when alloc
        printf("Memory Problem \n");
        return NULL;
    }
    strcpy(origin->name, name);
    origin->planet = planet;
    return origin;
}

bool PCExist(Jerry *jerry, char *Pcname) {
    //no need to check if argument are null because we checked it before we call to this function
    int i;
    for (i = 0; i < jerry->PCcount; i++)
        if (strcmp(jerry->PCArray[i]->name, Pcname) == 0)
            return true;
    return false;
}

//check if not exist already -> if not exist, add one more pc space by realloc -> add pc to the new cell
status Add_PC(Jerry *jerry, PC *newPC) {
    if (!jerry || !newPC)
        return memoryFail;
    if (!PCExist(jerry, newPC->name)) { // if not exist --> add pc
        jerry->PCArray = (PC **) realloc(jerry->PCArray, (jerry->PCcount + 1) * sizeof(PC *));
        if (jerry->PCArray == NULL) { //check if there was an error when alloc
            printf("Memory Problem \n");
            return memoryFail;
        }
        jerry->PCArray[jerry->PCcount] = newPC;
        jerry->PCcount++;
    }
    return success;
}

status Delete_PC(Jerry *jerry, char *Pcname) {
    if (!jerry || !Pcname)
        return memoryFail;
    PC **temp = NULL, **holder = NULL;
    int positionFixer = 0, positionToFree = 0;
    int i;
    if (PCExist(jerry, Pcname)) {//check if the pc already exist
        temp = (PC **) malloc(sizeof(PC *) * (jerry->PCcount - 1)); //malloc new smaller array size
        if (!temp) { //check if there was an error when alloc
            printf("Memory Problem \n");
            return memoryFail;
        }
        for (i = 0; i < jerry->PCcount; i++) {//move all the pc's instead the one we wish to remove to new array
            if (strcmp(jerry->PCArray[i]->name, Pcname) == 0) {
                positionFixer++; // will make the loop ignoring the pc we wish to remove
                positionToFree = i;
            } else
                temp[i - positionFixer] = jerry->PCArray[i];
        }
        jerry->PCcount--;
        holder = jerry->PCArray;//hold the old pc array
        jerry->PCArray = temp; // take the new pc array
        free(holder[positionToFree]->name);//free the old one
        holder[positionToFree]->name = NULL;
        free(holder[positionToFree]);
        holder[positionToFree] = NULL;
        free(holder);
        holder = NULL;
        return success;
    }
    return failure; //return if the pc doesnt exist
}

status JerryToString(Jerry *jerry) {
    if (!jerry)
        return memoryFail;
    printf("Jerry , ID - %s : \n", jerry->ID);
    printf("Happiness level : %d \n", jerry->happy);
    printf("Origin : %s \n", jerry->origin->name);
    printf("Planet : %s (%.2f,%.2f,%.2f) \n", jerry->origin->planet->name,
           jerry->origin->planet->x, jerry->origin->planet->y, jerry->origin->planet->z);
    if (jerry->PCcount != 0) {
        printf("Jerry's physical Characteristics available : \n\t");
        int i;
        for (i = 0; i < jerry->PCcount - 1; i++) {
            printf("%s : %.2f , ", jerry->PCArray[i]->name, jerry->PCArray[i]->value);
        }
        printf("%s : %.2f \n", jerry->PCArray[jerry->PCcount - 1]->name, jerry->PCArray[jerry->PCcount - 1]->value);
    }
    return success;
}

status PlanetToString(Planet *planet) {
    if (!planet)
        return memoryFail;
    printf("Planet : %s (%.2f,%.2f,%.2f) \n", planet->name,
           planet->x, planet->y, planet->z);
    return success;
}

status DeletePlanet(Planet *planet) {
    if (!planet)
        return memoryFail;
    free(planet->name);
    planet->name = NULL;
    free(planet);
    planet = NULL;
    return success;
}

status DeleteOrigin(Origin *origin) {
    if (!origin)
        return memoryFail;
    free(origin->name);
    origin->name = NULL;
    free(origin);
    origin = NULL;
    return success;
}

status DeleteJerry(Jerry *jerry) {
    if (!jerry)
        return memoryFail;
    free(jerry->ID);
    jerry->ID = NULL;
    int j;
    for (j = jerry->PCcount - 1; j >= 0; --j)
        Delete_PC(jerry, jerry->PCArray[j]->name);
    free(jerry->PCArray);
    jerry->PCArray = NULL;
    DeleteOrigin(jerry->origin);
    free(jerry);
    jerry = NULL;
    return success;
}

bool isEqualJerry(Jerry *jerry1, Jerry *jerry2) {
//    char *temp = (char *) malloc(strlen(jerry1->ID) + 1);
//    if (!temp) { //check if there was an error when alloc
//        printf("Memory Problem \n"); //TODO
//        return NULL;
//    }
//    strcpy(temp, jerry1->ID);
//    bool flag = (strcmp(temp, jerry2->ID) == 0);
//    free(temp);
//    return flag;

    return strcmp(jerry1->ID, jerry2->ID) == 0; //TODO: HAVE CHANGE BY FADLON, TRY RO FIX THE TODO BEFORE THIS ONE
}