#include "WirelessFan.h"

float WirelessFan::measureDutyCycle(uint16 pin)
{
    float res = 0.0;
    uint16 nbHigh = 0;

    for (uint16 i = 1; i > 0; i++)
        nbHigh += ((GPI & digitalPinToBitMask(pin)) != 0 ? 1 : 0);

    res = nbHigh;
    res *= 100;
    res /= 0xFFFF;

    return res;
}

void WirelessFan::refreshDutyCycle()
{
    //measure requested PWM and constrain it
    _requestedDutyCycle = round(measureDutyCycle(PWM_FROM_MB));

    //constrain and remap it to new range
    _dutyCycleToApply = map(constrain(_requestedDutyCycle, REQUESTED_DUTYCYCLE_MIN, REQUESTED_DUTYCYCLE_MAX), REQUESTED_DUTYCYCLE_MIN, REQUESTED_DUTYCYCLE_MAX, APPLIED_DUTYCYCLE_MIN, APPLIED_DUTYCYCLE_MAX);

    //   LOG_SERIAL.print(F("dutyCycleToApply="));
    //   LOG_SERIAL.println(dutyCycleToApply);

    //apply duty cycle
    analogWrite(PWM_OUTPUT_PIN, _dutyCycleToApply);
}

//------------------------------------------
//Used to initialize configuration properties to default values
void WirelessFan::setConfigDefaultValues(){};
//------------------------------------------
//Parse JSON object into configuration properties
void WirelessFan::parseConfigJSON(DynamicJsonDocument &doc){};
//------------------------------------------
//Parse HTTP POST parameters in request into configuration properties
bool WirelessFan::parseConfigWebRequest(AsyncWebServerRequest *request)
{
    return true;
};
//------------------------------------------
//Generate JSON from configuration properties
String WirelessFan::generateConfigJSON(bool forSaveFile = false)
{
    String gc('{');

    gc += '}';

    return gc;
};
//------------------------------------------
//Generate JSON of application status
String WirelessFan::generateStatusJSON()
{
    String gs('{');

    gs = gs + F("\"rdc\":\"") + _requestedDutyCycle + '%' + '"';
    gs = gs + F(",\"adc\":\"") + _dutyCycleToApply + '%' + '"';

    gs += '}';

    return gs;
};
//------------------------------------------
//code to execute during initialization and reinitialization of the app
bool WirelessFan::appInit(bool reInit)
{
    pinMode(PWM_OUTPUT_PIN, OUTPUT); // Initialize the PWM_OUTPUT_PIN pin as an output
    analogWriteFreq(25000);
    analogWriteRange(100);
    analogWrite(PWM_OUTPUT_PIN, _dutyCycleToApply);

    if (!_reInit)
        _refreshDutyCycleTicker.attach_ms(200, [this]() { this->_needRefreshDutyCycle = true; });

    return true;
};
//------------------------------------------
//Return HTML Code to insert into Status Web page
const uint8_t *WirelessFan::getHTMLContent(WebPageForPlaceHolder wp)
{
    switch (wp)
    {
    case status:
        return (const uint8_t *)status1htmlgz;
        break;
    case config:
        return (const uint8_t *)config1htmlgz;
        break;
    default:
        return nullptr;
        break;
    };
    return nullptr;
};
//and his Size
size_t WirelessFan::getHTMLContentSize(WebPageForPlaceHolder wp)
{
    switch (wp)
    {
    case status:
        return sizeof(status1htmlgz);
        break;
    case config:
        return sizeof(config1htmlgz);
        break;
    default:
        return 0;
        break;
    };
    return 0;
};

//------------------------------------------
//code to register web request answer to the web server
void WirelessFan::appInitWebServer(AsyncWebServer &server, bool &shouldReboot, bool &pauseApplication){};

//------------------------------------------
//Run for timer
void WirelessFan::appRun()
{
    if (_needRefreshDutyCycle)
    {
        _needRefreshDutyCycle = false;
        refreshDutyCycle();
    }
}

//------------------------------------------
//Constructor
WirelessFan::WirelessFan(char appId, String appName) : Application(appId, appName) {}