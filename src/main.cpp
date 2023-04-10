#include "main.h"


void device_getData()
{
  struct sensorData sensorDataTemp_st;

  if(sht_getdata(sensorDataTemp_st.temperature, sensorDataTemp_st.humidity) == ERROR_NONE)
    {
    sensorData_st.temperature   = sensorDataTemp_st.temperature;
    sensorData_st.humidity      = sensorDataTemp_st.humidity;
  }
  if(dfozone_getdata(sensorDataTemp_st.o3_ppb) == ERROR_NONE)
  {
    sensorData_st.o3_ppb  = sensorDataTemp_st.o3_ppb;
  }
  delay(1000);
}

void device_dataManagement()
{
  struct sensorData sensorDataTemp_st;
  sensorDataTemp_st = sensorData_st;

  DS3231_getStringDateTime(realTime, DateTime::TIMESTAMP_FULL , dateTime_string);	
  createSensorDataString(sensorDataString, NAME_DEVICE, dateTime_string ,sensorDataTemp_st);
	DS3231_getStringDateTime(realTime, DateTime::TIMESTAMP_DATE, nameFileSaveData);
	SDcard_saveStringDataToFile(&connectionStatus_st, sensorDataString);
  createMessageMQTTString(messageData, (const char *)espID, timeClient, sensorDataTemp_st);
  MQTT_postData(messageData.c_str(), &connectionStatus_st, mqttClient);
  Screen_displaysensorData(&sensorDataTemp_st, &calibData_st);
}

void device_updateCalibrateFromScreen()
{
    calibData calibDataTemp_st;

    Screen_getCalibDataFromUser(&calibDataTemp_st);
    DS3231_getStringDateTime(realTime, DateTime::TIMESTAMP_FULL , dateTime_string);	
		creatCalibDataString(dataCalib_string, calibDataTemp_st);
		SDcard_saveStringDataToFile(&connectionStatus_st, (const char*)dataCalib_string);
}

void setup() {
    Serial.begin(SERIAL_DEBUG_BAUDRATE);
    log_e("Booting...");
    WIFI_init();
    Wire.begin(PIN_SDA_GPIO, PIN_SCL_GPIO, I2C_CLOCK_SPEED);
	  DS3231_init(realTime, timeClient, Wire, connectionStatus_st);
    sht_init();
    dfozone_init();
#ifdef USING_MQTT
	MQTT_initClient(topic, espID, mqttClient, &connectionStatus_st);
	timeClient.begin();
#endif
#ifdef USING_SDCARD
	SDcard_init(PIN_NUM_CLK, PIN_NUM_MISO, PIN_NUM_MOSI, PIN_CS_SD_CARD, &connectionStatus_st);
#endif
  Screen_init(Serial2, 9600, 512);
    log_e("Init Done");
}

unsigned long last_send = 0;
void loop() {
  // put your main code here, to run repeatedly:
  device_getData();
  if(millis() - last_send > 5000) 
  {
    device_dataManagement();
    device_updateCalibrateFromScreen();
    last_send = millis();
  }
}