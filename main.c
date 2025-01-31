#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
    float x, y, angle; 
} stPto;

void defineVector(int m, stPto **v) {
    *v = (stPto *)malloc(m * sizeof(stPto));
    if (!*v) {
        printf("Error in allocation. Vector is undefined.\n");
        exit(1);
    }
    for (int i = 0; i < m; i++) {
        (*v)[i].x = rand() % 100;
        (*v)[i].y = rand() % 100;
    }
}

void showVector(int n, stPto vx[]) {
    printf("\n");
    for (int i = 0; i < n; i++) {
        printf("(%.2f, %.2f) ", vx[i].x, vx[i].y);
    }
    printf("\n\n");
}

void convertVector(int n, stPto vx[], stPto **vvx) {
    *vvx = (stPto *)malloc(n * sizeof(stPto));
    if (!*vvx) {
        printf("Error in allocation. Vector is undefined.\n");
        exit(1);
    }
    for (int i = 0; i < n; i++) {
        (*vvx)[i].x = vx[(i + 1) % n].x - vx[i].x;
        (*vvx)[i].y = vx[(i + 1) % n].y - vx[i].y;
    }
}

int convex(int i, int j, stPto a[]) {
    return a[i].x * a[j].y - a[j].x * a[i].y;
}

void convexityAnalysis(int n, stPto vvx[]) {
    for (int i = 0; i < n; i++) {
        int w = convex(i, (i + 1) % n, vvx);
        if (w < 0) {
            printf("\nVertex %d is concave", i + 1);
        } else {
            printf("\nVertex %d is convex", i + 1);
        }
    }
    printf("\n\n");
}

void polygonCenterOrigin(int n, stPto **vx) {
    stPto ccx = {0, 0};
    for (int i = 0; i < n; i++) {
        ccx.x += (*vx)[i].x;
        ccx.y += (*vx)[i].y;
    }
    ccx.x /= n;
    ccx.y /= n;
    for (int i = 0; i < n; i++) {
        (*vx)[i].x -= ccx.x;
        (*vx)[i].y -= ccx.y;
    }
}

void regularPolygonVertex(int n, stPto **vx) {
    polygonCenterOrigin(n, vx);
    for (int i = 0; i < n; i++) {
        float norma = sqrt((*vx)[i].x * (*vx)[i].x + (*vx)[i].y * (*vx)[i].y);
        (*vx)[i].angle = acos((*vx)[i].x / norma) * 180 / M_PI;
        if ((*vx)[i].y < 0) {
            (*vx)[i].angle = 360 - (*vx)[i].angle;
        }
    }
}

void orderAngles(int n, stPto **vx) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if ((*vx)[i].angle > (*vx)[j].angle) {
                stPto temp = (*vx)[i];
                (*vx)[i] = (*vx)[j];
                (*vx)[j] = temp;
            }
        }
    }
}

void showPolygon(int n, stPto vx[]) {
    printf("\nVectors = {\n");
    for (int i = 0; i < n; i++) {
        printf("    Vector[(%.2f, %.2f), (%.2f, %.2f)]", vx[i].x, vx[i].y, vx[(i + 1) % n].x, vx[(i + 1) % n].y);
        if (i != n - 1) {
            printf(",\n");
        }
    }
    printf("\n}\n");
}

void transformConvexPolygon(int n, stPto **vvx, stPto **vx) {
    int concave;
    do {
        concave = 0;
        for (int i = 0; i < n; i++) {
            if (convex(i, (i + 1) % n, *vvx) < 0) {
                concave = 1;
                for (int j = (i + 1) % n; j < n - 1; j++) {
                    (*vx)[j] = (*vx)[j + 1];
                }
                n--;
                convertVector(n, *vx, vvx);
                *vx = realloc(*vx, n * sizeof(stPto));
                *vvx = realloc(*vvx, n * sizeof(stPto));
                break;
            }
        }
    } while (concave);
    showPolygon(n, *vx);
}

int main() {
    int n;
    stPto *vx, *vvx;
    printf("Enter the number of vertices: ");
    scanf("%d", &n);
    
    defineVector(n, &vx);
    printf("\nVertices of the polygon:");
    showVector(n, vx);
    convertVector(n, vx, &vvx);
    regularPolygonVertex(n, &vx);
    orderAngles(n, &vx);
    
    printf("\n\nVertices of the regular ordered polygon:");
    showVector(n, vx);
    
    printf("\nEdges of the polygon:");
    showVector(n, vvx);
    
    printf("List of original vectors:");
    showPolygon(n, vx);
    
    printf("\nConvexity of the polygon before: ");
    convexityAnalysis(n, vvx);
    
    printf("List of vectors of the new polygon:");
    transformConvexPolygon(n, &vvx, &vx);
    
    free(vx);
    free(vvx);
    return 0;
}
