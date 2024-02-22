#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "imit_grep_options.c"
#include "imit_grep_processing.c"

int main(int argc, char **argv) {
  int optionsAmount = argc - 2;
  int patternsAmount = 0;
  int patternsFromFileAmount = 0;
  int regexFilesAmount = 0;
  char **patterns = calloc(optionsAmount, sizeof(char *));
  enum options *optionsToImplement =
      identifyStringOptions(&optionsAmount, argv, patterns, &patternsAmount,
                            &patternsFromFileAmount, &regexFilesAmount);

  int patternInCommand = patternsAmount - patternsFromFileAmount;
  int filesAmount =
      argc - 1 - optionsAmount - patternInCommand - regexFilesAmount;
  int regexWithoutOption = 0;

  if (patternsAmount == 0) {
    for (int i = 1; i < argc; i++) {
      if (argv[i][0] != '-') {
        patterns[patternsAmount] = strdup(argv[i]);
        patternsAmount += 1;
        patternInCommand += 1;
        filesAmount -= 1;
        regexWithoutOption = 1;
        break;
      }
    }
  }

  char **files = calloc(filesAmount, sizeof(char *));
  int *fileExists = calloc(filesAmount, sizeof(int));

  int fileNumber = 0;
  int firstIteration = 1;
  for (int i = 1; i < argc; i++) {
    int isRegex = strcmp(argv[i - 1], "-e") == 0;
    int isFileToRead = strcmp(argv[i - 1], "-f") == 0;
    if ((!isRegex) && (!isFileToRead) && (argv[i][0] != '-')) {
      if ((firstIteration) && (regexWithoutOption)) {
        firstIteration = 0;
      } else {
        files[fileNumber] = argv[i];
        fileNumber++;
      }
    }
  }

  processOptions(optionsToImplement, optionsAmount, files, fileExists,
                 filesAmount, patterns, patternsAmount);

  for (int i = 0; i < patternsAmount; i++) {
    free(patterns[i]);
  }

  free(patterns);
  free(fileExists);
  free(files);
  return 0;
}