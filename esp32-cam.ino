#include "src/OV2640.h"
#include <WiFi.h>
#include <WebServer.h>
#include <WiFiClient.h>

#include "camera_pins.h"
#include "home_wifi_multi.h"

OV2640 cam;
sensor_t * sens;

WebServer server(80);

const char JHEADER[] = "HTTP/1.1 200 OK\r\n" \
                       "Content-disposition: inline; filename=capture.jpg\r\n" \
                       "Content-type: image/jpeg\r\n\r\n";
const int jhdLen = strlen(JHEADER);

int last_request = 0; // 0 for jpg, 1 for highres-jpg

camera_config_t config;


void handle_jpg(void)
{
  if(last_request == 1){
    sens->set_framesize(sens, FRAMESIZE_VGA);
    last_request = 0;
    delay(500);
  }
  
  WiFiClient client = server.client();

  if (!client.connected()) return;
  cam.run();
  client.write(JHEADER, jhdLen);
  client.write((char *)cam.getfb(), cam.getSize());
}

void handle_highres_jpg(void)
{
  if(last_request == 0){
    sens->set_framesize(sens, FRAMESIZE_SXGA);
    last_request = 1;
    delay(500);
  }
  
  WiFiClient client = server.client();

  if (!client.connected()) return;
  cam.run();
  client.write(JHEADER, jhdLen);
  client.write((char *)cam.getfb(), cam.getSize());
}

void handleNotFound()
{
  String message = "Server is running!\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  server.send(200, "text / plain", message);
}

void setup()
{

  Serial.begin(115200);

  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  
  config.frame_size = FRAMESIZE_VGA;
  config.jpeg_quality = 5;
  config.fb_count = 1;
  
  cam.init(config);
  sens = esp_camera_sensor_get();
  
  IPAddress ip;

  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID1, PWD1);

  ip = WiFi.localIP();
  server.on("/jpg", HTTP_GET, handle_jpg);
  server.on("/highres-jpg", HTTP_GET, handle_highres_jpg);
  server.onNotFound(handleNotFound);
  server.begin();
}

void loop()
{
  server.handleClient();
}
