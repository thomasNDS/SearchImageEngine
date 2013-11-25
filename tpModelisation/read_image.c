#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "rdjpeg.h"

int main(int argc, char *argv[])
{
  int i,j,n,nx,ny,nb; /* j en vertical, i en horizontal */
  CIMAGE cim;
  /*------------------------------------------------*/
  /* lecture d'une image requête                    */
  /*------------------------------------------------*/
  read_cimage(argv[1],&cim);
  /*------------------------------------------------*/
  /* affichage des valeurs pour le premier bloc 8x8 */
  /* comme exemple de traitement                    */
  /*------------------------------------------------*/
  printf("Largeur de l'image : %d\n",cim.nx);
  printf("Heuteur de l'image : %d\n",cim.ny);
  printf("Plan rouge du premier bloc 8x8 :\n");
  for (j = 0; j < cim.ny; j++) {       /* ligne par ligne */
    printf("  ");
    for (i = 0; i < cim.nx; i++) {   /* pixel par pixel */
//       printf("%4d",cim.r[i][j]);
    }
    printf("\n");
  }
  printf("Plan vert du premier bloc 8x8 :\n");
  for (j = 0; j < cim.ny; j++) {       /* ligne par ligne */
    printf("  ");
    for (i = 0; i < cim.nx; i++) {   /* pixel par pixel */
//       printf("%4d",cim.g[i][j]);
    }
    printf("\n");
  }
  printf("Plan bleu du premier bloc 8x8 :\n");
  for (j = 0; j < cim.ny; j++) {       /* ligne par ligne */
    printf("  ");
    for (i = 0; i < cim.nx; i++) {   /* pixel par pixel */
//       printf("%4d",cim.b[i][j]);
    }
    printf("\n");
  }
  /*------------------------------------------------*/

  exit(0);
}
