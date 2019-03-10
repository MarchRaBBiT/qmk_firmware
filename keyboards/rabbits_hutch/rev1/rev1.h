#ifndef RABBITS_HUTCH_REV1_H
#define RABBITS_HUTCH_REV1_H
#include "rabbits_hutch.h"

#define LAYOUT(k00, k01, k02, k03, k04, \
               k10, k11, k12, k13, k14, \
               k20, k21, k22, k23, k24 ) \
              { { k24, k23, k22, k21, k20, \
                  k14, k13, k12, k11, k10, \
                  k04, k03, k02, k01, k00 }, }

#endif
