#ifndef FreqCount_timers_h_
#define FreqCount_timers_h_

#include <avr/io.h>
#include <avr/interrupt.h>

  #define COUNTER_USE_TIMER1    // T1 is pin 5
  #define TIMER_USE_TIMER2


/**********************************************/
/*   Counter Hardware Abstraction             */
/**********************************************/

//#if defined(COUNTER_USE_TIMER1) // 16 bit Timer 1

static uint8_t saveTCCR1A, saveTCCR1B;

static inline void counter_init(void)
{
    saveTCCR1A = TCCR1A;
    saveTCCR1B = TCCR1B;
    TCCR1B = 0;
    TCCR1A = 0;
    TCNT1 = 0;
    TIFR1 = (1<<TOV1);
    TIMSK1 = 0;
}

static inline void counter_start(void)
{
    TCCR1B = (0<<CS12) | (1<<CS11) | (0<<CS10);
}

static inline void counter_shutdown(void)
{
    TCCR1B = 0;
    TCCR1A = saveTCCR1A;
    TCCR1B = saveTCCR1B;
}

static inline uint16_t counter_read(void)
{
    return TCNT1;
}

static inline uint8_t counter_overflow(void)
{
    return TIFR1 & (1<<TOV1);
}

static inline uint8_t counter_overflow_reset(void)
{
    return TIFR1 = (1<<TOV1);
}




/**********************************************/
/*   Timer Hardware Abstraction               */
/**********************************************/

//#if defined(TIMER_USE_TIMER2) // 8 bit Timer 2

/*        1ms       2ms       4ms       8ms
16 MHz    128x125   256x125   256x250   1024x125
8 MHz     64x125    128x125   256x125   256x250
4 MHz     32x125    64x125    128x125   256x125
2 MHz     8x250     32x125    64x125    128x125
1 MHz     8x125     8x250     32x125    64x125
*/
#if F_CPU == 16000000L
  #define TIMER2_OCR2A_1MS_VAL  124                 // div 125
  #define TIMER2_TCCR2B_1MS_VAL             (1<<CS22) | (1<<CS20)   // div 128
  #define TIMER2_OCR2A_2MS_VAL  124                 // div 125
  #define TIMER2_TCCR2B_2MS_VAL (1<<CS22) | (1<<CS21)           // div 256
  #define TIMER2_OCR2A_4MS_VAL  249                 // div 250
  #define TIMER2_TCCR2B_4MS_VAL (1<<CS22) | (1<<CS21)           // div 256
  #define TIMER2_OCR2A_8MS_VAL  124                 // div 125
  #define TIMER2_TCCR2B_8MS_VAL (1<<CS22) | (1<<CS21) | (1<<CS20)   // div 1024
#elif F_CPU == 8000000L
  #define TIMER2_OCR2A_1MS_VAL  124                 // div 125
  #define TIMER2_TCCR2B_1MS_VAL (1<<CS22)               // div 64
  #define TIMER2_OCR2A_2MS_VAL  124                 // div 125
  #define TIMER2_TCCR2B_2MS_VAL (1<<CS22) |             (1<<CS20)   // div 128
  #define TIMER2_OCR2A_4MS_VAL  124                 // div 125
  #define TIMER2_TCCR2B_4MS_VAL (1<<CS22) | (1<<CS21)           // div 256
  #define TIMER2_OCR2A_8MS_VAL  249                 // div 250
  #define TIMER2_TCCR2B_8MS_VAL (1<<CS22) | (1<<CS21)           // div 256
#elif F_CPU == 4000000L
  #define TIMER2_OCR2A_1MS_VAL  124                 // div 125
  #define TIMER2_TCCR2B_1MS_VAL             (1<<CS21) | (1<<CS20)   // div 32
  #define TIMER2_OCR2A_2MS_VAL  124                 // div 125
  #define TIMER2_TCCR2B_2MS_VAL (1<<CS22)               // div 64
  #define TIMER2_OCR2A_4MS_VAL  124                 // div 125
  #define TIMER2_TCCR2B_4MS_VAL (1<<CS22) |             (1<<CS20)   // div 128
  #define TIMER2_OCR2A_8MS_VAL  124                 // div 125
  #define TIMER2_TCCR2B_8MS_VAL (1<<CS22) | (1<<CS21)           // div 256
#elif F_CPU == 2000000L
  #define TIMER2_OCR2A_1MS_VAL  249                 // div 250
  #define TIMER2_TCCR2B_1MS_VAL             (1<<CS21)           // div 8
  #define TIMER2_OCR2A_2MS_VAL  124                 // div 125
  #define TIMER2_TCCR2B_2MS_VAL             (1<<CS21) | (1<<CS20)   // div 32
  #define TIMER2_OCR2A_4MS_VAL  124                 // div 125
  #define TIMER2_TCCR2B_4MS_VAL (1<<CS22)               // div 64
  #define TIMER2_OCR2A_8MS_VAL  124                 // div 125
  #define TIMER2_TCCR2B_8MS_VAL (1<<CS22) | (1<<CS21)           // div 128
#elif F_CPU == 1000000L
  #define TIMER2_OCR2A_1MS_VAL  124                 // div 125
  #define TIMER2_TCCR2B_1MS_VAL             (1<<CS21)           // div 8
  #define TIMER2_OCR2A_2MS_VAL  249                 // div 250
  #define TIMER2_TCCR2B_2MS_VAL             (1<<CS21)           // div 8
  #define TIMER2_OCR2A_4MS_VAL  124                 // div 125
  #define TIMER2_TCCR2B_4MS_VAL             (1<<CS21) | (1<<CS20)   // div 32
  #define TIMER2_OCR2A_8MS_VAL  124                 // div 125
  #define TIMER2_TCCR2B_8MS_VAL (1<<CS22)               // div 64
#else
#error "Clock must be 16, 8, 4, 2 or 1 MHz"
#endif

/*  1ms 2ms 4ms 8ms
16 MHz  128x125 256x125 256x250 1024x125
8 MHz   64x125  128x125 256x125 256x250
4 MHz   32x125  64x125  128x125 256x125
2 MHz   8x250   32x125  64x125  128x125
1 MHz   8x125   8x250   32x125  64x125
*/

static uint8_t saveTCCR2A, saveTCCR2B;
static uint8_t startTCCR2B;

static inline uint16_t timer_init(uint16_t msec)
{
    uint16_t gate_len;

    saveTCCR2A = TCCR2A;
    saveTCCR2B = TCCR2B;
    TCCR2B = 0;
    TCCR2A = (1<<WGM21);
    if ((msec & 7) == 0) {
        gate_len = msec >> 3;
        OCR2A = TIMER2_OCR2A_8MS_VAL;
        startTCCR2B = TIMER2_TCCR2B_8MS_VAL;
    } else if ((msec & 3) == 0) {
        gate_len = msec >> 2;
        OCR2A = TIMER2_OCR2A_4MS_VAL;
        startTCCR2B = TIMER2_TCCR2B_4MS_VAL;
    } else if ((msec & 1) == 0) {
        gate_len = msec >> 1;
        OCR2A = TIMER2_OCR2A_2MS_VAL;
        startTCCR2B = TIMER2_TCCR2B_2MS_VAL;
    } else {
        gate_len = msec;
        OCR2A = TIMER2_OCR2A_1MS_VAL;
        startTCCR2B = TIMER2_TCCR2B_1MS_VAL;
    }
    TIFR2 = (1<<OCF2A);
    TCNT2 = 0;
    return gate_len;
}

static inline void timer_start(void)
{
    GTCCR = (1<<PSRASY);
    TCCR2B = startTCCR2B;
    TIMSK2 = (1<<OCIE2A);
}

static inline void timer_shutdown(void)
{
    TCCR2B = 0;
    TIMSK2 = 0;
    TCCR2A = saveTCCR2A;
    TCCR2B = saveTCCR2B;
}

#define TIMER_ISR_VECTOR TIMER2_COMPA_vect

/*
There is a typical latency from the timer interrupt until the first
actual line of code executes.  Here is a typical compiler output of
approximately 34 cycles.  When starting, this same delay is used to
begin counting, so the first reading will not have 34 cycles of
extra measurement.  Because each measurement period gates instantly
after the previous, this approximate correction only affects the
first measurement.  If you do not define TIMER_LATENCY_CYCLES, this
extra delay is skipped (saving a tiny bit of code space), and the
only downside is a slight inaccuracy in the first measurement.
2  00000326 <__vector_13>:
2       326:       1f 92           push    r1
2       328:       0f 92           push    r0
1       32a:       0f b6           in      r0, 0x3f        ; 63
2       32c:       0f 92           push    r0
1       32e:       11 24           eor     r1, r1
2       330:       ef 92           push    r14
2       332:       ff 92           push    r15
2       334:       0f 93           push    r16
2       336:       1f 93           push    r17
2       338:       2f 93           push    r18
2       33a:       3f 93           push    r19
2       33c:       4f 93           push    r20
2       33e:       5f 93           push    r21
2       340:       8f 93           push    r24
2       342:       9f 93           push    r25
2       344:       af 93           push    r26
2       346:       bf 93           push    r27
*/
#define TIMER_LATENCY_CYCLES 34





static inline void timer_isr_latency_delay(void)
{
#ifdef TIMER_LATENCY_CYCLES
    uint8_t cycles_times_3 = TIMER_LATENCY_CYCLES / 3;
    asm volatile(
        "L_%=_loop:"
        "subi   %0, 1"      "\n\t"
        "brne   L_%=_loop"  "\n\t"
        : "+d" (cycles_times_3)
        : "0" (cycles_times_3)
    );
#endif
}


/**********************************************/
/*   Board Specific Interrupts (to hog)       */
/**********************************************/

static inline void disable_other_interrupts(void)
{
}
static inline void restore_other_interrupts(void)
{
}



#endif
