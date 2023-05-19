#include <WiFi.h>
#include <time.h>
#include <string>

// const char *ssid = "esp";           // The SSID (name) of the Wi-Fi network you want to connect to
// const char *password = "asdfghjkl"; // The password of the Wi-Fi network
std::string IP;

// 函数申明
void syncTime();

std::string getTime();

char *strToChar(std::string str);

void connectWifi(char *ssid, char *password);

void setup()
{
  Serial.begin(9600);
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
  pinMode(15, OUTPUT);
  digitalWrite(15, HIGH);

  connectWifi("esp", "asdfghjkl");

  syncTime();
}

void loop()
{
  // 输出事件到串口
  Serial.println(strToChar(getTime()));

  delay(1000);
}

/**
 * @brief 连接wifi
 *
 * @param ssid
 * @param password
 */
void connectWifi(char *ssid, char *password)
{
  WiFi.begin(ssid, password); // Connect to the network
  Serial.print("Connecting to ");
  Serial.print(ssid);

  while (WiFi.status() != WL_CONNECTED)
  { // Wait for the Wi-Fi to connect
    delay(500);
    Serial.print('.');
  }

  Serial.println();
  Serial.println("WiFi connected.");
  Serial.print("IP address: ");
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
  strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:%M:%S", &timeinfo);
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