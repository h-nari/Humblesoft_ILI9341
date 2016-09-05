#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include "../../Humblesoft_ILI9341.h"

IMPORT_BIN("ILGH16XB.FNT", font_h);
IMPORT_BIN("ILGZ16XB.FNT", font_z);

extern uint8_t font_h[], font_z[];

Humblesoft_ILI9341 tft = Humblesoft_ILI9341();

void setup() {
  Serial.begin(19200);
  delay(100);
  Serial.println("\n\nReset:");
  tft.begin();
  tft.setFontx(font_h,font_z);
  tft.setRotation(3);

  tft.fillScreen(ILI9341_BLACK);
  
  tft.setTextSize(1);
  tft.print("HSES-LCD24漢字表示デモ\nABCDEFGHIJKLMNOPQRSTUVWXYZ\n");
  tft.print("熊本県熊本市中央区萩原町3番9号\n");
    
  tft.setTextSize(4);
  tft.setTextColor(ILI9341_GREEN);
  tft.setCursor(0, 60);
  tft.print("Hello世界");
  tft.setCursor(80, 140);
  tft.setTextSize(3);
  tft.setTextColor(ILI9341_YELLOW);
  tft.print("ILI9341");
}

void loop() {
}


