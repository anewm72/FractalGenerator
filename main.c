#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "main.h"

/**
 * Reads a text file and stores the information in a linked list
 * @param filename
 */
void readTextFile(char* filename){
    FILE* file;
    file = fopen(filename, "r");
    if (file == NULL){
        perror("The file cannot be loaded.");
    }
    else{
        Graphic* currentGraphic = NULL;
        Transform* currentTransform = NULL;
        Fractal* currentFractal = NULL;

        char keyword[20];

        while (fscanf(file, "%s", keyword) != EOF){
            printf("Keyword: %s\n", keyword);
            if(keyword[0] == '/'){
                fscanf(file, "%*[^\n]\n");
            }
            else if(strcmp(keyword, "TRANSFORM")== 0){
                Transform* newTransform = malloc(sizeof(Transform));
                newTransform->next = NULL;
                char instruction[20];
                fscanf(file, "%20ms", &newTransform->transformName);
                fscanf(file, "%s", instruction);
                fscanf(file, " %lf", &newTransform->rotation);
                fscanf(file, "%s", instruction);
                fscanf(file, " (%lf, %lf) ", &newTransform->xDistance, &newTransform->yDistance);
                fscanf(file, "%s",instruction);
                fscanf(file, " %lf\n", &newTransform->scale);

                if (currentTransform == NULL) {
                    startTransform = newTransform;
                    currentTransform = newTransform;
                } else {
                    currentTransform->next = newTransform;
                    currentTransform = newTransform;
                }
            }else if(strcmp(keyword, "GRAPHIC")== 0) {
                Graphic *newGraphic = malloc(sizeof(Graphic));
                newGraphic->next = NULL;
                newGraphic->coordinates = NULL;
                fscanf(file, "%20ms", &newGraphic->graphicName);
                double x = 0;
                double y = 0;
                while (fscanf(file, " (%lf,%lf)", &x, &y) == 2) {
                    Point *newPoint = malloc(sizeof(Point));
                    newPoint->x = x;
                    newPoint->y = y;
                    newPoint->next = NULL;

                    Point *lastPoint = newGraphic->coordinates;
                    while (lastPoint != NULL && lastPoint->next != NULL) {
                        lastPoint = lastPoint->next;
                    }

                    if (lastPoint == NULL) {
                        newGraphic->coordinates = newPoint;
                    } else {
                        lastPoint->next = newPoint;
                        newPoint->next = NULL;
                    }
                }
                if (currentGraphic == NULL) {
                    startGraphic = newGraphic;
                    currentGraphic = newGraphic;
                } else {
                    currentGraphic->next = newGraphic;
                    currentGraphic = newGraphic;
                }
            }else if (strcmp(keyword, "FRACTAL") == 0) {
                Fractal *newFractal = malloc(sizeof(Fractal));
                newFractal->next = NULL;
                newFractal->branch = NULL;

                fscanf(file, "%s\n", newFractal->fractal_name);

                char branch[20] = "";

                while (fscanf(file, "%s", branch) != EOF) {
                    if (strcmp(branch, "BRANCH") == 0) {
                        printf("BRANCH\n");

                        Branch *currentBranch = malloc(sizeof(Branch) + 100);
                        currentBranch->next = NULL;

                        fscanf(file, "%ms", &currentBranch->transformName);
                        fscanf(file, " [%d:%d]", &currentBranch->startRange, &currentBranch->endRange);
                        fscanf(file, "%20ms", &currentBranch->instructionType);
                        fscanf(file, "%9s\n", currentBranch->instructionName);

                        if (newFractal->branch == NULL) {
                            newFractal->branch = currentBranch;
                        } else {
                            Branch *newBranch = newFractal->branch;
                            while (newBranch->next != NULL) {
                                newBranch = newBranch->next;
                            }
                            newBranch->next = currentBranch;
                        }
                    } else {
                        fseek(file, -strlen(branch), SEEK_CUR);
                        break;
                    }
                }
                if (currentFractal == NULL) {
                    startFractal = newFractal;
                    currentFractal = newFractal;
                } else {
                    currentFractal->next = newFractal;
                    currentFractal = newFractal;
                }
            }else{
                perror("The file format is not correct.");
            }
        }
        fclose(file);  // Close the file after reading
        printf("Finished reading file.\n");
        return;
    }
}

/**
 * Prints the information stored in the linked list
 */
void printInformation() {
    Transform* transform = startTransform;
    while(transform != NULL){
        printf("\n");
        printf("----------------------TRANSFORM----------------------\n");
        printf("Transform name: %s\n", transform->transformName);
        printf("Rotation: %f\n", transform->rotation);
        printf("Translation coordinates: (%f, %f)\n", transform->xDistance, transform->yDistance);
        printf("Scale: %f\n", transform->scale);
        transform = transform->next;
        printf("\n");
    }

    Graphic* graphic = startGraphic;
    while(graphic != NULL){
        printf("\n");
        printf("----------------------GRAPHIC----------------------\n");
        printf("Graphic name: %s\n", graphic->graphicName);
        Point* coordinates = graphic->coordinates;
        while (coordinates != NULL) {
            printf("(%f, %f) \n", coordinates->x, coordinates->y);
            coordinates = coordinates->next;
        }
        graphic = graphic->next;
        printf("\n");
    }

    Fractal* fractal = startFractal;
    while (fractal != NULL){
        printf("\n");
        printf("----------------------FRACTAL----------------------\n");
        printf("Fractal name: %s\n", fractal->fractal_name);
        Branch* branch = fractal->branch;
        while (branch != NULL){
            printf("\n");
            printf("----------------------BRANCH for %s----------------------\n", fractal->fractal_name);
            printf("Transform name: %s\n", branch->transformName);
            printf("Start range: %d\n", branch->startRange);
            printf("End range: %d\n", branch->endRange);
            printf("Instruction type: %s\n", branch->instructionType);
            printf("Instruction name: %s\n", branch->instructionName);
            branch = branch->next;
            printf("\n");
        }
        fractal = fractal->next;
        printf("\n");
    }
}

/**
 * Frees the memory allocated for the linked list
 */
void freeMemory(){
    Transform* transform = startTransform;
    while(transform != NULL){
        Transform* nextTransform = transform->next;
        free(transform);
        transform = nextTransform;
    }

    Graphic* graphic = startGraphic;
    while(graphic != NULL){
        Graphic* nextGraphic = graphic->next;
        Point* coordinates = graphic->coordinates;
        while (coordinates != NULL) {
            Point* nextPoint = coordinates->next;
            free(coordinates);
            coordinates = nextPoint;
        }
        free(graphic);
        graphic = nextGraphic;
    }

    Fractal* fractal = startFractal;
    while (fractal != NULL){
        Branch* branch = fractal->branch;
        while (branch != NULL){
            Branch* nextBranch = branch->next;
            free(branch);
            branch = nextBranch;
        }
        Fractal* nextFractal = fractal->next;
        free(fractal);
        fractal = nextFractal;
    }
}

/**
* Applies a branch transform to a global transform
*/
Transform applyTransform(Transform globalTransform, Transform transform) {
    globalTransform.rotation = globalTransform.rotation + transform.rotation;
    globalTransform.scale = globalTransform.scale * transform.scale;

    transform.xDistance = transform.xDistance * globalTransform.scale;
    transform.yDistance = transform.yDistance * globalTransform.scale;

    //rotate the translation vector
    double radians = globalTransform.rotation * (M_PI / 180);
    double originalX = transform.xDistance;
    double originalY = transform.yDistance;

// Apply rotation
    transform.xDistance = originalX * cos(radians) - originalY * sin(radians);
    transform.yDistance = originalX * sin(radians) + originalY * cos(radians);

    globalTransform.xDistance = globalTransform.xDistance + transform.xDistance;
    globalTransform.yDistance = globalTransform.yDistance + transform.yDistance;

    printf("Global rotation: %f\n", globalTransform.rotation);
    printf("Global scale: %f\n", globalTransform.scale);
    printf("Global translation coordinates: (%f, %f)\n", globalTransform.xDistance, globalTransform.yDistance);
    printf("\n");

    return globalTransform;
}

/**
 * Draws a graphic to an SVG file
 */
void drawGraphic(Graphic* graphic, Transform transform, FILE* svgFile){
    // Apply the transformation to each point in the graphic
    Point* currentPoint = graphic->coordinates;
    fprintf(svgFile, "<polyline points=\"");

    while (currentPoint != NULL){
        currentPoint->x = currentPoint->x * transform.scale;
        currentPoint->y = currentPoint->y * transform.scale;
        double originalX = currentPoint->x;
        double originalY = currentPoint->y;
        currentPoint->x = originalX * cos(transform.rotation * (M_PI / 180)) - originalY * sin(transform.rotation * (M_PI / 180));
        currentPoint->y = originalX * sin(transform.rotation * (M_PI / 180)) + originalY * cos(transform.rotation * (M_PI / 180));
        currentPoint->x = currentPoint->x + transform.xDistance;
        currentPoint->y = currentPoint->y + transform.yDistance;

        printf("TRANSFORMED POINT COORDINATES\n");
        printf("Point: (%f, %f)\n", currentPoint->x, currentPoint->y);
        printf("\n");

        fprintf(svgFile, "%lf,%lf ", currentPoint->x, -currentPoint->y);

        currentPoint = currentPoint->next;
    }
    fprintf(svgFile, "\" fill=\"none\" stroke=\"black\" />\n");
}

/**
 * Generates a fractal
 Passes the root fractal to this function
  */
void generateFractal(Fractal* root) {
    //root fractal is the first fractal in the fractal linked list
    //same algorithm for every fractal
    branchDataList *BDL = malloc(sizeof(branchDataList) + 100);
    Transform initialTransform = {NULL, 0.0, 0.0, 0.0, 1.0, NULL};

    BDL->iterationCount = 0;
    BDL->rootFractal = root;
    BDL->globalTransform = initialTransform;
    BDL->next = NULL;
    FILE* svgFile = fopen("output.svg", "w");
    if (svgFile == NULL) {
        perror("Error opening SVG file");
        return;
    }
    fprintf(svgFile, "<svg viewBox=\"-1100 -1100 1500 1500\" xmlns=\"http://www.w3.org/2000/svg\">\n");

    while (BDL != NULL) {
        Branch *branch = BDL->rootFractal->branch;
        while (branch != NULL) {
            BDL->globalTransform.rotation = initialTransform.rotation;
            BDL->globalTransform.xDistance = initialTransform.xDistance;
            BDL->globalTransform.yDistance = initialTransform.yDistance;
            BDL->globalTransform.scale = initialTransform.scale;
            // Check if the iteration level is within the specified range of the branch
            if (BDL->iterationCount >= branch->startRange && BDL->iterationCount <= branch->endRange) {
                // Apply the current global transform to the branch transform
                Transform* transform = startTransform;
                while(strcmp(branch->transformName, transform->transformName) != 0) {
                    transform = transform->next;
                }
                printf("--------------------------------------------------\n");
                printf("APPLY TRANSFORM FOR BRANCH %s AT ITERATION %d\n", branch->transformName, BDL->iterationCount);
                Transform tr_new = applyTransform(BDL->globalTransform, *transform);

                // Create a new branch data list item (bn)
                branchDataList* BN = malloc(sizeof(branchDataList) + 100);
                BN->iterationCount = BDL->iterationCount + 1;
                BN->globalTransform = tr_new;

                // If the branch has a GRAPHIC item, draw the graphic to an SVG file
                if (strcmp(branch->instructionType, "GRAPHIC") == 0) {
                    Graphic* graphic = startGraphic;
                    while (strcmp(branch->instructionName, graphic->graphicName) != 0) {
                        graphic = graphic->next;
                    }
                    drawGraphic(graphic, tr_new, svgFile);
                }

                // Set bn.frac based on the branch's instruction type
                if (strcmp(branch->instructionType, "GRAPHIC") == 0) {
                    BN->rootFractal = BDL->rootFractal;
                } else if (strcmp(branch->instructionType, "FRACTAL") == 0) {
                    Fractal* fractal = startFractal;
                    while(strcmp(branch->instructionName, fractal->fractal_name) != 0) {
                        fractal = fractal->next;
                    }
                    BN->rootFractal = fractal;
                }
                // Add bn to the branch data list BDL
                BN->next = BDL->next;
                BDL->next = BN;
            }
            branch = branch->next;
        }
        // Move to the next item in BDL
        branchDataList* nextBDL = BDL->next;
        free(BDL);
        BDL = nextBDL;
    }
    fprintf(svgFile, "</svg>\n");
    fclose(svgFile);
}

/**
HELLO
*/
int main() {
    readTextFile("NFSF.txt");
    printInformation();
    Fractal* fractal = startFractal;
    while(fractal != NULL){
        generateFractal(fractal);
        fractal = fractal->next;
    }
    freeMemory();
    printf("\n");
    printf("FINISHED\n");
    return 0;
}