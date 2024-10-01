#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


typedef struct{
    float x, y, angulo, convex;
} stPto;

void defVetor(int m, stPto *v[]) {
    int i;
    stPto *vt;
    vt = (stPto *)malloc(m * sizeof(stPto));
    
    if (!vt) {
        printf("Erro na alocacao! Vetor nao definido.");
        exit(1);
    }
    
    for (i = 0; i < m; i++) {
        vt[i].x = rand() % 100;
        vt[i].y = rand() % 100;
    }
    *v = vt;
}

void mostraVetor(int n, stPto vx[]) {
    int i;
    printf("\n");
    for (i = 0; i < n; i++) {
        printf("(%.2f, %.2f) ", vx[i].x, vx[i].y);
    }
    printf("\n");
}

void converteVetorPolig(int n, stPto vx[], stPto *vvx[]) {
    *vvx = (stPto *)malloc(n * sizeof(stPto));
    
    if (!vvx) {
        printf("Erro na alocacao! Vetor nao definido.");
        exit(1);
    }
    
    int i = 0;
    while (i < n) {
        (*vvx)[i].x = vx[(i + 1) % n].x - vx[i].x;
        (*vvx)[i].y = vx[(i + 1) % n].y - vx[i].y;
        i++;
    }
}

int convexo(int i, int j, stPto a[]) {
    return a[i].x * a[j].y - a[j].x * a[i].y;
}

void analiseConvexidade(int n, stPto vvx[], stPto *vx[]) {
    // quando chegar no ultimo atribuir para o primeiro
    int i;
    for (i = 0; i < n; i++) {
        int w = convexo(i, (i + 1) % n, vvx);
        if (w < 0) {
            (*vx)[(i+1)%n].convex = 0;
            printf("\nVertice %d eh concavo", i + 1);
        } else {
            printf("\nVertice %d eh convexo", i + 1);
            (*vx)[(i+1)%n].convex = 1;
        }
    }
}

void poliOrigemCentro(int n, stPto *vx[]) {
    stPto ccx;
    ccx.x = 0;
    ccx.y = 0;
    int i;

    for (i = 0; i < n; i++) {
        ccx.x += (*vx)[i].x;
        ccx.y += (*vx)[i].y;
    }
    ccx.x /= n;
    ccx.y /= n;

    for (i = 0; i < n; i++) {
        (*vx)[i].x -= ccx.x;
        (*vx)[i].y -= ccx.y;
    }
}

void poliRegularEstrela(int n, stPto *vx[]) {

    poliOrigemCentro(n, vx);

    for (int i = 0; i < n; i++) {
        float norma = sqrt((*vx)[i].x * (*vx)[i].x + (*vx)[i].y * (*vx)[i].y);
        (*vx)[i].angulo = acos((*vx)[i].x / norma) * 180 / M_PI;
        if ((*vx)[i].y < 0) {
            (*vx)[i].angulo = 360 - (*vx)[i].angulo;
        }
    }
}

void ordenaVetorAng(int n, stPto *vx[]) {
    int i, j;
    for (i = 0; i < n - 1; i++) {
        for (j = i + 1; j < n; j++) {
            if ((*vx)[i].angulo > (*vx)[j].angulo) {

                float auxAngulos = (*vx)[i].angulo;
                (*vx)[i].angulo = (*vx)[j].angulo;
                (*vx)[j].angulo = auxAngulos;

                float auxVX = (*vx)[i].x;
                (*vx)[i].x = (*vx)[j].x;
                (*vx)[j].x = auxVX;

                float auxVY = (*vx)[i].y;
                (*vx)[i].y = (*vx)[j].y;
                (*vx)[j].y = auxVY;
            }
        }
    }
}
void mostrarPoligono (int n, stPto vx[]) {
    printf("\nVectors = {\n");
  for (int i = 0; i < n; i++) {
      printf("    Vector[(%.2f, %.2f), (%.2f, %.2f)]", vx[i].x, vx[i].y, vx[(i + 1) % n].x, vx[(i + 1) % n].y);
      if (i != n - 1) {
          printf(",\n");
      }
  }
  printf("\n}\n");
}

void transformarEmPoligonoConvexo(int *n, stPto *vvx[], stPto *vx[]) {
    int i, j, k;
    
    for (i = 0; i < *n; i++) {
        if ((*vx)[i].convex && (*vx)[(i + 1) % *n].convex) {
            continue; 
        } else if ((*vx)[i].convex) {
            for (j = (i + 2) % *n; j < *n; j++) {
                if ((*vx)[j].convex) {
                    (*vx)[(i + 1) % *n].x = (*vx)[j].x;
                    (*vx)[(i + 1) % *n].y = (*vx)[j].y;

                    for (k = i + 2; k < *n; k++) {
                        (*vx)[k].x = (*vx)[k + (j - (i + 1))].x;
                        (*vx)[k].y = (*vx)[k + (j - (i + 1))].y;
                    }
                    *n -= fabs(j - (i + 1));
                    stPto *temp = realloc(*vx, (*n) * sizeof(stPto));

                    if (temp == NULL) {
                        exit(1); 
                    }
                    *vx = temp;
                    break; 
                }
            }
        }
    }
    converteVetorPolig(*n, *vx, vvx);
    printf("\n");
    mostrarPoligono(*n, *vx);
}

int main() {
    int n;
    stPto *vx, *vvx;

    printf("Digite o valor de n: ");
    scanf("%d", &n);

    defVetor(n, &vx);

    printf("\nVertices do poligono:");
    mostraVetor(n, vx);
    printf("\n");

    converteVetorPolig(n, vx, &vvx);

    poliRegularEstrela(n, &vx);
    ordenaVetorAng(n, &vx);

    printf("\n\nVertices do poligono regular ordenados:");
    mostraVetor(n, vx);

    printf("\nArestas do poligono:");
    mostraVetor(n, vvx);

    mostrarPoligono(n, vx);

    printf("\nConvexidade do poligono antes: ");
    analiseConvexidade(n, vvx, &vx);

    transformarEmPoligonoConvexo(&n, &vvx, &vx);

    free(vx);
    free(vvx);
    return 0;
}
