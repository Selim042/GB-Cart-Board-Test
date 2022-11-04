#include "GBA.h"
#include "Gen3SaveSectionConstants.h"

GBA::GBA(uint8_t adrsAdrs, uint8_t dataAdrs) {
  cartAdrs.begin(adrsAdrs);
  cartData.begin(dataAdrs);
  resetMCPs();

  switchBank(0);
}

void GBA::writeData(uint8_t value) {
  for (uint8_t b = 0; b < 8; b++) {
    cartData.digitalWrite(b + 8, (value >> b) & 1);
  }
}


void GBA::rawWriteCycle(uint16_t adrs, uint8_t value) {
  for (uint8_t b = 8; b < 16; b++)
    cartData.pinMode(b, OUTPUT);
  cartAdrs.writeGPIOAB(adrs);
  writeData(value);
  
  cartData.digitalWrite(GB_WRITE_SELECT, LOW);
  cartData.digitalWrite(GB_CHIP_SELECT2, LOW);
  //cartData.digitalWrite(GB_READ_SELECT, LOW);
  delayMicroseconds(IO_DELAY);
  cartData.digitalWrite(GB_WRITE_SELECT, HIGH);
  cartData.digitalWrite(GB_CHIP_SELECT2, HIGH);
  //cartData.digitalWrite(GB_READ_SELECT, HIGH);
}

void GBA::switchBank(uint8_t bank) {
  flashCmd(FlashCmd::FLASH_CMD_SWITCH_BANK);
  rawWriteCycle(0x0000, bank);
}

void GBA::flashCmd(FlashCmd cmd) {
  rawWriteCycle(0x5555, 0xAA);
  rawWriteCycle(0x2AAA, 0x55);
  rawWriteCycle(0x5555, cmd);
}

void GBA::resetMCPs() {
  digitalWrite(RESET_PIN, LOW);
  digitalWrite(RESET_PIN, HIGH);

  cartData.pinMode(GB_CHIP_SELECT, OUTPUT);
  cartData.pinMode(GB_WRITE_SELECT, OUTPUT);
  cartData.pinMode(GB_READ_SELECT, OUTPUT);
  cartData.pinMode(IO_LED, OUTPUT);
  cartData.pinMode(GB_CHIP_SELECT2, OUTPUT);
  cartData.digitalWrite(GB_CHIP_SELECT2, HIGH);

  for (int i = 0; i < 16; i++)
    cartAdrs.pinMode(i, OUTPUT);
  for (int i = 0; i < 8; i++)
    cartData.pinMode(i, OUTPUT);
  for (int i = 8; i < 16; i++)
    cartData.pinMode(i, INPUT);
}

uint8_t GBA::readByte(uint32_t adrs) {
  if (adrs >= 0x8000000 && adrs <= 0xDFFFFFF)
    return readROMByte(adrs);
  else if (adrs >= 0xE000000 && adrs <= 0xE00FFFF)
    return readSRAMByte(adrs);
  return 0x00;
}


uint8_t GBA::readROMByte(uint32_t adrs) {
  bool odd = adrs & 1;
  adrs = adrs / 2;
  uint16_t lowAdrs = adrs;
  uint8_t highAdrs = adrs >> 16;
  for (uint8_t i = 0; i < 16; i++)
    cartAdrs.pinMode(i, OUTPUT);
  cartAdrs.writeGPIOAB(lowAdrs);
  for (uint8_t b = 0; b < 8; b++) {
    cartData.pinMode(b + 8, OUTPUT);
    cartData.digitalWrite(b + 8, (highAdrs >> b) & 1);
  }
  
  cartData.digitalWrite(GB_CHIP_SELECT, HIGH);
  delayMicroseconds(IO_DELAY);
  cartData.digitalWrite(GB_CHIP_SELECT, LOW);
  for (uint8_t i = 0; i < 16; i++)
    cartAdrs.pinMode(i, INPUT);
  cartData.digitalWrite(GB_READ_SELECT, LOW);
  delayMicroseconds(IO_DELAY);
  cartData.digitalWrite(GB_READ_SELECT, HIGH);

  delayMicroseconds(IO_DELAY);
  uint8_t ret;
  if (!odd)
    ret = cartAdrs.readGPIO(MCP23017_IODIRA);
  else
    ret = cartAdrs.readGPIO(MCP23017_IODIRB);
  return ret;
}

uint32_t GBA::GetSectionOffset(byte sectionId, unsigned int saveOffset) {
  for (unsigned int i = 0; i < Gen3SaveSectionConstants::SECTION_LENGTHS_LENGTH; i++)
  {
    unsigned int offset2 = i * 0x1000 + saveOffset;
    byte sId = readSRAMByte(offset2 + Gen3SaveSectionConstants::SECTION_ID_OFFSET);
    if (sId == sectionId)
      return offset2;
  }
  Serial.print("cannot find section ");
  Serial.println(sectionId);
  //ErrorHandler::HandleError(Error::UNKNOWN_ERROR, "could not find section " + sectionId);
  return 0;
}

uint8_t GBA::readSRAMByte(uint32_t adrs) {
  for (uint8_t i = 0; i < 16; i++)
    cartAdrs.pinMode(i, OUTPUT);
  cartAdrs.writeGPIOAB(0);
  for (uint8_t b = 8; b < 16; b++)
    cartData.pinMode(b, INPUT);
  cartAdrs.writeGPIOAB(adrs);
  cartData.digitalWrite(GB_READ_SELECT, LOW);
  cartData.digitalWrite(GB_CHIP_SELECT2, LOW);
  delayMicroseconds(IO_DELAY);
  
  uint8_t ret = cartData.readGPIO(MCP23017_IODIRB);
  cartData.digitalWrite(GB_CHIP_SELECT2, HIGH);
  cartData.digitalWrite(GB_READ_SELECT, HIGH);
  digitalWrite(12, HIGH);
  return ret;
}
