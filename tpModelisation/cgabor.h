/*------------------------------------------------------------------------*/

#include <math.h>
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

/*------------------------------------------------------------------------*/

#define NDIR     8
#define NSCA     7
#define SIGMA0   2.0
#define LAMBDA0  2.5
#define SCALE    sqrt(2.0)

/*------------------------------------------------------------------------*/

float cgabor(CIMAGE cim, float sigma, float theta, float lambda);

float *egabor(CIMAGE cim, int ndir, int nsca, float sigma0, float lambda0,
float scale);

void plotgabor(float *eg, int ndir, int nsca, char *name);

/*------------------------------------------------------------------------*/
