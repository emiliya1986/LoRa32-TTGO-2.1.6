#include <WiFi.h>
#include <SPI.h>
#include <LoRa.h>

// const char* ssid     = "TP-Link_57FC";
// const char* password = "05847242";
const char* ssid     = "Мира";
const char* password = "parolpar";


//define the pins used by the LoRa transceiver module
#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 23
#define DIO0 26
#define BAND    868E6

// Создаем переменные для считывания и записи LoRa-данных и RSSI:

String loRaMessage;
String temperature;
String press;
String altit;
String humid;
String readingID;

String rssi;
String packSize = "--";
String packet ;


// Создаем объект для веб-сервера и задаем ему номер порта «80»:
WiFiServer server(80);

// Переменная для хранения HTTP-запроса:
String header;

void loraData(){
  Serial.println("Received "+ packSize + " bytes");
  Serial.println(packet);
  Serial.println(rssi);
}

void cbk(int packetSize) {
  packet ="";
  packSize = String(packetSize,DEC);
  for (int i = 0; i < packetSize; i++) { packet += (char) LoRa.read(); }
  rssi = "RSSI " + String(LoRa.packetRssi(), DEC) ;
  loraData();
}

void setup() { 
  Serial.begin(115200);
  
  // Инициализируем LoRa.
  SPI.begin(SCK,MISO,MOSI,SS);
  LoRa.setPins(SS, RST, DIO0);
    if (!LoRa.begin(868E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  // Синхрослово (0xF3) должно быть таким же, как и у передатчика.
  LoRa.setSyncWord(0xF3);           
  Serial.println("LoRa Initializing OK!");
             //  "Инициализация LoRa прошла успешно!"

  // Подключаемся к WiFi-сети при помощи заданных ранее SSID и пароля:
  Serial.print("Connecting to ");
           //  "Подключаемся к "
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Печатаем локальный IP-адрес и запускаем веб-сервер:
  Serial.println("");
  Serial.println("WiFi connected.");  //  "Подключились к WiFi."
  Serial.println("IP address: ");  //  "IP-адрес: "
  Serial.println(WiFi.localIP());
  server.begin();
   
  delay(100);
}

void loop() {
  // Проверяем, пришел ли LoRa-пакет:
  int packetSize = LoRa.parsePacket();
  if (packetSize) { cbk(packetSize);  }
  delay(10);

  // WiFiClient client = server.available();   // Включаем прослушку 
  //                                           // входящих клиентов 

  // if (client) {                             // Если подключился
  //                                           // новый клиент,
  //   Serial.println("New Client.");          // пишем в монитор порта 
  //                                           // сообщение об этом.
  //   String currentLine = "";                // Создаем строку,
  //                                           // куда будем сохранять
  //                                           // данные от клиента.
  //   while (client.connected()) {            // Цикл while()
  //                                           // будет работать,
  //                                           // пока клиент подключен.
  //     if (client.available()) {             // Если у клиента
  //                                           // есть байты,
  //       char c = client.read();             // считываем байт,
  //       Serial.write(c);                    // и печатаем его
  //                                           // в монитор порта.
  //       header += c;
  //       if (c == '\n') {                    // если этот байт – 
  //                                           // символ новой строки,
  //         // и если у нас два символа новой строки подряд,
  //         // то текущая строка пуста.
  //         // Это конец HTTP-запроса клиента, т.е. пора слать ответ.
  //         if (currentLine.length() == 0) {
  //           // HTTP-заголовки всегда начинаются с кода ответа
  //           // (например, с «HTTP/1.1 200 OK») и типа контента,
  //           // чтобы клиент знал, что получает,
  //           // а затем пишем пустую строку.
  //           client.println("HTTP/1.1 200 OK");
  //           client.println("Content-type:text/html");
  //           client.println("Connection: close");
  //                      //  "Соединение: отключено"
  //           client.println();
            
  //           // Показываем веб-страницу:
  //           client.println("<!DOCTYPE html><html>");
  //           client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
  //           client.println("<link rel=\"icon\" href=\"data:,\">");
  //           // При помощи CSS задаем стиль для таблицы: 
  //           client.println("<style>body { text-align: center; font-family: \"Trebuchet MS\", Arial;}");
  //           client.println("table { border-collapse: collapse; width:35%; margin-left:auto; margin-right:auto; }");
  //           client.println("th { padding: 12px; background-color: #ffd164; color: white; }");
  //           client.println("tr { border: 1px solid #ddd; padding: 12px; }");
  //           client.println("tr:hover { background-color: #bcbcbc; }");
  //           client.println("td { border: none; padding: 12px; }");
  //           client.println(".sensor { color:white; font-weight: bold; background-color: #bcbcbc; padding: 1px; }");
            
  //           // заголовок веб-страницы:
  //           client.println("</style></head><body><h1>Environmental Monitoring with LoRa </h1>");
  //           client.println("<table><tr><th>MEASUREMENT</th><th>VALUE</th></tr>");
  //           client.println("<tr><td>Temperature</td><td><span class=\"sensor\">");
  //           client.println(temperature);
  //           client.println(" *C</span></td></tr>");   
  //           client.println("<tr><td>Pressure</td><td><span class=\"sensor\">");
  //           client.println(press);
  //           client.println("</span></td></tr>"); 
  //           client.println("<tr><td>Altitude</td><td><span class=\"sensor\">");
  //           client.println(altit);
  //           client.println(" %</span></td></tr>"); 
  //           client.println("<tr><td>Humidity</td><td><span class=\"sensor\">");
  //           client.println(humid);
  //           client.println(" %</span></td></tr>");
  //           client.println("<p>LoRa RSSI: " + String(rssi) + "</p>");
  //           client.println("</body></html>");
            
  //           // HTTP-ответ заканчивается еще одной пустой строкой:
  //           client.println();
  //           // выходим из цикла while():
  //           break;
  //         } else {  // если получили символ новой строки,
  //                   // то очищаем переменную «currentLine»
  //           currentLine = "";
  //         }
  //       } else if (c != '\r') {  // если получили что-либо,
  //                                // кроме символа возврата каретки,
  //         currentLine += c;      // добавляем это что-то
  //                                // в конец переменной «currentLine» 
  //       }
  //     }
  //   }
  //   // очищаем переменную «header»:
  //   header = "";
  //   // отключаем соединение:
  //   client.stop();
  //   Serial.println("Client disconnected.");
  //              //  "Клиент отключился."
  //   Serial.println("");
  // }
}


// Считываем LoRa-пакет с данными от датчиков:
void getLoRaData() {
  Serial.print("Lora packet received: ");
           //  "LoRa-пакет получен: "
  // Считываем пакет:
  while (LoRa.available()) {
    String LoRaData = LoRa.readString();
    // Формат LoRa-данных:
    // «readingID/temperature&press#altit^humid»
    // Например: «1/27.43&654#95.34».
    Serial.print(LoRaData); 
    
    // //Считываем ID пакета:
    // int pos1 = LoRaData.indexOf('/');
    // int pos2 = LoRaData.indexOf('&');
    // int pos3 = LoRaData.indexOf('#');
    // int pos4 = LoRaData.indexOf('^');
    // readingID = LoRaData.substring(0, pos1);
    // temperature = LoRaData.substring(pos1+1, pos2);
    // press = LoRaData.substring(pos2+1, pos3);
    // altit = LoRaData.substring(pos3+1, pos4);
    // humid = LoRaData.substring(pos4+1, LoRaData.length()); 


  }
  // считываем RSSI:
  rssi = LoRa.packetRssi();
  Serial.print(" with RSSI ");  //  " Мощность LoRa-сигнала: " 
  Serial.println(rssi);
  delay(10);
}



