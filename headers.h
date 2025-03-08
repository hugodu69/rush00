#ifndef HEADERS_H
#define HEADERS_H

// mathematics 
#define DIV_ROUND_CLOSEST(n, d)	((((n) < 0) == ((d) < 0)) ? (((n) + (d)/2)/(d)) : (((n) - (d)/2)/(d))) // https://stackoverflow.com/a/18067292

// text
#define SWITCH_CASE(ch)			(((ch) >= 'A' && (ch) <= 'Z') || ((ch) >= 'a' && (ch) <= 'z') ? ((ch) ^ (1 << 5)) : (ch))

// stringify
#define STRINGIFY_HELPER(x)		#x
#define STRINGIFY(x)			STRINGIFY_HELPER(x)

// concatenate
#define CONCAT_HELPER(x, y)		x ## y
#define CONCAT(x, y)			CONCAT_HELPER(x, y)

// get argument
#define ARG_1(v1, v2)			v1
#define ARG_2(v1, v2)			v2
#define GET_PORT(args)			ARG_1 args
#define GET_BIT(args)			ARG_2 args
// // version with "LED1 B, D1" without parenthesis
// #define ARG_1(v1, ...)			v1
// #define ARG_2(v1, v2, ...)		v2
// #define GET_PORT(...)			ARG_1(__VA_ARGS__)
// #define GET_BIT(...)			ARG_2(__VA_ARGS__)

// actions on registers
#define SET(register, bit)		register |= 1 << bit
#define CLEAR(register, bit)	register &= ~(1 << bit)
#define TEST(register, bit)		(register & 1 << bit)
#define TOGGLE(register, bit)	register ^= 1 << bit

// actions on elements
#define SET_ELEM(elem)			SET(CONCAT(PORT, GET_PORT(elem)), GET_BIT(elem))
// #define SET_ELEM(...)			SET(CONCAT(PORT, GET_PORT(__VA_ARGS__)), GET_BIT(__VA_ARGS__)) // version for "LED1 B, D1" without parenthesis
#define CLEAR_ELEM(elem)		CLEAR(CONCAT(PORT, GET_PORT(elem)), GET_BIT(elem))
#define TEST_ELEM(elem)			TEST(CONCAT(PORT, GET_PORT(elem)), GET_BIT(elem))
#define TOGGLE_ELEM(elem)		TOGGLE(CONCAT(PORT, GET_PORT(elem)), GET_BIT(elem))
#define MODE_OUTPUT(elem)		SET(CONCAT(DDR, GET_PORT(elem)), GET_BIT(elem))
#define MODE_INPUT(elem)		CLEAR(CONCAT(DDR, GET_PORT(elem)), GET_BIT(elem))
#define TOGGLE_PIN(elem)		SET(CONCAT(PIN, GET_PORT(elem)), GET_BIT(elem))
#define TEST_PIN(elem)			(TEST(CONCAT(PIN, GET_PORT(elem)), GET_BIT(elem)))
#define IS_PIN_SET(elem)		(TEST_PIN(elem) == 0)
#define IS_PIN_CLEAR(elem)		(TEST_PIN(elem) == 1)

// bits
#define D1	0
#define D2	1
#define D3	2
#define D4	4
#define SW1	2
#define SW2	4

// elements (port, bit)
// #define LED1	B, D1
#define LED1	(B, D1)
#define LED2	(B, D2)
#define LED3	(B, D3)
#define LED4	(B, D4)
#define BUTTON1	(D, SW1)
#define BUTTON2	(D, SW2)

// USART
// Table 20-1 : Baud Rate Calculation
#define USART_BAUDRATE					115200
#define BAUD_PRESCALER					(DIV_ROUND_CLOSEST(F_CPU, (16 * USART_BAUDRATE)) - 1)
// Table 20-8 : Mode Selection (USART Mode SELect)
#define ASYNCHRONOUS					(0<<UMSEL01 | 0<<UMSEL00)
#define SYNCHRONOUS						(0<<UMSEL01 | 1<<UMSEL00)
// Table 20-9 : Parity Bit Selection (USART Parity Mode)
#define PARITY_DISABLED					(0<<UPM01 | 0<<UPM00)
#define PARITY_EVEN						(1<<UPM01 | 0<<UPM00)
#define PARITY_ODD						(1<<UPM01 | 1<<UPM00)
// Table 20-10 : Stop Bit Selection (USART Stop Bit Select)
#define STOP_ONE_BIT					(0<<USBS0)
#define STOP_TWO_BIT					(1<<USBS0)
// Table 20-11 : Data Bit Selection (USART Character SiZe)
#define DATA_FIVE_BIT					(0<<UCSZ02 | 0<<UCSZ01 | 0<<UCSZ00)
#define DATA_SIX_BIT					(0<<UCSZ02 | 0<<UCSZ01 | 1<<UCSZ00)
#define DATA_SEVEN_BIT					(0<<UCSZ02 | 1<<UCSZ01 | 0<<UCSZ00)
#define DATA_EIGHT_BIT					(0<<UCSZ02 | 1<<UCSZ01 | 1<<UCSZ00)
#define DATA_NINE_BIT					(1<<UCSZ02 | 1<<UCSZ01 | 1<<UCSZ00)
// 20.11.3 : USART Control and Status Register B (UCSRnB)
#define RECEIVER_DISABLED				(0<<RXEN0)
#define RECEIVER_ENABLED				(1<<RXEN0)
#define TRANSMITTER_DISABLED			(0<<TXEN0)
#define TRANSMITTER_ENABLED				(1<<TXEN0)
#define INTERRUPT_RECEIVER_DISABLED		(0<<RXCIE0)
#define INTERRUPT_RECEIVER_ENABLED		(1<<RXCIE0)
#define INTERRUPT_TRANSMITTER_DISABLED	(0<<TXCIE0)
#define INTERRUPT_TRANSMITTER_ENABLED	(1<<TXCIE0)

// TIMER
#define PERIOD							2000
#define TIME_MS(ms)						(((F_CPU / PRESCALE_VALUE) * ms) / 1000)
#define PRESCALE_VALUE					1024		// can be 1, 8, 64, 256, 1024
// table 16-5 : prescale sets
#define PRESCALE_SET(value)  \
										((value) == 1   ? (0<<CS12 | 0<<CS11 | 1<<CS10) : \
										 (value) == 8   ? (0<<CS12 | 1<<CS11 | 0<<CS10) : \
										 (value) == 64  ? (0<<CS12 | 1<<CS11 | 1<<CS10) : \
										 (value) == 256 ? (1<<CS12 | 0<<CS11 | 0<<CS10) : \
										 (value) == 1024? (1<<CS12 | 0<<CS11 | 1<<CS10) : \
														  (0<<CS12 | 0<<CS11 | 0<<CS10))
// Table 16-4 : Waveform Generation Mode Bit Description
#define CTC_TOP_OCR1A_IN_TCCR1B			(0<<WGM13 | 1<<WGM12)
#define CTC_TOP_OCR1A_IN_TCCR1A									(0<<WGM11 | 0<<WGM10)
#define CTC_TOP_ICR1_IN_TCCR1B			(1<<WGM13 | 1<<WGM12)
#define CTC_TOP_ICR1_IN_TCCR1A									(0<<WGM11 | 0<<WGM10)
#define FAST_PWM_TOP_OCR1A_IN_TCCR1B	(1<<WGM13 | 1<<WGM12)
#define FAST_PWM_TOP_OCR1A_IN_TCCR1A							(1<<WGM11 | 1<<WGM10)
#define FAST_PWM_TOP_ICR1_IN_TCCR1B		(1<<WGM13 | 1<<WGM12)
#define FAST_PWM_TOP_ICR1_IN_TCCR1A								(1<<WGM11 | 0<<WGM10)
// 16.11.8 : Timer/Counter1 Interrupt Mask Register
#define INTERRUPT_ENABLE_CHANNEL_A		(1 << OCIE1A)
#define INTERRUPT_DISABLE_CHANNEL_A		(0 << OCIE1A)

#endif