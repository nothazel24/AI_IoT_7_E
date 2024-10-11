#include "DHT.h"
#define DHTPIN 4     
#define DHTTYPE DHT11  
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

DHT dht(DHTPIN, DHTTYPE);
int moist,sensor_analog;
const int sensor_pin = 34; 

const char* ssid ="realme 10";
const char* password = "12345678";
#define CHAT_ID "936037076"
#define BOTtoken "6308039951:AAH1APjB2Tupk7smxNQXMAjjAbNmpkREJQk"

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);
int botRequestDelay = 1000;
unsigned long lastTimeBotRan;

// const int ph_Pin = 35;
// float  PHlevel = 0;
// float PH_step;
// int nilai_analog_PH;
// double TeganganPh;

// float PH7=9.8;
// float PH4=10.2;

void setup() {
  
  Serial.begin(115200);

  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT);
  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  // pinMode(ph_Pin, INPUT);
  bot.sendMessage(CHAT_ID, "Tunggu dulu...monitoring akan segera dimulai", "");
  
  delay(2000);
  Serial.print("Workshop Monitoring Tanaman SMKN 7 Baleendah\n");
  dht.begin();
}
void loop() {
  sensor_analog = analogRead(sensor_pin);
  // int nilai_analog_PH =analogRead(ph_Pin);
  moist = ( 100 - ( (sensor_analog/4095.00) * 100 ) );

  // TeganganPh = 3.3/1024.0 * nilai_analog_PH;
  // PH_step =(PH4-PH7)/3;
  // PHlevel = 7.00 +((PH7 -TeganganPh)/PH_step);

  float tempe = dht.readTemperature();
  float humid = dht.readHumidity();
  if (isnan(humid) || isnan(tempe) || isnan(moist)) { /*|| isnan(PHlevel)*/
    Serial.println("Sensor tidak terbaca!");
    bot.sendMessage(CHAT_ID,"Sensor tidak terbaca!", "");
    return;
  }
  Serial.print("=========Monitoring tanaman==========\n");
  Serial.print("Humiditas gas udara: ");
  Serial.print(humid); /* Print humiditas gas */
  Serial.println(" %");
  Serial.print("Temperatur lingkungan: ");
  Serial.print(tempe); /* Print temperatur */
  Serial.println(" °C");
  Serial.print("Kelembaban tanah: ");
  Serial.print(moist);  /* Print kelembapan air */
  Serial.println(" %");
  // Serial.print("Nilai pH Cairan : ");
  // Serial.println(PHlevel);
  // Serial.println(" level");
  // bot.sendMessage(CHAT_ID,"Monitoring Tanaman");
  //delay(1000);
  kirimPesanTelegram(humid, tempe, moist); /*, PHlevel*/
}

void kirimPesanTelegram(float humid, float tempe, int moist) { /*,float PHlevel*/
  String pesan = "Suhu saat ini: " + String(tempe, 2) + "°C\n" +
                 "Humiditas udara saat ini: " + String(humid, 2) + "%\n" +
                 "Tingkat kelembaban tanah saat ini: " + String(moist,2) + "%\n";
                //  "pH level dari kandungan air tanah saat ini: "+ String(PHlevel, 2) + "%\n";
  if (bot.sendMessage(CHAT_ID, pesan, "Markdown")) {
    Serial.println("Pesan berhasil dikirim");
  } else {
    Serial.println("Gagal mengirim pesan");
  }

  delay(1000);  // Menunggu sejenak sebelum mengirim pesan lagi
}