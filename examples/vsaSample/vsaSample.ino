#include <Adafruit_GFX.h>		// https://github.com/adafruit/Adafruit-GFX-Library
#include <Fontx.h>							// https://github.com/h-nari/Fontx
#include <Humblesoft_GFX.h>			// https://github.com/h-nari/Humblesoft_GFX
#include <Humblesoft_ILI9341.h>	// https://github.com/h-nari/Humblesoft_ILI9341

#include <fontx/ILGH16XB.h>
#include <fontx/ILGZ16XB.h>

Humblesoft_ILI9341 tft = Humblesoft_ILI9341();
RomFontx fontx(ILGH16XB,ILGZ16XB);

void setup()
{
  Serial.begin(115200);
  delay(100);
  Serial.println("\n\nReset:");
  
  tft.begin();
  tft.setVerticalScrollArea(20,20);

  tft.tfa.fillScreen("RED");
  tft.vsa.fillScreen("BLACK");
  tft.bfa.fillScreen("BLUE");
  tft.tfa.setTextSize(2);
  tft.tfa.print("TFA");
  tft.vsa.setTextColor("WHITE");
  tft.vsa.setFont(&fontx);
  tft.vsa.print("VSA - Vertical Scroll Area\n");
  tft.bfa.print("BFA");
}

void loop()
{
  static int i=1;
  tft.vsa.printf("%d行目\n",i++);
  delay(1000);
}

/*** Local variables: ***/
/*** tab-width:2 ***/
/*** End: ***/
