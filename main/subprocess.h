#include <stdint.h>
#include <memory>

struct Message{
	uint8_t id[3];
	uint8_t MTI[4];
	uint8_t Trace[6];
	uint8_t PAN[16];
	uint8_t destination_id[3];
};