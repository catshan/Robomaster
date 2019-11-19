//
// Created by Administrator on 2019/11/15.
//

#include "CanBusUnit.h"
using namespace RoboFramework;
CanBus<Can1>*CanBusUnit::canBus1 = nullptr;
CanBus<Can2>*CanBusUnit::canBus2 = nullptr;
std::vector<CanNode*>*CanBusUnit::can1NodeList = nullptr;
std::vector<CanNode*>*CanBusUnit::can2NodeList = nullptr;
std::vector<u16>* CanBusUnit::sendIDList = nullptr;
CanBus<Can1> *RoboFramework::CanBusUnit::getCanBus1() {
    return canBus1;
}

CanBus<Can2> *RoboFramework::CanBusUnit::getCanBus2() {
    return canBus2;
}

std::vector<CanNode *> *RoboFramework::CanBusUnit::getCan1NodeList() {
    return can1NodeList;
}

std::vector<CanNode *> *RoboFramework::CanBusUnit::getCan2NodeList() {
    return can2NodeList;
}

std::vector<u16> *RoboFramework::CanBusUnit::getSendIdList() {
    return sendIDList;
}

void CanBusUnit::Init(u32 Can1Band, u32 Can2Band) {
    canBus1 = new CanBus<Can1 >(Can1Band,"PD0","PD1");
    canBus2 = new CanBus<Can2 >(Can2Band,"PB12","PB13");
    can1NodeList = new std::vector<CanNode*>();
    can2NodeList = new std::vector<CanNode*>();
    sendIDList = new std::vector<u16>();
}

void CanBusUnit::AddNode(CanType type, CanNode *node) {
    switch (type){
        case CanType ::Can1 :
            can1NodeList->push_back(node);
            break;
        case CanType ::Can2 :
            can2NodeList->push_back(node);
            break;
    }
    sendIDList->push_back(node->getSendId());
}

void CanBusThread::start() {
    for(;;Delay::ms(1)){
    //receive data on canbus1
        u16 rxID = 0;
        u8 rxBuffer[8] = {0};
        u16 size = CanBusUnit::getSendIdList()->size();
        if(!size){
            continue;
        }
        rxID = CanBusUnit::getCanBus1()->receiveDate(rxBuffer);
        u8 txData[8] = {0};

        for(auto node:*CanBusUnit::getCan1NodeList()){
            if(node->getListenId()==rxID){
                memcpy((void*)node->getRxBuffer(),rxBuffer,8);
                node->OnUpData();
                break;
            }
        }
        //receive data on canbus2
        for(auto node:*CanBusUnit::getCan2NodeList()){
            if(node->getListenId() == rxID){
                memcpy((void*)node->getRxBuffer(),rxBuffer,8);
                node->OnUpData();
                break;
            }
        }

        for(u16 id:*CanBusUnit::getSendIdList()){
            //send data on canbus1
            for(CanNode* node:*CanBusUnit::getCan1NodeList()){
                if(node->getSendId() == id){
                    memcpy(txData + node->getSendIndexStart(),node->getTxBuffer(),2);
                }
            }
            CanBusUnit::getCanBus1()->sendData(id,txData);
            memset(txData,0,8);
            //send data on canbus2
            for(CanNode* node:*CanBusUnit::getCan2NodeList()){
                if(node->getSendId() == id){
                    memcpy(txData + node->getSendIndexStart(),node->getTxBuffer(),2);
                }
            }
            CanBusUnit::getCanBus2()->sendData(id,txData);
            memset(txData,0,8);
        }
    }

}
