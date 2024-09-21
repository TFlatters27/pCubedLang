#include "include/io.h"
#include <stdlib.h>
#include <stdio.h>

char *get_file_contents(const char *filepath)
{

  if (filepath == NULL)
  {
    printf("Error: A file must be specified.\n");
    exit(EXIT_FAILURE);
  }

  char *buffer = 0;
  long length;

  FILE *file = fopen(filepath, "rb");

  if (file)
  {
    fseek(file, 0, SEEK_END);
    length = ftell(file);
    fseek(file, 0, SEEK_SET);

    buffer = calloc(length, length);

    if (buffer)
      fread(buffer, 1, length, file);

    fclose(file);
    return buffer;
  }

  printf("Error reading file %s\n", filepath);
  exit(2);
}