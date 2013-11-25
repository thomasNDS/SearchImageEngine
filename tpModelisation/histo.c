#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "rdjpeg.h"
#include "proc.h"

double produitScalaire(double* U, double* V, int N) {
    double ps;
    int i;
    for (ps = 0.0, i = 0; i < N; i++)
        if (U[i] - V[i] > 0)
            ps += (U[i] - V[i]);
        else
            ps -= (U[i] - V[i]);

    return ps;
}

float distanceEuclidienne(double* trainDescriptor, double* valDescriptor) {
    return produitScalaire(trainDescriptor, valDescriptor, 64);
}

double* makeHisto(char* name, FILE *fileToWrite);

int main(int argc, char *argv[]) {

    char **urlList;
    int nbAns;
    char *name;
    FILE* fileToWrite = NULL;
    int i;
    int plafond = 10;
    char* url = argv[1];

    urlList = readList("urls.txt", &nbAns);
    if (strcmp(argv[1], "init") == 0) {
        printf("init\n");
        fileToWrite = fopen("result.bin", "w");
        for (i = 0; i < plafond; i++) {
            printf("nom fichier lu %s, nbAns : %d\n", urlList[i], nbAns);
            name = urlList[i];
            makeHisto(name, fileToWrite);
        }
        fclose(fileToWrite);
    } else {
        printf("search   -%s-  %d\n  ", argv[1], strcmp(argv[1], "init"));
        double* h = makeHisto(url, NULL);
        KEY* image = determinePlusProche(3, h, "result.bin", plafond);
        qsort(image, plafond, sizeof (KEY), keyCompare);
        printf(" res :%d \n", image[0].k);
    }

    /*------------------------------------------------*/

    exit(0);
}

KEY* determinePlusProche(int nombreImagesProches, double* vecteurEntree, char* nomFichierVecteurs, int nombreImages) {

    FILE *fichierVecteurs = fopen(nomFichierVecteurs, "r");
    KEY* tabRes = calloc(nombreImages, sizeof (KEY));
    if (fichierVecteurs == NULL) {
        printf("Erreur lors de la lecture de %s", nomFichierVecteurs);
    } else {
        int i;
        for (i = 0; i < nombreImages; i++) {
            double buffer[64];
            fread(buffer, sizeof (double), 64, fichierVecteurs);
            double res = distanceEuclidienne(buffer, vecteurEntree);
            KEY k = (KEY) malloc(sizeof (KEY));
            k = {.d = i, .k = res};
            tabRes[i] = k;
        }
    }
    return tabRes;
}

double* makeHisto(char* name, FILE * fileToWrite) {
    int i, j, n, nx, ny, nb; /* j en vertical, i en horizontal */
    CIMAGE cim;
    unsigned char **r;
    unsigned char **g;
    unsigned char **b;
    int rr, bb, gg;
    int nbIntervalle = 64;
    double* h = calloc(nbIntervalle, sizeof (double));
    int k;

    for (i = 0; i < nbIntervalle; i++) {
        h[i] = 0;
    }

    /*------------------------------------------------*/
    /* lecture d'une image requête                    */
    /*------------------------------------------------*/
    read_cimage(name, &cim);
    r = cim.r;
    g = cim.g;
    b = cim.b;
    /*------------------------------------------------*/
    /* affichage des valeurs pour le premier bloc 8x8 */
    /* comme exemple de traitement                    */
    /*------------------------------------------------*/
    //printf("Largeur de l'image : %d\n", cim.nx);
    //  printf("Heuteur de l'image : %d\n", cim.ny);

    for (j = 0; j < cim.ny; j++) { /* ligne par ligne */
        for (i = 0; i < cim.nx; i++) { /* pixel par pixel */
            rr = r[i][j] / 64;
            gg = g[i][j] / 64;
            bb = b[i][j] / 64;
            k = rr + 4 * bb + 16 * gg;
            // 	printf("k : %d\n", k);
            h[k]++;
        }
    }

    double somme = 0;
    //Normalisation
    for (i = 0; i < nbIntervalle; i++) {
        h[i] = h[i] / (cim.nx * cim.ny);
        somme += h[i];
        // printf("%d ème valeur histo : %f\n", i, h[i]);
    }
    if (fileToWrite != NULL)
        fwrite(h, sizeof (double), 64, fileToWrite);
    return h;
    // printf("somme : %f\n", somme);
}
