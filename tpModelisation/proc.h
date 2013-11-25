#ifndef _PROC_H
#define _PROC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

typedef struct {
  int k;
  double d;
} KEY;

int keyCompare(const void *key1, const void *key2);
char **readList(char *name, int *n);
float **readDescriptors(char *name, int n, int *d);
int *readAnnotations(char *name, int n);

#endif