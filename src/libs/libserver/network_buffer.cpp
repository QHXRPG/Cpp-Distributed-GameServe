#include "packet.h"
#include "network_buffer.h"
#include "connect_obj.h"

#include <cstdlib>
#include <cstring>

NetworkBuffer::NetworkBuffer(const unsigned size, ConnectObj* pConnectObj)
{
    _pConnectObj = pConnectObj;
    _bufferSize = size;
    _beginIndex = 0;
    _endIndex = 0;
    _dataSize = 0;
    _buffer = new char[_bufferSize];
}

NetworkBuffer::~NetworkBuffer()
{
    if (_buffer != nullptr)
        delete[] _buffer;
}

bool NetworkBuffer::HasData() const
{
    if (_dataSize <= 0)
        return false;

    // ����Ҫ��һ��Э��ͷ
    if (_dataSize < sizeof(PacketHead))
        return false;

    return true;
}

unsigned int NetworkBuffer::GetEmptySize()
{
    return _bufferSize - _dataSize;
}

unsigned int NetworkBuffer::GetWriteSize() const
{
    if (_beginIndex <= _endIndex)
    {
        return _bufferSize - _endIndex;
    }
    else
    {
        return _beginIndex - _endIndex;
    }
}

unsigned int NetworkBuffer::GetReadSize() const
{
    if (_dataSize <= 0)
        return 0;

    if (_beginIndex < _endIndex)
    {
        return _endIndex - _beginIndex;
    }
    else
    {
        return _bufferSize - _beginIndex;
    }
}

void NetworkBuffer::FillDate(unsigned int  size)
{
    _dataSize += size;

    // �ƶ���ͷ��
    if ((_bufferSize - _endIndex) <= size)
    {
        size -= _bufferSize - _endIndex;
        _endIndex = 0;
    }

    _endIndex += size;
}

void NetworkBuffer::RemoveDate(unsigned int size)
{
    _dataSize -= size;

    if ((_beginIndex + size) >= _bufferSize)
    {
        size -= _bufferSize - _beginIndex;
        _beginIndex = 0;
    }

    _beginIndex += size;
}

void NetworkBuffer::ReAllocBuffer()
{
    Buffer::ReAllocBuffer(_dataSize);
}


///////////////////////////////////////////////////////////////////////////////////////////////

RecvNetworkBuffer::RecvNetworkBuffer(const unsigned int size, ConnectObj* pConnectObj) : NetworkBuffer(size, pConnectObj) {

}

void RecvNetworkBuffer::Dispose() {

}

int RecvNetworkBuffer::GetBuffer(char*& pBuffer) const
{
    pBuffer = _buffer + _endIndex;
    return GetWriteSize();
}

Packet* RecvNetworkBuffer::GetPacket()
{
    // ���ݳ��Ȳ���
    if (_dataSize < sizeof(TotalSizeType))
    {
        return nullptr;
    }

    // 1.���� ���峤��
    unsigned short totalSize;
    MemcpyFromBuffer(reinterpret_cast<char*>(&totalSize), sizeof(TotalSizeType));

    // Э���峤�Ȳ������ȴ�
    if (_dataSize < totalSize)
    {
        return nullptr;
    }

    RemoveDate(sizeof(TotalSizeType));

    // 2.���� PacketHead
    PacketHead head;
    MemcpyFromBuffer(reinterpret_cast<char*>(&head), sizeof(PacketHead));
    RemoveDate(sizeof(PacketHead));

    // 3.���� Э��
    const auto socket = _pConnectObj->GetSocket();
    Packet* pPacket = new Packet(head.MsgId, socket);
    const auto dataLength = totalSize - sizeof(PacketHead) - sizeof(TotalSizeType);
    while (pPacket->GetTotalSize() < dataLength)
    {
        pPacket->ReAllocBuffer();
    }

    MemcpyFromBuffer(pPacket->GetBuffer(), dataLength);
    pPacket->FillData(dataLength);
    RemoveDate(dataLength);

    return pPacket;
}

void RecvNetworkBuffer::MemcpyFromBuffer(char* pVoid, const unsigned int size)
{
    const auto readSize = GetReadSize();
    if (readSize < size)
    {
        // 1.copyβ������
        ::memcpy(pVoid, _buffer + _beginIndex, readSize);

        // 2.copyͷ������
        ::memcpy(pVoid + readSize, _buffer, size - readSize);
    }
    else
    {
        ::memcpy(pVoid, _buffer + _beginIndex, size);
    }
}


///////////////////////////////////////////////////////////////////////////////////////////////

SendNetworkBuffer::SendNetworkBuffer(const unsigned int size, ConnectObj* pConnectObj) : NetworkBuffer(size, pConnectObj)
{

}

void SendNetworkBuffer::Dispose()
{

}

int SendNetworkBuffer::GetBuffer(char*& pBuffer) const
{
    if (_dataSize <= 0)
    {
        pBuffer = nullptr;
        return 0;
    }

    if (_beginIndex < _endIndex)
    {
        pBuffer = _buffer + _beginIndex;
        return _endIndex - _beginIndex;
    }
    else
    {
        pBuffer = _buffer + _beginIndex;
        return _bufferSize - _beginIndex;
    }
}

void SendNetworkBuffer::AddPacket(Packet* pPacket)
{
    const auto dataLength = pPacket->GetDataLength();
    TotalSizeType totalSize = dataLength + sizeof(PacketHead) + sizeof(TotalSizeType);

    // ���Ȳ���������
    while (GetEmptySize() < totalSize) {
        ReAllocBuffer();
    }

    //std::cout << "send buffer::Realloc. _bufferSize:" << _bufferSize << std::endl;

    // 1.���峤��
    MemcpyToBuffer(reinterpret_cast<char*>(&totalSize), sizeof(TotalSizeType));

    // 2.ͷ��
    PacketHead head;
    head.MsgId = pPacket->GetMsgId();
    MemcpyToBuffer(reinterpret_cast<char*>(&head), sizeof(PacketHead));

    // 3.����
    MemcpyToBuffer(pPacket->GetBuffer(), pPacket->GetDataLength());
}

void SendNetworkBuffer::MemcpyToBuffer(char* pVoid, const unsigned int size)
{
    const auto writeSize = GetWriteSize();
    if (writeSize < size)
    {
        // 1.copy��β��
        ::memcpy(_buffer + _endIndex, pVoid, writeSize);

        // 2.copy��ͷ��
        ::memcpy(_buffer, pVoid + writeSize, size - writeSize);
    }
    else
    {
        ::memcpy(_buffer + _endIndex, pVoid, size);
    }

    FillDate(size);
}
