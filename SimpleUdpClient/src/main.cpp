#include <Arduino.h>
#include <string>

#include <WiFi.h>
#include <WiFiUdp.h>
#include <WiFiGeneric.h>

#include "driver/adc.h"
#include "esp_bt.h"
#include "esp_bt_main.h"

#include <MPU9250_asukiaaa.h>

#define SDA_M5 0
#define SCL_M5 26

#define LED 10

MPU9250_asukiaaa mySensor;
float aX, aY, aZ;

/* WiFi network name and password */
const char * ssid = "mateus";
const char * pwd = "abacates123";

// IP address to send UDP data to
const char * udpAddress = "10.42.0.1";
const int udpPort = 6666;

//create UDP instance
WiFiUDP udp;

void powerConfiguration();
void wifiConnection();

void setup(){
	powerConfiguration();

	Serial.begin(115200);

	pinMode(LED, OUTPUT);
	
	wifiConnection();
	
	Wire.begin(SDA_M5, SCL_M5);
	mySensor.setWire(&Wire);
	mySensor.beginAccel();
}

void loop(){
	mySensor.accelUpdate();
	aX = mySensor.accelX();
	aY = mySensor.accelY();
	aZ = mySensor.accelZ();

	//uint8_t buffer[50] = "hello world";
	String msg = String(aX) + "," + String(aY) + "," + String(aZ);

	udp.beginPacket(udpAddress, udpPort);
	udp.print(msg);
	udp.endPacket();
}

void powerConfiguration() {
	//setCpuFrequencyMhz(80);
	btStop();

	adc_power_off();
	esp_bluedroid_disable();
	esp_bt_controller_disable();
}

void wifiConnection() {
	//Connect to the WiFi network
	WiFi.begin(ssid, pwd);
	Serial.println("");

	// Wait for connection
	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}

	Serial.println("");
	Serial.print("Connected to ");
	Serial.println(ssid);
	Serial.print("IP address: ");
	Serial.println(WiFi.localIP());

	digitalWrite(LED, LOW);

	//This initializes udp and transfer buffer
	udp.begin(udpPort);
}