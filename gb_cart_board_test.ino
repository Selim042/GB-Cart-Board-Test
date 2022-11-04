#include "DMG.h"
#include "GBA.h"

DMG* dmgCart = NULL;
GBA* gbaCart = NULL;

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  delay(10);
  pinMode(RESET_PIN, OUTPUT);
  pinMode(START_BUTTON, INPUT_PULLUP);
  pinMode(CANCEL_BUTTON, INPUT_PULLUP);
  dmgCart = new DMG(0x03, 0x02);
  //gbaCart = new GBA(0x01, 0x00);

  dmgCart->printROMHeader();
  //gbaCart->printROMHeader();
  
  Serial.println("READY");
  Serial1.println("READY");
}

void loop() {
  if (digitalRead(START_BUTTON) == HIGH)
    return;
  delayMicroseconds(IO_DELAY);

  String chars = "";
  dmgCart->enableSRAM();
  for (uint32_t i = 0; i < 0x8000; i++) {
    if (i % 16 == 0) {
      //Serial.print(" | ");
      //Serial.print(chars);
      //chars = "";
      Serial.println();
    }
    uint8_t val = dmgCart->readBankedSRAMByte(i);
    if (val < 16) Serial.print('0');
    Serial.print(val, HEX);
    /*char c = (char) val;
    if (c >= ' ' && c <= '~') chars += c;
    else chars += '.';*/
  }
  dmgCart->disableSRAM();

  /*String chars = "";
  uint32_t start = 0xE000000;//gbaCart->GetSectionOffset(0, 0x0000);
  Serial.println(start,HEX);
  for (uint32_t i = 0; i < 0xFF; i++) {
    Serial.print(start,HEX);
    Serial.print(": ");
    for (uint32_t adrs = start; adrs < start + 16; adrs++) {
      uint8_t val = gbaCart->readByte(adrs);
      if (val < 16) Serial.print('0');
      Serial.print(val, HEX);
      char c = (char) val;
      if (c >= ' ' && c <= '~') chars += c;
      else chars += '.';
    }
    Serial.print(" | ");
    Serial.println(chars);
    chars = "";
    start += 16;
  }
  //gbaCart->resetMCPs();*/

  Serial.println("DONE!!!");
  Serial1.println("DONE!!!");
}
