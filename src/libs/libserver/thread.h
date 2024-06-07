#pragma once

#include <thread>
#include <list>

#include "sn_object.h"
#include "disposable.h"
#include "cache_swap.h"
#include "cache_refresh.h"
#include "thread_obj.h"
#include "packet.h"

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
    void Stop();
    bool IsRun() const;
   
private:
    bool _isRun;
    std::thread _thread;
};

