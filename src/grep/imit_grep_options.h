#ifndef IMIT_GREP_OPTION_H
#define IMIT_GREP_OPTION_H

enum options { e, i, v, c, l, n, h, f, s };

enum options *identifyStringOptions(int *argc, char **argv, char **patterns,
                                    int *patternsAmount,
                                    int *patternsFromFileAmount,
                                    int *regexFilesAmount);

int containsOption(enum options option, int optionsAmount,
                   enum options *optionsArray);

#endif