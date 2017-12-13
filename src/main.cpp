#include <Arduino.h>
//#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <U8g2lib.h>
#include <ArduinoOTA.h>


U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0);
//U8G2_ST7920_128X64_1_SW_SP u8g2(U8G2_R0);
//U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0);
  // Copy the contents of your .xbm file below
#define nowifi_width 32
#define nowifi_height 32
// Update these with values suitable for your network.
const char* ssid = "Levys";
const char* password = "dn760912";
const char* mqtt_server = "192.168.1.110";
//StaticJsonBuffer<200> jsonBuffer;
char     cstr[16];
int      Lboxheight;
int      Hboxheight;
int powerstate = 0;
bool tempdatabool = 0;
bool pwrstatebool= 0;
bool   nodata = 0;
int boxpos;
int idx =  0;
int nvalue = 0;
int svalue = 0;
static unsigned char nowifi_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x1e, 0x00,
   0x00, 0x00, 0x3e, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x78, 0x00,
   0x00, 0x20, 0x78, 0x00, 0x00, 0x30, 0xf0, 0x00, 0x00, 0x78, 0xf0, 0x00,
   0x00, 0xf8, 0xe0, 0x00, 0x00, 0xf0, 0xe0, 0x01, 0x00, 0x20, 0x00, 0x00,
   0x20, 0x00, 0x80, 0x1e, 0xf8, 0xc8, 0xff, 0x1f, 0xf8, 0xfc, 0xff, 0x3f,
   0xfc, 0xfc, 0xff, 0x1f, 0xfc, 0xfc, 0xff, 0x3f, 0xf8, 0xa0, 0xff, 0x1f,
   0x20, 0x00, 0x00, 0x1c, 0x00, 0xa0, 0x00, 0x00, 0x00, 0xe0, 0xe0, 0x01,
   0x00, 0xf0, 0xe0, 0x01, 0x00, 0x78, 0xe0, 0x00, 0x00, 0x78, 0xf0, 0x00,
   0x00, 0x30, 0xf0, 0x00, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x78, 0x00,
   0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x3e, 0x00, 0x00, 0x00, 0x1e, 0x00,
   0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00 };

#define smily_width 32
#define smily_height 32
static unsigned char smily_bits[] = {
   0x00, 0xf0, 0x0f, 0x00, 0x00, 0xfe, 0x7f, 0x00, 0x80, 0xff, 0xff, 0x01,
   0xc0, 0x1f, 0xf8, 0x03, 0xe0, 0x03, 0xc0, 0x07, 0xf0, 0x00, 0x1e, 0x0f,
   0x78, 0x80, 0x3f, 0x1e, 0x3c, 0x8c, 0x3f, 0x3c, 0x1c, 0xcc, 0x3f, 0x38,
   0x1e, 0xc6, 0x3f, 0x78, 0x0e, 0xc7, 0x3f, 0x70, 0x8e, 0xc3, 0x3f, 0x70,
   0x8f, 0xc3, 0x3f, 0xf0, 0xc7, 0x81, 0x3f, 0xe0, 0xc7, 0x81, 0x3f, 0xe0,
   0xc7, 0x00, 0x3e, 0xe0, 0xc7, 0x00, 0x3e, 0xe0, 0xc7, 0x81, 0x3f, 0xe0,
   0xc7, 0x81, 0x3f, 0xe0, 0x8f, 0xc3, 0x3f, 0xf0, 0x8e, 0xc3, 0x3f, 0x70,
   0x0e, 0xc7, 0x3f, 0x70, 0x1e, 0xce, 0x3f, 0x78, 0x1c, 0xcc, 0x3f, 0x38,
   0x3c, 0x80, 0x3f, 0x3c, 0x78, 0x80, 0x3f, 0x1e, 0xf0, 0x00, 0x1e, 0x0f,
   0xe0, 0x03, 0xc0, 0x07, 0xc0, 0x1f, 0xf8, 0x03, 0x80, 0xff, 0xff, 0x01,
   0x00, 0xfe, 0x7f, 0x00, 0x00, 0xf0, 0x0f, 0x00 };
WiFiClient espClient;
PubSubClient client(espClient);

long lastMsg = 0;
char msg[50];
int value = 0;
void setup_wifi() {

  u8g2.clearBuffer();
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  Serial.println("OTA_Test");
  WiFi.mode(WIFI_STA);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");

        u8g2.drawXBM( 50, 0, nowifi_width, nowifi_height, nowifi_bits);
        u8g2.sendBuffer();
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  nodata = 1;
      u8g2.drawXBM( 50, 0, smily_width, smily_height, smily_bits);
      Serial.println("Bool State after wifi connection");
      Serial.println("My Power StateBool is: " && pwrstatebool);
      Serial.println("My Temp StateBool is: " && tempdatabool);
      Serial.println("My NoData StateBool is: " && nodata);
      u8g2.sendBuffer();

  //ota block
   // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
   ArduinoOTA.setHostname("BoilerOLED");

  // No authentication by default
  // ArduinoOTA.setPassword((const char *)"123");

  ArduinoOTA.onStart([]() {
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());


}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("]");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(payload);
  if (!root.success())
  {
    Serial.println("parseObject() failed \n");
    return;
  }
/*char* nvalstr;
char* svalstr;
itoa(root["nvalue"],nvalstr, 10);
itoa(root["svalue"],svalstr, 10);
Serial.println(nvalstr);
Serial.println(svalstr);
*/
  idx =  root["idx"];
//if (root["nvalue"] !=(char*)0) {
//     nvalue = root["nvalue"];
//}
//if (root["svalue"] !=(char*)0) {
//svalue = root["svalue"];
//}

  if (idx == 119 || idx == 94) { //94 is outdoor boiler, and 119 is temp
    //my idx..
    Serial.println("found y HW.. dtermining which value i shoud be posting");
    //Serial.print(topic);
    if (idx == 94) {
      pwrstatebool = 1;
      nvalue = root["nvalue"];
      powerstate = nvalue;
      u8g2.clearBuffer();
    }
// lower temp sensor
    else if (idx == 119 && (strcmp(topic, "domoticz/in") == 0)) {
      //need to calculate the size of the box
      svalue = root["svalue"];
      tempdatabool = 1;
      //ppreviously font u8g2_font_logisoso28_tf for horizontal
      char temp[16];
      itoa(svalue, cstr, 10);
      strcat(cstr,"°");

      Serial.println("Bool State after 119/94 verification");
      Serial.println("My Power StateBool is: " && pwrstatebool);
      Serial.println("My Temp StateBool is: " && tempdatabool);
      Serial.println("My NoData StateBool is: " && nodata);
      if (svalue >= 50) {
          Lboxheight = 4;
      }
      else if (svalue < 50 && svalue >= 40){
        Lboxheight = 3;
      }
      else if (svalue < 40 && svalue >= 30){
        Lboxheight = 2;
      }
      else if (svalue < 30 && svalue >= 25){
        Lboxheight = 1;
      }
      else if (svalue < 25){
        Lboxheight = 0;
      }
      else {
        Lboxheight = 0;
      }

      boxpos = 65 - Lboxheight;

      u8g2.clearBuffer();


      }
      u8g2.clearBuffer();
      if (pwrstatebool == 1 && tempdatabool == 0) {
              Serial.println("Bool before drawing to screen");
      Serial.println("My Power StateBool is: " && pwrstatebool);
      Serial.println("My Temp StateBool is: " && tempdatabool);
      Serial.println("My NoData StateBool is: " && nodata);
        if (powerstate == 1){
          Serial.println("intodevice powerstate 1-0 1");
          //draw power 1
          u8g2.drawDisc(116, 16, 9, U8G2_DRAW_ALL);
        }
        else {
          //draw power 0
          u8g2.drawCircle(116, 16, 9, U8G2_DRAW_ALL);
        }
          //draw smily
          u8g2.drawXBM( 50, 0, smily_width, smily_height, smily_bits);
          u8g2.sendBuffer();
      }
      else if (pwrstatebool == 1 && tempdatabool == 1) {
      Serial.println("Bool before drawing to screen, PWR AND TMP 1");
      Serial.println("My Power StateBool is: " && pwrstatebool);
      Serial.println("My Temp StateBool is: " && tempdatabool);
      Serial.println("My NoData StateBool is: " && nodata);
        if (powerstate == 1){
          Serial.println("intodevice powerstate 1-1 1");
          //draw power 1
          u8g2.drawDisc(116, 16, 9, U8G2_DRAW_ALL);
        }
        else {
          //draw power 0
          u8g2.drawCircle(116, 16, 9, U8G2_DRAW_ALL);
        }
          Serial.println(cstr);
          u8g2.setFont(u8g2_font_logisoso20_tf);
          u8g2.setFontDirection(1);
          u8g2.drawUTF8(2,0,cstr);
          u8g2.drawBox(35,1,boxpos,30);
          u8g2.drawFrame(35,1,65,30);
      }
      if (pwrstatebool == 0 && tempdatabool == 0) {
          u8g2.drawXBM( 50, 0, smily_width, smily_height, smily_bits);
      }
        u8g2.sendBuffer();
      }
}


//void draw(void) {
//  u8g.setFont(u8g_font_unifont);
//  u8g.drawStr( 0, 22, "Hello World!");
//}


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      //client.publish("domoticz/out", "hello world");
      // ... and resubscribe
      client.subscribe("domoticz/in");
      delay(500);
      //client.loop();
      client.subscribe("domoticz/out");
      Serial.println("subscribed to out as well");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  u8g2.begin();
  u8g2.enableUTF8Print();
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

}

void loop() {
  ArduinoOTA.handle();

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  //long now = millis();
  // if (now - lastMsg > 1000) {
  //  lastMsg = now;
  //  ++value;
    //snprintf (msg, 75, "hello world #%ld", value);
    //Serial.print("Publish message: ");
    //Serial.println(msg);
    //client.publish("outTopic", msg);
    //client.setCallback(callback);
  //}
}
