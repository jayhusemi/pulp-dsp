#ifndef __PULP_DSP_TEST__DATA_T2_H__
#define __PULP_DSP_TEST__DATA_T2_H__

// include the common header
#include "common.h"

RT_L2_DATA int32_t t2__pSrc[25] = {
    -2247, -31108, -5689, 7488, 15089, 9431, -1464, -25220, -15350, -29410, 21183, 28432, -17573,
    -27168, 28136, -11201, -18442, 6188, 6718, 15661, -30169, -5689, -4258, 8467, 15490
};

uint32_t t2__blockSize = 25;

int32_t t2__scaleFactor = 93;

int32_t t2__shift = 7;

RT_L2_DATA int32_t t2__pDst[25] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

RT_L2_DATA int32_t t2__pDst__reference[25] = {
    -1633, -22602, -4134, 5440, 10963, 6852, -1064, -18324, -11153, -21369, 15390, 20657, -12768,
    -19740, 20442, -8139, -13400, 4495, 4881, 11378, -21920, -4134, -3094, 6151, 11254
};


#endif//__PULP_DSP_TEST__DATA_T2_H__
