#include <WiFi.h>
#include <time.h>
#include <string>
#include <list>
#include <Wire.h>
#include <U8g2lib.h>

// wifi信息结构体
struct WifiCredentials
{
  std::string ssid;
  std::string password;
};

std::string IP;
// 已知wifi的ssid和password
const std::list<WifiCredentials> myWifi = {
    {"esp", "asdfghjkl"},
    {"云镝智网", "yundizhiwang"},
    {"ZXW", "12345678"}};

U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE, /* clock=*/22, /* data=*/21);

// 函数申明
void syncTime();

std::string getTime();

char *strToChar(std::string str);

void scanAndConnectWifi();

void connectWifi(const char *ssid, const char *password);

void displayText(const std::string &text);

/**
 * @brief 初始化
 *
 */
void setup()
{
  // Serial.begin(9600);
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
  pinMode(15, OUTPUT);
  digitalWrite(15, HIGH);

  scanAndConnectWifi();

  syncTime();

  u8g2.begin();
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);
}

/**
 * @brief 主循环
 *
 */
void loop()
{
  // 输出事件到串口
  // Serial.println(strToChar(getTime()));
  displayText(getTime());
  delay(1000);
}

/**
 * @brief 扫描wifi并连接
 *
 */
void scanAndConnectWifi()
{
  WiFi.mode(WIFI_STA);
  displayText("Scanning WiFi...");
  int numNetworks = WiFi.scanNetworks();
  displayText("Found:" + std::to_string(numNetworks) + " networks");

  for (int i = 0; i < numNetworks; i++)
  {
    String ssid = WiFi.SSID(i);
    // 转化为string
    std::string ssidStr = ssid.c_str();
    displayText("Found network: " + ssidStr);

    for (const WifiCredentials &wifi : myWifi)
    {
      if (std::string(ssid.c_str()) == wifi.ssid)
      {
        displayText("Match found! Connecting...");
        connectWifi(wifi.ssid.c_str(), wifi.password.c_str());
        return;
      }
    }
  }

  displayText("No matching WiFi found");
}

/**
 * @brief 连接wifi
 *
 * @param ssid
 * @param password
 */
void connectWifi(const char *ssid, const char *password)
{
  WiFi.begin(ssid, password); // Connect to the network
  std::string ssidStr = ssid.c_str();
  displayText("Connecting to " + ssidStr + "...");

  while (WiFi.status() != WL_CONNECTED)
  { // Wait for the Wi-Fi to connect
    delay(500);
    Serial.print('.');
  }

  displayText("WiFi connected.");
  displayText("IP address: ");
  Serial.println(WiFi.localIP());
  IP = WiFi.localIP().toString().c_str();
}

/**
 * @brief 同步时间
 *
 */
void syncTime()
{
  configTime(8 * 3600, 0, "ntp1.aliyun.com"); // 配置时间服务器

  while (!time(nullptr))
  {
    Serial.println("Waiting for time synchronization...");
    delay(1000);
  }
}

/**
 * @brief 获取事件字符串
 *
 * @return std::string
 */
std::string getTime()
{
  time_t now = time(nullptr);
  struct tm timeinfo;

  localtime_r(&now, &timeinfo);

  char timeString[20];
  // strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:%M:%S", &timeinfo);
  strftime(timeString, sizeof(timeString), "%H:%M:%S", &timeinfo);
  return std::string(timeString);
}

/**
 * @brief string 转 char*
 *
 * @param str
 * @return char*
 */
char *strToChar(std::string str)
{
  char *cstr = new char[str.length() + 1];
  strcpy(cstr, str.c_str());
  return cstr;
}

/**
 * @brief 显示字符串
 *
 * @param text
 */
void displayText(const std::string &text)
{
  u8g2.clearBuffer();

  int maxWidth = u8g2.getDisplayWidth();
  int lineHeight = u8g2.getFontAscent() - u8g2.getFontDescent();
  int y = lineHeight;

  u8g2.setFontMode(1);  // 选项 1: 忽略填充像素
  u8g2.setDrawColor(1); // 选项 1: 白色文字
  u8g2.setCursor(0, y);

  u8g2.println(text.c_str());

  u8g2.sendBuffer();
}
