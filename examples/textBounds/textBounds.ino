#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include "Humblesoft_ILI9341.h"


IMPORT_BIN("fontx/ILGH16XB.FNT", font_h);
IMPORT_BIN("fontx/ILGZ16XB.FNT", font_z);

extern uint8_t font_h[], font_z[];

Humblesoft_ILI9341 tft = Humblesoft_ILI9341();

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
  
  Serial.begin(19200);
  delay(100);
  Serial.println("\n\nReset:");
  
  tft.begin();
  tft.setRotation(3);
  tft.fillScreen(ILI9341_BLACK);
#if 1
  tft.setTextSize(4);
  drawTextAndBounds(5, 10, s1);

  tft.setTextSize(2);
  drawTextAndBounds(5, 50, "Hello\nWorld");
  
  tft.setFontx(font_z, font_h);
  tft.setTextSize(2);
  drawTextAndBounds(160,10, "漢字");

  drawTextAndBounds(160,40, k1);

  tft.setTextSize(0);
  drawTextAndBounds(0,100, "1行目\n２行目");

  drawTextAndBounds(160,160, "line wrap test. Very long line.");
#else
  tft.setFontx(font_z, font_h);
  drawTextAndBounds(300,160, "abcd");
#endif
}

void loop()
{
}
