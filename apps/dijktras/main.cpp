#include "graph.h"
#include <stdio.h>

int main(int argc, char** argv)
{
  FILE *fp = fopen(argv[1], "r");
  if (fp == NULL)
  {
    printf("Fucked it\n");
  }
  return 0;
}