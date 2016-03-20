#ifndef FreqMeasure2_h
#define FreqMeasure2_h

#include <inttypes.h>

class FreqMeasureClass2 {
public:
    static void begin(void);
    static uint8_t available(void);
    static float read(void);
    static void end(void);

//    static uimt32_t read_sum(void);
//    static uimt32_t read_count(void);
};

extern FreqMeasureClass2 FreqMeasure2;

#endif
