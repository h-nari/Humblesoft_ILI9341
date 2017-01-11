#include <Adafruit_GFX.h>		// https://github.com/adafruit/Adafruit-GFX-Library
#include <Adafruit_ILI9341.h>		// https://github.com/adafruit/Adafruit_ILI9341
#include <Fontx.h>							// https://github.com/h-nari/Fontx
#include <Humblesoft_GFX.h>			// https://github.com/h-nari/Humblesoft_GFX
#include <Humblesoft_ILI9341.h>	// https://github.com/h-nari/Humblesoft_ILI9341
#include <Fonts/FreeSerifBoldItalic9pt7b.h>
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
RomFontx fontx(ILGH16XB,ILGZ16XB);

void drawTextAndBounds(int16_t cx, int16_t cy, char *str)
{
  int16_t x, y;
  uint16_t w, h;

  tft.getTextBounds(str, cx, cy, &x, &y, &w, &h);
	
  tft.drawRect(x, y, w, h, ILI9341_RED);
  tft.setCursor(cx, cy);
  tft.print(str);
}

void setup()
{
  char s1[]="ABC";
  char k1[]="日本語";
  
  Serial.begin(115200);
  delay(100);
  Serial.println("\n\nReset:");
  
  tft.begin();
  tft.setRotation(3);
  tft.fillScreen(ILI9341_BLACK);

  tft.setTextSize(4);
  drawTextAndBounds(5, 10, s1);

  tft.setTextSize(2);
  drawTextAndBounds(5, 50, "Hello\nWorld");
  
  tft.setFont(&fontx);
  tft.setTextSize(2);
  drawTextAndBounds(160,10, "漢字");

  drawTextAndBounds(160,40, k1);

	tft.setTextSize(0);
	tft.setFont(&FreeSerifBoldItalic9pt7b);
	drawTextAndBounds(20, 220, "FreeSerifBoldItalic9pt7b");

	tft.setFont(&fontx);
  tft.setTextSize(0);
  drawTextAndBounds(0,100, "1行目\n２行目");

  drawTextAndBounds(160,160, "line wrap test. Very long line.");
}

void loop()
{
}

/*** Local variables: ***/
/*** tab-width:2 ***/
/*** End: ***/
