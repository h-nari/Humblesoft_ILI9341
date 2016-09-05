#include "Humblesoft_ILI9341.h"
#include <SPI.h>
#include "uni2sjis.h"

Humblesoft_ILI9341::Humblesoft_ILI9341(int8_t cs, int8_t dc, int8_t rst) :
  Adafruit_ILI9341(cs, dc, rst) {
  m_cs = cs;
  m_dc = dc;
  m_spaceX = 1;
  m_spaceY = 1;
  m_fontH = m_fontZ = NULL;
}

void Humblesoft_ILI9341::writedata(uint8_t *data, uint32_t len)
{
  digitalWrite(m_dc, HIGH);
  digitalWrite(m_cs, LOW);
  SPI.writeBytes(data, len);
  digitalWrite(m_cs, HIGH);
}

size_t Humblesoft_ILI9341::write(uint8_t c)
{
  if(m_fontH && m_fontZ)
    process_utf8_byte(c);
  else
    Adafruit_ILI9341::write(c);
}

void Humblesoft_ILI9341::process_utf8_byte(uint8_t c)
{
  uint32_t ucode;
  const uint8_t *glyph = NULL;
  uint8_t w,h;
  uint32_t sjis;
  uint8_t s = textsize < 1 ? 0 : textsize;
    
  if(c == '\n'){
    if(m_fontH){
      uint8_t h = pgm_read_byte(&m_fontH[15]);
      cursor_y += (h + m_spaceY) * s;
      cursor_x = 0;
    }
  }
  else {
    if(m_u8d.decode(c, &ucode)){
      if(ucode < 0x100)
	glyph = getFontxGlyph(m_fontH, ucode, &w, &h);
      else {
	if (uni2sjis(ucode, &sjis) == r_ok) {
	  glyph = getFontxGlyph(m_fontZ, sjis, &w, &h);
	} else
	  glyph = NULL;
      }
    }
    if(glyph) drawFontxGlyph(glyph, w, h);
  }
}

const uint8_t * Humblesoft_ILI9341::getFontxGlyph (const uint8_t* font, uint16_t code ,
					   uint8_t *pw, uint8_t *ph)
{
  unsigned int nc, bc, sb, eb;
  uint32_t fsz;
  const uint8_t *cblk;
  uint8_t w = pgm_read_byte(&font[14]);
  uint8_t h = pgm_read_byte(&font[15]);

  fsz = (w + 7) / 8 * h;  /* Get font size */
  if(pw) *pw = w;
  if(ph) *ph = h;

  if (pgm_read_byte(&font[16]) == 0) { 
    if (code < 0x100)
      return &font[17 + code * fsz];
  } else {      
    cblk = &font[18]; nc = 0;
    bc = pgm_read_byte(&font[17]);
    while (bc--) {
      /* Get range of the code block */
      sb = pgm_read_byte(&cblk[0]) + pgm_read_byte(&cblk[1]) * 0x100;  
      eb = pgm_read_byte(&cblk[2]) + pgm_read_byte(&cblk[3]) * 0x100;
      if (code >= sb && code <= eb) {  
	nc += code - sb;             
	return &font[18 + 4 * pgm_read_byte(&font[17]) + nc * fsz];
      }
      nc += eb - sb + 1;    
      cblk += 4;            
    }
  }
  return NULL;   
}

void
Humblesoft_ILI9341::drawFontxGlyph(const uint8_t *glyph,uint8_t w,uint8_t h)
{
  const uint8_t *gp;
  uint8_t x,y;
  uint16_t xp, yp;
  uint8_t s = textsize < 1 ? 0 : textsize;

  gp = glyph;
  yp = cursor_y;
  for (y = 0; y < h; y++) {
    xp = cursor_x;
    for (x = 0; x < w; x += 8) {
      uint8_t d = pgm_read_byte(&gp[x / 8]);
      for (int i = 0, m = 0x80; i < 8 && i + x < w; i++, m >>= 1) {
	if (d & m)
	  fillRect(xp, yp, s, s , textcolor);
	else if(textcolor != textbgcolor)
	  fillRect(xp, yp, s, s , textbgcolor);
	xp += s;
      }
    }
    gp += (w + 7) / 8;
    yp += s;
  }
  cursor_x += (w + m_spaceX) * textsize;
}
