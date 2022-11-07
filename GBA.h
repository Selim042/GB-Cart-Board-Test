#ifndef GBA_H
#define GBA_H

#include <Adafruit_MCP23017.h>
#include <Arduino.h>

#define IO_DELAY      5
#define START_BUTTON  7
#define CANCEL_BUTTON 6
#define RESET_PIN     11

#define GB_WRITE_SELECT 0 // yellow wire off the cartridge into cartData
#define GB_READ_SELECT  1 // black wire off the cartridge into cartData
#define GB_CHIP_SELECT  2 // brown wire off the cart to cartData
#define GB_CHIP_SELECT2 7 // cartData
#define IO_LED          3 // LED off cartData

enum FlashCmd {
    FLASH_CMD_ERASE_CHIP = 0x10,
    FLASH_CMD_ERASE_SECTOR = 0x30,
    FLASH_CMD_ERASE_PREP = 0x80,
    FLASH_CMD_ENTER_ID_MODE = 0x90,
    FLASH_CMD_WRITE = 0xA0,
    FLASH_CMD_SWITCH_BANK = 0xB0,
    FLASH_CMD_LEAVE_ID_MODE = 0xF0,
};

class GBA {
  private:
    Adafruit_MCP23017 cartAdrs;
    Adafruit_MCP23017 cartData;

    void flashCmd(FlashCmd cmd);
    void switchBank(uint8_t bank);
    void rawWriteCycle(uint16_t adrs, uint8_t value);
  
  public:
    GBA(uint8_t adrsAdrs, uint8_t dataAdrs);

    void resetMCPs();
    uint8_t readByte(uint32_t adrs);
    uint8_t readROMByte(uint32_t adrs);
    uint8_t readSRAMByte(uint32_t adrs);
    uint32_t GetSectionOffset(byte sectionId, unsigned int saveOffset);
    void writeData(uint8_t value);
};

#endif