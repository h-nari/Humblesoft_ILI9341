#include <Adafruit_GFX.h>				// https://github.com/adafruit/Adafruit-GFX-Library
#include <Adafruit_ILI9341.h>		// https://github.com/adafruit/Adafruit_ILI9341
#include <Fontx.h>							// https://github.com/h-nari/Fontx
#include <FontxGfx.h>						// https://github.com/h-nari/FontxGfx
#include <Humblesoft_ILI9341.h>	// https://github.com/h-nari/Humblesoft_ILI9341

// #define IMPORT_FONTX_BINARY

#ifdef IMPORT_FONTX_BINARY
IMPORT_BIN("ILGH16XB.FNT", ILGH16XB);
IMPORT_BIN("ILGZ16XB.FNT", ILGZ16XB);
extern const uint8_t ILGH16XB[], ILGZ16XB[];
#else
#include <fontx/ILGH16XB.h>
#include <fontx/ILGZ16XB.h>
#endif

Humblesoft_ILI9341 tft = Humblesoft_ILI9341();

void setup()
{
  Serial.begin(115200);
  delay(100);
  Serial.println("\n\nReset:");
  
  tft.begin();
  tft.setVerticalScrollArea(20,20);

  tft.tfa.fillScreen(ILI9341_RED);
  tft.vsa.fillScreen(ILI9341_BLACK);
  tft.bfa.fillScreen(ILI9341_BLUE);
  tft.tfa.setTextSize(2);
  tft.tfa.print("TFA");
  tft.vsa.setTextColor(ILI9341_WHITE);
  tft.vsa.setFontx(ILGH16XB,ILGZ16XB);
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
