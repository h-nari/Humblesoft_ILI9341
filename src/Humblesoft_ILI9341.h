#ifndef _humblesoft_ili9341_h
#define _humblesoft_ili9341_h

#include <Arduino.h>
#include <Humblesoft_GFX.h>
#include <SPI.h>

#define PIXEL_BUF_SIZE	64		// 64byte

#define ILI9341_TFTWIDTH  240
#define ILI9341_TFTHEIGHT 320

#define ILI9341_NOP     0x00
#define ILI9341_SWRESET 0x01
#define ILI9341_RDDID   0x04
#define ILI9341_RDDST   0x09

#define ILI9341_SLPIN   0x10
#define ILI9341_SLPOUT  0x11
#define ILI9341_PTLON   0x12
#define ILI9341_NORON   0x13

#define ILI9341_RDMODE  0x0A
#define ILI9341_RDMADCTL  0x0B
#define ILI9341_RDPIXFMT  0x0C
#define ILI9341_RDIMGFMT  0x0D
#define ILI9341_RDSELFDIAG  0x0F

#define ILI9341_INVOFF  0x20
#define ILI9341_INVON   0x21
#define ILI9341_GAMMASET 0x26
#define ILI9341_DISPOFF 0x28
#define ILI9341_DISPON  0x29

#define ILI9341_CASET   0x2A
#define ILI9341_PASET   0x2B
#define ILI9341_RAMWR   0x2C
#define ILI9341_RAMRD   0x2E

#define ILI9341_PTLAR   0x30
#define ILI9341_MADCTL  0x36
#define ILI9341_PIXFMT  0x3A

#define ILI9341_FRMCTR1 0xB1
#define ILI9341_FRMCTR2 0xB2
#define ILI9341_FRMCTR3 0xB3
#define ILI9341_INVCTR  0xB4
#define ILI9341_DFUNCTR 0xB6

#define ILI9341_PWCTR1  0xC0
#define ILI9341_PWCTR2  0xC1
#define ILI9341_PWCTR3  0xC2
#define ILI9341_PWCTR4  0xC3
#define ILI9341_PWCTR5  0xC4
#define ILI9341_VMCTR1  0xC5
#define ILI9341_VMCTR2  0xC7

#define ILI9341_RDID1   0xDA
#define ILI9341_RDID2   0xDB
#define ILI9341_RDID3   0xDC
#define ILI9341_RDID4   0xDD

#define ILI9341_GMCTRP1 0xE0
#define ILI9341_GMCTRN1 0xE1

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
  // uint16_t rgb(uint8_t r, uint8_t g, uint8_t b) override;
};

class Humblesoft_ILI9341 : public Humblesoft_GFX {
  friend class VerticalScrollArea;
 public:
  VerticalScrollArea tfa;	// Top Fix Area
  VerticalScrollArea vsa;	// Vertical Scroll Area
  VerticalScrollArea bfa;	// Bottom FixArea
  
 protected:
  int8_t m_cs;
  int8_t m_dc;
  uint16_t m_cs_mask;
  uint16_t m_dc_mask;
  int16_t m_nScrollStep;
  int16_t m_nScrollPos;
  uint16_t m_cScrollBg;
  uint16_t m_y0;		// vsa start position
  uint16_t m_y1;		// bfa start position
  uint16_t m_nScrollDelay;

  uint16_t  m_pbi;
  uint8_t m_pixelBuf[PIXEL_BUF_SIZE];
  
 public:
  Humblesoft_ILI9341(int8_t _CS=2, int8_t _DC=15, int8_t _RST = -1);
  void begin();
  void writedata(uint8_t *data, uint32_t len);
  void writecommand(uint8_t c);
  void writedata(uint8_t c);
  uint8_t readdata(void);
  uint8_t readcommand8(uint8_t reg, uint8_t index = 0);

  void drawPixel(int16_t x, int16_t y, uint16_t color) override;
  void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) override;
  void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) override;
  void invertDisplay(boolean i) override;
  void drawBitmap(int16_t x, int16_t y,const uint8_t bitmap[],
		  int16_t w, int16_t h, uint16_t color, uint16_t bg);
  void drawRGBBitmap(int16_t x, int16_t y, const uint16_t bitmap[],
		     int16_t w, int16_t h);
  
  void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
    override;
  void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t color){
    fillRect(x, y, w, h, (uint16_t)color);
  }
  void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, const char *color){
    fillRect(x, y, w, h, rgb(color));
  }
  void setRotation(uint8_t r);
    
  void setScrollBg(uint16_t c) { m_cScrollBg = c;}
  void setScrollStep(int16_t s) {m_nScrollStep = s;}
  void setScrollPos(int16_t s);
  void setScrollDelay(uint16_t n) {m_nScrollDelay = n;}
  void scroll(bool bClear=false);
  void scrollDrawPixel(int16_t x, int16_t y, uint16_t color);
  void setVerticalScrollArea(uint16_t tfa, uint16_t bfa);
  void setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
  inline void spi_begin(void) {
    cs_active();
    // SPI.beginTransaction(SPISettings(24000000, MSBFIRST, SPI_MODE0));
    SPI.beginTransaction(SPISettings(40000000, MSBFIRST, SPI_MODE0));
  }
  inline void spi_end(void) {
    SPI.endTransaction();
    cs_idle();
  }
  inline void spi_write(uint8_t c) {
    SPI.transfer(c);
  }
  
 protected:

  inline void cs_active(void){GPOC=m_cs_mask;};
  inline void cs_idle(void)  {GPOS=m_cs_mask;};
  inline void dc_command(void){GPOC=m_dc_mask;};
  inline void dc_data(void)  {GPOS=m_dc_mask;};
  inline uint8_t spi_read(void) {
    return  SPI.transfer(0x00);
  };
  inline void pixel_write(uint16_t color){
    if(m_pbi > PIXEL_BUF_SIZE-2)
      pixel_flush();
    m_pixelBuf[m_pbi++] = color >> 8;
    m_pixelBuf[m_pbi++] = color;
  };
  inline void pixel_write(uint8_t hi, uint8_t lo){
    if(m_pbi > PIXEL_BUF_SIZE-2)
      pixel_flush();
    m_pixelBuf[m_pbi++] = hi;
    m_pixelBuf[m_pbi++] = lo;
  };
  void pixel_flush(void) {
    if(m_pbi > 0){
      writedata(m_pixelBuf, m_pbi);
      m_pbi = 0;
    }
  };
  void fill_color(uint16_t color, uint32_t len);
  void drawFontxGlyph(const uint8_t *glyph,uint8_t w,uint8_t h,
		      int16_t cx, int16_t cy,
		      uint8_t textsize_x,uint8_t textsize_y, boolean wrap,
		      uint16_t textcolor, uint16_t textbgcolor) override;
};


#endif /* _humblesoft_ili9341_h_ */
