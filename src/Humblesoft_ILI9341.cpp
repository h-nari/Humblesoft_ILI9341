#include "Humblesoft_ILI9341.h"
#include <SPI.h>
// #include "uni2sjis.h"

Humblesoft_ILI9341::Humblesoft_ILI9341(int8_t cs, int8_t dc, int8_t rst) :
  tfa(this), vsa(this), bfa(this),
  m_fontx(this),Adafruit_ILI9341(cs, dc, rst) {
  m_cs = cs;
  m_dc = dc;
  
  m_nScrollStep = 1;
  m_nScrollPos = 0;
  m_cScrollBg = ILI9341_BLACK;
  m_nScrollDelay = 10;
}

void Humblesoft_ILI9341::begin()
{
  Adafruit_ILI9341::begin();
  setVerticalScrollArea(0,0);
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
  m_fontx.resetFontx();
}
  
void Humblesoft_ILI9341::addFontx(uint8_t *fontx)
{
  m_fontx.addFontx(fontx);
}

void Humblesoft_ILI9341::setFontx(uint8_t *f0, uint8_t *f1, uint8_t *f2)
{
  m_fontx.setFontx(f0,f1,f2);
}

void Humblesoft_ILI9341::setFont(const GFXfont *f)
{
  resetFontx();
  Adafruit_ILI9341::setFont(f);
}


size_t Humblesoft_ILI9341::write(uint8_t c)
{
  m_fontx.write(c, textsize, wrap, textcolor, textbgcolor, gfxFont);
}


void Humblesoft_ILI9341::getTextBounds(char *string, int16_t x0, int16_t y0,
				       int16_t *x1, int16_t *y1,
				       uint16_t *w, uint16_t *h)
{
  m_fontx.getTextBounds(string, x0, y0, x1, y1, w, h, textsize, wrap);
}

void Humblesoft_ILI9341::setScrollPos(int16_t s)
{
  while(s < m_y0) s += m_y1 - m_y0;
  while(s >= m_y1) s += m_y0 - m_y1;
  m_nScrollPos = s;
  
  writecommand(0x37);	// VSCRSADD command
  Adafruit_ILI9341::writedata((uint8_t)(s >> 8));
  Adafruit_ILI9341::writedata((uint8_t)s);
}

void Humblesoft_ILI9341::scroll(bool bClear)
{
  if(bClear){
    uint8_t rot0 = getRotation();
    setRotation(0);
    int16_t i,j;
    if(m_nScrollStep > 0){
      for(i=0; i<m_nScrollStep; i++){
	j = m_nScrollPos + i;
	if(j > ILI9341_TFTHEIGHT) j -= ILI9341_TFTHEIGHT;
	drawLine(0, j, _width, j, m_cScrollBg);
      }
    }
    else if(m_nScrollStep < 0){
      for(i=1; i>= m_nScrollStep; i--){
	j = m_nScrollPos + i;
	if(j < 0) j += ILI9341_TFTHEIGHT;
	drawLine(0, j, _width, j, m_cScrollBg);
      }
    }
    setRotation(rot0);
  }
  setScrollPos(m_nScrollPos + m_nScrollStep);
}

void Humblesoft_ILI9341::scrollDrawPixel(int16_t x, int16_t y, uint16_t color)
{
  int16_t xx;
  if(m_nScrollStep > 0)
    xx = m_nScrollPos - m_nScrollStep + x;
  else if(m_nScrollStep < 0)
    xx = m_nScrollPos - m_nScrollStep + x + 1;
  if(xx < 0) xx += ILI9341_TFTHEIGHT;
  else if(xx > ILI9341_TFTHEIGHT) xx -= ILI9341_TFTHEIGHT;

  uint8_t rot0 = getRotation();
  setRotation(0);
  
  drawPixel(y, xx, color);
  setRotation(rot0);
}

void Humblesoft_ILI9341::setVerticalScrollArea(uint16_t hTfa, uint16_t hBfa)
{
  if(hTfa + hBfa < ILI9341_TFTHEIGHT){
    uint16_t hVsa = ILI9341_TFTHEIGHT - hTfa - hBfa;
    m_y0 = hTfa;
    m_y1 = ILI9341_TFTHEIGHT - hBfa;

    tfa._height = hTfa;
    vsa._height = hVsa;
    bfa._height = hBfa;

    writecommand(0x33);	// VSCRSADD command
    Adafruit_ILI9341::writedata(hTfa >> 8);
    Adafruit_ILI9341::writedata(hTfa);
    Adafruit_ILI9341::writedata(hVsa >> 8);
    Adafruit_ILI9341::writedata(hVsa);
    Adafruit_ILI9341::writedata(hBfa >> 8);
    Adafruit_ILI9341::writedata(hBfa);
  }
}


