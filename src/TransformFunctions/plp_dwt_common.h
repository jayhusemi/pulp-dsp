/* ----------------------------------------------------------------------
 * Project:      PULP DSP Library
 * Title:        plp_dwt_common.h
 * Description:  Signal Edge extension algorithms
 *
 * $Date:        10. Juli 2021
 * $Revision:    V1
 *
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

#ifndef __PLP_DWT_COMMON_H
#define __PLP_DWT_COMMON_H

static inline void copy_coefs_q32(int32_t *dec_hi_l1, int32_t *dec_lo_l1, plp_dwt_wavelet_q32 wavelet){

   hal_cl_dma_cmd_t copy;

   int32_t merge = 0;

   hal_cl_dma_cmd((uint32_t)wavelet.dec_hi, (uint32_t)dec_hi_l1, sizeof(int32_t) * wavelet.length, HAL_CL_DMA_DIR_EXT2LOC, merge, &copy);
   merge = 1;
   hal_cl_dma_cmd((uint32_t)wavelet.dec_lo, (uint32_t)dec_lo_l1, sizeof(int32_t) * wavelet.length, HAL_CL_DMA_DIR_EXT2LOC, merge, &copy);


   hal_cl_dma_cmd_wait(&copy);

}

static inline void copy_coefs_f32(float32_t *dec_hi_l1, float32_t *dec_lo_l1, plp_dwt_wavelet_f32 wavelet){
    hal_cl_dma_cmd_t copy;

   int32_t merge = 0;

   hal_cl_dma_cmd((uint32_t)wavelet.dec_hi, (uint32_t)dec_hi_l1, sizeof(float32_t) * wavelet.length, HAL_CL_DMA_DIR_EXT2LOC, merge, &copy);
   merge = 1;
   hal_cl_dma_cmd((uint32_t)wavelet.dec_lo, (uint32_t)dec_lo_l1, sizeof(float32_t) * wavelet.length, HAL_CL_DMA_DIR_EXT2LOC, merge, &copy);


   hal_cl_dma_cmd_wait(&copy);
}


static inline void copy_coefs_q16(int16_t *dec_hi_l1, int16_t *dec_lo_l1, plp_dwt_wavelet_q16 wavelet){
   hal_cl_dma_cmd_t copy;

   int32_t merge = 0;

   hal_cl_dma_cmd((uint32_t)wavelet.dec_hi, (uint32_t)dec_hi_l1, sizeof(int16_t) * wavelet.length, HAL_CL_DMA_DIR_EXT2LOC, merge, &copy);
   merge = 1;
   hal_cl_dma_cmd((uint32_t)wavelet.dec_lo, (uint32_t)dec_lo_l1, sizeof(int16_t) * wavelet.length, HAL_CL_DMA_DIR_EXT2LOC, merge, &copy);


   hal_cl_dma_cmd_wait(&copy);
}


static inline void copy_coefs_q8(int8_t *dec_hi_l1, int8_t *dec_lo_l1, plp_dwt_wavelet_q8 wavelet){
   hal_cl_dma_cmd_t copy;

   int32_t merge = 0;

   hal_cl_dma_cmd((uint32_t)wavelet.dec_hi, (uint32_t)dec_hi_l1, sizeof(int8_t) * wavelet.length, HAL_CL_DMA_DIR_EXT2LOC, merge, &copy);
   merge = 1;
   hal_cl_dma_cmd((uint32_t)wavelet.dec_lo, (uint32_t)dec_lo_l1, sizeof(int8_t) * wavelet.length, HAL_CL_DMA_DIR_EXT2LOC, merge, &copy);


   hal_cl_dma_cmd_wait(&copy);
}

#endif