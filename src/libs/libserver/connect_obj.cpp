#include <iostream>

#include "connect_obj.h"

#include "network.h"
#include "network_buffer.h"

#include "packet.h"
#include "thread_mgr.h"

ConnectObj::ConnectObj(Network* pNetWork, int socket) :_pNetWork(pNetWork), _socket(socket)
{
	_recvBuffer = new RecvNetworkBuffer(DEFAULT_RECV_BUFFER_SIZE, this);
	_sendBuffer = new SendNetworkBuffer(DEFAULT_SEND_BUFFER_SIZE, this);
}

ConnectObj::~ConnectObj()
{
	if (_recvBuffer != nullptr)
		delete _recvBuffer;

	if (_sendBuffer != nullptr)
		delete _sendBuffer;
}

void ConnectObj::Dispose()
{
	//std::cout << "close socket:" << _socket << std::endl;
	_sock_close(_socket);

	_recvBuffer->Dispose();
	_sendBuffer->Dispose();
}

bool ConnectObj::HasRecvData() const
{
	return _recvBuffer->HasData();
}

Packet* ConnectObj::GetRecvPacket() const
{
	return _recvBuffer->GetPacket();
}

bool ConnectObj::Recv() const
{
	char *pBuffer = nullptr;
	bool isRs = false;

	// ��ϵͳ���绺����ȡ������ �����ػ���
	while (true)
	{
		// �ܿռ����ݲ���һ��ͷ�Ĵ�С������
		if (_recvBuffer->GetEmptySize() < (sizeof(PacketHead) + sizeof(TotalSizeType)))
		{
			_recvBuffer->ReAllocBuffer();
		}

		const int emptySize = _recvBuffer->GetBuffer(pBuffer);
		const int dataSize = ::recv(_socket, pBuffer, emptySize, 0);
		if (dataSize > 0)
		{
			_recvBuffer->FillDate(dataSize);
		}
		else if (dataSize == 0)
		{
			break;
		}
		else
		{
			const auto socketError = _sock_err();
			if (socketError == EINTR 
				|| socketError == EWOULDBLOCK
				|| socketError == EAGAIN)
				isRs = true;
			break;
		}
	}

	if(isRs)
	{
		// �ӱ��ػ�����ȡ�����ݲ�ת��packet
		while(true)
		{
			const auto pPacket = _recvBuffer->GetPacket();
			if(pPacket == nullptr)
				break;
			ThreadMgr::GetInstance()->AddPacket(pPacket);
		}
	}
	return isRs;
}

bool ConnectObj::HasSendData() const
{
	return _sendBuffer->HasData();
}

void ConnectObj::SendPacket(Packet* pPacket) const
{
	_sendBuffer->AddPacket(pPacket);
}

bool ConnectObj::Send() const
{
	while (true) {
		char *pBuffer = nullptr;
		const int needSendSize = _sendBuffer->GetBuffer(pBuffer);

		// û�����ݿɷ���
		if (needSendSize <= 0)
		{
			return true;
		}

		const int size = ::send(_socket, pBuffer, needSendSize, 0);
		if (size > 0)
		{
			//std::cout << "send size:" << size << std::endl;
			_sendBuffer->RemoveDate(size);

			// ��һ֡�ٷ���
			if (size < needSendSize)
			{
				return true;
			}
		}

		if (size == -1)
		{
			const auto socketError = _sock_err();
			std::cout << "needSendSize:" << needSendSize << " error:" << socketError << std::endl;
			return false;
		}
	}
}

