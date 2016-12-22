#ifndef _LZ4_COMPRESSION_H
#define _LZ4_COMPRESSION_H
/*
 * File: lz4_compression.h
 * Description: compression function of lz4
 * Authro: Charles. 2016.12.22
 * Mailto: charlesliu.cn.bj@gmail.com
 */
#include "lz4.h"

#define COMPRESSION_BLOCK 8192 /* to gain best performance */

int lz4_compress(const char *input, int input_len, char *output, int &output_len);
int lz4_decompress(const char *input, int input_len, char *output, int &output_len);

#endif
