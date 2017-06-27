#include "conf.h"

#if USE_OTA
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include "ota_util.h"
#endif


#include <Adafruit_GFX.h>		// https://github.com/adafruit/Adafruit-GFX-Library
#include <Fontx.h>							// https://github.com/h-nari/Fontx
#include <FsFontx.h>
#include <Humblesoft_GFX.h>			// https://github.com/h-nari/Humblesoft_GFX
#include <Humblesoft_ILI9341.h>	// https://github.com/h-nari/Humblesoft_ILI9341
#include <Fonts/FreeSerifBoldItalic9pt7b.h>

Humblesoft_ILI9341 tft = Humblesoft_ILI9341();
FsFontx fontx("/ILGH16XB.FNT","/ILGZ16XB.FNT");

void setup()
{
  Serial.begin(115200);
  delay(100);
  Serial.println("\n\nReset:");

  if(!SPIFFS.begin())
    Serial.println("SPIFFS.begin() failed.");
	
  tft.begin();
  tft.setVerticalScrollArea(40,20);

  tft.tfa.fillScreen("RED");
  tft.vsa.fillScreen("BLACK");
  tft.bfa.fillScreen("BLUE");
  tft.tfa.setTextSize(2);
	tft.tfa.setFont(&FreeSerifBoldItalic9pt7b);
	tft.tfa.setCursor(0,30);
  tft.tfa.print("Top Fix Area");
  tft.vsa.setTextColor("WHITE");
  tft.vsa.setFont(&fontx);
  tft.vsa.print("VSA - Vertical Scroll Area\n");
  tft.bfa.print("BFA");

	tft.vsa.print("漢字");
	tft.vsa.print("ABC");
	tft.vsa.println("defg");
	tft.vsa.println("12345");
	
#if USE_OTA
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print('.');
  }
  Serial.println();
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

	tft.bfa.print("   IP:");
	tft.bfa.print(WiFi.localIP());

  ota_init();
#endif
}

void loop()
{
#if 1
  static int i=1;
  tft.vsa.printf("%d行\n",i++);
  delay(500);
#endif

#if USE_OTA
  ArduinoOTA.handle();
#endif
	
}

/*** Local variables: ***/
/*** tab-width:2 ***/
/*** End: ***/
