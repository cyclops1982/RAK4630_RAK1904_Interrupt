#include <Arduino.h>
#include <stdio.h>
#include <Wire.h>
#include "SparkFunLIS3DH.h"

LIS3DH Sensor(I2C_MODE, 0x18);

void lis3dh_read_data()
{
  // read the sensor value
  Serial.print(" X(g) = ");
  Serial.print(Sensor.readFloatAccelX(), 4);
  Serial.print(" Y(g) = ");
  Serial.print(Sensor.readFloatAccelY(), 4);
  Serial.print(" Z(g)= ");
  Serial.println(Sensor.readFloatAccelZ(), 4);
}

void describe_src_int(uint8_t src)
{
  if ((src & LIS3DHEnums::INT_SRC::IA) == LIS3DHEnums::INT_SRC::IA)
  {
    Serial.print("Interrupt active on: ");
  }
  if ((src & LIS3DHEnums::INT_SRC::ZH) == LIS3DHEnums::INT_SRC::ZH)
  {
    Serial.print("ZH ");
  }

  if ((src & LIS3DHEnums::INT_SRC::ZL) == LIS3DHEnums::INT_SRC::ZL)
  {
    Serial.print("ZL ");
  }

  if ((src & LIS3DHEnums::INT_SRC::YH) == LIS3DHEnums::INT_SRC::YH)
  {
    Serial.print("YH ");
  }

  if ((src & LIS3DHEnums::INT_SRC::YL) == LIS3DHEnums::INT_SRC::YL)
  {
    Serial.print("YL ");
  }

  if ((src & LIS3DHEnums::INT_SRC::XH) == LIS3DHEnums::INT_SRC::XH)
  {
    Serial.print("XH ");
  }

  if ((src & LIS3DHEnums::INT_SRC::XL) == LIS3DHEnums::INT_SRC::XL)
  {
    Serial.print("XL ");
  }

  Serial.println();
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

  Sensor.settings.tempEnabled = 0;
  Sensor.settings.adcEnabled = 0;
  if (Sensor.begin() != 0)
  {
    Serial.println("Problem starting the sensor at 0x18.");
  }
  else
  {
    Serial.println("Sensor at 0x18 started.");
  }

  // configurations for control registers
  
  Sensor.writeRegister(LIS3DH_CTRL_REG1, 0); // power it down.
  Sensor.writeRegister(LIS3DH_CTRL_REG0, LIS3DHEnums::CTRL_REG0::PullUpDisconnected);
  delay(1000);
  Sensor.writeRegister(LIS3DH_CTRL_REG1, (LIS3DHEnums::CTRL_REG1::ORD1 | LIS3DHEnums::CTRL_REG1::LPen | LIS3DHEnums::CTRL_REG1::XYZen));
  Sensor.writeRegister(LIS3DH_CTRL_REG2, (LIS3DHEnums::CTRL_REG2::HP_IA1 | LIS3DHEnums::CTRL_REG2::HP_IA2));
  Sensor.writeRegister(LIS3DH_CTRL_REG3, (LIS3DHEnums::CTRL_REG3::I1_IA1));
  Sensor.writeRegister(LIS3DH_CTRL_REG4, LIS3DHEnums::CTRL_REG4::FS_2G);
  Sensor.writeRegister(LIS3DH_CTRL_REG5, 0);
  Sensor.writeRegister(LIS3DH_CTRL_REG6, (LIS3DHEnums::CTRL_REG6::I2_IA2));
  Sensor.writeRegister(LIS3DH_REFERENCE, 0);

  Sensor.writeRegister(LIS3DH_INT1_THS, 0x04);      // Threshold (THS) = 0000 0001 = 1 LSBs * 15.625mg = 16mg
  Sensor.writeRegister(LIS3DH_INT1_DURATION, 0x01); // Duration = 1LSBs * (1/10Hz) = 0.1s.
  Sensor.writeRegister(LIS3DH_INT1_CFG, LIS3DHEnums::INT_CFG::YHIE);
  Sensor.writeRegister(LIS3DH_INT2_THS, 0x12);      // Threshold (THS) = 0001 0000 = 16 LSBs * 15.625mg/LSB = 250mg.
  Sensor.writeRegister(LIS3DH_INT2_DURATION, 0x01); // Duration = 1LSBs * (1/100Hz) = 0.1s.
  Sensor.writeRegister(LIS3DH_INT2_CFG, LIS3DHEnums::INT_CFG::YHIE);
  Sensor.writeRegister(LIS3DH_CTRL_REG5, (LIS3DHEnums::CTRL_REG5::LIR_INT1 | LIS3DHEnums::CTRL_REG5::LIR_INT2));

  uint8_t dummy = 0;
  Sensor.readRegister(&dummy, LIS3DH_REFERENCE);

  Serial.println("Wrote all the registers");
}

void loop()
{
  uint8_t data;
  lis3dh_read_data();
  delay(5000);

  Sensor.readRegister(&data, LIS3DH_INT1_SRC);
  if ((data & LIS3DHEnums::INT_SRC::IA) == LIS3DHEnums::INT_SRC::IA)
  {
    Serial.print("INT1_SRC :");
    describe_src_int(data);
  }

  Sensor.readRegister(&data, LIS3DH_INT2_SRC);
  if ((data & LIS3DHEnums::INT_SRC::IA) == LIS3DHEnums::INT_SRC::IA)
  {
    Serial.print("INT2_SRC :");
    describe_src_int(data);
  }
}