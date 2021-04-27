#include <Arduino.h>
#include <ArduinoOTA.h>
#include <ThingsBoard.h>

#define THINGSBOARD_TOKEN "Bg1tq3HxzoqLMDOteyOd"
#define THINGSBOARD_SERVER "thingsboard.cloud"
#define THINGSBOARD_SEND_INTERVAL 5

#include "wifi_id.h"
#define HOSTNAME "DemoBlink" 
WiFiClient espClient;
ThingsBoard tb(espClient);

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.waitForConnectResult() != WL_CONNECTED)
  {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }
  Serial.printf("\nConnected with IP ");
  Serial.println(WiFi.localIP());
  ArduinoOTA.setHostname(HOSTNAME);
  ArduinoOTA.begin();

}
int nCount=0;
void loop() {
  ArduinoOTA.handle();
  yield();
  Serial.printf("%03d Hello World with OTA!\n", nCount++);
  digitalWrite(LED_BUILTIN, LOW);
  delay(100);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(900);
  if (!tb.connected()) {
  // Connect to the ThingsBoard
    Serial.print("Reconnecting to thingsboard server...");
    if (tb.connect(THINGSBOARD_SERVER, THINGSBOARD_TOKEN))
      Serial.println("connected.");
    else  
      Serial.println("Failed!");
  }
  // Serial.print("Hello World "); Serial.println(ntp.getFormattedDateTime());
  if (nCount % THINGSBOARD_SEND_INTERVAL == 0)
  {
      Serial.println("Sending data to thingsboard...");
      tb.sendTelemetryInt("Counter", nCount);
  }
}