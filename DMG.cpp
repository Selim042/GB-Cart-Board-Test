#include "DMG.h"

DMG::DMG(uint8_t adrsAdrs, uint8_t dataAdrs) {
  cartAdrs.begin((uint8_t)0x03);
  cartData.begin((uint8_t)0x02);
  resetMCPs();
}

// configure all the necessary pins for startup
void DMG::resetMCPs() {
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
  setDMGDataPinMode(INPUT);
}

// prints all the info in the ROM header to verify reading
void DMG::printROMHeader() {
  printNintendoLogo();
  // checks if the logo was read correctly
  bool validLogo = checkNintendoLogo();
  Serial.print("Nintendo Logo: ");
  Serial.println(validLogo ? "VALID" : "NOT VALID");

  Serial.print("Game Title: ");
  char* title = getGameTitle();
  Serial.println(title);
  delete[] title;

  // TODO: manuf code

  // checks if the cart supports GBC extra functions or is GBC only
  Serial.print("GBC Flag: ");
  switch (readROMByte(0x143)) {
    case 0x80: Serial.println("Supports GBC functions"); break;
    case 0xC0: Serial.println("GBC only"); break;
    default: Serial.println("Non-GBC"); break;
  }

  // TODO: new licensee code

  // reads flags for super gameboy
  Serial.print("SGB Flag: ");
  if (readROMByte(0x146) == 0x03) Serial.println("Supports SGB functions");
  else Serial.println("No SGB functions");

  // prints cart type and rom/ram sizes
  Serial.print("Cart Type: ");
  printCartType();
  Serial.print("ROM Size: ");
  printROMSize();
  Serial.print("RAM Size: ");
  switch (readROMByte(0x149)) {
    case 0x00: Serial.println("None"); break;
    case 0x01: Serial.println("2 KBytes"); break;
    case 0x02: Serial.println("8 KBytes"); break;
    case 0x03: Serial.println("32 KBytes (4 banks)"); break;
  }

  // region code (sorta) and licensee code
  Serial.print("Desination: ");
  Serial.println(readROMByte(0x14A) ? "Non-Japanese" : "Japanese");
  Serial.print("Old Licensee Code: 0x");
  byte oldCode = readROMByte(0x14B);
  Serial.println(oldCode, HEX);
  if (oldCode == 0x33) {
    Serial.print("New Lincesee Code: 0x");
    Serial.print(readROMByte(0x144), HEX);
    Serial.println(readROMByte(0x145), HEX);
  }

  // version and checksum
  Serial.print("ROM Version: ");
  Serial.println(readROMByte(0x14C));
  Serial.print("ROM Checksum: 0x");
  Serial.println(readROMByte(0x14D), HEX);
}

// displays the Nintendo log as read from the cartridge
void DMG::printNintendoLogo() {
  for(int y=0; y<8; ++y) {
    int i = ((y/2)%2)+(y/4)*24;
    for(int x=0; x<12; ++x,i+=2) {
      const uint8_t nibble = (y%2) ? (readROMByte(i+0x104)&0xF) : (readROMByte(i+0x104)>>4);
      for(int b=4; b--;) Serial.print((((nibble>>b)&1) ? '*' : ' '));
    }
    Serial.println();
  }
}

// checks if the Nintendo logo is valid compared to the one saved in the program
bool DMG::checkNintendoLogo() {
  bool valid = true;
  for (uint8_t i = 0; i < 48 && valid; i++) {
    uint8_t val = readROMByte(0x104 + i);
    if (val != nintendoLogo[i])
      valid = false;
  }
  return valid;
}

// returns the title of the game
char* DMG::getGameTitle() {
  char* title = new char[16];
  for (uint8_t i = 0; i < 16; i++) {
    uint8_t val = readROMByte(0x134 + i);
    if (val == 0) {
      title[i] = '\0';
      break;
    }
    title[i] = val;
  }
  title[15] = '\0';
  return title;
}


void DMG::printCartType() {
  switch (readROMByte(0x147)) {
    case 0x00:  Serial.println("ROM ONLY"); break;
    case 0x01:  Serial.println("MBC1"); break;
    case 0x02:  Serial.println("MBC1+RAM"); break;
    case 0x03:  Serial.println("MBC1+RAM+BATTERY"); break;
    case 0x05:  Serial.println("MBC2"); break;
    case 0x06:  Serial.println("MBC2+BATTERY"); break;
    case 0x08:  Serial.println("ROM+RAM"); break;
    case 0x09:  Serial.println("ROM+RAM+BATTERY"); break;
    case 0x0B:  Serial.println("MMM01"); break;
    case 0x0C:  Serial.println("MMM01+RAM"); break;
    case 0x0D:  Serial.println("MMM01+RAM+BATTERY"); break;
    case 0x0F:  Serial.println("MBC3+TIMER+BATTERY"); break;
    case 0x10:  Serial.println("MBC3+TIMER+RAM+BATTERY"); break;
    case 0x11:  Serial.println("MBC3"); break;
    case 0x12:  Serial.println("MBC3+RAM"); break;
    case 0x13:  Serial.println("MBC3+RAM+BATTERY"); break;
    case 0x15:  Serial.println("MBC4"); break;
    case 0x16:  Serial.println("MBC4+RAM"); break;
    case 0x17:  Serial.println("MBC4+RAM+BATTERY"); break;
    case 0x19:  Serial.println("MBC5"); break;
    case 0x1A:  Serial.println("MBC5+RAM"); break;
    case 0x1B:  Serial.println("MBC5+RAM+BATTERY"); break;
    case 0x1C:  Serial.println("MBC5+RUMBLE"); break;
    case 0x1D:  Serial.println("MBC5+RUMBLE+RAM"); break;
    case 0x1E:  Serial.println("MBC5+RUMBLE+RAM+BATTERY"); break;
    case 0xFC:  Serial.println("POCKET CAMERA"); break;
    case 0xFD:  Serial.println("BANDAI TAMA5"); break;
    case 0xFE:  Serial.println("HuC3"); break;
    case 0xFF:  Serial.println("HuC1+RAM+BATTERY"); break;
    default:    Serial.println("INVALID"); break;
  }
}

void DMG::printROMSize() {
  switch(readROMByte(0x148)) {
    case 0x00:  Serial.println("32KByte (no ROM banking)"); break;
    case 0x01:  Serial.println("64KByte (4 banks)"); break;
    case 0x02:  Serial.println("128KByte (8 banks)"); break;
    case 0x03:  Serial.println("256KByte (16 banks)"); break;
    case 0x04:  Serial.println("512KByte (32 banks)"); break;
    case 0x05:  Serial.println("1MByte (64 banks) - only 63 banks used by MBC1"); break;
    case 0x06:  Serial.println("2MByte (128 banks) - only 125 banks used by MBC1"); break;
    case 0x07:  Serial.println("4MByte (256 banks)"); break;
    case 0x52:  Serial.println("1.1MByte (72 banks)"); break;
    case 0x53:  Serial.println("1.2MByte (80 banks)"); break;
    case 0x54:  Serial.println("1.5MByte (96 banks)"); break;
    default:    Serial.println("INVALID"); break;
  }
}

// reads
void DMG::readBankedROM () {
  String charVal = "";
  String charVal2 = "";
  for (uint32_t i = 0; i < 40 * 0x4000L; i++) {
    if (digitalRead(CANCEL_BUTTON) == LOW) {
      cartData.digitalWrite(IO_LED, LOW);
      Serial.println("\nCANCEL");
      digitalWrite(RESET_PIN, LOW);
      digitalWrite(RESET_PIN, HIGH);
      return;
    }
    uint8_t val = readBankedROMByte(i);
    char val2 = Gen1Encoding::GEN_1_ENGLISH->FromByte(val);
    if (DEBUG_OUTPUT && i % 16 == 0) {
      char buff[8];
      sprintf(buff, "0x%04X ", i);
      Serial.print(buff);
    }
    if (val < 16)
      Serial.print("0");
    Serial.print(val, HEX);
    if (DEBUG_OUTPUT) {
      if (isprint(val)) charVal += (char)val; else charVal += ".";
      if (isprint(val2)) charVal2 += val2; else charVal2 += '.';
    }
    if (i % 16 == 7 || i % 16 == 15)
      Serial.print(" ");
    if (DEBUG_OUTPUT) {
      if (i % 16 == 15) {
        Serial.print("| " + charVal);
        Serial.print(" | " + charVal2);
        charVal = "";
        charVal2 = "";
        Serial.println();
      }
    } else if (i % 32 == 31)
      Serial.println();
  }
}

// prints out the entire ROM
void DMG::readROM() {
  String charVal = "";
  String charVal2 = "";
  uint16_t bank = 0;
  for (; bank < 0x40; bank++) {
    if (bank > 0) {
      switchROMBank(bank);
      Serial1.print("switching to bank ");
      Serial1.println(bank);
    }
    for (uint16_t i = (bank == 0 ? 0x0000 : 0x4000); i < (bank == 0 ? 0x4000 : 0x8000); i++) {
      if (digitalRead(CANCEL_BUTTON) == LOW) {
        cartData.digitalWrite(IO_LED, LOW);
        Serial.println("\nCANCEL");
        disableSRAM();
        digitalWrite(RESET_PIN, LOW);
        digitalWrite(RESET_PIN, HIGH);
        return;
      }
      uint8_t val = readROMByte(i);
      char val2 = Gen1Encoding::GEN_1_ENGLISH->FromByte(val);
      if (DEBUG_OUTPUT && i % 16 == 0) {
        char buff[8];
        sprintf(buff, "0x%04X ", i);
        Serial.print(buff);
      }
      if (val < 16)
        Serial.print("0");
      Serial.print(val, HEX);
      if (DEBUG_OUTPUT) {
        if (isprint(val)) charVal += (char)val; else charVal += ".";
        if (isprint(val2)) charVal2 += val2; else charVal2 += '.';
      }
      if (i % 16 == 7 || i % 16 == 15)
        Serial.print(" ");
      if (DEBUG_OUTPUT) {
        if (i % 16 == 15) {
          Serial.print("| " + charVal);
          Serial.print(" | " + charVal2);
          charVal = "";
          charVal2 = "";
          Serial.println();
        }
      } else if (i % 32 == 31)
        Serial.println();
    }
  }
}

// reads byte at adrs and switches bank if needed
uint8_t DMG::readBankedROMByte(uint32_t adrs) {
  uint8_t bank = adrs / 0x4000;
  if (romBank != bank)
    switchROMBank(bank);
  uint16_t readAdrs = adrs - (bank * 0x4000);
  if (bank != 0)
    readAdrs += 0x4000;
  return readROMByte(readAdrs);
}

// reads byte at adrs without switching bank
uint8_t DMG::readROMByte(uint16_t adrs) {
  cartAdrs.writeGPIOAB(adrs);
  delayMicroseconds(IO_DELAY);
  return cartData.readGPIO(MCP23017_IODIRB);
}

void DMG::readBankedSRAM() {
  enableSRAM();
  String charVal = "";
  String charVal2 = "";
  for (uint16_t i = 0x0000; i < 0x8000; i++) {
    if (digitalRead(CANCEL_BUTTON) == LOW) {
      cartData.digitalWrite(IO_LED, LOW);
      Serial.println("\nCANCEL");
      disableSRAM();
      digitalWrite(RESET_PIN, LOW);
      digitalWrite(RESET_PIN, HIGH);
      return;
    }
    uint8_t val = readBankedSRAMByte(i);
    char val2 = Gen1Encoding::GEN_1_ENGLISH->FromByte(val);
    if (DEBUG_OUTPUT && i % 16 == 0) {
      char buff[8];
      sprintf(buff, "0x%04X ", i);
      Serial.print(buff);
    }
    if (val < 16)
      Serial.print("0");
    Serial.print(val, HEX);
    if (DEBUG_OUTPUT) {
      if (isprint(val)) charVal += (char)val; else charVal += ".";
      if (isprint(val2)) charVal2 += val2; else charVal2 += '.';
    }
    if (i % 16 == 7 || i % 16 == 15)
      Serial.print(" ");
    if (DEBUG_OUTPUT) {
      if (i % 16 == 15) {
        Serial.print("| " + charVal);
        Serial.print(" | " + charVal2);
        charVal = "";
        charVal2 = "";
        Serial.println();
      }
    } else if (i % 32 == 31)
      Serial.println();
  }
  disableSRAM();
}

void DMG::readSRAM() {
  enableSRAM();
  String charVal = "";
  String charVal2 = "";
  uint16_t bank = 0;
  for (; bank < 4; bank++) {
    switchSRAMBank(bank);
    Serial1.print("switching to bank ");
    Serial1.println(bank);
    for (uint16_t i = 0xA000; i < 0xC000; i++) {
      if (digitalRead(CANCEL_BUTTON) == LOW) {
        cartData.digitalWrite(IO_LED, LOW);
        Serial.println("\nCANCEL");
        disableSRAM();
        digitalWrite(RESET_PIN, LOW);
        digitalWrite(RESET_PIN, HIGH);
        return;
      }
      uint8_t val = readSRAMByte(i);
      char val2 = Gen1Encoding::GEN_1_ENGLISH->FromByte(val);
      if (DEBUG_OUTPUT && i % 16 == 0) {
        char buff[8];
        sprintf(buff, "0x%04X ", i);
        Serial.print(buff);
      }
      if (val < 16)
        Serial.print("0");
      Serial.print(val, HEX);
      if (DEBUG_OUTPUT) {
        if (isprint(val)) charVal += (char)val; else charVal += ".";
        if (isprint(val2)) charVal2 += val2; else charVal2 += '.';
      }
      if (i % 16 == 7 || i % 16 == 15)
        Serial.print(" ");
      if (DEBUG_OUTPUT) {
        if (i % 16 == 15) {
          Serial.print("| " + charVal);
          Serial.print(" | " + charVal2);
          charVal = "";
          charVal2 = "";
          Serial.println();
        }
      } else if (i % 32 == 31)
        Serial.println();
    }
  }
  disableSRAM();
}

// reads adrs from SRAM with bank switching
uint8_t DMG::readBankedSRAMByte(uint16_t adrs) {
  uint16_t offsetAdrs = adrs - 0xA000;
  uint8_t bank = adrs / 0x2000;
  if (sramBank != bank)
    switchSRAMBank(bank);
  uint16_t readAdrs = adrs - (bank * 0x2000);
  return readSRAMByte(0xA000 + readAdrs);
}

// reads adrs from SRAM without bank switching, will turn on/off SRAM if necessary
uint8_t DMG::readSRAMByte(uint16_t adrs) {
  bool toggledSRAM = false;
  if (!sramEnabled) {
    toggledSRAM = true;
    enableSRAM();
  }
  cartAdrs.writeGPIOAB(adrs);
  delayMicroseconds(IO_DELAY);
  uint8_t val = cartData.readGPIO(MCP23017_IODIRB);
  if (toggledSRAM)
    disableSRAM();
  return val;
}

// sets all the data pins to the appropriate IO mode
void DMG::setDMGDataPinMode(uint8_t mode) {
  if (DEBUG_OUTPUT) {
    Serial.print("SETTING DATA PINS TO ");
    Serial.println(mode == INPUT ? "INPUT" : "OUTPUT");
  }

  for (int i = 8; i < 16; i++) {
    cartData.pinMode(i, mode);
    cartData.pullUp(i, mode == INPUT ? LOW : HIGH);
  }
  delayMicroseconds(IO_DELAY);
}

// switches the active ROM bank to bankNum
void DMG::switchROMBank(int bankNum) {
  romBank = bankNum;
  if (DEBUG_OUTPUT) {
    Serial.print("SWITCHING TO ROM BANK #");
    Serial.println(bankNum);
  }
  cartData.digitalWrite(GB_WRITE_SELECT, HIGH);
  cartData.digitalWrite(GB_READ_SELECT, HIGH);
  setDMGDataPinMode(OUTPUT);
  delayMicroseconds(IO_DELAY);

  cartAdrs.writeGPIOAB(romBankAdrs);
  writeData(bankNum);
  delayMicroseconds(IO_DELAY);

  cartData.digitalWrite(GB_WRITE_SELECT, LOW);
  delayMicroseconds(IO_DELAY);

  cartData.digitalWrite(GB_WRITE_SELECT, HIGH);
  cartData.digitalWrite(GB_READ_SELECT, LOW);
  setDMGDataPinMode(INPUT);
  delayMicroseconds(IO_DELAY);
}

// writes value to the data pins
void DMG::writeData(uint8_t value) {
  for (uint8_t b = 0; b < 8; b++) {
    cartData.digitalWrite(b + 8, (value >> b) & 1);
  }
}

void DMG::switchSRAMBank(int bankNum) {
  sramBank = bankNum;
  if (DEBUG_OUTPUT) {
    Serial.print("SWITCHING SRAM BANK #");
    Serial.println(bankNum);
  }
  cartData.digitalWrite(GB_WRITE_SELECT, HIGH);
  cartData.digitalWrite(GB_READ_SELECT, HIGH);
  setDMGDataPinMode(OUTPUT);
  delayMicroseconds(IO_DELAY);

  cartAdrs.writeGPIOAB(sramBankAdrs);
  writeData(bankNum);
  delayMicroseconds(IO_DELAY);

  cartData.digitalWrite(GB_WRITE_SELECT, LOW);
  delayMicroseconds(IO_DELAY);
  
  cartData.digitalWrite(GB_WRITE_SELECT, HIGH);
  cartData.digitalWrite(GB_READ_SELECT, LOW);
  setDMGDataPinMode(INPUT);
  delayMicroseconds(IO_DELAY);
}

void DMG::enableSRAM() {
  sramEnabled = true;
  cartData.digitalWrite(IO_LED, HIGH);
  if (DEBUG_OUTPUT) Serial.println("ENABLING SRAM");

  cartData.digitalWrite(GB_WRITE_SELECT, LOW);
  cartData.digitalWrite(GB_READ_SELECT, HIGH);
  delayMicroseconds(IO_DELAY);
  setDMGDataPinMode(OUTPUT);
  delayMicroseconds(IO_DELAY);

  cartAdrs.writeGPIOAB(sramEnableAdrs);
  cartData.writeGPIOAB(sramEnable << 8);
  delayMicroseconds(IO_DELAY);

  cartData.digitalWrite(GB_WRITE_SELECT, HIGH);
  cartData.digitalWrite(GB_READ_SELECT, LOW);
  setDMGDataPinMode(INPUT);
  delayMicroseconds(IO_DELAY);
  cartData.digitalWrite(GB_CHIP_SELECT, HIGH);
}

void DMG::disableSRAM() {
  sramEnabled = false;
  cartData.digitalWrite(IO_LED, LOW);
  if (DEBUG_OUTPUT) Serial.println("DISABLING SRAM");

  cartData.digitalWrite(GB_WRITE_SELECT, LOW);
  cartData.digitalWrite(GB_READ_SELECT, HIGH);
  delayMicroseconds(IO_DELAY);
  setDMGDataPinMode(OUTPUT);
  delayMicroseconds(IO_DELAY);

  cartAdrs.writeGPIOAB(sramEnableAdrs);
  cartData.writeGPIOAB(0x00);
  delayMicroseconds(IO_DELAY);

  cartData.digitalWrite(GB_WRITE_SELECT, HIGH);
  cartData.digitalWrite(GB_READ_SELECT, LOW);
  setDMGDataPinMode(INPUT);
  delayMicroseconds(IO_DELAY);
  cartData.digitalWrite(GB_CHIP_SELECT, HIGH);
}
