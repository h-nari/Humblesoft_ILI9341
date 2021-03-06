#include "Humblesoft_ILI9341.h"

VerticalScrollArea::VerticalScrollArea(Humblesoft_ILI9341 *parent):
  Humblesoft_GFX(ILI9341_TFTWIDTH, ILI9341_TFTHEIGHT)
{
  m_parent = parent;
}

#if 0
uint16_t VerticalScrollArea::rgb(uint8_t r, uint8_t g, uint8_t b)
{
  return m_parent->rgb(r,g,b);
}
#endif

void VerticalScrollArea::drawPixel(int16_t x, int16_t y, uint16_t color)
{
  if(x >= 0 && x < _width && y >= 0 && y < _height){
    if(this == &m_parent->tfa){
      m_parent->drawPixel(x, y, color);
    } else if(this == &m_parent->vsa){
      int16_t s = m_parent->m_nScrollPos;
      if(s < m_parent->m_y0) s = m_parent->m_y0;
      int16_t yy = y + s;
      if(yy >= m_parent->m_y1) yy += -m_parent->m_y1 + m_parent->m_y0;
      m_parent->drawPixel(x, yy, color);
    } else if(this == &m_parent->bfa){
      m_parent->drawPixel(x, m_parent->m_y1 + y, color);
    }
  }
}

void VerticalScrollArea::fillRect(int16_t x, int16_t y, int16_t w, int16_t h,
				  uint16_t color)
{
  for(int16_t i=0; i<h; i++)
    drawFastHLine(x, y++, w, color);
}

void VerticalScrollArea::drawFastHLine(int16_t x, int16_t y, int16_t w,
				       uint16_t color)
{
  if(y >= 0 && y < _height){ 
    if(this == &m_parent->tfa){
      m_parent->drawFastHLine(x, y, w,color);
    } else if(this == &m_parent->vsa){
      int16_t s = m_parent->m_nScrollPos;
      if(s < m_parent->m_y0) s = m_parent->m_y0;
      int16_t yy = y + s;
      if(yy >= m_parent->m_y1) yy += -m_parent->m_y1 + m_parent->m_y0;
      m_parent->drawFastHLine(x, yy, w, color);
    } else if(this == &m_parent->bfa){
      m_parent->drawFastHLine(x, m_parent->m_y1 + y, w,color);
    }
  }
}

void VerticalScrollArea::lineFeedHook(int16_t *pX, int16_t *pY, int16_t h)
{
  if(this == &m_parent->vsa){
    *pX = 2;

    if(m_parent->m_nScrollPos < m_parent->m_y0)
      m_parent->m_nScrollPos = m_parent->m_y0;
    
    if(*pY + h > m_parent->m_y1 - m_parent->m_y0){
      int ss = *pY + h - m_parent->m_y1 + m_parent->m_y0;
      for(int i=0; i<ss; i++){
	m_parent->drawFastHLine(0, m_parent->m_nScrollPos, _width,
				m_parent->m_cScrollBg);
	m_parent->setScrollPos(m_parent->m_nScrollPos + 1);
	delay(m_parent->m_nScrollDelay);
      }
      *pY -= ss;
    }
  }
}


