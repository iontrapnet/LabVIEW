//#include "D:\Program Files\National Instruments\LabVIEW 2011\cintools\extcode.h"
//#pragma comment(lib, "D:\Program Files\National Instruments\LabVIEW 2011\cintools\labview.lib")
#undef DLLEXPORT
#undef DLLIMPORT
#ifdef _MSC_VER
#pragma warning(push,1)
#pragma warning(disable:4819)
#define DLLEXPORT __declspec(dllexport)
#define DLLIMPORT __declspec(dllimport)
#else
#define DLLEXPORT
#define DLLIMPORT
#endif
#undef CAPI
#define CAPI extern "C" DLLEXPORT

#include <stdint.h>
// typedef unsigned long long uint64_t;
// typedef unsigned int uint32_t;
// typedef unsigned char uint8_t;
typedef uint8_t byte;

//CAPI void StartPacket(byte packet[], uint32_t repeats, bool trigger);
//CAPI void SeqFinish(byte packet[]);
//CAPI void SeqPacket(byte packet[], uint64_t duration, const char state[]);

void StartPacket(byte packet[], uint32_t repeats, int trigger) {
	packet[0] = trigger ? 0x80 : 0x90;
	packet[1] = 0x00;
	packet[2] = 0x00;
	packet[3] = 0x00;
	packet[4] = 0x00;
	packet[5] = 0x00;
	packet[6] = 0x00;
	packet[7] = (repeats >> 16) & 0xFF;
	packet[8] = (repeats >> 8) & 0xFF;
	packet[9] = repeats & 0xFF;
}

inline int CharToBit(char bit) {
	return bit == '0' ? 0 : 1;
}
inline byte CharsToByte(const char bits[]) {
	byte r = 0;
	int i;
	for (i = 0; i < 8; ++i) {
		r |= CharToBit(bits[i]) << (7 - i);
	}
	return r;
}
inline uint64_t TimeStamp(uint64_t duration) {
	uint64_t r = 0;
	int i;
	duration = 41 - duration;
	for (i = 0; i < 40; ++i) {
		uint64_t t = duration & (((uint64_t)1 << (i + 1)) - 1);
		r |= ((uint64_t)((t < i) | (t >= ((uint64_t)1 << i) + i))) << i;
	}
	return r;
}
void SeqPacket(byte packet[], uint64_t duration, const char state[]) {
	int i;
	duration = TimeStamp(duration);
	packet[0] = 0x40;
	packet[1] = (duration >> 32) & 0xFF;
	packet[2] = (duration >> 24) & 0xFF;
	packet[3] = (duration >> 16) & 0xFF;
	packet[4] = (duration >> 8) & 0xFF;
	packet[5] = duration & 0xFF;
	for (i = 0; i < 4; ++i) {
		(packet + 6)[i] = CharsToByte(state + 8 * i);
	}
}
const char zeros[] = "00000000000000000000000000000000";
void SeqFinish(byte packet[]) {
	SeqPacket(packet, 8, zeros);
	SeqPacket(packet + 10, 2, zeros);
	SeqPacket(packet + 20, 1, zeros);
	packet[20] |= 0x20;
	/*static byte buf[30] = {0x40, 0xfc, 0x00, 0x00, 0x00, 0x1d, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x27, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x26, 0x00, 0x00, 0x00, 0x00};
	for (size_t i = 0; i < 30; ++i)
		packet[i] = buf[i];*/
}

