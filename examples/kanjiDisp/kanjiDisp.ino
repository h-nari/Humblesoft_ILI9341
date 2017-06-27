#include <Adafruit_GFX.h>		// https://github.com/adafruit/Adafruit-GFX-Library
#include <Fontx.h>							// https://github.com/h-nari/Fontx
#include <Humblesoft_GFX.h>			// https://github.com/h-nari/Humblesoft_GFX
#include <Humblesoft_ILI9341.h>

#include <fontx/ILGH16XB.h>
#include <fontx/ILGZ16XB.h>

Humblesoft_ILI9341 tft = Humblesoft_ILI9341();
RomFontx fontx(ILGH16XB,ILGZ16XB);

void setup() {
  delay(100);
  tft.begin();
  tft.setFont(&fontx);
  tft.setRotation(3);
  tft.fillScreen("BLACK");

  tft.setTextSize(1);
  tft.print("Humblesoft_ILI9341 漢字表示デモ\nABCDEFGHIJKLMNOPQRSTUVWXYZ\n");
  tft.print("\n");
  tft.print("このプログラムはILフォントを使用しています。");
  tft.print("ILフォントはIPAフォントを元に作成されたfontx形式のフォントです。\n");
    
  tft.setTextSize(4);
  tft.setTextColor("GREEN");
  tft.setCursor(0, tft.getCursorY() + 10);
  tft.print("Hello世界\n");
  tft.setCursor(80, tft.getCursorY() + 10);
  tft.setTextSize(3);
  tft.setTextColor("YELLOW");
  tft.print("ILI9341\n");
}

void loop() {
}

/*** Local variables: ***/
/*** tab-width:2 ***/
/*** End: ***/

