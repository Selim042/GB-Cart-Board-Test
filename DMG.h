#ifndef DMG_H
#define DMG_H

#include <Adafruit_MCP23017.h>
#include <Arduino.h>

#include "Byte.h"
#include "Gen1Encoding.h"

#define DEBUG_OUTPUT false

#define IO_DELAY      5
#define START_BUTTON  7
#define CANCEL_BUTTON 6
#define RESET_PIN     11

#define GB_WRITE_SELECT 0 // yellow wire off the cartridge into cartData
#define GB_READ_SELECT  1 // black wire off the cartridge into cartData
#define GB_CHIP_SELECT  2 // brown wire off the cart to cartData
#define GB_CHIP_SELECT2 7 // cartData
#define IO_LED          3 // LED off cartData

class DMG {
  private:
    int sramEnableAdrs = 0x0000;
    int sramBankingMode = 0x6000;
    int sramEnable = 0x0A;
    int sramDisable = 0x00;
    int sramBankAdrs = 0x4000;
    int romBankAdrs = 0x2000;
    
    Adafruit_MCP23017 cartAdrs;
    Adafruit_MCP23017 cartData;

    bool sramEnabled = false;
    uint8_t sramBank = 0;
    uint8_t romBank = 0;

    byte nintendoLogo[48] = {
      0xCE, 0xED, 0x66, 0x66, 0xCC, 0x0D, 0x00, 0x0B, 0x03, 0x73, 0x00, 0x83, 0x00, 0x0C, 0x00, 0x0D,
      0x00, 0x08, 0x11, 0x1F, 0x88, 0x89, 0x00, 0x0E, 0xDC, 0xCC, 0x6E, 0xE6, 0xDD, 0xDD, 0xD9, 0x99,
      0xBB, 0xBB, 0x67, 0x63, 0x6E, 0x0E, 0xEC, 0xCC, 0xDD, 0xDC, 0x99, 0x9F, 0xBB, 0xB9, 0x33, 0x3E
    };

  public:
    DMG(uint8_t adrsAdrs, uint8_t dataAdrs);
    void resetMCPs();
    void printROMHeader();
    void printNintendoLogo();
    bool checkNintendoLogo();
    char* getGameTitle();
    void printCartType();
    void printROMSize();
    void readBankedROM();
    void readROM();
    uint8_t readBankedROMByte(uint32_t adrs);
    uint8_t readROMByte(uint16_t adrs);
    void readBankedSRAM();
    void readSRAM();
    uint8_t readBankedSRAMByte(uint16_t adrs);
    uint8_t readSRAMByte(uint16_t adrs);
    void setDMGDataPinMode(uint8_t mode);
    void switchROMBank(int bankNum);
    void writeData(uint8_t value);
    void switchSRAMBank(int bankNum);
    void enableSRAM();
    void disableSRAM();

};

#endif