#include <Arduino.h>
#include <stepper.h>

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

    xTaskCreate(monitorStep, "MONITOR", 4096, NULL, 5, NULL);
    xTaskCreate(stepperHandler, "HANDLER", 4096, NULL, 5, NULL);

    delay(2000);
    Serial.println("Moving forward 100 steps");
    stepper.move(FORWARD, 100);

    delay(20000);
    Serial.println("Changing frequency to 20 Hz");
    stepper.setFrequency(20);
    Serial.println("Moving backward 200 steps");
    stepper.move(BACKWARD, 200);

    delay(3000);
    Serial.println("Stop now !");
    stepper.stop();
}

void loop()
{
    delay(100);
}