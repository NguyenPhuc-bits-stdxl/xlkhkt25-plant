#pragma region Libraries

#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>
#include <WiFiManager.h>
#include <Preferences.h>
#include <WebSocketMCP.h>
#include <DHT.h>

#pragma endregion Libraries

#pragma region Values

Preferences prefs;

#define LCD_CS   39
#define LCD_DC   38
#define LCD_SCLK 35
#define LCD_MOSI 36
#define LCD_RST  37
Adafruit_ST7735 tft = Adafruit_ST7735(LCD_CS, LCD_DC, LCD_RST);

#define BTN_RESET 47
bool sysResetx = false;

const int DHTPIN = 40;      
const int DHTTYPE = DHT11;  
DHT dht(DHTPIN, DHTTYPE);

const int LDR_AO = 42;
const int LDR_DO = 41;

WiFiManager wm;
bool wmShouldSaveConfig = false;
String wmSsid;
String wmPwd;

String mcpToken = "";
WebSocketMCP mcpClient;

int ssLight = 0;
float ssHumidity;
float ssTemperature;

#pragma endregion Values

#pragma region WiFi

void wmSaveConfigCallback() {
  Serial.println("wm Creds should be saved");
  wmShouldSaveConfig = true;
}
void wmSaveCreds(String newSsid, String newPwd, String newToken) {
  prefs.putString("ssid", newSsid);
  prefs.putString("pwd", newPwd);
  mcpSetToken(newToken);
  Serial.println("wm Creds are saved");  
}
void wmReadCreds() {
  Serial.println("wm Reading creds");
  wmSsid = prefs.getString("ssid", "");
  wmPwd = prefs.getString("pwd", "");
  
  delay(1000);
}
void wmConfig() {
  // clear_screenw();
  // tft.setCursor(8, 8);
  // tft.setTextColor(ST77XX_BLACK);
  // tft.setTextSize(1);
  // tft.println("Connect to ESPLANT and go to 192.168.4.1 to configure.");

  WiFiManagerParameter tokenField("token", "Xiaozhi MCP Token", "", 600);
  wm.addParameter(&tokenField);
  wm.startConfigPortal("ESPLANT");
  if (wmShouldSaveConfig) {
    wmSaveCreds(wm.getWiFiSSID(), wm.getWiFiPass(), tokenField.getValue());
    Serial.println("wm Creds are saved");
    ESP.restart();
  }
}
void wmConnect() {
  Serial.println("wm Reading credentials and connecting");
  wmReadCreds();
  bool res = wm.autoConnect(wmSsid.c_str(), wmPwd.c_str());
  if (!res) {
    Serial.println("wm Connection failed, rebooting");
    delay(2000);
    ESP.restart();
  }
}

#pragma endregion WiFi

#pragma region Xiaozhi MCP

void mcpGetToken() {
  mcpToken = prefs.getString("mcp_token", "");
  Serial.println("mcp Token is " + mcpToken);
}
void mcpSetToken(String newToken) {
  prefs.putString("mcp_token", newToken);
  mcpToken = newToken;
  Serial.println("mcp Token changed " + mcpToken);
}
void mcpOnConnectionStatus(bool connected) {
  if (connected) {
    Serial.println("mcp Success");
    mcpRegister();
  } else {
    Serial.println("mcp Failed");
  }
}
void mcpRegister() {
  mcpClient.registerTool("get_temp", "Đọc nhiệt độ", "{}", [](const String& args) {
  float value = 32.0;
  String response = "{\"temperature\":" + String(value) + "}";
  return WebSocketMCP::ToolResponse(response); });
  mcpClient.registerTool("get_humidity", "Đọc độ ẩm", "{}", [](const String& args) {
  float value = 40.0;
  String response = "{\"humidity\":" + String(value) + "}";
  return WebSocketMCP::ToolResponse(response); });
  mcpClient.registerTool("get_light", "Đọc ánh sáng theo đơn vị cd/m2", "{}", [](const String& args) {
  float value = 80.0;
  String response = "{\"light\":" + String(value) + "}";
  return WebSocketMCP::ToolResponse(response); });
  Serial.println("mcp Registered");
}

#pragma endregion Xiaozhi MCP

#pragma region DHT sensor
#pragma endregion DHT sensor

#pragma region LDR sensor
#pragma endregion LDR sensor

void clear_screenw() { }//tft.fillScreen(ST77XX_WHITE); }
void clear_screenb() { }//tft.fillScreen(ST77XX_BLACK); }

#pragma region System

void sysInit() {
  pinMode(47, INPUT);
  pinMode(DHTPIN, INPUT_PULLUP);
  pinMode(LDR_DO, INPUT);

  // Serial, Preferences, WiFiManager
  Serial.begin(115200);
  prefs.begin("config", false);
  dht.begin();
  wm.setSaveConfigCallback(wmSaveConfigCallback);

  // // Khởi tạo màn hình (SPI và INITR)
  // SPI.begin(LCD_SCLK, -1, LCD_MOSI, LCD_CS);
  // delay(100);
  // tft.initR(INITR_144GREENTAB);
  // tft.setRotation(0);

  // // Màn hình khởi động
  // clear_screenb();
  // tft.setTextColor(ST77XX_GREEN);
  // tft.setTextSize(2);
  // tft.setCursor(24, 48);
  // tft.println("WELCOME");
  // delay(2000);
}

void sysReset() {
  // clear_screenw();
  // tft.setCursor(8, 48);
  // tft.setTextColor(ST77XX_BLACK);
  // tft.setTextSize(1);
  // tft.println("Reset pressed!");
  delay(2000);
  wmConfig();
}

#pragma endregion System

void setup() {
  // Khởi tạo
  sysInit();  

  // Nút RESET, cấu hình lại từ đầu
  sysResetx = digitalRead(BTN_RESET);
  if (sysResetx) {
    delay(2000);
    sysReset();
  }

  wmConnect();
  mcpGetToken();
  mcpClient.begin(mcpToken.c_str(), mcpOnConnectionStatus);
}

void loop() {
  mcpClient.loop();
  delay(50);

   // ---- Đọc ánh sáng ----
  int lightValue = analogRead(LDR_AO);
  Serial.print("Anh sang: ");
  Serial.println(lightValue);
  Serial.print("AS DO:");
  Serial.println(digitalRead(LDR_DO));

  // ---- Đọc DHT ----
  float h = dht.readHumidity();    
  float t = dht.readTemperature(); 

  if (isnan(h) || isnan(t)) {
    Serial.println("Loi cam bien DHT!");
  } else {
    Serial.print("Nhiet do: ");
    Serial.println(t);

    Serial.print("Do am: ");
    Serial.println(h);
  }

  ssLight = lightValue;
  ssHumidity = h;
  ssTemperature = t;
}