#include <vector>
#include <iostream>
#include "subprocess.h"

SubProcess::SubProcess(uint8_t idNumber[3], uint8_t mti[4], uint8_t pan[16])
{
    for (int i = 0; i < 3; ++i)
    {
        m_idNumber[i] = idNumber[i];
    }
    for (int i = 0; i < 4; ++i)
    {
        m_MTI[i] = mti[i];
    }
    for (int i = 0; i < 16; ++i)
    {
        m_PAN[i] = pan[i];
    }
}

void SubProcess::send(uint8_t *trace, uint8_t *destIDNum)
{
    // m_message->id = m_idNumber;
    // m_message->MTI = m_MTI;
    // m_message->PAN = m_PAN;
    // m_message->destination_id = destIDNum;
    // m_message->Trace = trace;
}


void SubProcess::sendback()
{
    // m_message->destination_id = m_message->id;
    // int mti_value = (m_message->MTI[0] << 24) | (m_message->MTI[1] << 16) | (m_message->MTI[2] << 8) | m_message->MTI[3];
    // mti_value += 10;
    // m_message->MTI[0] = (mti_value >> 24) & 0xFF;
    // m_message->MTI[1] = (mti_value >> 16) & 0xFF;
    // m_message->MTI[2] = (mti_value >> 8) & 0xFF;
    // m_message->MTI[3] = mti_value & 0xFF;
    // m_notifier.notify();
}
