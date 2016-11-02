#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include "Fontx.h"
#include "FontxGfx.h"
#include "Humblesoft_ILI9341.h"

IMPORT_BIN("fontx/ILGH16XB.FNT", font_h);
IMPORT_BIN("fontx/ILGZ16XB.FNT", font_z);
extern uint8_t font_h[], font_z[];

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
  tft.vsa.setFontx(font_h, font_z);
  tft.vsa.print("VSA - Vertical Scroll Area\n");
  tft.bfa.print("BFA");
}

void loop()
{
  static int i=1;
  tft.vsa.printf("%d行目\n",i++);
  delay(1000);
}

