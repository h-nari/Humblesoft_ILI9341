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


class Humblesoft_ILI9341 : public Adafruit_ILI9341 {
 public:
  uint8_t m_spaceX;
  uint8_t m_spaceY;
  
 protected:
  int32_t m_cs,m_dc;
  uint8_t *m_fontH;
  uint8_t *m_fontZ;
  Utf8Decoder m_u8d;

 public:
  Humblesoft_ILI9341(int8_t _CS=2, int8_t _DC=15, int8_t _RST = -1);
  void writedata(uint8_t *data, uint32_t len);

  virtual size_t write(uint8_t);
  void setFontx(uint8_t *font_h, uint8_t *font_z) {
    m_fontH = font_h;
    m_fontZ = font_z;
  }
  
 protected:
  void process_utf8_byte(uint8_t c);
  const uint8_t * getFontxGlyph (const uint8_t* font, uint16_t code ,
				 uint8_t *pw, uint8_t *ph);
  void drawFontxGlyph(const uint8_t *glyph,uint8_t w,uint8_t h);
};


#endif /* _humblesoft_ili9341_h_ */
