/* ----------------------------------------------------------------------
 * Project:      PULP DSP Library
 * Title:        plp_dwt_q32s_xpulpv2.c
 * Description:  32bit Fixed-point Discret Wavelet Transform on real input data for XPULPV2
 *
 * $Date:        10. Juli 2021
 * $Revision:    V1
 *
 * Target Processor: PULP cores
 * -------------------------------------------------------------------- */
/*
 * Copyright (C) 2021 ETH Zurich and University of Bologna. All rights reserved.
 *
 * Author: Jakub Mandula, ETH Zurich
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "plp_math.h"
#include "plp_const_structs.h"
/* HELPER FUNCTIONS */

#define HAAR_COEF ((int64_t) 0x5a82799a)


#define MAC_SHIFT 31U
#define MAC(Acc, A, B) Acc += (int64_t)((int64_t) A * (int64_t) B);
#define MSU(Acc, A, B) Acc -= (int64_t)((int64_t) A * (int64_t) B);

#include "plp_dwt_signal_ext.h"

/**
  @ingroup dwt
 */

/**
  @defgroup q32DWTKernels DWT kernels on Q31 input values
  These kernels calculate the DWT transform on Q31 fixed point data.
*/

/**
  @addtogroup q32DWTKernels
  @{
 */

/**
   @brief  Q31 fixed-point DWT on real input data for XPULPV2 extension.
   @param[in]   pSrc     points to the input buffer (real data)
   @param[in]   length   length of input buffer
   @param[in]   wavelet  wavelet structure for calculating DWT
   @param[in]   mode     boundary extension mode

   @param[out]  pDstA    points to ouput buffer with Approximate coefficients
   @param[out]  pDstD    points to ouput buffer with Detailed coefficients
   @return      none
*/
void plp_dwt_q32s_xpulpv2(const int32_t *__restrict__ pSrc,
                         uint32_t length,
                         const plp_dwt_wavelet_q32 wavelet,
                         plp_dwt_extension_mode mode,
                         int32_t *__restrict__ pDstA,
                         int32_t *__restrict__ pDstD) {
    int32_t *pCurrentA = pDstA;
    int32_t *pCurrentD = pDstD;

    static uint32_t step = 2;

    int32_t offset;
        
    /***
     * The filter convolution is done in 4 steps handling cases where
     *  1. Filter is hanging over the left side of the signal
     *  2. Filter is same size, or totally enclosed in signal
     *  3. Filter is larger than the enclosed signal and hangs over both edges
     *  4. Filter hangs over the right side of the signal
     * 
     *  Each of the cases, where signal hangs over the boundary of the signal, values are computed 
     *  on demand based on the edge extension mode.
     */

    
    /*  Step 1.
     *  Handle Left overhanging
     *
     * X() =  x x[A B C D E F]
     * H() = [d c b a]
     *          ^   ^
     *          |   First compute the filter part overlapping with the signal
     *          Then extend the signal (x x) by computing the values based on the extension mode
     */
    for(offset = step-1; offset < wavelet.length - 1 && offset < length; offset += step){
        int64_t sum_lo = 0;
        int64_t sum_hi = 0;

        uint32_t filt_j = 0;

        // Compute Filter overlapping with signal
        for(; filt_j <= offset; filt_j++){
            MAC(sum_lo, wavelet.dec_lo[filt_j], pSrc[offset - filt_j]);
            MAC(sum_hi, wavelet.dec_hi[filt_j], pSrc[offset - filt_j]);
        }

        // Compute Left edge extension
        switch(mode){
            case PLP_DWT_MODE_CONSTANT:
                CONSTANT_EDGE_LEFT(sum_lo, sum_hi, pSrc, length, wavelet, filt_j, offset);
                break;
            case PLP_DWT_MODE_SYMMETRIC:
                SYMMETRIC_EDGE_LEFT(sum_lo, sum_hi, pSrc, length, wavelet, filt_j, offset);
                break;
            case PLP_DWT_MODE_REFLECT:
                REFLECT_EDGE_LEFT(sum_lo, sum_hi, pSrc, length, wavelet, filt_j, offset);
                break;
            case PLP_DWT_MODE_ANTISYMMETRIC:
                ANTISYMMETRIC_EDGE_LEFT(sum_lo, sum_hi, pSrc, length, wavelet, filt_j, offset);
                break;
            case PLP_DWT_MODE_ANTIREFLECT:
                ANTIREFLECT_EDGE_LEFT(sum_lo, sum_hi, pSrc, length, wavelet, filt_j, offset, int64_t);
                break;
            case PLP_DWT_MODE_PERIODIC:
            case PLP_DWT_MODE_ZERO:
            default:
                break;
        }
    
    
        *pCurrentA++ = sum_lo >> MAC_SHIFT;
        *pCurrentD++ = sum_hi >> MAC_SHIFT;
    }

    /*  Step 2.
     *  Compute center (length >= wavelet.length)
     *
     *  X() = [A B C D E F]
     *  h() =   [d c b a]
     *                 ^
     *                 Compute a full convolution of the filter with the signal
     */    
    for(;offset < length; offset += step){

        int64_t sum_lo = 0;
        int64_t sum_hi = 0;
        

        const int32_t *pS = pSrc + offset;
        const int32_t *dec_lo = wavelet.dec_lo;
        const int32_t *dec_hi = wavelet.dec_hi;

        uint32_t blkCnt = wavelet.length >> 1;

        do{
            int32_t S1 = *pS--;
            int32_t S2 = *pS--;
            MAC(sum_lo, *dec_lo++, S1);
            MAC(sum_hi, *dec_hi++, S1);
            MAC(sum_lo, *dec_lo++, S2);
            MAC(sum_hi, *dec_hi++, S2);
        }while(--blkCnt);


        *pCurrentA++ = sum_lo >> MAC_SHIFT;
        *pCurrentD++ = sum_hi >> MAC_SHIFT;
    }

    /*  Step 3.
     *  Compute center (length < wavelet.length)
     *
     *  X() =   y y[A B C]x x x
     *  h() =  [h g f e d c b a]
     *            ^     ^     ^
     *            |     |     Compute Right extension (x x x) based on extension mode
     *            |     Compute a full convolution of the filter overlapping with the signal
     *            Compute Left extension (y y) based on extension mode
     */      

    for(;offset < wavelet.length - 1; offset += step){
        int64_t sum_lo = 0;
        int64_t sum_hi = 0;

        uint32_t filt_j = 0;

        // Filter Right extension
        switch(mode){
            case PLP_DWT_MODE_CONSTANT:
                CONSTANT_EDGE_RIGHT(sum_lo, sum_hi, pSrc, length, wavelet, filt_j, offset);
                break;
            case PLP_DWT_MODE_SYMMETRIC:
                SYMMETRIC_EDGE_RIGHT(sum_lo, sum_hi, pSrc, length, wavelet, filt_j, offset);
                break;
            case PLP_DWT_MODE_REFLECT:
                REFLECT_EDGE_RIGHT(sum_lo, sum_hi, pSrc, length, wavelet, filt_j, offset);
                break;
            case PLP_DWT_MODE_ANTISYMMETRIC:
                ANTISYMMETRIC_EDGE_RIGHT(sum_lo, sum_hi, pSrc, length, wavelet, filt_j, offset);
                break;
            case PLP_DWT_MODE_ANTIREFLECT:
                ANTIREFLECT_EDGE_RIGHT(sum_lo, sum_hi, pSrc, length, wavelet, filt_j, offset, int64_t);
                break;
            case PLP_DWT_MODE_PERIODIC:
            case PLP_DWT_MODE_ZERO:
            default:
                filt_j = offset - length + 1;
                break;
        }

        // Filter Center overlapp
        for(; filt_j <= offset; filt_j++){
            MAC(sum_lo, wavelet.dec_lo[filt_j], pSrc[offset - filt_j]);
            MAC(sum_hi, wavelet.dec_hi[filt_j], pSrc[offset - filt_j]);
        }   

        // Filter Left extension
        switch(mode){
            case PLP_DWT_MODE_CONSTANT:
                CONSTANT_EDGE_LEFT(sum_lo, sum_hi, pSrc, length, wavelet, filt_j, offset);
                break;
            case PLP_DWT_MODE_SYMMETRIC:
                SYMMETRIC_EDGE_LEFT(sum_lo, sum_hi, pSrc, length, wavelet, filt_j, offset);
                break;
            case PLP_DWT_MODE_REFLECT:
                REFLECT_EDGE_LEFT(sum_lo, sum_hi, pSrc, length, wavelet, filt_j, offset);
                break;
            case PLP_DWT_MODE_ANTISYMMETRIC:
                ANTISYMMETRIC_EDGE_LEFT(sum_lo, sum_hi, pSrc, length, wavelet, filt_j, offset);
                break;
            case PLP_DWT_MODE_ANTIREFLECT:
                ANTIREFLECT_EDGE_LEFT(sum_lo, sum_hi, pSrc, length, wavelet, filt_j, offset, int64_t);
                break;
            case PLP_DWT_MODE_PERIODIC:
            case PLP_DWT_MODE_ZERO:
            default:
                break;
        }

        *pCurrentA++ = sum_lo >> MAC_SHIFT;
        *pCurrentD++ = sum_hi >> MAC_SHIFT;
    }


    /*  Step 4.
     *  Handle Right overhanging
     *
     * X() = [A B C D E F]x x
     * H() =         [d c b a]
     *                  ^   ^
     *                  |   First extend the signal (x x) by computing the values based on the extension mode
     *                  Then compute the filter part overlapping with the signal
     */
    for(; offset < length + wavelet.length - 1; offset += step){
        int64_t sum_lo = 0;
        int64_t sum_hi = 0;

        uint32_t filt_j = 0;

        // Compute Left edge extension
        switch(mode){
            case PLP_DWT_MODE_CONSTANT:
                CONSTANT_EDGE_RIGHT(sum_lo, sum_hi, pSrc, length, wavelet, filt_j, offset);
                break;
            case PLP_DWT_MODE_SYMMETRIC:
                SYMMETRIC_EDGE_RIGHT(sum_lo, sum_hi, pSrc, length, wavelet, filt_j, offset);
                break;
            case PLP_DWT_MODE_REFLECT:
                REFLECT_EDGE_RIGHT(sum_lo, sum_hi, pSrc, length, wavelet, filt_j, offset);
                break;
            case PLP_DWT_MODE_ANTISYMMETRIC:
                ANTISYMMETRIC_EDGE_RIGHT(sum_lo, sum_hi, pSrc, length, wavelet, filt_j, offset);
                break;
            case PLP_DWT_MODE_ANTIREFLECT:
                ANTIREFLECT_EDGE_RIGHT(sum_lo, sum_hi, pSrc, length, wavelet, filt_j, offset, int64_t);
                break;
            case PLP_DWT_MODE_PERIODIC:
            case PLP_DWT_MODE_ZERO:
            default:
                filt_j = offset - length + 1;
                break;
        }
    
        // Filter overlapping with signal
        for(; filt_j < wavelet.length; filt_j++){
            MAC(sum_lo, wavelet.dec_lo[filt_j], pSrc[offset - filt_j]);
            MAC(sum_hi, wavelet.dec_hi[filt_j], pSrc[offset - filt_j]);
        }

        *pCurrentA++ = sum_lo >> MAC_SHIFT;
        *pCurrentD++ = sum_hi >> MAC_SHIFT;
    }
}






#define MAKE_HAAR(NAME, COEF, SHIFT)                                                                           \
void NAME(const int32_t *__restrict__ pSrc,                                                                    \
                         uint32_t length,                                                                      \
                         plp_dwt_extension_mode mode,                                                          \
                         int32_t *__restrict__ pDstA,                                                          \
                         int32_t *__restrict__ pDstD) {                                                        \
    int32_t *pCurrentA = pDstA;                                                                                \
    int32_t *pCurrentD = pDstD;                                                                                \
                                                                                                               \
                                                                                                               \
    /***                                                                                                       \
     * The filter convolution is done in 2 steps handling cases where                                          \
     *  1. Filter is same size, or totally enclosed in signal center                                           \
     *  2. Filter hangs over the right side of the signal                                                      \
     *                                                                                                         \
     *  In of the cases, where signal hangs over the boundary of the signal, values are computed               \
     *  on demand based on the edge extension mode.                                                            \
     */                                                                                                        \
                                                                                                               \
                                                                                                               \
    /*  Step 1.                                                                                                \
     *  Compute center (length >= wavelet.length)                                                              \
     *                                                                                                         \
     *  X() = [A B C D E F]                                                                                    \
     *  h() =       [b a]                                                                                      \
     *                 ^                                                                                       \
     *                 Compute a full convolution of the filter with the signal                                \
     */                                                                                                        \
    uint32_t blkCnt = length >> 2;                                                                             \
                                                                                                               \
    const int32_t *pS = pSrc;                                                                                  \
    while(blkCnt--){                                                                                           \
        int32_t s0 = *pS++;                                                                                    \
        int32_t s1 = *pS++;                                                                                    \
        int32_t s2 = *pS++;                                                                                    \
        int32_t s3 = *pS++;                                                                                    \
                                                                                                               \
        *pCurrentA++ = (COEF * (s0 + s1)) >> SHIFT;                                                            \
        *pCurrentD++ = (COEF * (s0 - s1)) >> SHIFT;                                                            \
        *pCurrentA++ = (COEF * (s2 + s3)) >> SHIFT;                                                            \
        *pCurrentD++ = (COEF * (s2 - s3)) >> SHIFT;                                                            \
                                                                                                               \
    }                                                                                                          \
                                                                                                               \
    if(length % 4 > 1){                                                                                        \
        int32_t s0 = *pS++;                                                                                    \
        int32_t s1 = *pS++;                                                                                    \
                                                                                                               \
        *pCurrentA++ = (COEF * (s0 + s1)) >> SHIFT;                                                            \
        *pCurrentD++ = (COEF * (s0 - s1)) >> SHIFT;                                                            \
    }                                                                                                          \
                                                                                                               \
                                                                                                               \
                                                                                                               \
                                                                                                               \
                                                                                                               \
    /*  Step 2.                                                                                                \
     *  Handle Right overhanging (only for odd signal lengths)                                                 \
     *                                                                                                         \
     * X() = [A B C D E F]x                                                                                    \
     * H() =           [b a]                                                                                   \
     *                  ^ ^                                                                                    \
     *                  | Extend the signal (x) by computing the values based on the extension mode            \
     *                  Then compute the filter part overlapping with the signal                               \
     */                                                                                                        \
    if(length % 2U){                                                                                           \
        int64_t sum_lo = 0;                                                                                    \
        int64_t sum_hi = 0;                                                                                    \
                                                                                                               \
        uint32_t filt_j = 0;                                                                                   \
                                                                                                               \
        /* Compute Left edge extension  */                                                                     \
        switch(mode){                                                                                          \
            case PLP_DWT_MODE_CONSTANT:                                                                        \
            case PLP_DWT_MODE_SYMMETRIC:                                                                       \
                /* dec_lo[0] * src[N-1] + dec_lo[1] * src[N-1] */                                              \
                sum_lo = 2 * COEF * pSrc[length - 1];                                                          \
                /* dec_hi[0] * src[N-1] + dec_hi[1] * src[N-1] == -dec_hi[1] * src[N-1] + dec_hi[1] * src[N-1]*/\
                sum_hi = 0;                                                                                    \
                break;                                                                                         \
            case PLP_DWT_MODE_REFLECT:                                                                         \
                sum_lo = COEF * (pSrc[length - 1] + pSrc[length - 2]);                                         \
                sum_hi = COEF * (pSrc[length - 1] - pSrc[length - 2]);                                         \
                break;                                                                                         \
            case PLP_DWT_MODE_ANTISYMMETRIC:                                                                   \
                sum_lo = COEF * (pSrc[length - 1] - pSrc[length - 1]);                                         \
                sum_hi = COEF * (pSrc[length - 1] + pSrc[length - 1]);                                         \
                break;                                                                                         \
            case PLP_DWT_MODE_ANTIREFLECT:                                                                     \
                sum_lo = COEF * (3*pSrc[length - 1] - pSrc[length - 2]);                                       \
                sum_hi = COEF * ( -pSrc[length - 1] + pSrc[length - 2]);                                       \
                break;                                                                                         \
            case PLP_DWT_MODE_PERIODIC:                                                                        \
            case PLP_DWT_MODE_ZERO:                                                                            \
            default:                                                                                           \
                sum_lo = COEF * pSrc[length - 1];                                                              \
                sum_hi = COEF * pSrc[length - 1];                                                              \
                break;                                                                                         \
        }                                                                                                      \
                                                                                                               \
        *pCurrentA = sum_lo >> SHIFT;                                                                          \
        *pCurrentD = sum_hi >> SHIFT;                                                                          \
    }                                                                                                          \
}                                                                                                              \






/**
   @brief  Q31 Fixed-point DWT kernel optimized for Haar Wavelet for XPULPV2 extension.
   @param[in]   pSrc     points to the input buffer (q31)
   @param[in]   length   length of input buffer
   @param[in]   mode     boundary extension mode

   @param[out]  pDstA    points to ouput buffer with Approximate coefficients
   @param[out]  pDstD    points to ouput buffer with Detailed coefficients
   @return      none
*/
MAKE_HAAR(plp_dwt_haar_q32s_xpulpv2, HAAR_COEF, MAC_SHIFT)


/**
   @brief  Q31 Fixed-point DWT kernel optimized for un-normalized Haar Wavelet for XPULPV2 extension.
   @param[in]   pSrc     points to the input buffer (q31)
   @param[in]   length   length of input buffer
   @param[in]   mode     boundary extension mode

   @param[out]  pDstA    points to ouput buffer with Approximate coefficients
   @param[out]  pDstD    points to ouput buffer with Detailed coefficients
   @return      none
*/
MAKE_HAAR(plp_dwt_haar_u_q32s_xpulpv2, 1U, 0U)

