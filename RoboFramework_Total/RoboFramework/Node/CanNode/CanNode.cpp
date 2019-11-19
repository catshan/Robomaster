//
// Created by Administrator on 2019/11/15.
//

#include "CanNode.h"

u16 RoboFramework::CanNode::getSendId() const {
    return sendID;
}

u16 RoboFramework::CanNode::getListenId() const {
    return listenID;
}

u8 RoboFramework::CanNode::getSendIndexStart() const {
    return sendIndexStart;
}

const u8 *RoboFramework::CanNode::getTxBuffer() const {
    return txBuffer;
}

const u8 *RoboFramework::CanNode::getRxBuffer() const {
    return rxBuffer;
}
