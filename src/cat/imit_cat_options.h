#ifndef IMIT_CAT_OPTION_H
#define IMIT_CAT_OPTION_H

enum options { b, e, v, n, s, T, E, t };

enum options *identifyStringOptions(int *argc, char **argv);

int containsOption(enum options option, int optionsAmount,
                   enum options *optionsArray);

#endif