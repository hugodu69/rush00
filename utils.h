#ifndef UTILS_H
#define UTILS_H

// stringify
#define STRINGIFY_HELPER(x)			#x
#define STRINGIFY(x)				STRINGIFY_HELPER(x)

// concatenate
#define CONCAT_HELPER(x, y)			x ## y
#define CONCAT(x, y)				CONCAT_HELPER(x, y)

// mathematics
#define PERCENT(percent, total)		(((float)percent / 100) * total)
#define DIV_ROUND_CLOSEST(n, d)		((((n) < 0) == ((d) < 0)) ? (((n) + (d)/2)/(d)) : (((n) - (d)/2)/(d)))

// text
#define SWITCH_CASE(ch) (((ch) >= 'A' && (ch) <= 'Z') || ((ch) >= 'a' && (ch) <= 'z') ? ((ch) ^ (1 << 5)) : (ch))

// boolean
#define TRUE						1
#define FALSE						0

#endif // UTILS_H