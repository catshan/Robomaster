//
// Created by Administrator on 2019/11/17.
//

#ifndef ROBOFRAMEWORK_TOTAL_REMOTEUNIT_H
#define ROBOFRAMEWORK_TOTAL_REMOTEUNIT_H

#define REMOTE_MAX_VAL 1684
#define REMOTE_MID_VAL 1024
#define REMOTE_MIN_VAL 364

#include "stm32el.h"
//RemoteData
namespace RoboFramework {
    typedef __packed struct
    {
        struct
        {
            u16 ch0:11;
            u16 ch1:11;
            u16 ch2:11;
            u16 ch3:11;
            u8 s1:2;
            u8 s2:2;
        }rc;
        struct
        {
            short x;
            short y;
            short z;
            u8 press_l;
            u8 press_r;
        }mouse;
        struct
        {
            u16 v;
        }key;
    }RemoteData;

//Remote
class Remote{
private:
    static SerialPort<Usart1 >* pUsart;
public:
    static void Init(u8* rxBuffer);
    static void DeInit();
};

//RemoteInputStream
class RemoteInputStream{
private:
    u8 rxBuffer[18] = {0};
public:
    const u8 *getRxBuffer() const;
    RemoteInputStream();
};

//RemoteListenerInterface
class RemoteListenerInterface{
public:
    static bool IsOnline(RemoteData& RC_data);
    virtual void OnGetDataReady(RemoteData& data) = 0;
};

//RemoteUnit
class RemoteUnit{
private:
    static RemoteInputStream* stream;
    static RemoteListenerInterface* listenerInterface;
public:
    static RemoteInputStream *getStream();
    static RemoteListenerInterface *getListener();

public:
    static void Init();
    static void BindListener(RemoteListenerInterface* listener);
    static void Decode(RemoteData* data);
};

//RemoteThread
class RemoteUnitThread:public ThreadTask{
private:
    RemoteData data = {0};
protected:
    void start() override;
};


}

#endif //ROBOFRAMEWORK_TOTAL_REMOTEUNIT_H
