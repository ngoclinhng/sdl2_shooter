#include <stdlib.h>
#include "shooter/utils.h"

int randomInt(int min, int max) {
  return min + rand() % (max - min + 1);
}
