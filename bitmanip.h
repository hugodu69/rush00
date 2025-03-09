#ifndef BITMANIP_H
#define BITMANIP_H

#include "utils.h"

// Bit operations on registers
#define SET(register, bit)		(register |= (1 << (bit)))
#define CLEAR(register, bit)	(register &= ~(1 << (bit)))
#define TEST(register, bit)		(register & 1 << bit) // (register & (1 << (bit)))
#define TOGGLE(register, bit)	(register ^= (1 << (bit)))

// Get arguments from tuple-like definitions
#define ARG_1(v1, v2)			v1
#define ARG_2(v1, v2)			v2
#define GET_PORT(args)			ARG_1 args
#define GET_BIT(args)			ARG_2 args
// // version with "LED1 B, D1" without parenthesis
// #define ARG_1(v1, ...)			v1
// #define ARG_2(v1, v2, ...)		v2
// #define GET_PORT(...)			ARG_1(__VA_ARGS__)
// #define GET_BIT(...)			ARG_2(__VA_ARGS__)

// Actions on elements
// #define SET_ELEM(...)			SET(CONCAT(PORT, GET_PORT(__VA_ARGS__)), GET_BIT(__VA_ARGS__)) // version for "LED1 B, D1" without parenthesis
#define SET_ELEM(elem)			SET(CONCAT(PORT, GET_PORT(elem)), GET_BIT(elem))
#define CLEAR_ELEM(elem)		CLEAR(CONCAT(PORT, GET_PORT(elem)), GET_BIT(elem))
#define TEST_ELEM(elem)			TEST(CONCAT(PORT, GET_PORT(elem)), GET_BIT(elem))
#define TOGGLE_ELEM(elem)		TOGGLE(CONCAT(PORT, GET_PORT(elem)), GET_BIT(elem))

#define MODE_OUTPUT(elem)		SET(CONCAT(DDR, GET_PORT(elem)), GET_BIT(elem))
#define MODE_INPUT(elem)		CLEAR(CONCAT(DDR, GET_PORT(elem)), GET_BIT(elem))
#define TOGGLE_PIN(elem)		SET(CONCAT(PIN, GET_PORT(elem)), GET_BIT(elem))
#define TEST_PIN(elem)			(TEST(CONCAT(PIN, GET_PORT(elem)), GET_BIT(elem)))
#define IS_PIN_SET(elem)		(TEST_PIN(elem) == 0)
#define IS_PIN_CLEAR(elem)		(TEST_PIN(elem) == 1)

// Bit definitions
#define D1 0
#define D2 1
#define D3 2
#define D4 4
#define SW1 2
#define SW2 4

// Elements (port, bit)
#define LED1	(B, D1)
#define LED2	(B, D2)
#define LED3	(B, D3)
#define LED4	(B, D4)
#define BUTTON1	(D, SW1)
#define BUTTON2	(D, SW2)

#endif // BITMANIP_H