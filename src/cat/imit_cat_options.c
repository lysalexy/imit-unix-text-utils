#include "imit_cat_options.h"

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct charToOptionNumber {
  char ch;
  int optionNumber;
};

enum options *identifyStringOptions(int *argc, char **argv) {
  if (*argc != 0) {
    enum options *allOptions = calloc(*argc, sizeof(enum options));

    int option_index = 0;

    struct option baseOptions[] = {{"number-nonblank", 0, 0, 'b'},
                                   {"number", 0, 0, 'n'},
                                   {"squeeze-blank", 0, 0, 's'}};

    int optionsAmount = 0;
    int i = 1;

    struct charToOptionNumber charsToNumbers[8] = {{'b', 0}, {'e', 1}, {'v', 2},
                                                   {'n', 3}, {'s', 4}, {'T', 5},
                                                   {'E', 6}, {'t', 7}};

    while ((i = getopt_long(*argc + 2, argv, "bevnsTEt", baseOptions,
                            &option_index)) != -1) {
      int number = 0;
      while (i != charsToNumbers[number].ch) {
        number++;
      }
      allOptions[optionsAmount] = charsToNumbers[number].optionNumber;
      optionsAmount++;
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
