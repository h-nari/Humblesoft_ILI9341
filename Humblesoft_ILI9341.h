#ifndef _humblesoft_ili9341_h
#define _humblesoft_ili9341_h

#include "Arduino.h"
#include "Adafruit_ILI9341.h"
#include "Utf8Decoder.h"

#define IMPORT_BIN(file,sym) asm (					\
				  ".section \".irom.text\"\n"		\
				  ".balign 4\n"				\
				  ".global " #sym "\n"			\
				  #sym ":\n"				\
				  ".incbin \"" file "\"\n"		\
				  ".global sizeof_" #sym "\n"		\
				  ".set _sizeof_" #sym ", . - " #sym "\n" \
				  ".balign 4\n"				\
				  ".section \".text\"\n")

#define FontxMax	3

class Humblesoft_ILI9341 : public Adafruit_ILI9341 {
 public:
  uint8_t m_spaceX;
  uint8_t m_spaceY;
  
 protected:
  int32_t m_cs,m_dc;
  uint8_t  m_cFontx;
  uint8_t *m_aFontx[FontxMax];
  // uint8_t *m_fontH;
  // uint8_t *m_fontZ;
  Utf8Decoder m_u8d;

 public:
  Humblesoft_ILI9341(int8_t _CS=2, int8_t _DC=15, int8_t _RST = -1);
  void writedata(uint8_t *data, uint32_t len);

  virtual size_t write(uint8_t);
  void resetFontx(void);
  void addFontx(uint8_t *fontx);
  void setFontx(uint8_t *f0, uint8_t *f1=NULL, uint8_t *f2=NULL);
  void setFont(const GFXfont *f = NULL);
  void getTextBounds(char *string, int16_t x, int16_t y,
		     int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h);
  void getTextBounds(const __FlashStringHelper *s, int16_t x, int16_t y,
		     int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h);
  
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
