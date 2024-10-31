#include <stdio.h>
#include <stdint.h>
#include "SharedMemory.h"
#include "Buffer.h"


int main()
{

    RawBuffer3 rb_, *rb;
    uint8_t *bufdata;
    uint8_t *bufwrtn;

    SharedMemory buf_data;
    bool inited = buf_data.init((void**)&bufdata, 0x12345, 1000);

    SharedMemory buf_wrtn;
    buf_data.init((void**)&bufwrtn, 0x12346, 8);

    rbInit3RawBuffer3(&rb_, 1000, 1, bufdata, (int64_t*)bufwrtn);
    rb = &rb_;
    if (inited) rbReset(rb);

    uint8_t temp_buf[1000];
    int len = rbPull(rb, 1, temp_buf, 100, 0);
    printf("LEN: %i\n", len);
    for(int i=0; i<len; i++)
    {
        printf("len[%i] = %i\n", i, temp_buf[i]);
    }

    while(true){}
}