#pragma once

#include <thread>
#include <list>

#include "sn_object.h"
#include "disposable.h"
#include "cache_swap.h"
#include "cache_refresh.h"
#include "thread_obj.h"
#include "packet.h"

enum ThreadState
{
    ThreadState_Init,
    ThreadState_Run,
    ThreadState_Stoped,
};

class ThreadObjectList: public IDisposable
{
public:
    void AddObject(ThreadObject* _obj);
    void Update();
    void AddPacketToList(Packet* pPacket);
    void Dispose() override;

protected:
    // ���̵߳����ж���        
    std::mutex _obj_lock;
    CacheRefresh<ThreadObject> _objlist;

    // ���߳��е����д������
    std::mutex _packet_lock;
    CacheSwap<Packet> _cachePackets;
};

class Thread : public ThreadObjectList, public SnObject {
public:
    Thread();
    void Start();

    bool IsRun() const;
    bool IsStop() const;
    bool IsDispose();

private:
    ThreadState _state;
    std::thread _thread;
};

