#ifndef _PIECES_OF_CODE_H
#define _PIECES_OF_CODE_H
/*
 * File: pieces.h
 * Decription: This file contains a collection of awsome and useful code pieces.
 * Author: Charles.
 * Mail: charles.cn.bj@gmail.com
 */

/* 
 * De Morgan's Laws 
 * ~(x & y) = ~x | ~y
 * ~(x | y) = ~x & ~y
 * ~(x + 1) = ~x - 1
 * ~(x - 1) = ~x + 1
 * ~(-x) = x - 1
 * ~(x ^ y) = ~x ^ y = x ≡ y
 * ~(x ≡ y) = ~x ≡ y = x ^ y
 * ~(x + y) = ~x - y
 * ~(x - y) = ~x + y
 */
/*
 * Eg. ~(x | -(x + 1)) = ~x & ~(-(x + 1)) = ~x & (x + 1 - 1) = ~x & x = 0
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

/*
 * Use the following formula to create a word with a single 1-bit at the 
 * position of the rightmost 0-bit in x, producing 0 if none 
 * (e.g., 10100111 ⇒ 00001000)
 */
#define MAKE_WORD_LAST_0TO1(x) (~(x) & ((x) + 1))

/*
 * Use the following formula to create a word with a single 0-bit at the position
 * of the rightmost 1-bit in x, producing all 1’s if none
 * (e.g., 10101000 ⇒ 11110111)
 */
#define MAKE_WORD_LAST_1TO0(x) (~(x) | ((x) - 1))

/*
 * Use one of the following formulas to create a word with 1’s at the positions
 * of the trailing 0’s in x, and 0’s elsewhere, producing 0 if none
 * (e.g., 01011000 ⇒ 00000111)
 * or <~(x | -x)> or <(x & -x) - 1>
 */
#define MAKE_WORD_TRAILING_0TO1(x) (~(x) & ((x) - 1))

/*
 * Use either of the following formulas to turn off the rightmost contiguous string
 * of 1’s 
 * (e.g. 01011100 ==> 01000000) [Wood]
 * or ((x & −x) + x)&x
 * this can be used to judge whether an unsigned integer is 2^j - 2^k j >= k >= 0
 */

#define OFF_RIGHTMOST_CONT_1BITS(x) ((((x) | ((x) - 1)) + 1) & x)


#endif
