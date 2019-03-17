#include "WirelessFan.h"

float WirelessFan::MeasureDutyCycle(uint16 pin)
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

void WirelessFan::RefreshDutyCycle()
{
    //measure requested PWM and constrain it
    _requestedDutyCycle = round(MeasureDutyCycle(PWM_FROM_MB));

    //constrain and remap it to new range
    _dutyCycleToApply = map(constrain(_requestedDutyCycle, REQUESTED_DUTYCYCLE_MIN, REQUESTED_DUTYCYCLE_MAX), REQUESTED_DUTYCYCLE_MIN, REQUESTED_DUTYCYCLE_MAX, APPLIED_DUTYCYCLE_MIN, APPLIED_DUTYCYCLE_MAX);

    //   Serial.print(F("dutyCycleToApply="));
    //   Serial.println(dutyCycleToApply);

    //apply duty cycle
    analogWrite(PWM_OUTPUT_PIN, _dutyCycleToApply);
}

//------------------------------------------
//Used to initialize configuration properties to default values
void WirelessFan::SetConfigDefaultValues(){
    //TODO
    //property1 = 0;
    //property2 = F("test");
};
//------------------------------------------
//Parse JSON object into configuration properties
void WirelessFan::ParseConfigJSON(DynamicJsonDocument &doc){
    //TODO
    //if (!doc["prop1"].isNull()) property1 = doc[F("prop1")];
    //if (!doc["prop2"].isNull()) strlcpy(property2, doc["prop2"], sizeof(property2));
};
//------------------------------------------
//Parse HTTP POST parameters in request into configuration properties
bool WirelessFan::ParseConfigWebRequest(AsyncWebServerRequest *request)
{
    //TODO
    // if (!request->hasParam(F("prop1"), true))
    // {
    //     request->send(400, F("text/html"), F("prop1 missing"));
    //     return false;
    // }
    //if (request->hasParam(F("prop1"), true)) property1 = request->getParam(F("prop1"), true)->value().toInt();
    //if (request->hasParam(F("prop2"), true) && request->getParam(F("prop2"), true)->value().length() < sizeof(property2)) strcpy(property2, request->getParam(F("prop2"), true)->value().c_str());

    return true;
};
//------------------------------------------
//Generate JSON from configuration properties
String WirelessFan::GenerateConfigJSON(bool forSaveFile = false)
{
    String gc('{');
    //TODO
    // gc = gc + F("\"p1\":") + (property1 ? true : false);
    // gc = gc + F("\"p2\":\"") + property2 + '"';

    gc += '}';

    return gc;
};
//------------------------------------------
//Generate JSON of application status
String WirelessFan::GenerateStatusJSON()
{
    String gs('{');

    gs = gs + F("\"rdc\":\"") + _requestedDutyCycle + '%' + '"';
    gs = gs + F(",\"adc\":\"") + _dutyCycleToApply + '%' + '"';

    gs += '}';

    return gs;
};
//------------------------------------------
//code to execute during initialization and reinitialization of the app
bool WirelessFan::AppInit(bool reInit)
{
    pinMode(PWM_OUTPUT_PIN, OUTPUT); // Initialize the PWM_OUTPUT_PIN pin as an output
    analogWriteFreq(25000);
    analogWriteRange(100);
    analogWrite(PWM_OUTPUT_PIN, _dutyCycleToApply);

    if (!_reInit)
        _refreshDutyCycleTimer.setInterval(200, [this]() { this->RefreshDutyCycle(); });

    return true;
};
//------------------------------------------
//Return HTML Code to insert into Status Web page
const uint8_t *WirelessFan::GetHTMLContent(WebPageForPlaceHolder wp)
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
size_t WirelessFan::GetHTMLContentSize(WebPageForPlaceHolder wp)
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
void WirelessFan::AppInitWebServer(AsyncWebServer &server, bool &shouldReboot, bool &pauseApplication){
    //TODO
    //server.on("/getColor", HTTP_GET, [this](AsyncWebServerRequest * request) {request->send(200, F("text/html"), GetColor());});
};

//------------------------------------------
//Run for timer
void WirelessFan::AppRun()
{
    _refreshDutyCycleTimer.run();
}

//------------------------------------------
//Constructor
WirelessFan::WirelessFan(char appId, String appName) : Application(appId, appName)
{
    //TODO : Initialize special structure or libraries in constructor
    //Note : most of the time, init is done during AppInit based on configuration
}