#include <Humblesoft_ILI9341.h>
#include <SPI.h>

void Humblesoft_ILI9341::writecommand(uint8_t c) {
  dc_command();
  m_spi->transfer(c);
  dc_data();
}

void Humblesoft_ILI9341::writedata(uint8_t c) { spi_write(c); }

Humblesoft_ILI9341::Humblesoft_ILI9341(int8_t cs, int8_t dc, int8_t rst,
                                       SPIClass *spi)
    : Humblesoft_GFX(ILI9341_TFTWIDTH, ILI9341_TFTHEIGHT),
      tfa(this),
      vsa(this),
      bfa(this) {
  if (spi)
    m_spi = spi;
  else
    m_spi = &SPI;
  log_i("c:%d dc:%d rst:%d", cs, dc, rst);
  m_cs = cs;
  m_cs_mask = 1 << cs;
  m_dc = dc;
  m_dc_mask = 1 << dc;
  m_rst = rst;

  m_nScrollStep = 1;
  m_nScrollPos = 0;
  m_cScrollBg = 0;
  m_nScrollDelay = 10;

  m_pbi = 0;
}

void Humblesoft_ILI9341::begin() {
  pinMode(m_dc, OUTPUT);
  pinMode(m_cs, OUTPUT);
  if (m_rst >= 0) {
    log_i("LCD reset");
    pinMode(m_rst, OUTPUT);
    digitalWrite(m_rst, HIGH);
    delay(10);
    digitalWrite(m_rst, LOW);
    delay(100);
    digitalWrite(m_rst, HIGH);
    delay(10);
  }

  m_spi->begin();
  m_spi->setBitOrder(MSBFIRST);
  m_spi->setDataMode(SPI_MODE0);

  spi_begin();
  writecommand(0xEF);
  writedata(0x03);
  writedata(0x80);
  writedata(0x02);

  writecommand(0xCF);
  writedata(0x00);
  writedata(0XC1);
  writedata(0X30);

  writecommand(0xED);
  writedata(0x64);
  writedata(0x03);
  writedata(0X12);
  writedata(0X81);

  writecommand(0xE8);
  writedata(0x85);
  writedata(0x00);
  writedata(0x78);

  writecommand(0xCB);
  writedata(0x39);
  writedata(0x2C);
  writedata(0x00);
  writedata(0x34);
  writedata(0x02);

  writecommand(0xF7);
  writedata(0x20);

  writecommand(0xEA);
  writedata(0x00);
  writedata(0x00);

  writecommand(ILI9341_PWCTR1);  // Power control
  writedata(0x23);               // VRH[5:0]

  writecommand(ILI9341_PWCTR2);  // Power control
  writedata(0x10);               // SAP[2:0];BT[3:0]

  writecommand(ILI9341_VMCTR1);  // VCM control
  writedata(0x3e);               //¶Ô±È¶Èµ÷½Ú
  writedata(0x28);

  writecommand(ILI9341_VMCTR2);  // VCM control2
  writedata(0x86);               //--

  writecommand(ILI9341_MADCTL);  // Memory Access Control
  writedata(0x48);

  writecommand(ILI9341_PIXFMT);
  writedata(0x55);

  writecommand(ILI9341_FRMCTR1);
  writedata(0x00);
  writedata(0x18);

  writecommand(ILI9341_DFUNCTR);  // Display Function Control
  writedata(0x08);
  writedata(0x82);
  writedata(0x27);

  writecommand(0xF2);  // 3Gamma Function Disable
  writedata(0x00);

  writecommand(ILI9341_GAMMASET);  // Gamma curve selected
  writedata(0x01);

  writecommand(ILI9341_GMCTRP1);  // Set Gamma
  writedata(0x0F);
  writedata(0x31);
  writedata(0x2B);
  writedata(0x0C);
  writedata(0x0E);
  writedata(0x08);
  writedata(0x4E);
  writedata(0xF1);
  writedata(0x37);
  writedata(0x07);
  writedata(0x10);
  writedata(0x03);
  writedata(0x0E);
  writedata(0x09);
  writedata(0x00);

  writecommand(ILI9341_GMCTRN1);  // Set Gamma
  writedata(0x00);
  writedata(0x0E);
  writedata(0x14);
  writedata(0x03);
  writedata(0x11);
  writedata(0x07);
  writedata(0x31);
  writedata(0xC1);
  writedata(0x48);
  writedata(0x08);
  writedata(0x0F);
  writedata(0x0C);
  writedata(0x31);
  writedata(0x36);
  writedata(0x0F);

  writecommand(ILI9341_SLPOUT);  // Exit Sleep
  spi_end();
  delay(120);
  spi_begin();
  writecommand(ILI9341_DISPON);  // Display on
  spi_end();

  setVerticalScrollArea(0, 0);
}

void Humblesoft_ILI9341::invertDisplay(boolean i) {
  spi_begin();
  writecommand(i ? ILI9341_INVON : ILI9341_INVOFF);
  spi_end();
}

uint8_t Humblesoft_ILI9341::readdata(void) { return spi_read(); }

uint8_t Humblesoft_ILI9341::readcommand8(uint8_t c, uint8_t index) {
  spi_begin();
  dc_command();
  spi_write(0xD9);
  dc_data();
  spi_write(0x10 + index);

  dc_command();
  spi_write(c);
  dc_data();

  uint8_t r = spi_read();

  spi_end();
  return r;
}

#define MADCTL_MY 0x80
#define MADCTL_MX 0x40
#define MADCTL_MV 0x20
#define MADCTL_ML 0x10
#define MADCTL_RGB 0x00
#define MADCTL_BGR 0x08
#define MADCTL_MH 0x04

void Humblesoft_ILI9341::setRotation(uint8_t r) {
  Humblesoft_GFX::setRotation(r);

  spi_begin();
  writecommand(ILI9341_MADCTL);
  rotation = r % 4;
  switch (rotation) {
    case 0:
      writedata(MADCTL_MX | MADCTL_BGR);
      _width = ILI9341_TFTWIDTH;
      _height = ILI9341_TFTHEIGHT;
      break;
    case 1:
      writedata(MADCTL_MV | MADCTL_BGR);
      _width = ILI9341_TFTHEIGHT;
      _height = ILI9341_TFTWIDTH;
      break;
    case 2:
      writedata(MADCTL_MY | MADCTL_BGR);
      _width = ILI9341_TFTWIDTH;
      _height = ILI9341_TFTHEIGHT;
      break;
    case 3:
      writedata(MADCTL_MX | MADCTL_MY | MADCTL_MV | MADCTL_BGR);
      _width = ILI9341_TFTHEIGHT;
      _height = ILI9341_TFTWIDTH;
      break;
  }
  spi_end();
}

void Humblesoft_ILI9341::setScrollPos(int16_t s) {
  while (s < m_y0) s += m_y1 - m_y0;
  while (s >= m_y1) s += m_y0 - m_y1;
  m_nScrollPos = s;

  spi_begin();
  writecommand(0x37);  // VSCRSADD command
  writedata((uint8_t)(s >> 8));
  writedata((uint8_t)s);
  spi_end();
}

void Humblesoft_ILI9341::scroll(bool bClear) {
  if (bClear) {
    uint8_t rot0 = getRotation();
    setRotation(0);
    int16_t i, j;
    if (m_nScrollStep > 0) {
      for (i = 0; i < m_nScrollStep; i++) {
        j = m_nScrollPos + i;
        if (j > ILI9341_TFTHEIGHT) j -= ILI9341_TFTHEIGHT;
        drawLine(0, j, _width, j, m_cScrollBg);
      }
    } else if (m_nScrollStep < 0) {
      for (i = 1; i >= m_nScrollStep; i--) {
        j = m_nScrollPos + i;
        if (j < 0) j += ILI9341_TFTHEIGHT;
        drawLine(0, j, _width, j, m_cScrollBg);
      }
    }
    setRotation(rot0);
  }
  setScrollPos(m_nScrollPos + m_nScrollStep);
}

void Humblesoft_ILI9341::scrollDrawPixel(int16_t x, int16_t y, uint16_t color) {
  int16_t xx;
  if (m_nScrollStep > 0)
    xx = m_nScrollPos - m_nScrollStep + x;
  else if (m_nScrollStep < 0)
    xx = m_nScrollPos - m_nScrollStep + x + 1;
  else
    xx = x;
  if (xx < 0)
    xx += ILI9341_TFTHEIGHT;
  else if (xx > ILI9341_TFTHEIGHT)
    xx -= ILI9341_TFTHEIGHT;

  uint8_t rot0 = getRotation();
  setRotation(0);

  drawPixel(y, xx, color);
  setRotation(rot0);
}

void Humblesoft_ILI9341::setVerticalScrollArea(uint16_t hTfa, uint16_t hBfa) {
  if (hTfa + hBfa < ILI9341_TFTHEIGHT) {
    uint16_t hVsa = ILI9341_TFTHEIGHT - hTfa - hBfa;
    m_y0 = hTfa;
    m_y1 = ILI9341_TFTHEIGHT - hBfa;

    tfa._height = hTfa;
    vsa._height = hVsa;
    bfa._height = hBfa;

    spi_begin();
    writecommand(0x33);  // VSCRSADD command
    writedata(hTfa >> 8);
    writedata(hTfa);
    writedata(hVsa >> 8);
    writedata(hVsa);
    writedata(hBfa >> 8);
    writedata(hBfa);
    spi_end();

    setScrollPos(m_y0);
  }
}

void Humblesoft_ILI9341::writedata(uint8_t *data, uint32_t len) {
  // cs_active();
  m_spi->writeBytes(data, len);
  // cs_idle();
}

void Humblesoft_ILI9341::drawPixel(int16_t x, int16_t y, uint16_t color) {
  if ((x < 0) || (x >= _width) || (y < 0) || (y >= _height)) return;

  spi_begin();
  setAddrWindow(x, y, x + 1, y + 1);

  spi_write(color >> 8);
  spi_write(color);
  spi_end();
}

void Humblesoft_ILI9341::drawFastVLine(int16_t x, int16_t y, int16_t h,
                                       uint16_t color) {
  // Rudimentary clipping
  if ((x >= _width) || (y >= _height)) return;

  if ((y + h - 1) >= _height) h = _height - y;

  spi_begin();
  setAddrWindow(x, y, x, y + h - 1);
  fill_color(color, h);
  spi_end();
}

void Humblesoft_ILI9341::drawFastHLine(int16_t x, int16_t y, int16_t w,
                                       uint16_t color) {
  // Rudimentary clipping
  if ((x >= _width) || (y >= _height)) return;
  if ((x + w - 1) >= _width) w = _width - x;

  spi_begin();
  setAddrWindow(x, y, x + w - 1, y);
  fill_color(color, w);
  spi_end();
}

// fill a rectangle
void Humblesoft_ILI9341::fillRect(int16_t x, int16_t y, int16_t w, int16_t h,
                                  uint16_t color) {
  if (w > 0 && h > 0) {
    // rudimentary clipping (drawChar w/big text requires this)
    if ((x >= _width) || (y >= _height)) return;
    if ((x + w - 1) >= _width) w = _width - x;
    if ((y + h - 1) >= _height) h = _height - y;

    spi_begin();
    setAddrWindow(x, y, x + w - 1, y + h - 1);
    fill_color(color, w * h);
    spi_end();
  }
}

void Humblesoft_ILI9341::drawBitmap(int16_t x, int16_t y,
                                    const uint8_t bitmap[], int16_t w,
                                    int16_t h, uint16_t color, uint16_t bg) {
  int16_t xi, yi;
  const uint8_t *p = bitmap;

  spi_begin();
  setAddrWindow(x, y, x + w - 1, y + h - 1);
  for (yi = 0; yi < h; yi++) {
    for (xi = 0; xi < w; xi += 8) {
      uint8_t m = 0x80;
      uint8_t d = *p++;
      for (int i = 0; i < 8 && xi + i < w; i++, m >>= 1)
        pixel_write(m & d ? color : bg);
    }
  }
  pixel_flush();
  spi_end();
}

void Humblesoft_ILI9341::drawRGBBitmap(int16_t x, int16_t y,
                                       const uint16_t bitmap[], int16_t w,
                                       int16_t h) {
  int16_t xi, yi;
  const uint16_t *p = bitmap;

  spi_begin();
  setAddrWindow(x, y, x + w - 1, y + h - 1);
  for (yi = 0; yi < h; yi++) {
    for (xi = 0; xi < w; xi++) pixel_write(*p++);
  }
  pixel_flush();
  spi_end();
}

void Humblesoft_ILI9341::setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1,
                                       uint16_t y1) {
  dc_command();
  spi_write(ILI9341_CASET);  // Column addr set
  dc_data();
  spi_write(x0 >> 8);
  spi_write(x0);
  spi_write(x1 >> 8);
  spi_write(x1);

  dc_command();
  spi_write(ILI9341_PASET);  // Row addr set
  dc_data();
  spi_write(y0 >> 8);
  spi_write(y0);  // YSTART
  spi_write(y1 >> 8);
  spi_write(y1);  // YEND

  dc_command();
  spi_write(ILI9341_RAMWR);  // write to RAM
  dc_data();
}

void Humblesoft_ILI9341::fill_color(uint16_t color, uint32_t len) {
  uint8_t hi = color >> 8;
  uint8_t lo = color;
  for (uint32_t i = 0; i < len; i++) pixel_write(hi, lo);
  pixel_flush();
}

void Humblesoft_ILI9341::drawFontxGlyph(const uint8_t *glyph, uint8_t w,
                                        uint8_t h, int16_t cx, int16_t cy,
                                        uint8_t textsize_x, uint8_t textsize_y,
                                        boolean /* wrap */, uint16_t textcolor,
                                        uint16_t textbgcolor) {
  if (glyph == NULL)
    return;
  else if (textcolor == textbgcolor)
    Humblesoft_GFX::drawFontxGlyph(glyph, w, h, cx, cy, textsize_x, textsize_y,
                                   wrap, textcolor, textbgcolor);
  else {
    const uint8_t *gp = glyph;
    uint8_t sx = textsize_x < 1 ? 1 : textsize_x;
    uint8_t sy = textsize_y < 1 ? 1 : textsize_y;
    int16_t x0 = cx;
    int16_t y0 = cy;
    int16_t x1 = x0 + w * sx;
    int16_t y1 = y0 + h * sy;

    if (x0 < _width && y0 < _height && x1 > 0 && y1 > 0) {
      if (x0 < 0) x0 = 0;
      if (y0 < 0) y0 = 0;
      if (x1 > _width) x1 = _width;
      if (y1 > _height) y1 = _height;

      spi_begin();
      setAddrWindow(x0, y0, x1 - 1, y1 - 1);

      int yp = cy;
      for (int y = 0; y < h; y++) {
        for (int yi = 0; yi < sy; yi++, yp++) {
          if (yp >= y0 && yp < y1) {
            int xp = cx;
            for (int x = 0; x < w; x += 8) {
              uint8_t d = pgm_read_byte(&gp[x / 8]);
              for (int i = 0, m = 0x80; i < 8 && i + x < w; i++, m >>= 1) {
                for (int xi = 0; xi < sx; xi++, xp++)
                  if (xp >= x0 && xp < x1)
                    pixel_write(d & m ? textcolor : textbgcolor);
              }
            }
          }
        }
        gp += (w + 7) / 8;
      }
      pixel_flush();
      spi_end();
    }
  }
}
