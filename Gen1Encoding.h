#ifndef GEN_1_ENCODING_H
#define GEN_1_ENCODING_H

#include "IGameEncoding.h"
#include "Pair.h"

/* https://bulbapedia.bulbagarden.net/wiki/Character_encoding_in_Generation_I */
class Gen1Encoding : public IGameEncoding
{
public:
  static const IGameEncoding* GEN_1_ENGLISH;
};

class Gen1English : public Gen1Encoding
{
private:
  const static int GEN_1_ENGLISH_NUM_MAPPINGS = 122;
  const static Pair<byte, wchar_t> GEN_1_ENGLISH_MAPPING[GEN_1_ENGLISH_NUM_MAPPINGS];

public:
  char FromByte(byte dat) const override;
  byte ToByte(char dat) const override;
  byte* ToEncoding(char* str, int length, const IGameEncoding* targetEnc, byte terminator) const override;
};
#endif
