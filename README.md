# xlkhkt25-plant
Chậu cây thông minh - Project KHKT 2025

# Installation
Download và mở trong Arduino IDE, đảm bảo các file thư viện (cpp, c, h) chung thư mục với file .ino

# Diagram
```cpp
// |   PART   |  PIN  |  ESP  |
// |   DISP   |  GND  |  GND  |
// |          |  VCC  |  3V3  |
// |          |  SCL  |  D15  |
// |          |  SDA  |  D02  |
// |   AMP    |  GND  |  GND  |
// |          |  VDD  |  3V3  |
// |          | BCLK1 |  D04  | (from corner of M98357A)
// |          | BCLK2 |  D16  |
// |          | BCLK3 |  D17  |
// |          | GAIN0 |  GND  |
// |          | GAIN1 |  3V3  |
// |          |  GND  |  GND  |
// |          |  VIN  |  3V3  |
```
