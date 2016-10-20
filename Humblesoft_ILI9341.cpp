#include "Humblesoft_ILI9341.h"
#include <SPI.h>
#include "uni2sjis.h"

static void dump_var(const char *name, uint32_t value, int base=10)
{
  Serial.print(name);
  Serial.print("=");
  if(base == 16)
    Serial.print("0x");
  Serial.print(value,base);
  Serial.println();
  delay(0);
}


Humblesoft_ILI9341::Humblesoft_ILI9341(int8_t cs, int8_t dc, int8_t rst) :
  Adafruit_ILI9341(cs, dc, rst) {
  m_cs = cs;
  m_dc = dc;
  m_spaceX = 1;
  m_spaceY = 1;
  m_cFontx = 0;
}

void Humblesoft_ILI9341::writedata(uint8_t *data, uint32_t len)
{
  digitalWrite(m_dc, HIGH);
  digitalWrite(m_cs, LOW);
  SPI.writeBytes(data, len);
  digitalWrite(m_cs, HIGH);
}

void Humblesoft_ILI9341::resetFontx(void)
{
  m_cFontx = 0;
}
  
void Humblesoft_ILI9341::addFontx(uint8_t *fontx)
{
  if(m_cFontx < FontxMax && fontx)
    m_aFontx[m_cFontx++] = fontx;
}

void Humblesoft_ILI9341::setFontx(uint8_t *f0, uint8_t *f1, uint8_t *f2)
{
  resetFontx();
  addFontx(f0);
  addFontx(f1);
  addFontx(f2);
}

void Humblesoft_ILI9341::setFont(const GFXfont *f)
{
  resetFontx();
  Adafruit_ILI9341::setFont(f);
}


size_t Humblesoft_ILI9341::write(uint8_t c)
{
  if(m_cFontx)
    process_utf8_byte(c, &cursor_x, &cursor_y);
  else
    Adafruit_ILI9341::write(c);
}

void Humblesoft_ILI9341::process_utf8_byte(uint8_t c, int16_t *pX,
					   int16_t *pY, bool bDraw,
					   int16_t *pX2)
{
  uint32_t ucode;
  const uint8_t *glyph;
  uint8_t w,h;
  uint32_t sjis;
  uint8_t s = textsize < 1 ? 0 : textsize;
  
  if(pX2) *pX2 = *pX;
  
  if(c == '\n'){
    uint8_t h;
    if(getFontxGlyph(' ', NULL, NULL, &h))
      *pY += (h + m_spaceY) * s;
    *pX = 0;
  }
  else {
    if(m_u8d.decode(c, &ucode) && getFontxGlyph(ucode, &glyph, &w, &h)){
      if(wrap && *pX + (w + m_spaceX) * s > _width){
	*pX = 0;
	if(pX2) *pX2 = 0;
	*pY += (h + m_spaceY) * s;
      }
      if(bDraw)
	drawFontxGlyph(glyph, w, h, *pX, *pY);
      *pX += (w + m_spaceX) * textsize;
    }
  }
}

bool Humblesoft_ILI9341::getFontxGlyph (uint16_t code , const uint8_t **pGlyph,
					uint8_t *pw, uint8_t *ph)
{
  uint32_t sjis;

  if(code >= 0x100 && uni2sjis(code, &sjis) != r_ok)
    return false;
  
  for(int i=0; i<m_cFontx; i++){
    const uint8_t *font = m_aFontx[i];
    bool is_ank = pgm_read_byte(&font[16]) == 0;

    uint8_t w = pgm_read_byte(&font[14]);
    uint8_t h = pgm_read_byte(&font[15]);
    uint32_t fsz = (w + 7) / 8 * h;

    if(code < 0x100){
      if(is_ank){
	if(pGlyph) *pGlyph = &font[17 + code * fsz];
	if(pw) *pw = w;
	if(ph) *ph = h;
	return true;
      }
    }
    else {
      unsigned int nc, bc, sb, eb;
      const uint8_t *cblk;

      cblk = &font[18]; nc = 0;
      bc = pgm_read_byte(&font[17]);
      while (bc--) {
	sb = pgm_read_byte(&cblk[0]) + pgm_read_byte(&cblk[1]) * 0x100;  
	eb = pgm_read_byte(&cblk[2]) + pgm_read_byte(&cblk[3]) * 0x100;
	if (sjis >= sb && sjis <= eb) {  
	  nc += sjis - sb;
	  if(pGlyph)
	    *pGlyph = &font[18 + 4 * pgm_read_byte(&font[17]) + nc * fsz];
	  if(pw) *pw = w;
	  if(ph) *ph = h;
	  return true;
	}
	nc += eb - sb + 1;    
	cblk += 4;            
      }
    }
  }
  return false;
}

void
Humblesoft_ILI9341::drawFontxGlyph(const uint8_t *glyph,uint8_t w,uint8_t h,
				   uint16_t cx, uint16_t cy)
{
  const uint8_t *gp;
  uint8_t x,y;
  uint16_t xp, yp;
  uint8_t s = textsize < 1 ? 0 : textsize;

  if(glyph == NULL) return;
  
  gp = glyph;
  yp = cy;
  for (y = 0; y < h; y++) {
    xp = cx;
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
}

void Humblesoft_ILI9341::getTextBounds(char *string, int16_t x0, int16_t y0,
				       int16_t *x1, int16_t *y1,
				       uint16_t *w, uint16_t *h)
{
  if(!m_cFontx)
    Adafruit_ILI9341::getTextBounds(string, x0, y0, x1, y1, w, h);
  else {
    char *p = string;
    int16_t x,y,max_x,max_y,min_x,min_y,x2;

    Serial.println(__FUNCTION__);
    
    x = min_x = max_x = x0;
    y = min_y = max_y = y0;
    while(*p){
      process_utf8_byte(*p++, &x, &y, false, &x2);
      if(x < min_x) min_x = x;
      if(x > max_x) max_x = x;
      if(x2 < min_x) min_x = x2;
      if(x2 > max_x) max_x = x2;
      if(y < min_y) min_y = y;
      if(y > max_y) max_y = y;
    } 
    uint8_t th;
    if(getFontxGlyph(' ', NULL, NULL, &th))
      max_y += th * textsize;

    *x1 = min_x;
    *y1 = min_y;
    *w = max_x - min_x - 1;
    *h = max_y - min_y - 1;
  }
}

void Humblesoft_ILI9341::getTextBounds(const __FlashStringHelper *s,
				       int16_t x0, int16_t y0,
				       int16_t *x1, int16_t *y1,
				       uint16_t *w, uint16_t *h)
{
  if(!m_cFontx)
    Adafruit_ILI9341::getTextBounds(s, x0, y0, x1, y1, w, h);
  else {
    const char *p = (const char *)s;
    char c;
    int16_t x,y,max_x,max_y,min_x,min_y,x2;
    
    x = max_x = min_x = x0;
    y = max_y = min_y = y0;
    c = pgm_read_byte(p++);
    while(c){
      process_utf8_byte(c, &x, &y, false, &x2);
      dump_var("c",c,16);
      dump_var("x",x);
      dump_var("y",y);
      if(x < min_x) min_x = x;
      if(x > max_x) max_x = x;
      if(x2 < min_x) min_x = x2;
      if(x2 > max_x) max_x = x2;
      if(y < min_y) min_y = y;
      if(y > max_y) max_y = y;
      c = pgm_read_byte(p++);
    }
    uint8_t th;
    if(getFontxGlyph(' ', NULL, NULL, &th))
      max_y += th * textsize;

    *x1 = min_x;
    *y1 = min_y;
    *w = max_x - min_x - 1;
    *h = max_y - min_y - 1;

    dump_var("min_x",min_x);
    dump_var("max_x",max_x);
    dump_var("min_y",min_y);
    dump_var("max_y",max_y);
  }
  
}
