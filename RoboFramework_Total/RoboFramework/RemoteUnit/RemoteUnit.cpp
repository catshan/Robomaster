//
// Created by Administrator on 2019/11/17.
//

#include "RemoteUnit.h"

const u8 *RoboFramework::RemoteInputStream::getRxBuffer() const {
    return rxBuffer;
}

RoboFramework::RemoteInputStream::RemoteInputStream() {
    Remote::Init(rxBuffer);
}

RoboFramework::RemoteInputStream *RoboFramework::RemoteUnit::getStream() {
    return stream;
}

RoboFramework::RemoteListenerInterface *RoboFramework::RemoteUnit::getListener() {
    return listenerInterface;
}
RoboFramework::RemoteInputStream* RoboFramework::RemoteUnit::stream = nullptr;
RoboFramework::RemoteListenerInterface* RoboFramework::RemoteUnit::listenerInterface = nullptr;
void RoboFramework::RemoteUnit::BindListener(RoboFramework::RemoteListenerInterface *listener) {
     listenerInterface = listener;
}

void RoboFramework::RemoteUnit::Init() {
    stream = new RemoteInputStream();
}

void RoboFramework::RemoteUnit::Decode(RoboFramework::RemoteData *data) {
    data->rc.ch0 = ((int16_t)stream->getRxBuffer()[0] | ((int16_t)stream->getRxBuffer()[1] << 8)) & 0x07FF;
    data->rc.ch1 = (((int16_t)stream->getRxBuffer()[1] >> 3) | ((int16_t)stream->getRxBuffer()[2] << 5))
                   & 0x07FF;
    data->rc.ch2 = (((int16_t)stream->getRxBuffer()[2] >> 6) | ((int16_t)stream->getRxBuffer()[3] << 2) |
                    ((int16_t)stream->getRxBuffer()[4] << 10)) & 0x07FF;
    data->rc.ch3 = (((int16_t)stream->getRxBuffer()[4] >> 1) | ((int16_t)stream->getRxBuffer()[5]<<7)) &
                   0x07FF;

    data->rc.s1 = ((stream->getRxBuffer()[5] >> 4) & 0x000C) >> 2;
    data->rc.s2 = ((stream->getRxBuffer()[5] >> 4) & 0x0003);
    data->mouse.x = ((int16_t)stream->getRxBuffer()[6]) | ((int16_t)stream->getRxBuffer()[7] << 8);
    data->mouse.y = ((int16_t)stream->getRxBuffer()[8]) | ((int16_t)stream->getRxBuffer()[9] << 8);
    data->mouse.z = ((int16_t)stream->getRxBuffer()[10]) | ((int16_t)stream->getRxBuffer()[11] << 8);
    data->mouse.press_l = stream->getRxBuffer()[12];
    data->mouse.press_r = stream->getRxBuffer()[13];
    data->key.v = ((int16_t)stream->getRxBuffer()[14]);
}

void RoboFramework::RemoteUnitThread::start() {
    RemoteUnit::Init();
    for(;;Delay::ms(10)){
        RemoteUnit::Decode(&data);
        if(RemoteUnit::getListener() != nullptr){
            RemoteUnit::getListener()->OnGetDataReady(data);
        }
    }
}

SerialPort<Usart1 >* RoboFramework::Remote::pUsart = nullptr;
void RoboFramework::Remote::Init(u8 *rxBuffer) {
    pUsart = new SerialPort<Usart1 >(100000,"PB6","PB7");
    pUsart->getUart()->setMode(SerialPort_Mode::RX);
    pUsart->getUart()->setParity(SerialPort_Parity::Even);
    pUsart->getUart()->setOverSampling(SerialPort_OverSampling::OVERSAMPLING_16);
    pUsart->reloadUart();
    pUsart->receiveData(new DMA(DMA_Type::Dma2_Stream2,DMA_Channel::CH4,DMA_Mode::Circular,DMA_Direction::Per2Mem),rxBuffer,18);
}

void RoboFramework::Remote::DeInit() {
    delete Remote::pUsart;
}

bool RoboFramework::RemoteListenerInterface::IsOnline(RoboFramework::RemoteData &RC_data) {
    if(RC_data.rc.ch0 < REMOTE_MIN_VAL || RC_data.rc.ch0 > REMOTE_MAX_VAL){return false;}
    if(RC_data.rc.ch1 < REMOTE_MIN_VAL || RC_data.rc.ch1 > REMOTE_MAX_VAL){return false;}
    if(RC_data.rc.ch2 < REMOTE_MIN_VAL || RC_data.rc.ch2 > REMOTE_MAX_VAL){return false;}
    if(RC_data.rc.ch3 < REMOTE_MIN_VAL || RC_data.rc.ch3 > REMOTE_MAX_VAL){return false;}
    if(RC_data.rc.s1==0){ return false;}
    return RC_data.rc.s2 != 0;
}


