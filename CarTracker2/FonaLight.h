#pragma once

#include "Adafruit_FONA.h"

class FonaLight : public Adafruit_FONA {
 public:
    FonaLight(int8_t rst);
    boolean HTTP_para_func(FONAFlashStringPtr parameter, void (*url_func)(Stream* ser));
    boolean HTTP_setup(void (*url_func)(Stream* ser));
    boolean HTTP_POST_start_func(void (*url_func)(Stream* ser), FONAFlashStringPtr contenttype, uint16_t (*postdata_func)(Stream*),  uint16_t *status, uint16_t *datalen);

};

