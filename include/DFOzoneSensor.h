/**
 * @file PMS7003Sensor.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-11-20
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef DFOZONESENSOR_H
#define DFOZONESENSOR_H

#include "DFRobot_OzoneSensor.h"
#include "esp_log.h"
#include "configs.h"

#define Ozone_IICAddress 0x73
#define COLLECT_NUMBER  50              // collect number, the collection range is 1-100
#define ERROR_OZONE_INIT_FAILED       0xe0
#define ERROR_OZONE_GET_DATA_FAILED   0xe1

DFRobot_OzoneSensor Ozone;


/**
 * @brief Initialize OZONE SENSOR 
 *  
 * @return ERROR_CODE 
 * @retval ERROR_NONE: if success
 * @retval ERROR_PMS_INIT_FAILED: if failed
 */
ERROR_CODE dfozone_init();

/**
 * @brief Get new data from OZONE SENSOR
 * 
 * @param[out] _co_2: PM CO2
 * 
 * @return ERROR_CODE 
 * @retval ERROR_NONE: if success
 * @retval ERROR_PMS_GET_DATA_FAILED: if failed
 */
ERROR_CODE dfozone_getdata(uint32_t &_o_3);


ERROR_CODE dfozone_init()
{
    int ozone_decect_count = 0;
    while ( !Ozone.begin(Ozone_IICAddress)) {
        log_e("I2c device number error !");
        delay(1000);
        ozone_decect_count++;
        if(ozone_decect_count>10) 
        {
            return ERROR_OZONE_INIT_FAILED;
        }
    };
    log_e("I2c connect success !");

  /**
   * set measuer mode
   * MEASURE_MODE_AUTOMATIC         active  mode
   * MEASURE_MODE_PASSIVE           passive mode
   */
    Ozone.setModes(MEASURE_MODE_AUTOMATIC);
    return ERROR_NONE;
}

ERROR_CODE dfozone_getdata(uint32_t &_o_3)
{
    /**
     * Smooth data collection
     * COLLECT_NUMBER                 The collection range is 1-100
     */
    int16_t ozoneConcentration = Ozone.readOzoneData(COLLECT_NUMBER);
    if (ozoneConcentration <= 0) {
        // An error occurred
        _o_3 = -1;
        log_e("Ozone Sensor get new data failed!");
        return ERROR_OZONE_GET_DATA_FAILED;
    } else 
    {
        // Print CO2 concentration in ppm
        _o_3 = ozoneConcentration;
        log_e("Ozone Sensor get new data successfully! %d", ozoneConcentration);
        return ERROR_NONE;
    }
}

#endif