#include "imit_cat_processing.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "imit_cat_options.h"

void convertToSpecialIfNeeded(enum options *option, char symbol,
                              int optionsAmount) {
  if (((containsOption(T, optionsAmount, option) == 1) ||
       (containsOption(t, optionsAmount, option) == 1)) &&
      (symbol == '\t')) {
    printf("^");
    symbol = '\t' + 64;
  }
  if (((containsOption(v, optionsAmount, option) == 1) ||
       (containsOption(e, optionsAmount, option) == 1) ||
       (containsOption(t, optionsAmount, option) == 1)) &&
      symbol >= 0 && symbol <= 31 && symbol != '\t') {
    printf("^");
    symbol = symbol + 64;
  }
  printf("%c", symbol);
}

void printSymbol(FILE *fileToRead, enum options *option, char symbol,
                 int optionsAmount, int *stringNumber, int *isFirstSymbol,
                 char *previousSymbol, int *squeezedBlank) {
  if (*isFirstSymbol == 1) {
    int needToPrint = 0;
    int bOptionNumeration =
        (containsOption(b, optionsAmount, option) == 1) && (symbol != '\n');
    int bOptionWithBlanc =
        (containsOption(b, optionsAmount, option) == 1) && (symbol == '\n');
    int nOptionNumeration = (containsOption(n, optionsAmount, option) == 1);
    if ((bOptionNumeration == 1) ||
        (nOptionNumeration == 1)) {  ///будет нумерация
      *stringNumber = *stringNumber + 1;
      needToPrint = 1;
      if ((bOptionWithBlanc == 1) && (nOptionNumeration == 1)) {
        *stringNumber = *stringNumber - 1;
        needToPrint = 0;
      }
      if (needToPrint == 1) {
        printf("%6d\t", *stringNumber);  ///печать номера
      }
    }
    *isFirstSymbol = 0;
  }

  if (symbol == '\n') {
    if (containsOption(s, optionsAmount, option) == 1) {
      if (*previousSymbol == '\n') {  /////склеиваем строки
        while (symbol == '\n') {
          symbol = getc(fileToRead);
        }
        *squeezedBlank = 1;
      } else {
        *squeezedBlank = 0;
      }
    }
    if ((containsOption(e, optionsAmount, option) == 1) ||
        (containsOption(E, optionsAmount, option) == 1)) {
      printf("$");
    }
    printf("\n");
    *isFirstSymbol = 1;
  } else {
    /////проверка текущего и предыдущего символа на tab и принадлежность к спец
    ///символам
    if (*squeezedBlank == 1) {
      convertToSpecialIfNeeded(option, *previousSymbol, optionsAmount);
      *squeezedBlank = 0;
    }
    convertToSpecialIfNeeded(option, symbol, optionsAmount);
  }

  *previousSymbol = symbol;
};

void processOptions(enum options *option, int optionsAmount, char **files,
                    int filesAmount) {
  int symbol = 0;

  int stringNumber = 0;
  int isFirstSymbol = 1;
  char previousSymbol = symbol;
  int squeezedBlank = 0;

  for (int i = 0; i < filesAmount; i++) {
    FILE *fileToRead;

    if (strcmp(files[i], "-") == 0) {
      fileToRead = stdin;
    } else {
      fileToRead = fopen(files[i], "r");
    }

    if (fileToRead != NULL) {
      previousSymbol = symbol;
      squeezedBlank = 0;

      while ((symbol = getc(fileToRead)) != EOF) {
        printSymbol(fileToRead, option, symbol, optionsAmount, &stringNumber,
                    &isFirstSymbol, &previousSymbol, &squeezedBlank);
      }
      if (strcmp(files[i], "-") != 0) {
        fclose(fileToRead);
      }
      if ((i != filesAmount - 1) && (symbol != EOF)) {
        printf("\n");
      }
    } else {
      fprintf(stderr, "cat: %s: No such file or directory\n", files[i]);
    }
  }
  free(option);
}