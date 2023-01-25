# RAK4630 with RAK1904 Example project

This is a simple example project that demonstrates the usage of teh RAK1904 accelerometer sensor. The aim is to use interrupts and optimize the battery usage.

## RAK1904
The RAK1904 module has the LIS3DH accelerometer on it. 
Info page: https://www.st.com/en/mems-and-sensors/lis3dh.html
DataSheet: https://www.st.com/resource/en/datasheet/lis3dh.pdf

Info on the INT1_THS/INT2_THS configuration: https://community.st.com/s/question/0D50X00009fFFXzSAO/how-is-the-int1ths-scaled-on-the-lis3dh-sensor (in short, it's whatever FS is configured times the value of that register)
