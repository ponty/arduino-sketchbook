// common interface for frequency counter libraries


#ifdef FreqMeasure2_h
void freq_setup()
{
    FreqMeasure2.begin();
}

float read_freq(uint16_t gate_time)
{
    uint32_t pp=0;
    int32_t i=0;
//    while (!FreqMeasure2.available())         // wait until counter ready
//    {
//        i++;
//        if (i>1000)
//            return 0;
//        delayMicroseconds(1000);
//    }
    delayMicroseconds(1000);
    pp=FreqMeasure2.read();


    return 16000000.0 / pp;
}
#endif

#ifdef FreqMeasure_h
void freq_setup()
{
    FreqMeasure.begin();
}

float read_freq(uint16_t gate_time)
{
//    return 25;
    uint32_t pp=0;
    int32_t i=0;
    while (!FreqMeasure.available())         // wait until counter ready
    {
        i++;
        if (i>1000)
            return 0;
        delayMicroseconds(1000);
    }
    pp=FreqMeasure.read();

    // clear buffer
    FreqMeasure.read();
    FreqMeasure.read();
    FreqMeasure.read();
    FreqMeasure.read();
    FreqMeasure.read();
    FreqMeasure.read();
    FreqMeasure.read();
    FreqMeasure.read();
    FreqMeasure.read();
    FreqMeasure.read();
    FreqMeasure.read();
    FreqMeasure.read();

    return 16000000.0 / pp*1000;
}
#endif

#ifdef FreqPeriod_h
void freq_setup()
{
    FreqPeriod::begin();
}

float read_freq(uint16_t gate_time)
{
    uint32_t pp=0;
    while (!pp)         // wait until counter ready
    {
        pp=FreqPeriod::getPeriod();
    }
    return 16000000.0 / pp;
}
#endif

#ifdef FreqCounter_h

float read_freq(uint16_t gate_time)
{
    FreqCounter::start(gate_time);
    while (FreqCounter::f_ready == 0)         // wait until counter ready
    {
    }
    return FreqCounter::f_freq;            // read result

}
void freq_setup()
{
    FreqCounter::f_comp = 8;             // Set compensation to 12

    // drop first read, wich is buggy
    read_freq(10);
}
#endif

#ifdef FreqCount_h

void freq_setup()
{
    FreqCount.begin(1000);
}

float read_freq(uint16_t gate_time)
{
    FreqCount.begin(gate_time);
    while (!FreqCount.available())         // wait until counter ready
    {
    }
    return FreqCount.read();            // read result

}

#endif
