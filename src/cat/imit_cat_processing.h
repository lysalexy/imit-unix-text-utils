#ifndef IMIT_CAT_PROCESSING_H
#define IMIT_CAT_PROCESSING_H

#include "imit_cat_options.h"

void processOptions(enum options *option, int optionsAmount, char **files,
                    int filesAmount);

#endif