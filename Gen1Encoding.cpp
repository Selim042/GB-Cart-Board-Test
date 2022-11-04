#ifndef GEN_1_ENCODING_CPP
#define GEN_1_ENCODING_CPP

#include "Gen1Encoding.h"

// converts encoded byte to char
char Gen1English::FromByte(byte dat) const
{
  for (int i = 0; i < GEN_1_ENGLISH_NUM_MAPPINGS; i++)
    if (GEN_1_ENGLISH_MAPPING[i].left == dat)
      return GEN_1_ENGLISH_MAPPING[i].right;
  return (char)0x0;
}

// converts char to encoded byte
byte Gen1English::ToByte(char dat) const
{
  for (int i = 0; i < GEN_1_ENGLISH_NUM_MAPPINGS; i++)
    if (GEN_1_ENGLISH_MAPPING[i].right == dat)
      return GEN_1_ENGLISH_MAPPING[i].left;
  return 0x0;
}

byte* Gen1English::ToEncoding(char* str, int length, const IGameEncoding* targetEnc, byte terminator) const
{
  byte* ret = new byte[length + 1];
  for (int i = 0; i < length; i++)
    ret[i] = targetEnc->ToByte(str[i]);
  ret[length] = terminator;
  return ret;
}

const Pair<byte, wchar_t> Gen1English::GEN_1_ENGLISH_MAPPING[GEN_1_ENGLISH_NUM_MAPPINGS] = {
  // 0x00, null
  // 0x01 - 0x47, junk
  // 0x48 - 0x5F, control wchar_ts
  Pair<byte, wchar_t>(0x5D, L'*'), // used for in game trades, displays "TRAINER"
  Pair<byte, wchar_t>(0x60, L'A'), Pair<byte, wchar_t>(0x61, L'B'), Pair<byte, wchar_t>(0x62, L'C'), Pair<byte, wchar_t>(0x63, L'D'), Pair<byte, wchar_t>(0x64, L'E'), Pair<byte, wchar_t>(0x65, L'F'), Pair<byte, wchar_t>(0x66, L'G'), Pair<byte, wchar_t>(0x67, L'H'), Pair<byte, wchar_t>(0x68, L'I'), Pair<byte, wchar_t>(0x69, L'V'), Pair<byte, wchar_t>(0x6A, L'S'), Pair<byte, wchar_t>(0x6B, L'L'), Pair<byte, wchar_t>(0x6C, L'M'), Pair<byte, wchar_t>(0x6D, L':'), Pair<byte, wchar_t>(0x6E, L'ぃ'), Pair<byte, wchar_t>(0x6F, L'ぅ'),
  Pair<byte, wchar_t>(0x70, L'‘'), Pair<byte, wchar_t>(0x71, L'’'), Pair<byte, wchar_t>(0x72, L'“'), Pair<byte, wchar_t>(0x73, L'”'), Pair<byte, wchar_t>(0x74, L'・'), Pair<byte, wchar_t>(0x75, L'⋯'), Pair<byte, wchar_t>(0x76, L'ぁ'), Pair<byte, wchar_t>(0x77, L'ぇ'), Pair<byte, wchar_t>(0x78, L'ぉ'), // 0x79 - 0x7F, text box borders
  Pair<byte, wchar_t>(0x80, L'A'), Pair<byte, wchar_t>(0x81, L'B'), Pair<byte, wchar_t>(0x82, L'C'), Pair<byte, wchar_t>(0x83, L'D'), Pair<byte, wchar_t>(0x84, L'E'), Pair<byte, wchar_t>(0x85, L'F'), Pair<byte, wchar_t>(0x86, L'G'), Pair<byte, wchar_t>(0x87, L'H'), Pair<byte, wchar_t>(0x88, L'I'), Pair<byte, wchar_t>(0x89, L'J'), Pair<byte, wchar_t>(0x8A, L'K'), Pair<byte, wchar_t>(0x8B, L'L'), Pair<byte, wchar_t>(0x8C, L'M'), Pair<byte, wchar_t>(0x8D, L'N'), Pair<byte, wchar_t>(0x8E, L'O'), Pair<byte, wchar_t>(0x8F, L'P'),
  Pair<byte, wchar_t>(0x90, L'Q'), Pair<byte, wchar_t>(0x91, L'R'), Pair<byte, wchar_t>(0x92, L'S'), Pair<byte, wchar_t>(0x93, L'T'), Pair<byte, wchar_t>(0x94, L'U'), Pair<byte, wchar_t>(0x95, L'V'), Pair<byte, wchar_t>(0x96, L'W'), Pair<byte, wchar_t>(0x97, L'X'), Pair<byte, wchar_t>(0x98, L'Y'), Pair<byte, wchar_t>(0x99, L'Z'), Pair<byte, wchar_t>(0x9A, L'('), Pair<byte, wchar_t>(0x9B, L')'), Pair<byte, wchar_t>(0x9C, L':'), Pair<byte, wchar_t>(0x9D, L';'), Pair<byte, wchar_t>(0x9E, L'['), Pair<byte, wchar_t>(0x9F, L']'),
  Pair<byte, wchar_t>(0xA0, L'a'), Pair<byte, wchar_t>(0xA1, L'b'), Pair<byte, wchar_t>(0xA2, L'c'), Pair<byte, wchar_t>(0xA3, L'd'), Pair<byte, wchar_t>(0xA4, L'e'), Pair<byte, wchar_t>(0xA5, L'f'), Pair<byte, wchar_t>(0xA6, L'g'), Pair<byte, wchar_t>(0xA7, L'h'), Pair<byte, wchar_t>(0xA8, L'i'), Pair<byte, wchar_t>(0xA9, L'j'), Pair<byte, wchar_t>(0xAA, L'k'), Pair<byte, wchar_t>(0xAB, L'l'), Pair<byte, wchar_t>(0xAC, L'm'), Pair<byte, wchar_t>(0xAD, L'n'), Pair<byte, wchar_t>(0xAE, L'o'), Pair<byte, wchar_t>(0xAF, L'p'),
  Pair<byte, wchar_t>(0xB0, L'q'), Pair<byte, wchar_t>(0xB1, L'r'), Pair<byte, wchar_t>(0xB2, L's'), Pair<byte, wchar_t>(0xB3, L't'), Pair<byte, wchar_t>(0xB4, L'u'), Pair<byte, wchar_t>(0xB5, L'v'), Pair<byte, wchar_t>(0xB6, L'w'), Pair<byte, wchar_t>(0xB7, L'x'), Pair<byte, wchar_t>(0xB8, L'y'), Pair<byte, wchar_t>(0xB9, L'z'), Pair<byte, wchar_t>(0xBA, L'é'), Pair<byte, wchar_t>(0xBB, L'd'), Pair<byte, wchar_t>(0xBC, L'l'), Pair<byte, wchar_t>(0xBD, L's'), Pair<byte, wchar_t>(0xBE, L't'), Pair<byte, wchar_t>(0xBF, L'v'),
  // 0xC0 - 0xDF, junk
  Pair<byte, wchar_t>(0xE0, L'\''), Pair<byte, wchar_t>(0xE1, L'b'), Pair<byte, wchar_t>(0xE2, L'c'), Pair<byte, wchar_t>(0xE3, L'd'), Pair<byte, wchar_t>(0xE4, L'e'), Pair<byte, wchar_t>(0xE5, L'f'), Pair<byte, wchar_t>(0xE6, L'g'), Pair<byte, wchar_t>(0xE7, L'h'), Pair<byte, wchar_t>(0xE8, L'i'), Pair<byte, wchar_t>(0xE9, L'j'), Pair<byte, wchar_t>(0xEA, L'k'), Pair<byte, wchar_t>(0xEB, L'l'), Pair<byte, wchar_t>(0xEC, L'm'), Pair<byte, wchar_t>(0xED, L'n'), Pair<byte, wchar_t>(0xEE, L'o'), Pair<byte, wchar_t>(0xEF, L'p'),
  Pair<byte, wchar_t>(0xF0, L'q'), Pair<byte, wchar_t>(0xF1, L'r'), Pair<byte, wchar_t>(0xF2, L's'), Pair<byte, wchar_t>(0xF3, L't'), Pair<byte, wchar_t>(0xF4, L'u'), Pair<byte, wchar_t>(0xF5, L'v'), Pair<byte, wchar_t>(0xF6, L'w'), Pair<byte, wchar_t>(0xF7, L'x'), Pair<byte, wchar_t>(0xF8, L'y'), Pair<byte, wchar_t>(0xF9, L'z'), Pair<byte, wchar_t>(0xFA, L'é'), Pair<byte, wchar_t>(0xFB, L'd'), Pair<byte, wchar_t>(0xFC, L'l'), Pair<byte, wchar_t>(0xFD, L's'), Pair<byte, wchar_t>(0xFE, L't'), Pair<byte, wchar_t>(0xFF, L'v')
};

const IGameEncoding* Gen1Encoding::GEN_1_ENGLISH = new Gen1English();
#endif
