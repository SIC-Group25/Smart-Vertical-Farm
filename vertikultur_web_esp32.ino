#include <FirebaseESP32.h>
#include <WiFi.h>

//wifi
#define FIREBASE_HOST "https://vertikultur-7b393-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "AIzaSyD_8VkwW2IURyk2ICwYjLm3uUsddh-B8xk"
#define WIFI_SSID "3 jagoan"
#define WIFI_PASSWORD "Gantengaqu3"
FirebaseData firebaseData;
FirebaseAuth firebaseAuth;
FirebaseConfig firebaseConfig;

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

/* #include <DHT.h>
#define DHTPIN 15
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE); */

#define TdsSensorPin 27
#define VREF 3.3
#define SCOUNT 30

int analogBuffer[SCOUNT];
int analogBufferIndex = 0;

float averageVoltage = 0;
float tdsValue = 0;
float temperature = 25;

int getMedianNum(int bArray[], int iFilterLen) {
  int bTab[iFilterLen];
  for (byte i = 0; i < iFilterLen; i++)
    bTab[i] = bArray[i];
  int i, j, bTemp;
  for (j = 0; j < iFilterLen - 1; j++) {
    for (i = 0; i < iFilterLen - j - 1; i++) {
      if (bTab[i] > bTab[i + 1]) {
        bTemp = bTab[i];
        bTab[i] = bTab[i + 1];
        bTab[i + 1] = bTemp;
      }
    }
  }
  if ((iFilterLen & 1) > 0) {
    bTemp = bTab[(iFilterLen - 1) / 2];
  } else {
    bTemp = (bTab[iFilterLen / 2] + bTab[iFilterLen / 2 - 1]) / 2;
  }
  return bTemp;
}

int batasTanah = 2047;
int sensorTanah = 14;
int pompa = 4;

void setup() {
  Serial.begin(115200);  // Mulai komunikasi serial
   WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("Connected to WiFi, IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  firebaseConfig.host = FIREBASE_HOST;
  firebaseConfig.signer.tokens.legacy_token = FIREBASE_AUTH;
  Firebase.begin(&firebaseConfig, &firebaseAuth);

  // dht.begin();
  pinMode(sensorTanah, INPUT);
  pinMode(pompa, OUTPUT);
  pinMode(TdsSensorPin, INPUT);
  lcd.init();
  lcd.backlight();
}
void loop() {
  /*
  // Sensor Kelembaban
  float h = dht.readHumidity();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Humidity: ");
  lcd.print(h);
  lcd.print(" %");
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.println(" %");

  delay(2000);  // Tahan selama 2 detik

  // Sensor Suhu
  float t = dht.readTemperature();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(t);
  lcd.print(" C");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.println(" C");

  delay(2000); */  // Tahan selama 2 detik

  // Sensor Tanah
  float kelembabanTanah;
  int nilaiTanah = analogRead(sensorTanah);
  kelembabanTanah = (100 - ((nilaiTanah / 4095.00) * 100));
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Lembab: ");
  lcd.print(kelembabanTanah);
  lcd.print(" %");
  Serial.print("Soil Humidity: ");
  Serial.print(kelembabanTanah);
  Serial.println(" %");
  Serial.print("Nilai Tanah: ");
  Serial.println(nilaiTanah);
  
  lcd.setCursor(0, 1);
  if (nilaiTanah < batasTanah) {
    lcd.print("Tanah: Basah");
    digitalWrite(pompa, LOW);
  } else {
    lcd.print("Tanah: Kering");
    digitalWrite(pompa, HIGH);
  }

  delay(2000);  // Tahan selama 2 detik

  // Sensor TDS
  static unsigned long analogSampleTimepoint = millis();
  if (millis() - analogSampleTimepoint > 40U) {
    analogSampleTimepoint = millis();
    analogBuffer[analogBufferIndex] = analogRead(TdsSensorPin);
    analogBufferIndex++;
    if (analogBufferIndex == SCOUNT) {
      analogBufferIndex = 0;
    }
  }

  static unsigned long printTimepoint = millis();
  if (millis() - printTimepoint > 800U) {
    printTimepoint = millis();
    for (int i = 0; i < SCOUNT; i++) {
      averageVoltage = getMedianNum(analogBuffer, SCOUNT) * (float)VREF / 4095.0;
      float compensationCoefficient = 1.0 + 0.02 * (temperature - 25.0);
      float compensationVoltage = averageVoltage / compensationCoefficient;
      tdsValue = (133.42 * compensationVoltage * compensationVoltage * compensationVoltage - 255.86 * compensationVoltage * compensationVoltage + 857.39 * compensationVoltage) * 0.5;
    }
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("TDS: ");
  lcd.print(tdsValue, 0);
  lcd.print(" ppm");
  Serial.print("TDS Value: ");
  Serial.print(tdsValue, 0);
  Serial.println(" ppm");

  // Firebase.setFloat(firebaseData, "/humidity", h);
  // Firebase.setFloat(firebaseData, "/temperature", t);
  Firebase.setFloat(firebaseData, "/tds", tdsValue);
  Firebase.setFloat(firebaseData, "/Kelembaban_Tanah", kelembabanTanah);

   if (firebaseData.httpCode() != 200)
  {
    Serial.println("Firebase update failed!");
    Serial.println(firebaseData.errorReason());
  }

  delay(2000);  // Tahan selama 2 detik
}
