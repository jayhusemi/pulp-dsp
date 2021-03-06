/* =====================================================================
 * Project:      PULP DSP Library
 * Title:        plp_mult_f32.c
 * Description:  32-bit float vector multiplication glue code
 *
 * $Date:        06. July 2021
 * $Revision:    V0
 *
 * Target Processor: PULP cores
 * ===================================================================== */
/*
 * Copyright (C) 2021 ETH Zurich and University of Bologna. 
 *
 * Author: Aron Szakacs, ETH Zurich
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
 *
 * Notice: project inspired by ARM CMSIS DSP and parts of source code
 * ported and adopted for RISC-V PULP platform from ARM CMSIS DSP
 * released under Copyright (C) 2010-2019 ARM Limited or its affiliates
 * with Apache-2.0.
 */


#include "plp_math.h"

/**
  @ingroup groupMath
 */

/**
  @defgroup BasicMult Vector Multiplication
  This module contains the glue code for Vector Multiplication. The kernel codes (kernels) are in
  the Moducle Vector Multiplication Kernels.

  The Vector Multiplication computes element-by-element multiplication of two vectors.

  <pre>
  pDst[n] = pSrcA[n] * pSrcB[n],   0 <= n < blockSize.
  </pre>

  There are separate functions for floating point, integer, and fixed point 32- 16- 8-bit data
  types. For lower precision integers (16- and 8-bit), functions exploiting SIMD instructions are
  provided.

  The naming scheme of the functions follows the following pattern (for example plp_dot_prod_i32s):
  <pre>
  \<pulp\> _ \<function name\> _ \<data type\> \<precision\> \<method\> _ \<isa extension\>, with

  data type = {f, i, q} respectively for floats, integers, fixed points

  precision = {32, 16, 8} bits

  method = {s, p} respectively meaning single core or parallel multicore implementation.

  isa extension = rv32im, xpulpv2, etc. of which rv32im is the most general one.

  </pre>


 */

/**
  @addtogroup BasicMult
  @{
 */

/**
  @brief Glue code for element-by-element multiplication of 32-bit float vectors.
  @param[in]     pSrcA      points to first input vector
  @param[in]     pSrcB      points to second input vector
  @param[out]    pDst       points to output vector
  @param[in]     blockSize  number of samples in each vector
  @return        none
 */

void plp_mult_f32(const float32_t * pSrcA,
                 const float32_t * pSrcB,
                 float32_t * pDst,
                 uint32_t blockSize) {

    if (rt_cluster_id() == ARCHI_FC_CID) {
      printf("floating point multiplication is not supported on FC.\n");
    } else {
      plp_mult_f32s_xpulpv2(pSrcA, pSrcB, pDst, blockSize);
    }
}

/**
  @} end of BasicMult group
 */
