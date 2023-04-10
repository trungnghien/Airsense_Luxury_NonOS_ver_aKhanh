#include "DataStructure.h"

ERROR_CODE creatCalibDataString(char *_calibDataString, struct calibData _calibData)
{
	sprintf(_calibDataString, "%f|%f|%f|%f|%f|%f|%f\n",
							  _calibData.temperature_calibA,
							  _calibData.humidity_calibA,
							  _calibData.pm1_calibA,
							  _calibData.pm10_calibA,
							  _calibData.pm25_calibA,
                              _calibData.o3_calibA,
							  _calibData.temperature_calibB,
							  _calibData.humidity_calibB,
                              _calibData.pm10_calibB,
                              _calibData.pm1_calibB,
                              _calibData.pm25_calibB,
                              _calibData.o3_calibB );
	if (_calibDataString[0] == '\0')
	{
		log_e("Create string failed!");
		return ERROR_DATA_CREATE_STRINGDATA_FAILED;
	} else {    
        log_e("%s",_calibDataString);

		log_e("Create string complete!");
		return ERROR_NONE;
	}
}


ERROR_CODE createSensorDataString(char *_sensorDataString,
                                  const char *_nameDevice,
                                  const char *_dateTimeString,
                                  struct sensorData _sensorData_st)
{
    strcpy(_sensorDataString, "");
    sprintf(_sensorDataString, "%s,%s,%.1f,%.1f,%f,%f,%f,%f,%u,%u,",
                                _nameDevice,
                                _dateTimeString,
                                _sensorData_st.temperature,
                                _sensorData_st.humidity,
                                _sensorData_st.pressure_u32,
                                _sensorData_st.pm1,
                                _sensorData_st.pm25,
                                _sensorData_st.pm10,
                                _sensorData_st.co_2_u32,
                                _sensorData_st.o3_ppb);
    
	if (_sensorDataString[0] == '\0')
	{
		log_e("Create string failed!");
		return ERROR_DATA_CREATE_STRINGDATA_FAILED;
	} else {
		log_e("Create string complete!: %s",_sensorDataString);
		return ERROR_NONE;
	}
}

ERROR_CODE createMessageMQTTString( String &messageMQTTString,
                                    const char *_espID,
                                    NTPClient& _timeClient,
                                    struct sensorData _sensorData_st)
{
	_timeClient.update();

    StaticJsonDocument<200> doc;

    doc["station_id"] = _espID;
    doc["Time"] = _timeClient.getEpochTime();
    
    doc["Temperature"] = _sensorData_st.temperature;
    doc["Humidity"] = _sensorData_st.humidity;
    // doc["Pressure"] = _sensorData_st.pressure_u32;
    // doc["PM1"]  = _sensorData_st.pm1;
    // doc["PM2p5"] = _sensorData_st.pm25;
    // doc["PM10"] = _sensorData_st.pm10;
    // doc["CO2_PPM"] = _sensorData_st.co_2_u32;
    // doc["CO3_PPB"] = _sensorData_st.o3_ppb;

    String output;
    serializeJson(doc, output);
    messageMQTTString = output;
    return ERROR_NONE;
}