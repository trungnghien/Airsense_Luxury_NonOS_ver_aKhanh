#include "WiFiDriver.h"

ERROR_CODE WIFI_init()
{
#ifdef USING_WIFI_CONFIG

    WiFi.begin(ssid, pass);
    int retry_counter = 0;
    while (WiFi.status() != WL_CONNECTED && retry_counter <= 20) {
        delay(1000);
        retry_counter++;
    }
    if (WiFi.status() == wl_status_t::WL_CONNECTED)
    {
        log_e("WiFi connected!");
        log_e("IP: %s", WiFi.localIP().toString().c_str());
        connectionStatus_st.wifiStatus = status_et::CONNECTED;
        return ERROR_NONE;
    } else 
    {
        log_e("WiFi initialized failed!");
        connectionStatus_st.wifiStatus = status_et::DISCONNECTED;
        return ERROR_WIFI_INIT_FAILED;
    }
#else
 //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;

  //exit after config instead of connecting
  wifiManager.setBreakAfterConfig(true);

  wifiManager.setTimeout(60);

  //reset settings - for testing
  //wifiManager.resetSettings();


  //tries to connect to last known settings
  //if it does not connect it starts an access point with the specified name
  //here  "AutoConnectAP" with password "password"
  //and goes into a blocking loop awaiting configuration
  if (!wifiManager.autoConnect("AirSENSE-Ozone-1", "airsense")) 
  {
        log_e("WiFi initialized failed!");
        connectionStatus_st.wifiStatus = status_et::DISCONNECTED;
        return ERROR_WIFI_INIT_FAILED;
  }

    log_e("WiFi connected!");
    log_e("IP: %s", WiFi.localIP().toString().c_str());
    connectionStatus_st.wifiStatus = status_et::CONNECTED;
    return ERROR_NONE;
#endif
}