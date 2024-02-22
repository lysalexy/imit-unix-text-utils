#include "imit_grep_options.h"

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct charToOptionNumber {
  char ch;
  int optionNumber;
};

enum options *identifyStringOptions(int *argc, char **argv, char **patterns,
                                    int *patternsAmount,
                                    int *patternsFromFileAmount,
                                    int *regexFilesAmount) {
  if (*argc != 0) {
    enum options *allOptions = calloc(*argc, sizeof(enum options));

    int option_index = 0;

    struct option baseOptions[] = {{"regexp", 1, 0, 'e'},
                                   {"ignore-case", 0, 0, 'i'},
                                   {"invert-match", 0, 0, 'v'},
                                   {"count", 0, 0, 'c'},
                                   {"files-with-matches", 0, 0, 'l'},
                                   {"line-number", 0, 0, 'n'}};

    int optionsAmount = 0;
    *patternsAmount = 0;
    int i = 1;

    struct charToOptionNumber charsToNumbers[9] = {
        {'e', 0}, {'i', 1}, {'v', 2}, {'c', 3}, {'l', 4},
        {'n', 5}, {'h', 6}, {'f', 7}, {'s', 8}};

    while ((i = getopt_long(*argc + 2, argv, "e:ivclnhf:so", baseOptions,
                            &option_index)) != -1) {
      int number = 0;
      while (i != charsToNumbers[number].ch) {
        number++;
      }
      allOptions[optionsAmount] = charsToNumbers[number].optionNumber;
      optionsAmount++;
      if (i == 'f') {
        char *string = NULL;
        size_t stringLength = 0;
        FILE *fileToRead;
        fileToRead = fopen(optarg, "r");
        if (fileToRead != NULL) {
          while (getline(&string, &stringLength, fileToRead) != EOF) {
            char *newString = strdup(string);
            patterns[*patternsAmount] = newString;
            newString[strlen(string)] = '\0';
            if (string[strlen(string) - 1] == '\n') {
              newString[strlen(string) - 1] = '\0';
            }
            *patternsAmount = *patternsAmount + 1;
            *patternsFromFileAmount = *patternsFromFileAmount + 1;
          }
          free(string);
          *regexFilesAmount = *regexFilesAmount + 1;
          fclose(fileToRead);
        } else {
          fprintf(stderr, "grep: %s: No such file or directory\n", optarg);
        }
      }
      if (i == 'e') {
        patterns[*patternsAmount] = strdup(optarg);
        *patternsAmount = *patternsAmount + 1;
      }
    }

    *argc = optionsAmount;

    if (optionsAmount != 0) {
      return allOptions;
    } else {
      free(allOptions);
    }
  }
  return NULL;
};

int containsOption(enum options option, int optionsAmount,
                   enum options *optionsArray) {
  for (int i = 0; i < optionsAmount; i++) {
    if (optionsArray[i] == option) {
      return 1;
    }
  }
  return 0;
}
