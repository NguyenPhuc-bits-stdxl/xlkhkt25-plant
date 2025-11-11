// 
// === DIAGRAM ===
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
     
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>

// --- Map lại đúng chân bạn đang dùng ---
#define TFT_CS   16
#define TFT_DC   4
#define TFT_RST  5
#define TFT_SCLK 15
#define TFT_MOSI 2

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

void setup() {
  Serial.begin(115200);
  Serial.println("Khoi dong ST7735S...");

  // Quan trọng: bắt buộc khai báo lại SPI pins cho ESP32-S2
  SPI.begin(TFT_SCLK, -1, TFT_MOSI, TFT_CS);

  // Nếu WDT reset xảy ra sớm, thêm delay trước init
  delay(100);

  // Khởi tạo màn hình 1.44" ST7735S
  tft.initR(INITR_144GREENTAB);
  tft.setRotation(1);
  tft.fillScreen(ST77XX_BLACK);

  // Hiển thị thử
  tft.setTextColor(ST77XX_YELLOW);
  tft.setTextSize(2);
  tft.setCursor(10, 30);
  tft.println("ESP32");
}

void loop() {
  // Chớp chữ
  tft.setTextColor(ST77XX_RED);
  tft.setCursor(20, 90);
  tft.println("Xin chao!");
  delay(500);

  tft.fillRect(20, 90, 100, 20, ST77XX_BLACK);
  delay(500);
}