#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Defs.h"
#include "Jerry.h"
#include "MultiValueHashTable.h"
#include "HashTable.h"
#include "LinkedList.h"

int main(int argc, char *argv[]);

char *toUpper(char *arr);

int getClosePrime(int num);

int LoadingJerries(int numberOfPlanets, char *configurationFile, Planet **planets, GenLL jerryList);

status findPlanetByName(Planet **planets, int size, char *id, Planet **planet);

Element copyString(Element element);

Element deepCopyString(Element element);

status freeStr(Element element);

status printString(Element element);

int TransformToNum(Element element);

status fakeFree(Element element);

status removeJerryFromSystem(MVHT multiHash, hashTable jerryHash, GenLL jerryList, Jerry *jerry);

void adjustHappiness(Jerry *jerry, int value);

void cleanExit(Planet **planets, MVHT multiHash, hashTable jerryHash, GenLL jerryList, int numberOfPlanets);

// function to use in ADTs
Element copyJerry(Element element) {
    return (Jerry *) element;
}

status printJerry(Element elem) {
    return JerryToString((Jerry *) elem);
}

status DelFreeJerry(Element element) {
    return DeleteJerry((Jerry *) element);
}

bool isEqualJerries(Element element1, Element element2) {
    return isEqualJerry((Jerry *) element1, (Jerry *) element2);
}

bool equalPCName(Element element1, Element element2) {
    if (strcmp(((char *) element1), (char *) element2) == 0)
        return true;
    return false;
}

bool equalJerryWithID(Element element1, Element element2) {
    if (strcmp((char *) element1, (char *) element2) == 0)
        return true;
    return false;

}


int main(int argc, char *argv[]) {

    int numberOfPlanets, numberOfJerries, numberOfPC = 0;
    int i, j;

    char ch[BUFFER_SIZE], idToFind[BUFFER_SIZE], pcName[BUFFER_SIZE], planetToFind[BUFFER_SIZE], dimension[BUFFER_SIZE];
    int happy;
    float valToPC;
    bool flag = true;
    status stat = success;
    Jerry *jerry = NULL;
    Planet *planet = NULL;
    PC *pc = NULL;
    Origin *origin = NULL;
    GenLL list = NULL;
    //use in case 5:
    Jerry *closestJerry = NULL;
    float minDiff = 0;
    // use in case 6
    int minHappy;

    if (argv[2] == NULL)
        exit(EXIT_CODE);


    numberOfPlanets = atoi(argv[1]);
    Planet **planets = (Planet **) malloc(numberOfPlanets * sizeof(Planet *));
    if (!planets) { //check if there was an error when alloc
        printf("Memory Problem \n");
        exit(EXIT_CODE);//TODO
    }
    GenLL jerryList = createLinkedList(copyJerry, DelFreeJerry, isEqualJerries, printJerry);

    if (!jerryList) {
        printf("Memory Problem \n");
        exit(EXIT_CODE);
    }

    numberOfJerries = LoadingJerries(numberOfPlanets, argv[2], planets, jerryList);
    for (i = 1; i <= getLengthList(jerryList); i++) {
        numberOfPC += ((Jerry *) getDataByIndex(jerryList, i))->PCcount;
    }

    hashTable jerryHash = createHashTable(copyString, fakeFree, printString, copyJerry, fakeFree, printJerry,
                                          equalJerryWithID, TransformToNum, getClosePrime(numberOfJerries));
    MVHT multiHash = createMultiValueHashTable(deepCopyString, freeStr, printString, copyJerry, fakeFree,
                                               printJerry, equalPCName, isEqualJerries, TransformToNum,
                                               getClosePrime(numberOfPC));
    for (i = 1; i <= getLengthList(jerryList); i++) {
        jerry = (Jerry *) getDataByIndex(jerryList, i);
        addToHashTable(jerryHash, jerry->ID, jerry);

        for (j = 0; j < jerry->PCcount; j++)
            if (addToMultiValueHashTable(multiHash, jerry->PCArray[j]->name, jerry) == memoryFail)
                cleanExit(planets, multiHash, jerryHash, jerryList, numberOfPlanets);
    }


    while (flag) {
        printf("Welcome Rick, what are your Jerry's needs today ? \n");
        printf("1 : Take this Jerry away from me \n");
        printf("2 : I think I remember something about my Jerry \n");
        printf("3 : Oh wait. That can't be right \n");
        printf("4 : I guess I will take back my Jerry now \n");
        printf("5 : I can't find my Jerry. Just give me a similar one \n");
        printf("6 : I lost a bet. Give me your saddest Jerry \n");
        printf("7 : Show me what you got \n");
        printf("8 : Let the Jerries play \n");
        printf("9 : I had enough. Close this place \n");
        scanf(" %s", ch);
        if (strlen(ch) != 1)
            ch[0] = '.';
        switch (ch[0]) {
            case '1':  /* Take this Jerry away from me */
                printf("What is your Jerry's ID ? \n");
                scanf(" %s", idToFind);
                if (lookupInHashTable(jerryHash, idToFind)) {//jerry exist !
                    printf("Rick did you forgot ? you already left him here ! \n");
                    break;
                }
                // add new jerry !
                printf("What planet is your Jerry from ? \n");
                scanf(" %s", planetToFind);
                stat = findPlanetByName(planets, numberOfPlanets, planetToFind, &planet);
                if (stat == memoryFail)
                    cleanExit(planets, multiHash, jerryHash, jerryList, numberOfPlanets);
                if (stat == failure) {//planet not found
                    printf("%s is not a known planet ! \n", planetToFind);
                    break;
                }
                printf("What is your Jerry's dimension ? \n");
                scanf(" %s", dimension);
                printf("How happy is your Jerry now ? \n");
                scanf(" %d", &happy);
                origin = Create_Origin(dimension, planet);
                if (!origin) {
                    cleanExit(planets, multiHash, jerryHash, jerryList, numberOfPlanets);
                }
                jerry = Create_Jerry(idToFind, happy, origin);
                if (!jerry) {
                    cleanExit(planets, multiHash, jerryHash, jerryList, numberOfPlanets);
                }
                //add the new jerry to the structures
                if (appendNode(jerryList, jerry) == memoryFail ||
                    addToHashTable(jerryHash, jerry->ID, jerry) == memoryFail)
                    cleanExit(planets, multiHash, jerryHash, jerryList, numberOfPlanets);
                // not add him to the multi cuz don't have PC !
                JerryToString(jerry);
                break;

            case '2':  /* I think I remember something about my Jerry */
                //search in hash, search pc in jerry, add pc to jerry, add jerry to multi by this new pc, print all jerries with this pc
                printf("What is your Jerry's ID ? \n");
                scanf(" %s", idToFind);
                jerry = lookupInHashTable(jerryHash, idToFind);
                if (!jerry) {//jerry not exist !
                    printf("Rick this Jerry is not in the daycare ! \n");
                    break;
                }
                printf("What physical characteristic can you add to Jerry - %s ? \n", idToFind);
                scanf(" %s", pcName);
                if (PCExist(jerry, pcName)) {
                    printf("The information about his %s already available to the daycare ! \n", pcName);
                    break;
                }
                printf("What is the value of his %s ?\n", pcName);
                scanf(" %f", &valToPC);
                //add this PC
                if (Add_PC(jerry, Create_PC(pcName, (float) valToPC)) == memoryFail)
                    cleanExit(planets, multiHash, jerryHash, jerryList, numberOfPlanets);
                if (addToMultiValueHashTable(multiHash, pcName, jerry) == memoryFail)
                    cleanExit(planets, multiHash, jerryHash, jerryList, numberOfPlanets);
                //print all jerries with this new pc
                if (displayMultiValueHashElementsByKey(multiHash, pcName) == memoryFail)
                    cleanExit(planets, multiHash, jerryHash, jerryList, numberOfPlanets);
                break;

            case '3':  /* Oh wait. That can't be right */
                printf("What is your Jerry's ID ? \n");
                scanf(" %s", idToFind);
                jerry = lookupInHashTable(jerryHash, idToFind);
                if (!jerry) {//jerry not exist !
                    printf("Rick this Jerry is not in the daycare ! \n");
                    break;
                }
                printf("What physical characteristic do you want to remove from Jerry - %s ? \n", idToFind);
                scanf(" %s", pcName);
                if (!PCExist(jerry, pcName)) {
                    printf("The information about his %s not available to the daycare ! \n", pcName);
                    break;
                }

                if (Delete_PC(jerry, pcName) == memoryFail)
                    cleanExit(planets, multiHash, jerryHash, jerryList, numberOfPlanets);
                if (removeFromMultiValueHashTable(multiHash, pcName, jerry) == memoryFail)
                    cleanExit(planets, multiHash, jerryHash, jerryList, numberOfPlanets);

                JerryToString(jerry);
                break;

            case '4':  /* I guess I will take back my Jerry now */
                printf("What is your Jerry's ID ? \n");
                scanf(" %s", idToFind);
                jerry = lookupInHashTable(jerryHash, idToFind);
                if (!jerry) {//jerry not exist !
                    printf("Rick this Jerry is not in the daycare ! \n");
                    break;
                }
                if (removeJerryFromSystem(multiHash, jerryHash, jerryList, jerry) == memoryFail)
                    cleanExit(planets, multiHash, jerryHash, jerryList, numberOfPlanets);
                printf("Rick thank you for using our daycare service ! Your Jerry awaits ! \n");

                break;

            case '5':  /*  I can't find my Jerry. Just give me a similar one */
                printf("What do you remember about your Jerry ? \n");
                scanf(" %s", pcName);
                list = lookupInMultiValueHashTable(multiHash, pcName);
                if (!list) {//PC not exist !
                    printf("Rick we can not help you - we do not know any Jerry's %s ! \n", pcName);
                    break;
                }
                printf("What do you remember about the value of his %s ? \n", pcName);
                scanf(" %f", &valToPC);

                closestJerry = getDataByIndex(list, 1);
                for (i = 0; i < closestJerry->PCcount; ++i) {
                    if (strcmp(closestJerry->PCArray[i]->name, pcName) == 0) {
                        minDiff = fabs(closestJerry->PCArray[i]->value - valToPC);
                        break;
                    }
                }
                for (i = 1; i < getLengthList(list); ++i) {
                    jerry = getDataByIndex(list, i); //TODO!!! its O(N^2)!!! INBAL ASKED FOR O(N)
                    for (j = 0; j < jerry->PCcount; ++j) {
                        if (strcmp(jerry->PCArray[j]->name, pcName) == 0) {
                            if (minDiff > fabs(closestJerry->PCArray[j]->value - valToPC)) {
                                closestJerry = jerry;
                                minDiff = fabs(closestJerry->PCArray[j]->value - valToPC);
                            }
                            break;
                        }
                    }
                }
                printf("Rick this is the most suitable Jerry we found : \n");
                JerryToString(closestJerry);
                if (removeJerryFromSystem(multiHash, jerryHash, jerryList, closestJerry) == memoryFail)
                    cleanExit(planets, multiHash, jerryHash, jerryList, numberOfPlanets);
                printf("Rick thank you for using our daycare service ! Your Jerry awaits ! \n");

                break;

            case '6':  /* I lost a bet. Give me your saddest Jerry */
                // check if there is any jerries at all, give me the saddest one
                if (getLengthList(jerryList) == 0) {
                    printf("Rick we can not help you - we currently have no Jerries in the daycare ! \n");
                    break;
                }
                // else there is at least 1 jerry
                minHappy = 101;
                for (i = 1; i <= getLengthList(jerryList); ++i) {
                    jerry = getDataByIndex(jerryList, i);
                    if (jerry->happy < minHappy) {
                        closestJerry = jerry;
                        minHappy = jerry->happy;
                    }
                }
                printf("Rick this is the most suitable Jerry we found : \n");
                JerryToString(closestJerry);

                if (removeJerryFromSystem(multiHash, jerryHash, jerryList, closestJerry) == memoryFail)
                    cleanExit(planets, multiHash, jerryHash, jerryList, numberOfPlanets);
                printf("Rick thank you for using our daycare service ! Your Jerry awaits ! \n");

                break;

            case '7':  /* Show me what you got */
                printf("What information do you want to know ? \n");
                printf("1 : All Jerries \n");
                printf("2 : All Jerries by physical characteristics \n");
                printf("3 : All known planets \n");
                scanf(" %s", ch);
                if (strlen(ch) != 1)
                    ch[0] = '.';
                switch (ch[0]) {
                    case '1': /* All jerries */
                        if (getLengthList(jerryList) == 0) {
                            printf("Rick we can not help you - we currently have no Jerries in the daycare ! \n");
                            break;
                        }
                        if (displayList(jerryList) == memoryFail)
                            cleanExit(planets, multiHash, jerryHash, jerryList, numberOfPlanets);
                        break;

                    case '2':
                        printf("What physical characteristics ? \n");
                        scanf(" %s", pcName);
                        stat = displayMultiValueHashElementsByKey(multiHash, pcName);
                        if (stat == memoryFail)
                            cleanExit(planets, multiHash, jerryHash, jerryList, numberOfPlanets);
                        else if (stat == failure)
                            printf("Rick we can not help you - we do not know any Jerry's %s ! \n", pcName);
                        break;
                    case '3':  /* Print all Planets */
                        for (i = 0; i < numberOfPlanets; ++i) {
                            if (PlanetToString(planets[i]) == memoryFail)
                                cleanExit(planets, multiHash, jerryHash, jerryList, numberOfPlanets);
                        }
                        break;

                    default:
                        printf("Rick this option is not known to the daycare ! \n");
                        break;
                }


                break;
            case '8':  /*  Let the Jerries play */
                if (getLengthList(jerryList) == 0) {
                    printf("Rick we can not help you - we currently have no Jerries in the daycare ! \n");
                    break;
                }

                printf("What activity do you want the Jerries to partake in ? \n");
                printf("1 : Interact with fake Beth \n");
                printf("2 : Play golf \n");
                printf("3 : Adjust the picture settings on the TV \n");
                scanf(" %s", ch);
                if (strlen(ch) != 1)
                    ch[0] = '.';
                switch (ch[0]) {
                    case '1': /* Interact with fake Beth */
                        for (i = 1; i <= getLengthList(jerryList); ++i) {
                            jerry = getDataByIndex(jerryList, i);
                            if (jerry->happy >= 20)
                                adjustHappiness(jerry, 15);
                            else
                                adjustHappiness(jerry, -5);
                        }
                        printf("The activity is now over ! \n");
                        displayList(jerryList);
                        break;

                    case '2': /* Play golf */
                        for (i = 1; i <= getLengthList(jerryList); ++i) {
                            jerry = getDataByIndex(jerryList, i);
                            if (jerry->happy >= 50)
                                adjustHappiness(jerry, 10);
                            else
                                adjustHappiness(jerry, -10);
                        }

                        printf("The activity is now over ! \n");
                        displayList(jerryList);
                        break;

                    case '3':  /* Adjust the picture settings on the TV */
                        for (i = 1; i <= getLengthList(jerryList); ++i) {
                            jerry = getDataByIndex(jerryList, i);
                            adjustHappiness(jerry, 20);
                        }

                        printf("The activity is now over ! \n");
                        displayList(jerryList);
                        break;

                    default:
                        printf("Rick this option is not known to the daycare ! \n");
                        break;
                }
                break;

            case '9':  /* */
                printf("The daycare is now clean and close ! \n");
                cleanExit(planets, multiHash, jerryHash, jerryList, numberOfPlanets);
                flag = false;
                break;

            default:
                printf("Rick this option is not known to the daycare ! \n");
                break;
        }
    }
    return 0;
}

int getClosePrime(int num) {
    bool flag = true;
    int i;
    while (true) {
        flag = true;
        for (i = 2; i < num; i++) {
            if (num % i == 0) {
                flag = false;
                break;
            }
        }
        if (!flag)
            num++;
        else if (num <= 11)
            return 11;
        else
            return num;
    }
}

char *toUpper(char *arr) {
    //TODO: delete ???
    char *c;
    for (c = arr; *c != '\0'; c++)
        *c = TO_UPPER(*c);
    return arr;
}

int LoadingJerries(int numberOfPlanets, char *configurationFile, Planet **planets, GenLL jerryList) {

    FILE *fp = fopen(configurationFile, "r");
    char line[BUFFER_SIZE], *token = NULL;
    int planetsCounter = 0, i;
    bool idiotFlag = true;
    fgets(line, BUFFER_SIZE, fp);

    // Insert Planets
    char *name = NULL;
    float x, y, z;
    for (i = 0; i < numberOfPlanets; ++i) {
        fgets(line, BUFFER_SIZE, fp);
        token = strtok(line, ",");
        name = token;
        token = strtok(NULL, ",");
        x = atof(token);
        token = strtok(NULL, ",");
        y = atof(token);
        token = strtok(NULL, ",");
        z = atof(token);
        planets[i] = Create_Planet(name, x, y, z);
        if (planets[i] == NULL)
            cleanExit(planets, NULL, NULL, jerryList, planetsCounter);
        planetsCounter++;
    }

    // Insert Jerries
    fgets(line, BUFFER_SIZE, fp);
    char *id = NULL, *originName = NULL, *planetName = NULL, *pcName = NULL;
    int happy, numberOfJerries = 0;
    Origin *origin = NULL;
    Planet *planet = NULL;
    Jerry *tmpJerry;
    Element elem = fgets(line, BUFFER_SIZE, fp);

    while (elem != NULL) {
        token = strtok(line, ",");
        id = token;
        token = strtok(NULL, ",");
        originName = token;
        token = strtok(NULL, ",");
        planetName = token;
        token = strtok(NULL, ",");
        happy = atoi(token);
        for (int j = 0; j < numberOfPlanets; ++j) {
            if (strcmp(planets[j]->name, planetName) == 0) {
                planet = planets[j];
                break;
            }
        }
        origin = Create_Origin(originName, planet);
        if (origin == NULL) {
            cleanExit(planets, NULL, NULL, jerryList, planetsCounter);
        }

        tmpJerry = Create_Jerry(id, happy, origin);// create the jerry
        if (tmpJerry == NULL) {
            DeleteOrigin(origin);
            cleanExit(planets, NULL, NULL, jerryList, planetsCounter);
        }

        elem = fgets(line, BUFFER_SIZE, fp);
        while (elem != NULL && *line == '\t') {
            token = strtok(&line[1], ":");
            pcName = token;
            token = strtok(NULL, ":");
            if (Add_PC(tmpJerry, Create_PC(pcName, atof(token))) == memoryFail)
                cleanExit(planets, NULL, NULL, jerryList, planetsCounter);
            elem = fgets(line, BUFFER_SIZE, fp);
        }

        if (appendNode(jerryList, tmpJerry) == memoryFail)
            cleanExit(planets, NULL, NULL, jerryList, planetsCounter);
        numberOfJerries++;
    }
    fclose(fp);
    return numberOfJerries;
}

// sub-function to print jerries by planet case
// gets pointer to planet pointer and make it point to the planet with the name we got.
// if not found, point to NULL
status findPlanetByName(Planet **planets, int size, char *id, Planet **planet) {
    //update the **planet inplace cuz return only status
    if (!planets || !id)
        return memoryFail;
    int i;
    for (i = 0; i < size; ++i) {
        if (strcmp(planets[i]->name, id) == 0) {
            *planet = planets[i];
            return success;
        }
    }
    *planet = NULL;
    return failure;
}

//char *copyString(char *str) {
Element copyString(Element element) {
    if (!element)
        return NULL;
    return (char *) element;
}

Element deepCopyString(Element element) {
    if (!element)
        return NULL;
    char *newString = (char *) malloc(strlen((char *) element) + 1);
    if (!newString) { //check if there was an error when alloc
        printf("Memory Problem \n");
        return NULL;
    }
    strcpy(newString, (char *) element);
    return newString;
}

status freeStr(Element element) {
    if (!element)
        return memoryFail;
    free((char *) element);
    return success;
}


status printString(Element element) {
    if (!element)
        return memoryFail;
    printf("%s : \n", (char *) element); //TODO: :::::: we have add it for the kvp print
    return success;
}

int TransformToNum(Element element) {
    if (!element)
        return -1;
    int cnt = 0;
    char *tmp = (char *) element;
    int i;
    for (i = 0; i < strlen(tmp); ++i) {
        cnt += (int) tmp[i];
    }
    return cnt;
}

status fakeFree(Element element) {
    return success;
}

//the multi hash and the hash use fakeFree on jerry so the jerry stay live in the heap
//when we delete the jerry from the LinkedList then we free the jerry.
status removeJerryFromSystem(MVHT multiHash, hashTable jerryHash, GenLL jerryList, Jerry *jerry) {
    int i;
    for (i = 0; i < jerry->PCcount; ++i) {
        if (removeFromMultiValueHashTable(multiHash, jerry->PCArray[i]->name, jerry) == memoryFail)
            return memoryFail;
    }
    if (removeFromHashTable(jerryHash, jerry->ID) == memoryFail || deleteNode(jerryList, jerry) == memoryFail)
        return memoryFail;
    return success;
}

void adjustHappiness(Jerry *jerry, int value) {
    jerry->happy += value;
    if (jerry->happy > 100)
        jerry->happy = 100;
    else if (jerry->happy < 0)
        jerry->happy = 0;
}

void
cleanExit(Planet **planets, MVHT multiHash, hashTable jerryHash, GenLL jerryList, int numberOfPlanets) {
    int i;
    for (i = numberOfPlanets - 1; i >= 0; --i)
        DeletePlanet(planets[i]);

    free(planets);
    planets = NULL;

    if (multiHash) {
        destroyMultiValueHashTable(multiHash);
        multiHash = NULL;
    }

    if (jerryHash) {
        destroyHashTable(jerryHash);
        jerryHash = NULL;
    }
    destroyList(jerryList);
    jerryList = NULL;

    exit(EXIT_CODE);
}
