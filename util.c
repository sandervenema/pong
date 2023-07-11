#include "util.h"

#include <stdlib.h>

float random_between(float min, float max)
{    
    return min + (((float) rand()) / (float) RAND_MAX) * (max - min);    
}
