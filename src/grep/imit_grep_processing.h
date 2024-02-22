#ifndef IMIT_GREP_PROCESSING_H
#define IMIT_GREP_PROCESSING_H

#include "imit_grep_options.h"

void processOptions(enum options *option, int optionsAmount, char **files,
                    int *fileExists, int filesAmount, char **patterns,
                    int patternsAmount);

#endif