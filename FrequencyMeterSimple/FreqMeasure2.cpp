/* FreqMeasure Library, for measuring relatively low frequencies
 * http://www.pjrc.com/teensy/td_libs_FreqMeasure.html
 * Copyright (c) 2011 PJRC.COM, LLC - Paul Stoffregen <paul@pjrc.com>
 *
 * Version 1.0
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "FreqMeasure2.h"
#include "capture2.h"
#include <HardwareSerial.h>

//#define FREQMEASURE_BUFFER_LEN 12
//static volatile uint32_t buffer_value[FREQMEASURE_BUFFER_LEN];
static volatile uint8_t buffer_head;
static volatile uint8_t buffer_tail;
static uint16_t capture_msw;
static uint32_t capture_previous;
static uint32_t count;
static uint32_t count2;
static uint32_t sum;
static uint32_t sum2;


void FreqMeasureClass2::begin(void)
{
    capture_init();
    capture_msw = 0;
//    capture_previous = 0;
//    buffer_head = 0;
//    buffer_tail = 0;
    sum=0;
    count=0;
    sum2=0;
    count2=0;

    capture_start();
}
//uimt32_t FreqMeasureClass2::read_sum(void)
//{
//    return sum2;
//}
//uimt32_t FreqMeasureClass2::read_count(void)
//{
//    return count2;
//}

uint8_t FreqMeasureClass2::available(void)
{
    return 0;
//    uint8_t head, tail;
//
//    head = buffer_head;
//    tail = buffer_tail;
//    if (head >= tail) return head - tail;
//    return FREQMEASURE_BUFFER_LEN + head - tail;
}

float FreqMeasureClass2::read(void)
{
//    uint8_t head, tail;
//    uint32_t value;

//    head = buffer_head;
//    tail = buffer_tail;
//    if (head == tail) return 0xFFFFFFFF;
//    tail = tail + 1;
//    if (tail >= FREQMEASURE_BUFFER_LEN) tail = 0;
//    value = buffer_value[tail];
//    buffer_tail = tail;

    Serial.print(" sum:");        Serial.print(sum2);
    Serial.print(" count:");        Serial.print(count2);
        float p =1.0*sum/count;
        Serial.print(" period:");        Serial.print(p);
        Serial.print(" f:");        Serial.print(1.0*F_CPU/p);
    Serial.println();

    return 1.0*sum2/count2;
//    return sum2;
}

void FreqMeasureClass2::end(void)
{
    capture_shutdown();
}


ISR(TIMER_OVERFLOW_VECTOR)
{
    capture_msw++;
}

ISR(TIMER_CAPTURE_VECTOR)
{
    uint16_t capture_lsw;
    uint32_t capture, period;
//    uint8_t i;

    // get the timer capture
    capture_lsw = capture_read();
    // Handle the case where but capture and overflow interrupts were pending
    // (eg, interrupts were disabled for a while), or where the overflow occurred
    // while this ISR was starting up.  However, if we read a 16 bit number that
    // is very close to overflow, then ignore any overflow since it probably
    // just happened.
    if (capture_overflow() && capture_lsw < 0xFF00) {
        capture_overflow_reset();
        capture_msw++;
    }
    // compute the waveform period
    capture = ((uint32_t)capture_msw << 16) | capture_lsw;

    period = capture - capture_previous;
    capture_previous = capture;
    // store it into the buffer
//    i = buffer_head + 1;
//    if (i >= FREQMEASURE_BUFFER_LEN) i = 0;
//    if (i != buffer_tail) {
//        buffer_value[i] = period;
//        buffer_head = i;
//    }


//    sum2=period;
//    count2=1;

    sum+=period;
    count++;
    if (sum>1000000)
    {

        sum2=sum;
        count2=count;

        sum=0;
        count=0;

    }


}


FreqMeasureClass2 FreqMeasure2;

