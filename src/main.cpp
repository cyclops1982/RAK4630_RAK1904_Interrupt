#include <Arduino.h>
#include <stdio.h>
#include <Wire.h>
#include "SparkFunLIS3DH.h"

LIS3DH Sensor(I2C_MODE, 0x18);

volatile bool g_moved = false;

void lis3dh_read_data()
{
  // read the sensor value
  Serial.print(" X(g) = ");
  Serial.println(Sensor.readFloatAccelX(), 4);
  Serial.print(" Y(g) = ");
  Serial.println(Sensor.readFloatAccelY(), 4);
  Serial.print(" Z(g)= ");
  Serial.println(Sensor.readFloatAccelZ(), 4);
}

void pin_high()
{
  g_moved = true;
}

void setup()
{
  delay(3000);
  time_t timeout = millis();
  Serial.begin(115200);
  delay(3000);
  Serial.println("Starting");
  while (!Serial)
  {
    if ((millis() - timeout) < 5000)
    {
      delay(100);
    }
    else
    {
      break;
    }
  }

  Wire.begin();
  delay(1000);

  if (Sensor.begin() != 0)
  {
    Serial.println("Problem starting the sensor at 0x18.");
  }
  else
  {
    Serial.println("Sensor at 0x18 started.");
  }

  // // Set low power mode
  // uint8_t data_to_write = 0;
  // Sensor.readRegister(&data_to_write, LIS3DH_CTRL_REG1);
  // data_to_write |= 0x08;
  // Sensor.writeRegister(LIS3DH_CTRL_REG1, data_to_write);
  // delay(100);

  // data_to_write = 0;
  // Sensor.readRegister(&data_to_write, 0x1E);
  // data_to_write |= 0x90;
  // Sensor.writeRegister(0x1E, data_to_write);
  // delay(100);

  // configurations for control registers
  Sensor.writeRegister(0x20, 0xA7); // Write A7h into CTRL_REG1;      // Turn on the sensor, enable X, Y, Z axes with ODR = 100Hz normal mode.
  Sensor.writeRegister(0x21, 0x09); // Write 09h into CTRL_REG2;      // High-pass filter (HPF) enabled
  Sensor.writeRegister(0x22, 0x40); // Write 40h into CTRL_REG3;      // ACC AOI1 interrupt signal is routed to INT1 pin.
  Sensor.writeRegister(0x23, 0x00); // Write 00h into CTRL_REG4;      // Full Scale = +/-2 g
  Sensor.writeRegister(0x24, 0x08); // Write 08h into CTRL_REG5;      // Default value is 00 for no latching. Interrupt signals on INT1 pin is not latched.
                                    // Users don’t need to read the INT1_SRC register to clear the interrupt signal.
  // configurations for wakeup and motionless detection
  Sensor.writeRegister(0x32, 0x10); // Write 10h into INT1_THS;          // Threshold (THS) = 16LSBs * 15.625mg/LSB = 250mg.
  Sensor.writeRegister(0x33, 0x00); // Write 00h into INT1_DURATION;     // Duration = 1LSBs * (1/10Hz) = 0.1s.
  // readRegister();  //Dummy read to force the HP filter to set reference acceleration/tilt value
  Sensor.writeRegister(0x30, 0x2A); // Write 2Ah into INT1_CFG;          // Enable XLIE, YLIE, ZLIE interrupt generation, OR logic.

  Serial.println("Wrote all the registers");

  pinMode(WB_IO5, INPUT);
  attachInterrupt(WB_IO5, pin_high, RISING);
  Serial.println("Interrupt is setup");
}

void loop()
{
  Serial.println("======");
  uint8_t data;
  Sensor.readRegister(&data, LIS3DH_INT1_SRC);
  Serial.printf("INT SOURC : %d\r\n", data);
  lis3dh_read_data();
  delay(10000);
  if (g_moved == true)
  {
    Serial.println("We moved a bit!");
    g_moved = false;
  }
}