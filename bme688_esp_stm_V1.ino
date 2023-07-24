#include "Arduino.h"
#include "bme68xLibrary.h"
#include "Wire.h"

Bme68x bme;

/**
 * @brief Initializes the sensor and hardware settings
 */
void setup(void)
{
    Wire.begin();
    Serial.begin(115200);

    while (!Serial)
        delay(10);

    /* initializes the sensor based on I2C library */
    bme.begin(0x77, Wire);

    if (bme.checkStatus())
    {
        if (bme.checkStatus() == BME68X_ERROR)
        {
            Serial.println("Sensor error:" + bme.statusString());
            return;
        }
        else if (bme.checkStatus() == BME68X_WARNING)
        {
            Serial.println("Sensor Warning:" + bme.statusString());
        }
    }

    /* Set the default configuration for temperature, pressure, and humidity */
    bme.setTPH();

    /* Set the heater configuration to 300 deg C for 100ms for Forced mode */
    bme.setHeaterProf(300, 100);

    Serial.println("TimeStamp(ms), Temperature(deg C), Pressure(Pa), Humidity(%), Gas resistance(ohm), Status");
}

void loop(void)
{
    bme68xData data;

    bme.setOpMode(BME68X_FORCED_MODE);
    delayMicroseconds(bme.getMeasDur());

    if (bme.fetchData())
    {
        bme.getData(data);

        // Send data over UART
        Serial.print("T:");
        Serial.print(data.temperature);
        Serial.print(",H:");
        Serial.print(data.humidity);
        Serial.print(",P:");
        Serial.print(data.pressure);
        Serial.print(",G:");
        Serial.print(data.gas_resistance);
        Serial.println();

        // Add some delay to prevent flooding the UART buffer
        delay(1000);
    }
}
