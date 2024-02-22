#include "imit_grep_processing.h"

#include <pcre.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *createIgnoreCasePattern(char *original) {
  char *resultString = calloc(strlen(original) + 8, sizeof(char));
  resultString[0] = '(';
  resultString[1] = '?';
  resultString[2] = 'i';
  resultString[3] = ')';
  resultString[4] = '(';
  resultString[5] = '\0';

  char *postfix = calloc(2, sizeof(char));

  postfix[0] = ')';
  postfix[1] = '\0';
  resultString = strcat(resultString, original);
  resultString = strcat(resultString, postfix);
  free(postfix);

  return resultString;
}

int stringMatchesPatterns(char *string, char **patterns, int patternsAmount,
                          int ignoreCase, int *ovector, int *count) {
  int result = 0;
  int *previousOvector = calloc(strlen(string) * 3, sizeof(int));
  for (int i = 0; i < patternsAmount; i++) {
    pcre *re;
    int options = 0;
    const char *error;
    int erroffset;

    char *pattern = patterns[i];

    if (ignoreCase == 1) {
      pattern = createIgnoreCasePattern(patterns[i]);
    }

    re = pcre_compile(pattern, options, &error, &erroffset, NULL);

    if (!re) {
      printf("Failed\n");
    } else {
      *count = pcre_exec(re, NULL, string, strlen(string), 0, 0, ovector,
                         strlen(string) * 3);
      if (*count >= 0) {
        result = 1;
      }

      if (ignoreCase == 1) {
        free(pattern);
      }
      pcre_free(re);
    }
  }
  free(previousOvector);

  return result;
};

void printFileString(int needToPrintName, char *fileName, int fileExists,
                     enum options *option, int optionsAmount,
                     int amountOfMatchedStrings, int onlyString) {
  int printed = 0;
  if ((needToPrintName) && (fileExists)) {
    printed = 1;
    printf("%s", fileName);
  }
  if ((containsOption(c, optionsAmount, option) == 1) &&
      (containsOption(l, optionsAmount, option) == 0) && (fileExists)) {
    if ((!onlyString) && (containsOption(h, optionsAmount, option) == 0)) {
      printf(":");
    }
    printf("%d", amountOfMatchedStrings);
    printed = 1;
  }
  if (printed == 1) {
    printf("\n");
  }
}

void printFileInfo(char *fileName, enum options *option, int optionsAmount,
                   int amountOfMatchedStrings, int onlyString, int *printed) {
  int needToPrint = (containsOption(c, optionsAmount, option) == 1);
  if (containsOption(l, optionsAmount, option) == 1) {
    needToPrint = amountOfMatchedStrings != 0;
  }
  int needToPrintName =
      ((containsOption(l, optionsAmount, option) == 1)) ||
      ((!onlyString) && (containsOption(h, optionsAmount, option) == 0));

  if (needToPrint == 1) {
    printFileString(needToPrintName, fileName, 1, option, optionsAmount,
                    amountOfMatchedStrings, onlyString);
    *printed = 1;
  }
};

void printString(int needToPrintName, char *fileName, int optionsAmount,
                 enum options *option, int stringNumber, char *string,
                 int newLineNeeded) {
  if (needToPrintName == 1) {
    printf("%s:", fileName);
  }
  if (containsOption(n, optionsAmount, option) == 1) {
    printf("%d:", stringNumber);
  }

  printf("%s", string);
  if (newLineNeeded == 1) {
    printf("\n");
  }
}

void printLine(char *string, enum options *option, int optionsAmount,
               int *stringNumber, char **patterns, int patternsAmount,
               int *amountOfMatchedStrings, int needToPrintName,
               char *fileName) {
  int *ovector = calloc(strlen(string) * 3, sizeof(int));
  int count = 0;
  int matches = stringMatchesPatterns(
      string, patterns, patternsAmount,
      (containsOption(i, optionsAmount, option) == 1), ovector, &count);
  if (containsOption(v, optionsAmount, option) == 1) {
    matches = (matches == 1) ? 0 : 1;
  }
  *amountOfMatchedStrings = *amountOfMatchedStrings + matches;
  int needToPrint = !((containsOption(c, optionsAmount, option) == 1) ||
                      (containsOption(l, optionsAmount, option) == 1));
  if (needToPrint == 1) {
    if (matches == 1) {
      printString(needToPrintName, fileName, optionsAmount, option,
                  *stringNumber, string, 0);
    }
  }

  *stringNumber = *stringNumber + 1;
  free(ovector);
};

void processOptions(enum options *option, int optionsAmount, char **files,
                    int *fileExists, int filesAmount, char **patterns,
                    int patternsAmount) {
  int amountOfMatchedStrings = 0;
  int summaryAmountOfMatchedStrings = 0;
  int printed = 0;

  int stringNumber = 1;
  int amountOfOpenedFiles = 0;

  for (int i = 0; i < filesAmount; i++) {
    FILE *fileToRead;

    if (strcmp(files[i], "-") == 0) {
      fileToRead = stdin;
      files[i] = "(standart input)";
    } else {
      fileToRead = fopen(files[i], "r");
    }

    if (fileToRead != NULL) {
      fileExists[i] = 1;
      amountOfOpenedFiles += 1;

      char *string = NULL;
      size_t stringLength = 0;
      amountOfMatchedStrings = 0;
      stringNumber = 1;
      stringLength = 1;

      while (getline(&string, &stringLength, fileToRead) != EOF) {
        int needToPrintFileName =
            (filesAmount != 1) &&
            (containsOption(h, optionsAmount, option) == 0);
        printLine(string, option, optionsAmount, &stringNumber, patterns,
                  patternsAmount, &amountOfMatchedStrings, needToPrintFileName,
                  files[i]);
      }
      summaryAmountOfMatchedStrings += amountOfMatchedStrings;

      printFileInfo(files[i], option, optionsAmount, amountOfMatchedStrings,
                    filesAmount == 1, &printed);

      if (strcmp(files[i], "(standart input)") != 0) {
        fclose(fileToRead);
      }
      free(string);

    } else {
      if (containsOption(s, optionsAmount, option) == 0) {
        fprintf(stderr, "grep: %s: No such file or directory\n", files[i]);
      }
    }
  }
  if ((summaryAmountOfMatchedStrings == 0) && (amountOfOpenedFiles != 0)) {
    int onlyString = filesAmount == 1;
    int needToPrint = (containsOption(c, optionsAmount, option) == 1) &&
                      (containsOption(l, optionsAmount, option) == 0);
    int needToPrintName =
        ((!onlyString) && (containsOption(h, optionsAmount, option) == 0));
    for (int i = 0; i < filesAmount; i++) {
      if ((needToPrint) && (printed == 0)) {
        printFileString(needToPrintName, files[i], fileExists[i], option,
                        optionsAmount, 0, onlyString);
      }
    }
  }

  free(option);
};