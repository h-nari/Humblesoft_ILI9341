#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include "Humblesoft_ILI9341.h"

IMPORT_BIN("fontx/ILGH16XB.FNT", font_h);
IMPORT_BIN("fontx/ILGZ16XB.FNT", font_z);

extern uint8_t font_h[], font_z[];

Humblesoft_ILI9341 tft = Humblesoft_ILI9341();

void setup() {
  delay(100);
  tft.begin();
  tft.setFontx(font_h,font_z);
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


