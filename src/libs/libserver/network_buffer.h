#pragma once

#include "base_buffer.h"

#if TestNetwork
#define DEFAULT_SEND_BUFFER_SIZE	10
#define DEFAULT_RECV_BUFFER_SIZE	10
#else
// Ĭ�ϴ�С 128KB
#define DEFAULT_SEND_BUFFER_SIZE	1024 * 128
#define DEFAULT_RECV_BUFFER_SIZE	1024 * 128
#endif

class Packet;
class ConnectObj;

// �洢Э���ܳ��ȵ�����
using TotalSizeType = unsigned short;

class NetworkBuffer : public Buffer
{
public:
    NetworkBuffer(const unsigned int size, ConnectObj* pConnectObj);
    virtual ~NetworkBuffer();
    void BackToPool();

    bool HasData() const;

    // ��������ͷ�뻷��βһ���Ŀ��ֽ���
    unsigned int GetEmptySize() override;

    // ��ǰ��д����
    unsigned int GetWriteSize() const;

    // ��ǰ�ɶ�����
    unsigned int GetReadSize() const;

    void FillDate(unsigned int  size);
    void RemoveDate(unsigned int size);
    void ReAllocBuffer();

protected:
    // �ڻ����У���������� _endIndex ������ _beginIndex �غ�
    // �غ�ʱ�����ֿ��ܣ�һ����û�����ݣ���һ����������
    unsigned int _dataSize; // ��Ч����
    ConnectObj* _pConnectObj{ nullptr };
};


class RecvNetworkBuffer : public NetworkBuffer
{
public:
    RecvNetworkBuffer(unsigned int _size, ConnectObj* pConnectObj);
    int GetBuffer(char*& pBuffer) const;
    Packet* GetPacket();

private:
    void MemcpyFromBuffer(char* pVoid, unsigned int size);
};

class SendNetworkBuffer : public NetworkBuffer
{
public:
    SendNetworkBuffer(unsigned int _size, ConnectObj* pConnectObj);
    int GetBuffer(char*& pBuffer) const;
    void AddPacket(Packet* pPacket);

private:
    void MemcpyToBuffer(char* pVoid, unsigned int size);
};
