#ifndef _humblesoft_ili9341_h
#define _humblesoft_ili9341_h

#include "Arduino.h"
#include "Adafruit_ILI9341.h"
#include "FontxGfx.h"

class Humblesoft_ILI9341;
class VerticalScrollArea;

enum TextAlign { TA_NONE, TA_LEFT, TA_CENTER, TA_RIGHT, TA_TOP, TA_BOTTOM};

class FontxGfxVs : public FontxGfx {
 public:
  FontxGfxVs(Adafruit_GFX *pGfx);
 protected:
  void lineFeedHook(int16_t *px, int16_t *py, int16_t h) override;
};

class VerticalScrollArea: public Adafruit_GFX {
  friend class Humblesoft_ILI9341;
 protected:
  FontxGfxVs m_fontx;
  Humblesoft_ILI9341 *m_parent;
  
 public:
  VerticalScrollArea(Humblesoft_ILI9341 *parent);
  void drawPixel(int16_t x, int16_t y, uint16_t color) override;
  size_t write(uint8_t) override;
  void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
    override;
  void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) override;
  void lineFeedHook(int16_t *px, int16_t *py, int16_t h);
  void resetFontx(void) {m_fontx.resetFontx();}
  void setFontx(uint8_t *f0, uint8_t *f1=NULL, uint8_t *f2=NULL){
    m_fontx.setFontx(f0,f1,f2);
  }
  void setFont(const GFXfont *f = NULL) {
    Adafruit_GFX::setFont(f);
    m_fontx.resetFontx();
  }
};

class Humblesoft_ILI9341 : public Adafruit_ILI9341 {
  friend class VerticalScrollArea;
 public:
  VerticalScrollArea tfa;	// Top Fix Area
  VerticalScrollArea vsa;	// Vertical Scroll Area
  VerticalScrollArea bfa;	// Bottom FixArea
  
 protected:
  FontxGfx m_fontx;
  int32_t m_cs,m_dc;
  int16_t m_nScrollStep;
  int16_t m_nScrollPos;
  uint16_t m_cScrollBg;
  uint16_t m_y0;		// vsa start position
  uint16_t m_y1;		// bfa start position
  uint16_t m_nScrollDelay;

 public:
  Humblesoft_ILI9341(int8_t _CS=2, int8_t _DC=15, int8_t _RST = -1);
  void begin();
  void writedata(uint8_t *data, uint32_t len);

  size_t write(uint8_t) override;
  void resetFontx(void);
  void addFontx(uint8_t *fontx);
  void setFontx(uint8_t *f0, uint8_t *f1=NULL, uint8_t *f2=NULL);
  void setFont(const GFXfont *f = NULL);
  void getTextBounds(char *string, int16_t x, int16_t y,
		     int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h);
  void setScrollBg(uint16_t c) { m_cScrollBg = c;}
  void setScrollStep(int16_t s) {m_nScrollStep = s;}
  void setScrollPos(int16_t s);
  void setScrollDelay(uint16_t n) {m_nScrollDelay = n;}
  void scroll(bool bClear=false);
  void scrollDrawPixel(int16_t x, int16_t y, uint16_t color);
  void setVerticalScrollArea(uint16_t tfa, uint16_t bfa);
  void posPrintf(int16_t x,int16_t y,const char *fmt,...)
    __attribute__ ((format (print,3,4)));
  void alignPrintf(int16_t x,int16_t y,TextAlign hAlign,
		   TextAlign vAlign,const char *fmt,...)
    __attribute__ ((format (print,5,6)));
  
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
