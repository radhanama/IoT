#include <ESP8266WiFi.h> // Importa a Biblioteca ESP8266WiFi
#include <WiFiManager.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
#include <ESP8266WebServer.h>
                               
 
//defines - mapeamento de pinos do NodeMCU
#define D0    16
#define D1    5 
#define D2    4 
#define D3    0 
#define D4    2 
#define D5    14  
#define D6    12 
#define D7    13 
#define D8    15 
#define D9    3  
#define D10   1  


                         
 
// WIFI
const char* myHostname = "esp"; // Nome do host na rede
const char* SSID = "NodeMCU"; // SSID / nome da rede WI-FI que deseja se conectar
const char* PASSWORD = "DeUmAOito"; // Senha da rede WI-FI que deseja se conectar
 

 
//Prototypes"
void initSerial();
void initWiFi();
void initOTA();
void initMQTT();
void reconectWiFi(); 
void InitOutput(void);
 
/* 
 *  Implementações das funções
 */
void setup() 
{
    //inicializações:
    initSerial();
    initWiFi();
    initOTA();
    Serial.println("Programa carregado");

}
 

//Função: inicializa comunicação serial com baudrate 115200 (para fins de monitorar no terminal serial 
//        o que está acontecendo.
//Parâmetros: nenhum
//Retorno: nenhum
void initSerial() 
{
    Serial.begin(115200);
}

 
//Função: inicializa e conecta-se na rede WI-FI desejada
//Parâmetros: nenhum
//Retorno: nenhum
void initWiFi()
{
  WiFi.hostname(myHostname);
  WiFiManager wifiManager;
   
  //wifiManager.resetSettings(); //Usado para resetar sssid e senhas armazenadas
  
  wifiManager.autoConnect(SSID, PASSWORD);
  Serial.print("Conectado com sucesso na rede via WifiManager na rede: ");
  Serial.println(WiFi.SSID());
  Serial.println();
  Serial.print("IP obtido: ");
  Serial.print(WiFi.localIP());  // mostra o endereço IP obtido via DHCP
  Serial.println();
  Serial.print("Endereço MAC: ");
  Serial.print(WiFi.macAddress()); // mostra o endereço MAC do esp8266
}



//Função inicializa OTA - permite carga do novo programa via Wifi
void initOTA()
{
  Serial.println();
  Serial.println("Iniciando OTA....");
  ArduinoOTA.setHostname("MeuNodeMcu"); // Define o nome da porta

  // No authentication by default
   ArduinoOTA.setPassword((const char *)"senhaforte"); // senha para carga via WiFi (OTA)
  ArduinoOTA.onStart([]() {
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("\nProgress: %u%%\r", (progress / (total / 100)));
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
}
 

 
//Função: reconecta-se ao WiFi
//Parâmetros: nenhum
//Retorno: nenhum
void reconectWiFi() 
{
  if (WiFi.status() == WL_CONNECTED)
      return;
      
  WiFi.hostname(myHostname);
  WiFi.begin(); // Conecta na rede WI-FI
    
  while (WiFi.status() != WL_CONNECTED) 
  {
      delay(100);
      Serial.print(".");

      Serial.println();
      Serial.print("Conectado com sucesso na rede: ");
      Serial.print(SSID);
      Serial.println();
      Serial.print("IP obtido: ");
      Serial.print(WiFi.localIP());  // mostra o endereço IP obtido via DHCP
      Serial.println();
      Serial.print("Endereço MAC: ");
      Serial.print(WiFi.macAddress()); // mostra o endereço MAC do esp8266
  }
}



 
 
//programa principal
void loop() 
{   
    reconectWiFi(); //garante funcionamento da conexão WiFi
    
    ArduinoOTA.handle(); // keep-alive da comunicação OTA
}
