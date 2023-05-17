#include <WiFi.h>
#include <time.h>

const char *ssid = "云镝智网";         // The SSID (name) of the Wi-Fi network you want to connect to
const char *password = "yundizhiwang"; // The password of the Wi-Fi network

void setup()
{

  Serial.begin(9600);

  WiFi.begin(ssid, password); // Connect to the network
  Serial.print("Connecting to ");
  Serial.print(ssid);

  while (WiFi.status() != WL_CONNECTED)
  { // Wait for the Wi-Fi to connect
    delay(500);
    Serial.print('.');
  }

  configTime(8 * 3600, 0, "ntp1.aliyun.com"); // 配置时间服务器

  while (!time(nullptr))
  {
    Serial.println("Waiting for time synchronization...");
    delay(1000);
  }
}

void loop()
{
  time_t now = time(nullptr);
  struct tm timeinfo;

  localtime_r(&now, &timeinfo);

  char timeString[20];
  strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:%M:%S", &timeinfo);

  Serial.println(timeString);

  delay(1000);
}