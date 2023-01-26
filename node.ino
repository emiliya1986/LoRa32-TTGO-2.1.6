//Libraries for LoRa
#include <SPI.h>
#include <LoRa.h>

//Libraries for sensor
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

//define the pins used by the LoRa transceiver module
#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 23
#define DIO0 26
#define BAND 868E6
#define SEALEVELPRESSURE_HPA (1013.25)

char message[100];
RTC_DATA_ATTR int readingID = 0;
String rssi = "RSSI --";
String packSize = "--";
String packet ;

//packet counter
int counter = 0;

Adafruit_BME280 bme; // I2C

// Переменные для bme280:
float temp;
float pressure;
float altitude;
float humidity;

unsigned long delayTime;

void setup() {
  Serial.begin(115200);
  bme.begin(0x76);  
  
  SPI.begin(SCK,MISO,MOSI,SS);
  LoRa.setPins(SS, RST, DIO0);
 
  Serial.println("initializing LoRa");  //  "Инициализируем LoRa"
 int counter = 0;  
  while (!LoRa.begin(868E6) && counter < 10) {
    Serial.print("Starting LoRa failed!");
    counter++;
    delay(500);
  }
  if (counter == 10) {
    // C каждым новым считыванием 
    // увеличиваем значение в «readingID» на единицу:
    readingID++; 
    // Запускаем режим глубокого сна:
    Serial.println("Failed to initialize LoRa. Going to sleep now");
               //  "Не удалось инициализировать LoRa.
  }
  // Cинхрослово (0xF3) должно быть таким же, как и у приемника.
  LoRa.setSyncWord(0xF3);
  Serial.println("LoRa initializing OK!");
             //  "Инициализация LoRa прошла успешно!"


}



void loop() {
  getReadings();

  Serial.print("Temperature = ");
  Serial.print(temp);
  Serial.println(" *C");

  Serial.print("Pressure = ");
  Serial.print(pressure);
  Serial.println(" hPa");

  Serial.print("Altitude = ");
  Serial.print(altitude);
  Serial.println(" m" );

  Serial.print("Humidity = ");
  Serial.print(humidity);   
  Serial.println(" %" ); 

  Serial.print("Reading ID = ");
  Serial.println(readingID );
  
  sendReadings();
  Serial.print("Message sent = ");
  Serial.println(message);
  
  // Увеличиваем значение в «readingID» с каждым новым считыванием:
  readingID++;
  counter++;

  delay(1000);
}



void getReadings() {
   
  temp = bme.readTemperature(); // температура в Цельсиях
  pressure = bme.readPressure(); 
  altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);
  humidity = bme.readHumidity();
   
}

// Это функция для отправки пакета данных:
void sendReadings() {

  sprintf(message, "%05f-%05f-%05f-%05f-%05f", readingID, temp, pressure, altitude, humidity);
  // message = String(readingID) + "/" + String(temp) + "&" + 
  //           String(pressure) + "#" + String(altitude) + "^" + String(humidity);

  delay(1000);
  LoRa.beginPacket();
  LoRa.print(message);
  LoRa.endPacket();
}


