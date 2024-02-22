#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "imit_cat_options.h"
#include "imit_cat_processing.h"

int main(int argc, char **argv) {
  int optionsAmount = argc - 2;
  enum options *optionsToImplement =
      identifyStringOptions(&optionsAmount, argv);

  int filesAmount = argc - 1 - optionsAmount;

  char **files = calloc(filesAmount, sizeof(char *));
  for (int i = 0; i < filesAmount; i++) {
    files[i] = argv[optionsAmount + i + 1];
  }

  processOptions(optionsToImplement, optionsAmount, files, filesAmount);

  free(files);
  return 0;
}