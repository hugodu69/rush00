#ifndef TIMER_H
#define TIMER_H

// table 16-5 : prescale sets
#define PRESCALE_SET(value)  \
									((value) == 1   ? (0<<CS12 | 0<<CS11 | 1<<CS10) : \
									 (value) == 8   ? (0<<CS12 | 1<<CS11 | 0<<CS10) : \
									 (value) == 64  ? (0<<CS12 | 1<<CS11 | 1<<CS10) : \
									 (value) == 256 ? (1<<CS12 | 0<<CS11 | 0<<CS10) : \
									 (value) == 1024? (1<<CS12 | 0<<CS11 | 1<<CS10) : \
													  (0<<CS12 | 0<<CS11 | 0<<CS10))
#define TIME_MS(ms, prescale_value)	(((F_CPU / prescale_value) * ms) / 1000)

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

#endif // TIMER_H