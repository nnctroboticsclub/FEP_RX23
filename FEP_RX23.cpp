/**
 *  @file   FEP_RX23.cpp
 *  @brief  FEP高速受信用ライブラリ
 *  @author 梅沢晄平
 *  @date   23/10/21
 */
#include "FEP_RX23.h"
#include <cstring>

FEP_RX23::FEP_RX23(PinName tx, PinName rx, uint8_t addr_, uint8_t datalen_, int baud) :
    UnbufferedSerial(tx, rx, baud)
{
    datalen  = datalen_;
    msgdata=(uint8_t*)malloc(sizeof(uint8_t)*datalen);
    for(int i=0; i<datalen; i++) msgdata[i]=0;
    timeout  = TIMEOUT_COUNT;
    bufindex = 0;
    for(uint8_t i=3,j=100;i<6;i++,j/=10) header[i] = (addr_%(j*10))/j+48;
    for(uint8_t i=6,j=100;i<9;i++,j/=10) header[i] = (datalen_%(j*10))/j+48;
}

void FEP_RX23::StartReceive()
{
    attach(callback(this,&FEP_RX23::ReceiveBytes));
    timeoutTimer.attach(callback(this, &FEP_RX23::TimeoutLoop), 100ms);
}

void FEP_RX23::TimeoutLoop()
{
    if (timeout >= TIMEOUT_COUNT) {
        status = false;
    } else {
        status = true;
        timeout++;
    }
}

void FEP_RX23::ReceiveBytes()
{
    this->read(buffer + bufindex, 1);

    if ( (!strncmp((char*)(buffer + ((256 + bufindex - 1)%256) ), "\r\n", 2)) ){
        CheckData();
    }
    bufindex++;
}

void FEP_RX23::CheckData()
{
    indexofR = (256 + bufindex - datalen - 10) % 256;
    if ( !strncmp((char*)(buffer + indexofR) , (char*)header, 9) ) {
        for (int i_msg=0; i_msg<datalen; i_msg++) {
            msgdata[i_msg] = buffer[(indexofR+9+i_msg)%256];
        }
        this->timeout = 0;
    }
}

bool FEP_RX23::getData(uint8_t *data)
{
    for(int i=0; i<datalen; i++) data[i] = msgdata[i];
    return getStatus();
}

bool FEP_RX23::getStatus()
{
    return status;
}
