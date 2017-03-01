#pragma once

#define Q "\""


#ifdef __MBED__
//char data[2];

sprintf (data, "{"
        Q "battery" Q ":%d"

        "," Q "temp" Q ":%f"
        "," Q "speed" Q ":%d"
        "," Q "event" Q ":%d"

        "," Q "rssi" Q ": {" Q "value" Q ":%d, "
        Q "context" Q ":{" Q "lat" Q ":%f, " Q "lng" Q ":%f}}"
        "," Q "hdop" Q ": {" Q "value" Q ":%f, "
        Q "context" Q ":{" Q "lat" Q ":%f, " Q "lng" Q ":%f}}"
        "}"
        ,
        vbat,
        temp,
        int(speed),
        event,

        rssi,
        gsmlat,
        gsmlon,

        hdop,
        latitude,
        longitude

);
#else

void url_func(Stream* ser)
{
    ser->print(F("http://things.ubidots.com/api/v1.6/devices/Demo?token="));
    ser->print(F(UBIDOTS_TOKEN));

}

uint16_t postdata_func(Stream* ser)
{
    uint16_t len = 0;

    len += ser->print(F("{"));

    len += ser->print(F( "\"battery\":"));
    len += ser->print(vbat);

    len += ser->print(F(",\"temp\":"));
    len += ser->print(temp);

    len += ser->print(F(",\"speed\":"));
    len += ser->print(int(speed));

    len += ser->print(F(",\"event\":"));
    len += ser->print(event);

    len += ser->print(F(",\"rssi\": {\"value\":"));
    len += ser->print(rssi);
    len += ser->print(F(", \"context\":{\"lat\":"));
    len += ser->print(gsmlat, 7);
    len += ser->print(F(", \"lng\":"));
    len += ser->print(gsmlon, 7);
    len += ser->print(F("}}"));

    len += ser->print(F(",\"hdop\": {\"value\":"));
    len += ser->print(hdop);
    len += ser->print(F(", \"context\":{\"lat\":"));
    len += ser->print(latitude, 7);
    len += ser->print(F(", \"lng\":"));
    len += ser->print(longitude, 7);
    len += ser->print(F("}}"));

    len += ser->print(F("}"));
    return len;
}


#endif
