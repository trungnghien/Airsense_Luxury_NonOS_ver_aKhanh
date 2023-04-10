/**
 * @file WiFiManager.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-11-11
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef WIFIDRIVER_H
#define WIFIDRIVER_H

#include <WiFi.h>
#include <string>
#include "globals.h"
#include "configs.h"
#include "esp_log.h"

#include <FS.h>                   //this needs to be first, or it all crashes and burns...


//needed for library
#include <DNSServer.h>
#include <WebServer.h>
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager

#define ERROR_WIFI_INIT_FAILED (0xa1)

/**
 * @brief WiFi initialized
 * 
 * @return ERROR_CODE 
 */
ERROR_CODE WIFI_init();


#endif