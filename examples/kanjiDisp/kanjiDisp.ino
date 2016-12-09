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

void setup() {
  delay(100);
  tft.begin();
  tft.setFontx(ILGH16XB,ILGZ16XB);
  tft.setRotation(3);
  tft.fillScreen(ILI9341_BLACK);

  tft.setTextSize(1);
  tft.print("Humblesoft_ILI9341 漢字表示デモ\nABCDEFGHIJKLMNOPQRSTUVWXYZ\n");
  tft.print("\n");
  tft.print("このプログラムはILフォントを使用しています。");
  tft.print("ILフォントはIPAフォントを元に作成されたfontx形式のフォントです。\n");
    
  tft.setTextSize(4);
  tft.setTextColor(ILI9341_GREEN);
  tft.setCursor(0, tft.getCursorY() + 10);
  tft.print("Hello世界\n");
  tft.setCursor(80, tft.getCursorY() + 10);
  tft.setTextSize(3);
  tft.setTextColor(ILI9341_YELLOW);
  tft.print("ILI9341\n");
}

void loop() {
}

/*** Local variables: ***/
/*** tab-width:2 ***/
/*** End: ***/

