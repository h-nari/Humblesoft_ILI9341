#include "conf.h"

#if USE_OTA
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include "ota_util.h"
#endif


#include <Adafruit_GFX.h>		// https://github.com/adafruit/Adafruit-GFX-Library
#include <Adafruit_ILI9341.h>		// https://github.com/adafruit/Adafruit_ILI9341
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
  tft.setVerticalScrollArea(20,20);

  tft.tfa.fillScreen(ILI9341_RED);
  tft.vsa.fillScreen(ILI9341_BLACK);
  tft.bfa.fillScreen(ILI9341_BLUE);
  tft.tfa.setTextSize(2);
  tft.tfa.print("TFA");
  tft.vsa.setTextColor(ILI9341_WHITE);
  tft.vsa.setFont(&fontx);
  tft.vsa.print("VSA - Vertical Scroll Area\n");
  tft.bfa.print("BFA");

	tft.vsa.print("漢字");
	tft.vsa.setFont(&FreeSerifBoldItalic9pt7b);
	tft.vsa.print("ABC");
	tft.vsa.setFont();
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

	tft.vsa.print("IP:");
	tft.vsa.println(WiFi.localIP());

  ota_init();
#endif
}

void loop()
{
#if 1
	tft.vsa.setFont(&FreeSerifBoldItalic9pt7b);
  static int i=1;
  tft.vsa.printf("%dline\n",i++);
  delay(500);
#endif

#if USE_OTA
  ArduinoOTA.handle();
#endif
	
}

/*** Local variables: ***/
/*** tab-width:2 ***/
/*** End: ***/
