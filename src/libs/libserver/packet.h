#pragma once

#include "base_buffer.h"

#pragma pack(push)
#pragma pack(4)

struct PacketHead {
	unsigned short MsgId;
};

#pragma pack(pop)

#if TestNetwork
#define DEFAULT_PACKET_BUFFER_SIZE	10
#else
// Ĭ�ϴ�С 10KB
#define DEFAULT_PACKET_BUFFER_SIZE	1024 * 10
#endif

class Packet : public Buffer {
public:
	Packet();
	Packet(const int msgId);
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

private:
	int _msgId;
};
