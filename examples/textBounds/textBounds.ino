#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include "Humblesoft_ILI9341.h"


IMPORT_BIN("fontx/ILGH16XB.FNT", font_h);
IMPORT_BIN("fontx/ILGZ16XB.FNT", font_z);

extern uint8_t font_h[], font_z[];

Humblesoft_ILI9341 tft = Humblesoft_ILI9341();

static void dump_var(const char *name, uint32_t value)
{
  Serial.print(name);
  Serial.print("=0x");
  Serial.print(value,16);
  Serial.println();
}

void drawTextAndBounds(int16_t cx, int16_t cy, char *str)
{
  int16_t x, y;
  uint16_t w, h;

  tft.getTextBounds(str, cx, cy, &x, &y, &w, &h);
  dump_var("x",x);
  dump_var("y",y);
  dump_var("w",w);
  dump_var("h",h);

  tft.drawRect(x, y, w, h, ILI9341_RED);
  tft.setCursor(cx, cy);
  tft.print(str);
}

void drawTextAndBounds(int16_t cx, int16_t cy, const __FlashStringHelper *s)
{
  int16_t x, y;
  uint16_t w, h;

  tft.getTextBounds(s, cx, cy, &x, &y, &w, &h);
  dump_var("x",x);
  dump_var("y",y);
  dump_var("w",w);
  dump_var("h",h);

  tft.drawRect(x, y, w, h, ILI9341_GREEN);
  tft.setCursor(cx, cy);
  tft.print(s);
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
  drawTextAndBounds(5, 50, F("Hello\nWorld"));
  
  tft.setFontx(font_z, font_h);
  tft.setTextSize(2);
  drawTextAndBounds(160,10, F("漢字"));

  drawTextAndBounds(160,40, k1);

  tft.setTextSize(0);
  drawTextAndBounds(0,100, F("1行目\n２行目"));

  drawTextAndBounds(160,160, F("line wrap test. Very long line."));
#else
  tft.setFontx(font_z, font_h);
  drawTextAndBounds(300,160, F("abcd"));
#endif
}

void loop()
{
}
