// Copyright(C) 2013-2014 Square, Inc. All Rights Reserved.

#pragma once

#include <stdint.h>
#include <math.h>

float cr_math_util_variance(float * src, int length);
float cr_math_util_mean(float * src, int length);
void cr_math_util_remove_mean(float * src, int length);
float cr_math_util_find_max(float * src, int length, int start, int stop, unsigned long * index);
float cr_math_util_average_vector_distance(float * src1, float * src2, float * temp_buffer, int length);
void cr_math_util_int16_array_to_scaled_float_array(float * output, int16_t const * input, float divide_by, int length);
int cr_math_util_count_bit_differences_for_bytes(uint8_t a, uint8_t b);
void cr_math_util_vector_set_root_sum_of_squares(float * src1, float * src2, float * dest, int length);
void cr_math_util_vector_add_root_sum_of_squares(float * src1, float * src2, float * dest, int length);
void cr_math_util_sort(float * data, int length);
