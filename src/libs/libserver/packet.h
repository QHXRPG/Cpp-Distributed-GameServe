#pragma once

#include "base_buffer.h"
#include "common.h"

#pragma pack(push)
#pragma pack(4)

struct PacketHead {
	unsigned short MsgId;
};

#pragma pack(pop)
// Ĭ�ϴ�С 10KB
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

	// �����л��� Proto ����> Э��ṹ
	template<class ProtoClass>
	ProtoClass ParseToProto()
	{
		ProtoClass proto;
		
		// ���ṹ���л���Packet�Ļ�������
		proto.ParsePartialFromArray(GetBuffer(), GetDataLength());
		return proto;
	}

	// ���л��� Э��ṹ ����> �������ļ�
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
