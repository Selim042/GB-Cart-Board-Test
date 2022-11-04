#ifndef GEN_3_SAVE_SECTION_CONSTANTS_H
#define GEN_3_SAVE_SECTION_CONSTANTS_H

class Gen3SaveSectionConstants
{
public:
	static const unsigned int FOOTER_LENGTH = 8;
	static const unsigned int BASE_LENGTH = 3968;

	static const unsigned int SECTION_ID_OFFSET = 0x0FF4;
	static const unsigned int CHECKSUM_OFFSET = 0x0FF6;
	static const unsigned int SAVE_INDEX_OFFSET = 0x0FFC;

	static const unsigned int SECTION_ID_LENGTH = 2;
	static const unsigned int CHECKSUM_LENGTH = 2;
	static const unsigned int SAVE_INDEX_LENGTH = 4;

	static const unsigned int SECTION_LENGTHS_LENGTH = 14;
	static const unsigned int SECTION_LENGTHS[SECTION_LENGTHS_LENGTH];
};
#endif
