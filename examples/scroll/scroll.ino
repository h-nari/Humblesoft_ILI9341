// Required Libraries
// https://github.com/adafruit/Adafruit-GFX-Library
// https://github.com/adafruit/Adafruit_ILI9341
// https://github.com/h-nari/Fontx
// https://github.com/h-nari/Humblesoft_GFX
// https://github.com/h-nari/Humblesoft_ILI9341

#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <Fontx.h>
#include <Humblesoft_GFX.h>
#include <Humblesoft_ILI9341.h>

Humblesoft_ILI9341 tft = Humblesoft_ILI9341();

void setup()
{
  Serial.begin(115200);
  delay(100);
  Serial.println("\n\nReset:");
  
  tft.begin();
  tft.setRotation(3);
  tft.fillScreen(ILI9341_BLACK);

  tft.setTextSize(8);
  tft.print("Hello");

  tft.setCursor(0, 120);
  tft.setTextSize(2);
  tft.setTextColor(ILI9341_RED);
  tft.print("1234567890");
  tft.setTextColor(ILI9341_GREEN);
  tft.print("1234567890");
  tft.setTextColor(ILI9341_BLUE);
  tft.print("123456");

  tft.setScrollStep(-3);
}

void loop()
{
  tft.scroll(false);
  delay(10);
}

/*** Local variables: ***/
/*** tab-width:2 ***/
/*** End: ***/
