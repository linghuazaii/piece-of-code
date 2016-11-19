#ifndef _PIECES_OF_CODE_H
#define _PIECES_OF_CODE_H
/*
 * File: pieces.h
 * Decription: This file contains a collection of awsome and useful code pieces.
 * Author: Charles.
 * Mail: charles.cn.bj@gmail.com
 */

/* 
 * This macro can be used to test if x = 2^n, if x = 2^n, then the result of
 * this macro is 0. And also, this macro can be used to ignore the rightest
 * 0 bit. Eg. 01011000 => 01010000
 */
#define OFF_RIGHTEST_1BIT(x) ((x) & ((x) - 1))

/*
 * This macro can be used to test if x = 2^n - 1, if x = 2^n - 1, then the result of 
 * this macro is 0. And also, this macro can be used to ignore the trialing 1's. 
 * Eg. 010100111 => 010100000
 */
#define OFF_TRIAL_1BITS(x) ((x) & ((x) + 1))

/*
 * This macro can be used to turn on the rightest 0 bits, if none return x.
 * Eg. 10100111 => 10101111
 */
#define ON_RIGHTEST_0BIT(x) ((x) | ((x) + 1))

/*
 * This macro can be used to turn on the trailing 0 bits, if none return x.
 * Eg. 10101000 => 10101111
 */
#define ON_TRIAL_0BITS(x) ((x) | ((x) - 1))

#endif
