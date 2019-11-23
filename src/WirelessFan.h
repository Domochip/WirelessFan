#ifndef WirelessFan_h
#define WirelessFan_h

#include "Main.h"
#include "base\Utils.h"
#include "base\Application.h"

const char appDataPredefPassword[] PROGMEM = "ewcXoCt4HHjZUvY1";

#include "data\status1.html.gz.h"
#include "data\config1.html.gz.h"

#include <Ticker.h>

#define PWM_OUTPUT_PIN D2
#define PWM_FROM_MB D1

//define dutycycles in % for adjusting Fan speed
#define REQUESTED_DUTYCYCLE_MIN 22
#define REQUESTED_DUTYCYCLE_MAX 100
#define APPLIED_DUTYCYCLE_MIN 6
#define APPLIED_DUTYCYCLE_MAX 100

//TODO : Rename WirelessFan
class WirelessFan : public Application
{
private:
  //TODO : Declare configuration properies

  //TODO : Declare run/status properties
  float _requestedDutyCycle = 100.0;
  float _dutyCycleToApply = (APPLIED_DUTYCYCLE_MAX + APPLIED_DUTYCYCLE_MIN) / 2; //default value is medium duty cycle
  bool _needRefreshDutyCycle = false;
  Ticker _refreshDutyCycleTicker;

  //TODO : Declare required private methods
  float measureDutyCycle(uint16 pin);
  void refreshDutyCycle();

  void setConfigDefaultValues();
  void parseConfigJSON(DynamicJsonDocument &doc);
  bool parseConfigWebRequest(AsyncWebServerRequest *request);
  String generateConfigJSON(bool forSaveFile);
  String generateStatusJSON();
  bool appInit(bool reInit);
  const uint8_t *getHTMLContent(WebPageForPlaceHolder wp);
  size_t getHTMLContentSize(WebPageForPlaceHolder wp);
  void appInitWebServer(AsyncWebServer &server, bool &shouldReboot, bool &pauseApplication);
  void appRun();

public:
  WirelessFan(char appId, String fileName);
};

#endif
