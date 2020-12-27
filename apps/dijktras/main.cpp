#include "graph.h"
#include <stdio.h>

int main(int argc, char** argv)
{
  FILE *fp = fopen("graph.txt", "r");
  if (fp == NULL)
  {
    printf("Fucked it\n");
  }
  return 0;
}