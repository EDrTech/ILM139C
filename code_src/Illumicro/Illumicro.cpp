#include "Illumicro.h"

// LED mapping table: ILM139C[row][led], row=0-8, led=0-38 (13 pixels × 3 colors)
const uint8_t Illumicro::ILM139C[9][39] =
{
  {
    0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,0x5A,0x5B,0x5C,0x5D,0x5E,0x5F,0x60,0x61,0x62
  },
  {
    0x1E,0x1F,0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,0x2E,0x2F,0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6A,0x6B
  },
  {
    0x3C,0x3D,0x3E,0x3F,0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x4B,0x4C,0x4D,0x4E,0x4F,0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x6C,0x6D,0x6E,0x6F,0x70,0x71,0x72,0x73,0x74
  },
  {
    0x5A,0x5B,0x5C,0x5D,0x5E,0x5F,0x60,0x61,0x62,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6A,0x6B,0x6C,0x6D,0x6E,0x6F,0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77,0x75,0x76,0x77,0x78,0x79,0x7A,0x7B,0x7C,0x7D
  },
  {
    0x78,0x79,0x7A,0x7B,0x7C,0x7D,0x7E,0x7F,0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8A,0x8B,0x8C,0x8D,0x8E,0x8F,0x90,0x91,0x92,0x93,0x94,0x95,0x7E,0x7F,0x80,0x81,0x82,0x83,0x84,0x85,0x86
  },
  {
    0x96,0x97,0x98,0x99,0x9A,0x9B,0x9C,0x9D,0x9E,0x9F,0xA0,0xA1,0xA2,0xA3,0xA4,0xA5,0xA6,0xA7,0xA8,0xA9,0xAA,0xAB,0xAC,0xAD,0xAE,0xAF,0xB0,0xB1,0xB2,0xB3,0x87,0x88,0x89,0x8A,0x8B,0x8C,0x8D,0x8E,0x8F
  },
  {
    0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,0x98
  },
  {
    0x1E,0x1F,0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,0x2E,0x2F,0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x99,0x9A,0x9B,0x9C,0x9D,0x9E,0x9F,0xA0,0xA1
  },
  {
    0x3C,0x3D,0x3E,0x3F,0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x4B,0x4C,0x4D,0x4E,0x4F,0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0xA2,0xA3,0xA4,0xA5,0xA6,0xA7,0xA8,0xA9,0xAA
  }
};

Illumicro::Illumicro(uint8_t addr)
  : _addr(addr), _gcc(32)
{
}

void Illumicro::begin()
{
  Wire.begin();
  Wire.setClock(1000000);
  selectPage(4);
  writeReg(0x3F, 0xAE); // Reset
  writeReg(0x00, 0x01); // Normal operation
  writeReg(0x01, _gcc); // GCC = 127
  writeReg(0x02, 0x55); // 8kΩ pull resistors
  writeTwoPages(2, 32); // Default scaling
}

void Illumicro::setLED(uint8_t row, uint8_t col, uint8_t color, uint8_t pwm)
{
  if (row > 8 || col > 12 || color > 2)
    return;
  uint8_t addr = ILM139C[row][col * 3 + color];
  int8_t page;
  if (row >= 6 || col >= 10)
  {
    page = 1;
  }
  else
  {
    page = 0;
  }
  selectPage(page);
  writeReg(addr, pwm);
}

void Illumicro::setGCC(uint8_t gcc)
{
  _gcc = gcc;
  selectPage(4);
  writeReg(0x01, _gcc);
}

void Illumicro::setGlobalMaxCurrentPercent(uint8_t percent, uint16_t r_iset)
{
  if (percent > 100 || r_iset == 0)
    return;
  uint8_t sl = (uint16_t)(percent * 255) / 100;
  writeTwoPages(2, sl);
}

void Illumicro::setLEDMaxCurrent(uint8_t row, uint8_t col, uint8_t color, float current_mA, uint16_t r_iset)
{
  if (row > 8 || col > 12 || color > 2 || current_mA <= 0 || r_iset == 0)
    return;
  float alpha = (255.0 / 256.0) * (1.0 / 10.125);
  float i_out_peak = current_mA / alpha;
  float sl_float = i_out_peak * 256.0 * r_iset / (383.0 * (_gcc / 256.0));
  uint8_t sl = min(max(static_cast<int>(sl_float + 0.5), 0), 255);
  uint8_t addr = ILM139C[row][col * 3 + color];
  uint8_t page;
  if (row >= 6 || col >= 10)
  {
    page = 1;
  }
  else
  {
    page = 0;
  }
  selectPage(page);
  writeReg(addr, sl);
}

void Illumicro::pushFrame(uint8_t frame[9][13][3])
{
  uint8_t buffer[351] = {0};
  for (uint8_t row = 0; row < 9; row++)
  {
    for (uint8_t col = 0; col < 13; col++)
    {
      for (uint8_t color = 0; color < 3; color++)
      {
        uint8_t addr = ILM139C[row][col * 3 + color];
        uint8_t page;
        if (row >= 6 || col >= 10)
        {
          page = 1;
        }
        else
        {
          page = 0;
        }
        uint16_t buffer_index = (page == 0) ? addr : (180 + addr);
        buffer[buffer_index] = frame[row][col][color];
      }
    }
  }
  writeBlock(0, 0x00, buffer, 180);
  writeBlock(1, 0x00, buffer + 180, 171);
}

uint8_t Illumicro::getRegisterAddress(uint8_t row, uint8_t col, uint8_t color)
{
  if (row > 8 || col > 12 || color > 2)
    return 0;
  return ILM139C[row][col * 3 + color];
}

void Illumicro::demo()
{
  for (uint8_t step = 0; step < 200; step++)
  {
    uint8_t offset = step * 5;
    for (uint8_t row = 0; row < 9; row++)
    {
      for (uint8_t col = 0; col < 13; col++)
      {
        uint8_t h = (row * 28 + col * 20 + offset) % 255;
        uint8_t r, g, b;
        hsvToRgb(h, 255, 255, r, g, b);
        setLED(row, col, 0, b);
        setLED(row, col, 1, g);
        setLED(row, col, 2, r);
      }
    }
    delay(50);
  }
}

void Illumicro::demoBreathing()
{
  for (uint8_t row = 0; row < 9; row++)
  {
    for (uint8_t col = 0; col < 13; col++)
    {
      setLED(row, col, 0, 255);
      setLED(row, col, 1, 255);
      setLED(row, col, 2, 255);
    }
  }
  for (uint8_t step = 0; step < 200; step++)
  {
    float angle = (PI * step) / 200.0;
    uint8_t percent = (uint8_t)(sin(angle) * sin(angle) * 100.0);
    setGlobalMaxCurrentPercent(percent, 2550);
    delay(10);
  }
  for (uint8_t row = 0; row < 9; row++)
  {
    for (uint8_t col = 0; col < 13; col++)
    {
      setLED(row, col, 0, 0);
      setLED(row, col, 1, 0);
      setLED(row, col, 2, 0);
    }
  }
}

void Illumicro::hsvToRgb(uint8_t h, uint8_t s, uint8_t v, uint8_t &r, uint8_t &g, uint8_t &b)
{
  if (s == 0)
  {
    r = v;
    g = v;
    b = v;
    return;
  }
  uint8_t region = h / 43;
  uint8_t remainder = (h - (region * 43)) * 6;
  uint8_t p = (v * (255 - s)) >> 8;
  uint8_t q = (v * (255 - ((s * remainder) >> 8))) >> 8;
  uint8_t t = (v * (255 - ((s * (255 - remainder)) >> 8))) >> 8;
  switch (region)
  {
    case 0:
      r = v;
      g = t;
      b = p;
      break;
    case 1:
      r = q;
      g = v;
      b = p;
      break;
    case 2:
      r = p;
      g = v;
      b = t;
      break;
    case 3:
      r = p;
      g = q;
      b = v;
      break;
    case 4:
      r = t;
      g = p;
      b = v;
      break;
    default:
      r = v;
      g = p;
      b = q;
      break;
  }
}

void Illumicro::unlock()
{
  Wire.beginTransmission(_addr);
  Wire.write(0xFE);
  Wire.write(0xC5);
  Wire.endTransmission();
}

void Illumicro::selectPage(uint8_t page)
{
  unlock();
  Wire.beginTransmission(_addr);
  Wire.write(0xFD);
  Wire.write(page);
  Wire.endTransmission();
}

void Illumicro::writeReg(uint8_t reg, uint8_t val)
{
  Wire.beginTransmission(_addr);
  Wire.write(reg);
  Wire.write(val);
  Wire.endTransmission();
}

void Illumicro::writeBlock(uint8_t page, uint8_t startAddr, const uint8_t* data, uint16_t len)
{
  selectPage(page);
  uint16_t offset = 0;
  uint8_t addr = startAddr;
  while (len)
  {
    uint8_t bytesThisPass = min(len, 30);
    Wire.beginTransmission(_addr);
    Wire.write(addr);
    for (uint8_t i = 0; i < bytesThisPass; i++)
    {
      Wire.write(data[offset + i]);
    }
    Wire.endTransmission();
    offset += bytesThisPass;
    len -= bytesThisPass;
    addr += bytesThisPass;
  }
}

void Illumicro::writeTwoPages(uint8_t first_page, uint8_t value)
{
  uint8_t buf[32];
  memset(buf, value, sizeof buf);
  uint8_t page_bytes = 180;
  for (uint8_t page = 0; page < 2; page++)
  {
    selectPage(first_page + page);
    uint8_t addr = 0;
    while (page_bytes)
    {
      uint8_t bytesThisPass = min((int)page_bytes, 31);
      buf[0] = addr;
      Wire.beginTransmission(_addr);
      Wire.write(buf, bytesThisPass + 1);
      Wire.endTransmission();
      page_bytes -= bytesThisPass;
      addr += bytesThisPass;
    }
    page_bytes = 255;
  }
}
