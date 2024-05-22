#pragma once

#include "base_buffer.h"

#if TestNetwork
#define DEFAULT_SEND_BUFFER_SIZE	10
#define DEFAULT_RECV_BUFFER_SIZE	10
#else
// 默认大小 128KB
#define DEFAULT_SEND_BUFFER_SIZE	1024 * 128
#define DEFAULT_RECV_BUFFER_SIZE	1024 * 128
#endif

class Packet;

// 存储协议总长度的类型
using TotalSizeType = unsigned short;

class NetworkBuffer : public Buffer
{
public:
	explicit NetworkBuffer(const unsigned int size);

	virtual ~NetworkBuffer();

	bool HasData() const;

	// 包括环的头与环的尾一共的空字节数
	unsigned int GetEmptySize() override;

	// 当前可写长度
	unsigned int GetWriteSize() const;

	// 当前可读长度
	unsigned int GetReadSize() const;

	void FillDate(unsigned int  size);
	void RemoveDate(unsigned int size);
	void ReAllocBuffer();

protected:
	// 在环形中，极端情况下 _endIndex 可能与 _beginIndex 重合
	// 重合时有两种可能，一种是没有数据，另一种是满数据
	unsigned int _dataSize; // 有效数据
};


class RecvNetworkBuffer : public NetworkBuffer {
public:
	explicit RecvNetworkBuffer(unsigned int _size);
	void Dispose() override;
	int GetBuffer(char*& pBuffer) const;
	Packet* GetPacket();

private:
	void MemcpyFromBuffer(char* pVoid, unsigned int size);
};

class SendNetworkBuffer : public NetworkBuffer {
public:
	explicit SendNetworkBuffer(unsigned int _size);
	void Dispose() override;
	int GetBuffer(char*& pBuffer) const;
	void AddPacket(Packet* pPacket);

private:
	void MemcpyToBuffer(char* pVoid, unsigned int size);
};
