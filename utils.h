/*
 * linux.h
 *
 *  Created on: 17.09.2014
 *      Author: pascal
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <stdint.h>

#ifndef _WIN64
typedef unsigned int __attribute__((__mode__(__TI__))) uint128_t;

inline uint64_t _umul128(uint64_t a, uint64_t b, uint64_t *c)
{
	uint128_t tmp = a * b;
	uint64_t result = (uint64_t)tmp;
	*c = (uint64_t)(tmp - result);
	return result;
}
#endif


#endif /* UTILS_H_ */
