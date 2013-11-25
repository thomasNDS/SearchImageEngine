#include "proc.h"

/*------------------------------------------------------------------------------------*/

int keyCompare(const void *key1, const void *key2)
{
  if (((KEY *) key1)->d < ((KEY *) key2)->d) return(-1);
  if (((KEY *) key1)->d > ((KEY *) key2)->d) return(+1);
  return(0);
}

/*------------------------------------------------------------------------------------*/

char **readList(char *name, int *n)
{
  int i;
  size_t len = 0;
  char *line = NULL;
  char **list,*p;
  FILE *fp;
  fp = fopen(name,"r");
  if (fp == NULL) return(NULL);
  *n = 0;
  while (getline(&line,&len,fp) > 0) (*n)++;
  rewind(fp);
  list = malloc((*n)*sizeof(char *));
  if (list == NULL) return(NULL);
  for (i = 0; i < *n; i++) {
    if (getline(&line,&len,fp) <= 0) return(NULL);
    if ((p = rindex(line,'\n'))) *p = 0;
    if ((p = rindex(line,'\r'))) *p = 0;
    list[i] = strdup(line);
    if (list[i] == NULL) return(NULL);
  }
  return(list);
}

float **readDescriptors(char *name, int n, int *d)
{
  int i;
  float **m;
  FILE *fp;
  fp = fopen(name,"r");
  if (fp == NULL) return(NULL);
  fseek(fp,0,SEEK_END);
  *d = ftell(fp)/(n*sizeof(float));
  rewind(fp);
  m = malloc(n*sizeof(float *));
  if (m == NULL) return(NULL);
  m[0] = malloc(n*(*d)*sizeof(float));
  if (m[0] == NULL) return(NULL);
  for (i = 1; i < n; i++) m[i] = m[i-1]+(*d);
  for (i = 0; i < n; i++) if (fread(m[i],sizeof(float),*d,fp) != *d) return(NULL);
  return(m);
}

int *readAnnotations(char *name, int n)
{
  int i,*a;
  size_t len = 0;
  char *line = NULL;
  FILE *fp;
  fp = fopen(name,"r");
  if (fp == NULL) return(NULL);
  a = malloc(n*sizeof(int));
  if (a == NULL) return(NULL);
  for (i = 0; i < n; i++) {
    if (getline(&line,&len,fp) <= 0) return(NULL);
    strtok(line," ");
    a[i] = atoi(strtok(NULL,"\n"));
  }
  return(a);
}

/*------------------------------------------------------------------------------------*/
