//
// Created by Abby Newman on 04/12/2023.
//

#ifndef COLOURINGBOOK_MAIN_H
#define COLOURINGBOOK_MAIN_H


/**
Here I have the transform name, start and end range and the instruction type 
*/
typedef struct Branch{
    char* transformName;
    int startRange;
    int endRange;
    char* instructionType;
    struct Branch* next;
    char instructionName[];
}Branch;


typedef struct Fractal{
    struct Branch* branch;
    struct Fractal* next;
    char fractal_name[];
}Fractal;

Fractal* startFractal = NULL;

typedef struct Transform{
    char* transformName;
    double rotation;
    double xDistance;
    double yDistance;
    double scale;
    struct Transform* next;
}Transform;

Transform* startTransform = NULL;

typedef struct Point {
    double x;
    double y;
    struct Point* next;
} Point;

typedef struct Graphic{
    char* graphicName;
    Point* coordinates;
    struct Graphic* next;
}Graphic;

Graphic* startGraphic = NULL;

typedef struct branchDataList{
    int iterationCount;
    Fractal* rootFractal;
    Transform globalTransform;
    struct branchDataList* next;
}branchDataList;

void readTextFile(char* filename);
void printInformation();
void generateFractal(Fractal* root);
void freeMemory();
Transform applyTransform(Transform globalTransform,Transform transform);
void drawGraphic(Graphic* graphic, Transform transform, FILE* svgFile);
#endif //COLOURINGBOOK_MAIN_H
