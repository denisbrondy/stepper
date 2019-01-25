#define STEPPER_DEBUG
#include <Arduino.h>
#include <WiFi.h>
#include "stepper.h"

#include "soc/timer_group_struct.h"
#include "soc/timer_group_reg.h"

#define WIFI_SSID "WIFI_599"
#define WIFI_PSK "denisetgozlane"

Stepper stepper(GPIO_NUM_0, GPIO_NUM_15, GPIO_NUM_2, 10);

void monitorStep(void *parameters)
{
  while (true)
  {
    Serial.print("Stepper position is : ");
    Serial.println(stepper.getPosition());
    vTaskDelay(pdMS_TO_TICKS(250));
  }
  vTaskDelete(NULL);
}

void stepperHandler(void *parameters)
{
  while (true)
  {
    stepper.handle();
    taskYIELD();
  }
  vTaskDelete(NULL);
}

void setup()
{
  Serial.begin(115200);
  TIMERG0.wdt_wprotect=TIMG_WDT_WKEY_VALUE;
TIMERG0.wdt_feed=1;
TIMERG0.wdt_wprotect=0;
  // Connect to WiFi
  // Serial.println("Setting up WiFi\n");
  // WiFi.begin(WIFI_SSID, WIFI_PSK);
  // while (WiFi.status() != WL_CONNECTED)
  // {
  //   Serial.print(".");
  //   delay(500);
  // }
  // Serial.println("");
  // Serial.println("Connected");

  xTaskCreate(monitorStep, "MONITOR", 4096, NULL, 5, NULL);
  xTaskCreate(stepperHandler, "HANDLER", 4096, NULL, 5, NULL);

  delay(2000);
  Serial.println("Moving forward 100 steps");
  stepper.move(FORWARD, 100);
  Serial.println("WAITS");
  long now = micros();
  delay(20000);
  Serial.println((micros()-now));
  stepper.setFrequency(20);
  Serial.println("Moving backward 100 steps");
  stepper.move(BACKWARD, 100);

  delay(3000);
  Serial.println("Stop !");
  stepper.stop();
}

void loop()
{
  //stepper.handle();
  //delay(100);
  //Serial.println(stepper.getPosition());
}