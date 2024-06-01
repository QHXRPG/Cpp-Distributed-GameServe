#pragma once

#include "base_buffer.h"
#include "common.h"

#pragma pack(push)
#pragma pack(4)

struct PacketHead {
	unsigned short MsgId;
};

#pragma pack(pop)
// 默认大小 10KB
#define DEFAULT_PACKET_BUFFER_SIZE	1024 * 10


class Packet : public Buffer {
public:
	Packet(const int msgId, SOCKET socket);
	~Packet();

	void Dispose() override;
	void CleanBuffer();

	char* GetBuffer() const;
	void AddBuffer(const char* pBuffer, const unsigned int size);
	unsigned short GetDataLength() const;
	int GetMsgId() const;
	void FillData(unsigned int size);
	void ReAllocBuffer();

	// 反序列化： Proto ——> 协议结构
	template<class ProtoClass>
	ProtoClass ParseToProto()
	{
		ProtoClass proto;
		
		// 将结构序列化到Packet的缓存区中
		proto.ParsePartialFromArray(GetBuffer(), GetDataLength());
		return proto;
	}

	// 序列化： 协议结构 ——> 二进制文件
	template<class ProtoClass>
	void SerializeToBuffer(ProtoClass &protoClass)
	{
		auto total = protoClass.ByteSizeLong();
		while(GetEmptySize() < total)
		{
			ReAllocBuffer();
		}
		protoClass.SerializePartialToArray(GetBuffer(), total);
		FillData(total);
	}
	SOCKET GetSocket() const;

private:
	int _msgId;
	SOCKET _socket;
};
