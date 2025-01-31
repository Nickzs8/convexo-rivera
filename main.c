#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
    float x, y, angulo;
} stPto;

void defVetor(int m, stPto **v) {
    *v = (stPto *)malloc(m * sizeof(stPto));
    if (!*v) {
        printf("Error in alocation. Vector is undefined");
        exit(1);
    }
    for (int i = 0; i < m; i++) {
        (*v)[i].x = rand() % 100;
        (*v)[i].y = rand() % 100;
    }
}

void mostraVetor(int n, stPto vx[]) {
    printf("\n");
    for (int i = 0; i < n; i++) {
        printf("(%.2f, %.2f) ", vx[i].x, vx[i].y);
    }
    printf("\n\n");
}

void converteVetorPolig(int n, stPto vx[], stPto **vvx) {
    *vvx = (stPto *)malloc(n * sizeof(stPto));
    if (!*vvx) {
        printf("Error in alocation. Vector is undefined.");
        exit(1);
    }
    for (int i = 0; i < n; i++) {
        (*vvx)[i].x = vx[(i + 1) % n].x - vx[i].x;
        (*vvx)[i].y = vx[(i + 1) % n].y - vx[i].y;
    }
}

int convexo(int i, int j, stPto a[]) {
    return a[i].x * a[j].y - a[j].x * a[i].y;
}

void analiseConvexidade(int n, stPto vvx[]) {
    for (int i = 0; i < n; i++) {
        int w = convexo(i, (i + 1) % n, vvx);
        if (w < 0) {
            printf("\nVextex %d is concave", i + 1);
        } else {
            printf("\n Vextex %d is convex", i + 1);
        }
    }
    printf("\n\n");
}

void poliOrigemCentro(int n, stPto **vx) {
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

void poliRegularEstrela(int n, stPto **vx) {
    poliOrigemCentro(n, vx);
    for (int i = 0; i < n; i++) {
        float norma = sqrt((*vx)[i].x * (*vx)[i].x + (*vx)[i].y * (*vx)[i].y);
        (*vx)[i].angulo = acos((*vx)[i].x / norma) * 180 / M_PI;
        if ((*vx)[i].y < 0) {
            (*vx)[i].angulo = 360 - (*vx)[i].angulo;
        }
    }
}

void ordenaVetorAng(int n, stPto **vx) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if ((*vx)[i].angulo > (*vx)[j].angulo) {
                stPto temp = (*vx)[i];
                (*vx)[i] = (*vx)[j];
                (*vx)[j] = temp;
            }
        }
    }
}

void mostrarPoligono(int n, stPto vx[]) {
    printf("\nVectors = {\n");
    for (int i = 0; i < n; i++) {
        printf("    Vector[(%.2f, %.2f), (%.2f, %.2f)]", vx[i].x, vx[i].y, vx[(i + 1) % n].x, vx[(i + 1) % n].y);
        if (i != n - 1) {
            printf(",\n");
        }
    }
    printf("\n}\n");
}

void transformarEmPoligonoConvexo(int n, stPto **vvx, stPto **vx) {
    int concavo;
    do {
        concavo = 0;
        for (int i = 0; i < n; i++) {
            if (convexo(i, (i + 1) % n, *vvx) < 0) {
                concavo = 1;
                for (int j = (i + 1) % n; j < n - 1; j++) {
                    (*vx)[j] = (*vx)[j + 1];
                }
                n--;
                converteVetorPolig(n, *vx, vvx);
                *vx = realloc(*vx, n * sizeof(stPto));
                *vvx = realloc(*vvx, n * sizeof(stPto));
                break;
            }
        }
    } while (concavo);
    mostrarPoligono(n, *vx);
}

int main() {
    int n;
    stPto *vx, *vvx;
    printf("Enter the number of vertex: ");
    scanf("%d", &n);
    
    defVetor(n, &vx);
    printf("\nVertex of the polygon:");
    
    mostraVetor(n, vx);
    converteVetorPolig(n, vx, &vvx);
    poliRegularEstrela(n, &vx);
    ordenaVetorAng(n, &vx);
    
    printf("\n\nVertex of the regular ordered polygon:");
    mostraVetor(n, vx);
    
    printf("\nEdges of the polygon:");
    mostraVetor(n, vvx);
    
    printf("List of original vectors");
    mostrarPoligono(n, vx);
    
    printf("\nConvexity of the polygon before: ");
    analiseConvexidade(n, vvx);
    
    printf("List of vectors of the new polygon:");
    transformarEmPoligonoConvexo(n, &vvx, &vx);
    
    free(vx);
    free(vvx);
    return 0;
}
