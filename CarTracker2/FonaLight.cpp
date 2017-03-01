#include "FonaLight.h"

#define debug_serial  Serial

FonaLight::FonaLight(int8_t rst) : Adafruit_FONA(rst)
{
}

boolean FonaLight::HTTP_para_func(FONAFlashStringPtr parameter,
        void (*url_func)(Stream* ser)) {
  url_func(&debug_serial);
  debug_serial.println();
  HTTP_para_start(parameter, true);
  url_func(mySerial);
  return HTTP_para_end(true);
}

boolean FonaLight::HTTP_setup(void (*url_func)(Stream* ser)) {
  // Handle any pending
  HTTP_term();

  // Initialize and set parameters
  if (! HTTP_init())
    return false;
  if (! HTTP_para(F("CID"), 1))
    return false;
  if (! HTTP_para(F("UA"), useragent))
    return false;
  if (! HTTP_para_func(F("URL"), url_func))
    return false;

  // HTTPS redirect
  if (httpsredirect) {
    if (! HTTP_para(F("REDIR"),1))
      return false;

    if (! HTTP_ssl(true))
      return false;
  }

  return true;
}

#define MAXTIME  20000
boolean FonaLight::HTTP_POST_start_func(void (*url_func)(Stream* ser),
              FONAFlashStringPtr contenttype,
              uint16_t (*postdata_func)(Stream*),
              uint16_t *status, uint16_t *datalen){
  if (! HTTP_setup(url_func))
    return false;

  if (! HTTP_para(F("CONTENT"), contenttype)) {
    return false;
  }

  // HTTP POST data
  uint16_t postdatalen = postdata_func(&debug_serial);
  debug_serial.println();
  if (! HTTP_data(postdatalen, MAXTIME))
    return false;
  postdata_func(mySerial);
//  mySerial->write(postdata, postdatalen);
  if (! expectReply(ok_reply))
    return false;

  // HTTP POST
  if (! HTTP_action(FONA_HTTP_POST, status, datalen))
    return false;

  DEBUG_PRINT(F("Status: ")); DEBUG_PRINTLN(*status);
  DEBUG_PRINT(F("Len: ")); DEBUG_PRINTLN(*datalen);

  // HTTP response data
  if (! HTTP_readall(datalen))
    return false;

  return true;
}
