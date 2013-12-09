#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "rdjpeg.h"
#include "cgabor.h"

int main(int argc, char *argv[]){
  float *eg;
  CIMAGE cim;

  read_cimage(argv[1],&cim);
  eg = egabor(cim,NDIR,NSCA,SIGMA0,LAMBDA0,SCALE);
  if (argc > 2) plotgabor(eg,NDIR,NSCA,argv[2]);
  exit(0);
}
