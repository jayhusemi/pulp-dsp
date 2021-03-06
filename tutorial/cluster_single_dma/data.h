#include "rtos_hal.h"
#include "defines.h"

#define VLEN 80

int32_t a[VLEN] = {9,   8,   9,  -5,   6,  -8,  -1,  -8,   8,  -3,   2,   5,  -2,
             3,  -7,   2, -10, -10,   1,   7,   1, -10,  -1,   8,  -7,  -4,
             4,  -8,   2,  -9,  -9,   9,  -3,   6,  -6,   6,   4,   8,  -9,
             -9,  -9,   4,  -4,   3,  -9,   1,  -1,  -1,   2,   7,   9,   4,
             -9,   2,  -7,   3,   9,  -7,   1,   5,  -8,   2,  -6,  -3,  -8,
             -2,  -1,  -4,   8,  -8,  -4,   3,   3, -10,   9,   1,  -3,   6,
             -2,  -9};
int32_t b[VLEN] = {-5,  -3,   6,   8,  -9,  -6,   9,  -8,  -2,   0,  -9,   5,   1,
             1,   3,   7,   9,  -6,  -2,   1,  -7,   0,   1,  -3,  -6,   2,
             -10,   0,  -7,  -8,  -8,  -4,  -6,  -6,  -7,   4, -10,   2,  -5,
             -1,  -9,   6,   5,   5,   3,   2,  -2,  -7,   5,  -5,   0,   4,
             -4,   0,   6,  -6,   1,  -9,  -2,   2,   7,   7,  -9,   9, -10,
             9,   0,  -2,   1,   6,  -5,  -3,   2,   3,  -3,   0,  -3,  -5,
             9,  -7};

int32_t res = 218;


#ifndef USEDMA

RT_CL_DATA int32_t a_cl[VLEN] = {9,   8,   9,  -5,   6,  -8,  -1,  -8,   8,  -3,   2,   5,  -2,
             3,  -7,   2, -10, -10,   1,   7,   1, -10,  -1,   8,  -7,  -4,
             4,  -8,   2,  -9,  -9,   9,  -3,   6,  -6,   6,   4,   8,  -9,
             -9,  -9,   4,  -4,   3,  -9,   1,  -1,  -1,   2,   7,   9,   4,
             -9,   2,  -7,   3,   9,  -7,   1,   5,  -8,   2,  -6,  -3,  -8,
             -2,  -1,  -4,   8,  -8,  -4,   3,   3, -10,   9,   1,  -3,   6,
                                 -2,  -9}; // you can also use RT_L1_DATA
RT_CL_DATA int32_t b_cl[VLEN] = {-5,  -3,   6,   8,  -9,  -6,   9,  -8,  -2,   0,  -9,   5,   1,
             1,   3,   7,   9,  -6,  -2,   1,  -7,   0,   1,  -3,  -6,   2,
             -10,   0,  -7,  -8,  -8,  -4,  -6,  -6,  -7,   4, -10,   2,  -5,
             -1,  -9,   6,   5,   5,   3,   2,  -2,  -7,   5,  -5,   0,   4,
             -4,   0,   6,  -6,   1,  -9,  -2,   2,   7,   7,  -9,   9, -10,
             9,   0,  -2,   1,   6,  -5,  -3,   2,   3,  -3,   0,  -3,  -5,
             9,  -7};

RT_CL_DATA int32_t res_cl = 218;

#endif //USEDMA
