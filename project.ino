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

bool sysReady = false;
Preferences prefs;

#define LCD_CS   9
#define LCD_DC   10
#define LCD_SCLK 13
#define LCD_MOSI 12
#define LCD_RST  11
Adafruit_ST7735 tft = Adafruit_ST7735(LCD_CS, LCD_DC, LCD_RST);

#define BTN_RESET 20
bool sysResetx = false;

const int DHTPIN = 17;      
const int DHTTYPE = DHT11;  
DHT dht(DHTPIN, DHTTYPE);

const int LDR_AO = 15;
const int LDR_DO = 16;

WiFiManager wm;
bool wmShouldSaveConfig = false;
String wmSsid;
String wmPwd;

const uint8_t wmAttempts = 10;
const char* wmBroadcast = "CAY XANH LILY";

const char* wmsReset = "RESET button\nis pressed!";
const char* wmsPleaseConfig = "Connect to\n'CAY XANH LILY'\nto configure";
const char* wmsConnecting = "Connecting\nto MCP...";
const char* wmsConnectionFailed = "Connection failed,\ntry restart\nor reconfigure";
const char* wmsSaveRequest = "Receiving data\nfrom WiFiManager...";
const char* wmsSaveSuccess = "WiFi credentials\nare saved\nsuccessfully. Wait...";
const char* wmsReading = "Reading WiFi\nconfiguration...";
const char* mcpsSuccess = "MCP connection\nestablished";

String mcpToken = "";
WebSocketMCP mcpClient;
WebSocketsClient webSocket;

int ssLightAo;
bool ssLightDo; 
float ssHumidity;
float ssTemperature;

#pragma endregion Values

#pragma region WiFi

void wmSaveConfigCallback() {
  scrShowMessage(wmsSaveRequest);
  wmShouldSaveConfig = true;
}
void wmSaveCreds(String newSsid, String newPwd, String newToken) {
  prefs.putString("ssid", newSsid);
  prefs.putString("pwd", newPwd);
  mcpSetToken(newToken);
  scrShowMessage(wmsSaveSuccess);
}
void wmReadCreds() {
  scrShowMessage(wmsReading);
  wmSsid = prefs.getString("ssid", "");
  wmPwd = prefs.getString("pwd", "");
  
  delay(1000);
}
void wmConfig() {
  WiFiManagerParameter tokenField("token", "Xiaozhi MCP Token", "", 600);
  wm.addParameter(&tokenField);
  wm.startConfigPortal(wmBroadcast);
  scrShowMessage(wmsPleaseConfig);
  if (wmShouldSaveConfig) {
    wmSaveCreds(wm.getWiFiSSID(), wm.getWiFiPass(), tokenField.getValue());
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
  float value = ssTemperature;
  String response = "{\"temperature\":" + String(value) + "}";
  return WebSocketMCP::ToolResponse(response); });
  mcpClient.registerTool("get_humidity", "Đọc độ ẩm", "{}", [](const String& args) {
  float value = ssHumidity;
  String response = "{\"humidity\":" + String(value) + "}";
  return WebSocketMCP::ToolResponse(response); });
  mcpClient.registerTool("get_light", "Đọc ánh sáng theo cảm biến LDR từ 0 đến 4095", "{}", [](const String& args) {
  int value = ssLightAo;
  String response = "{\"light\":" + String(value) + "}";
  return WebSocketMCP::ToolResponse(response); });
  Serial.println("mcp Registered");
}

#pragma endregion Xiaozhi MCP

#pragma region Screen ST7735

void scrShowMessage(const char* msg) {
  tft.setCursor(0, 0);
  tft.fillScreen(ST77XX_WHITE);
  tft.setTextColor(ST77XX_BLACK);
  tft.setTextSize(1);

  tft.println(msg);
}

void scrShowStatus(int vLight, float vTemp, float vHumid, int vBat) {
  tft.setCursor(0, 0);
  tft.fillScreen(ST77XX_WHITE);
  tft.setTextColor(ST77XX_BLACK);
  tft.setTextSize(1);

  tft.print("Light: ");
  tft.println(vLight);
  tft.print("Temp: ");
  tft.println(vTemp);
  tft.print("Humidity: ");
  tft.println(vHumid);
  tft.print("Battery: ");
  tft.println(vBat);
}

void scrStartUp() {
  tft.setCursor(0, 0);
  tft.fillScreen(ST77XX_WHITE);
  tft.setTextColor(ST77XX_GREEN);
  tft.setTextSize(2);
  tft.print("LILY");
}

#pragma endregion Screen ST7735

#pragma region System

void sysInit() {
  sysReady = false;

  pinMode(BTN_RESET, INPUT_PULLUP);
  pinMode(DHTPIN, INPUT_PULLUP);
  pinMode(LDR_DO, INPUT);

  // Serial, Preferences, WiFiManager
  Serial.begin(115200);
  prefs.begin("config", false);
  dht.begin();
  wm.setSaveConfigCallback(wmSaveConfigCallback);

  // Khởi tạo màn hình (SPI và INITR)
  SPI.begin(LCD_SCLK, -1, LCD_MOSI, LCD_CS);
  delay(100);
  tft.initR(INITR_144GREENTAB);
  tft.setRotation(0);

  // Màn hình khởi động
  scrStartUp();
}

void sysReset() {
  scrShowMessage(wmsReset);
  wmConfig();
}

#pragma endregion System

void sendMCPRequest(String text) {
  StaticJsonDocument<256> doc;
  doc["type"] = "message";
  doc["text"] = text;
  String jsonStr;
  serializeJson(doc, jsonStr);

  webSocket.sendTXT(jsonStr);
  Serial.println("[MCP] Sent request: " + text);
}

void setup() {
  // Khởi tạo
  sysInit();  

  // Nút RESET, cấu hình lại từ đầu
  sysResetx = digitalRead(BTN_RESET);
  if (digitalRead(BTN_RESET) == LOW) {
    delay(1000);
    sysReset();
  }

  // Kết nối WiFi
  wmReadCreds();
  wm.autoConnect(wmSsid.c_str(), wmPwd.c_str());

  mcpGetToken();
  mcpClient.begin(mcpToken.c_str(), mcpOnConnectionStatus);
  scrShowMessage(mcpsSuccess);
  delay(1000);
}
bool mcptestsent = false;
void loop() {
  mcpClient.loop();
  if ()
  sendMCPRequest("Nhiệt độ phòng đang cao");
  ssLightAo = analogRead(LDR_AO);
  ssLightDo = digitalRead(LDR_DO);
  ssHumidity = dht.readHumidity();    
  ssTemperature = dht.readTemperature(); 

  if (isnan(ssHumidity) || isnan(ssTemperature)) {
    ssHumidity = -1;
    ssTemperature = -1;
  }

  scrShowStatus(ssLightAo, ssTemperature, ssHumidity, 92);
  delay(100);
}