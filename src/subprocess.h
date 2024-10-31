#include <stdint.h>
#include "SharedMemory.h"
#include "notifier.h"
#include <memory>

struct Message{
	uint8_t id[3];
	uint8_t MTI[4];
	uint8_t Trace[6];
	uint8_t PAN[16];
	uint8_t destination_id[3];
};

class SubProcess{
    public:
        SubProcess(uint8_t idNumber[3],uint8_t mti[4],uint8_t pan[16]);
        void send(uint8_t* trace,uint8_t* destIDNum);
        void sendback();
        void listen();

    private:
        int sharedMemoryInit();
        uint8_t m_idNumber[3];
        uint8_t m_MTI[4];
        uint8_t m_PAN[16];
};