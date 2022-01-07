#ifndef __PULP_DSP_TEST__DATA_T1_H__
#define __PULP_DSP_TEST__DATA_T1_H__

// include the common header
#include "common.h"

float* t1__pSrc;
RT_L2_DATA uint32_t t1__pSrc__l2__int[24] = {
    0x3ea160b2, 0x3f7d7f69, 0xbea0bb50, 0xbf3def2f, 0xbee8360e, 0x3f5b70d4, 0x3edab859, 0x3d6b48cf,
    0x3e71167b, 0xbf38c24a, 0xbf62d1a0, 0xbe66d031, 0x3e950ddf, 0x3f641951, 0xbefc81c9, 0x3e9233ed,
    0xbf42a72f, 0x3f30ba7c, 0x3eb33221, 0x3f12b60d, 0x3f5b9aef, 0x3e6612c8, 0x3f0b190a, 0xbef9c906
};

float* t1__pSrc__l2 = (float*)((void*)t1__pSrc__l2__int);
                
__u2f t1__scaleFactor = {.u = 0x3d828524U};

float* t1__pDst;
RT_L2_DATA uint32_t t1__pDst__l2__int[24] = {
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0
};

float* t1__pDst__l2 = (float*)((void*)t1__pDst__l2__int);
                
uint32_t t1__blockSize = 24;

RT_L2_DATA uint32_t t1__pDst__reference__int[24] = {
    0x3ca48e11, 0x3d813e8a, 0xbca3e56d, 0xbd41ac7c, 0xbcecc870, 0x3d5fc2d8, 0x3cdf06bb, 0x3b6feaae,
    0x3c75d59a, 0xbd3c6581, 0xbd6748d4, 0xbc6b5b87, 0x3c97fd21, 0x3d6896f9, 0xbd00bd3b, 0x3c9514d0,
    0xbd467c44, 0x3d343539, 0x3cb6b94e, 0x3d159980, 0x3d5fedc7, 0x3c6a9a63, 0x3d0dd61d, 0xbcfeb3fb
};

float* t1__pDst__reference = (float*)((void*)t1__pDst__reference__int);


#endif//__PULP_DSP_TEST__DATA_T1_H__