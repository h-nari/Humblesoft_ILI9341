#ifndef _humblesoft_ili9341_h
#define _humblesoft_ili9341_h

#include <Arduino.h>
#include <Adafruit_ILI9341.h>
#include <Humblesoft_GFX.h>

class Humblesoft_ILI9341;
class VerticalScrollArea;

class VerticalScrollArea: public Humblesoft_GFX {
  friend class Humblesoft_ILI9341;
 protected:
  Humblesoft_ILI9341 *m_parent;
  
 public:
  VerticalScrollArea(Humblesoft_ILI9341 *parent);
  void drawPixel(int16_t x, int16_t y, uint16_t color) override;
  void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
    override;
  void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) override;
  void lineFeedHook(int16_t *px, int16_t *py, int16_t h);
  uint16_t colorRGB(uint8_t r, uint8_t g, uint8_t b) override;
};

class Humblesoft_ILI9341 : public Humblesoft_GFX {
  friend class VerticalScrollArea;
 public:
  VerticalScrollArea tfa;	// Top Fix Area
  VerticalScrollArea vsa;	// Vertical Scroll Area
  VerticalScrollArea bfa;	// Bottom FixArea
  
 protected:
  int32_t m_cs,m_dc;
  int16_t m_nScrollStep;
  int16_t m_nScrollPos;
  uint16_t m_cScrollBg;
  uint16_t m_y0;		// vsa start position
  uint16_t m_y1;		// bfa start position
  uint16_t m_nScrollDelay;
  Adafruit_ILI9341 m_lcd;

 public:
  Humblesoft_ILI9341(int8_t _CS=2, int8_t _DC=15, int8_t _RST = -1);
  void begin();
  void writedata(uint8_t *data, uint32_t len);

  void drawPixel(int16_t x, int16_t y, uint16_t color) override{
    m_lcd.drawPixel(x, y, color);
  };
  void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) override {
    m_lcd.drawFastVLine(x, y, h, color);
  };
  void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) override {
    m_lcd.drawFastHLine(x, y, w, color);
  };
  uint16_t colorRGB(uint8_t r, uint8_t g, uint8_t b) override {
    return m_lcd.color565(r,g,b);
  } ;
  void setRotation(uint8_t r);
    
  void setScrollBg(uint16_t c) { m_cScrollBg = c;}
  void setScrollStep(int16_t s) {m_nScrollStep = s;}
  void setScrollPos(int16_t s);
  void setScrollDelay(uint16_t n) {m_nScrollDelay = n;}
  void scroll(bool bClear=false);
  void scrollDrawPixel(int16_t x, int16_t y, uint16_t color);
  void setVerticalScrollArea(uint16_t tfa, uint16_t bfa);
  
 protected:
  void process_utf8_byte(uint8_t c, int16_t *pX, int16_t *pY, bool bDraw=true,
			 int16_t *pX2=NULL);
  bool getFont(uint16_t code, const uint8_t **pFont);
  bool getFontxGlyph (uint16_t code , const uint8_t **pGlyph,
		      uint8_t *pw, uint8_t *ph);
  void drawFontxGlyph(const uint8_t *glyph,uint8_t w,uint8_t h,
		      uint16_t cx, uint16_t cy);
};


#endif /* _humblesoft_ili9341_h_ */
