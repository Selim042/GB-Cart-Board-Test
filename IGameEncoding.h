#ifndef I_GAME_ENCODING_H
#define I_GAME_ENCODING_H

#include "Byte.h"

class IGameEncoding {
public:
  virtual char FromByte(byte dat) const = 0;
  virtual byte ToByte(char dat) const = 0;
  virtual byte* ToEncoding(char* str, int length, const IGameEncoding* targetEnc, byte terminator) const = 0;
};

#endif
