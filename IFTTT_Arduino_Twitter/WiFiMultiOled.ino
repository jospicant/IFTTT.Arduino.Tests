/*
 *  This sketch trys to Connect to the best AP based on a given list
 *
 */

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

//******* Oled *************************
#include <Wire.h>
#include <SSD1306.h>
#define flipDisplay true
SSD1306 display(0x3c, 5, 4); //GPIO 5 = D1, GPIO 4 = D2

// Definimos parámetros ******************************
#define WIFISID "Escribe aquí tu SSID"
#define WIFIPASS "Escribe aquí tu password"
 
// Definimos los parámetros para el IFTTT **********************************
#define HOSTIFTTT "maker.ifttt.com"
#define EVENTO "Escribe aquí el nombre de tu evento"
#define IFTTTKEY "Escribe aquí tu Key obtenida desde Settings del servicio Webhooks "

ESP8266WiFiMulti WiFiMulti;

// Utilizamos la conexión SSL del ESP8266 ************************************
WiFiClientSecure client;

// Variable que permite ejecutar una sola vez la función
bool ejecutado = false;
 
//**************************************************************************

void setup() {
  
 Serial.begin(115200);
 delay(10);

 /* Inicializamos la pantalla */
int col=0;
display.init();
if (flipDisplay) display.flipScreenVertically();
 
 // Conectamos a la red WiFi
 WiFiMulti.addAP(WIFISID, WIFIPASS);

display.clear();
display.setFont(ArialMT_Plain_10);
display.drawString(10, 25, "Conectando Wifi");
display.display();

 
 while (WiFiMulti.run() != WL_CONNECTED) {
display.drawString(10+col,40, " ..      ");
col++;
display.display();
delay(70);
 }

display.clear();
display.drawString(5,25, "Conectado con IP:");
display.drawString(5,40,WiFi.localIP().toString());
display.display();
delay(500);
 
 //Establecemos la semilla en un pin analogico
 randomSeed(analogRead(A0));
}

//*************************************************************************************************************************************************************
void loop() {

if (!ejecutado)
  {
    // Obtenemos los valores
    int valor1 = (int)random(12, 40);
    int valor2 = (int)random(50, 99);
    int valor3 = analogRead(A0);

    
    display.drawString(10, 50,"valor1="+String(valor1));
    display.display();
 
    enviar_tweet(valor1, valor2, valor3);
    ejecutado = true;
  }
}

//************** función envio tweet **********************************************

void enviar_tweet(int valor1, int valor2, int valor3)
{
  // Cerramos cualquier conexión anterior
  
  if (client.connected())
  {
    client.stop();
  }
 
  // Esperamos hasta que se hayan enviado todos los datos
  client.flush();
 
  // Hacemos la petición mediante SSL
  if (client.connect(HOSTIFTTT, 443)) {
  
    // Construimos la petición HTTP
    String toSend = "GET /trigger/";
    toSend += EVENTO;
    toSend += "/with/key/";
    toSend += IFTTTKEY;
    toSend += "?value1=";
    toSend += valor1;
    toSend += " HTTP/1.1\r\n";
    toSend += "Host: ";
    toSend += HOSTIFTTT;
    toSend += "\r\n";
    toSend += "Connection: close\r\n\r\n";
 
    client.print(toSend);
    
    display.drawString(1,1,"Petición enviada OK");
    display.display();
    
  }
 
  // Esperamos hasta que se hayan enviado todos los datos
  client.flush();
  // Desconectamos del cliente
  client.stop();
 
}
