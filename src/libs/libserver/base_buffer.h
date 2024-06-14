#pragma once

// ׷�Ӵ�С
#if TestNetwork
#define ADDITIONAL_SIZE 10
#else
#define ADDITIONAL_SIZE 1024 * 128
#endif

// ��󻺳�
#define MAX_SIZE		1024 * 1024 // 1M

class Buffer
{
public:
	virtual unsigned int GetEmptySize();
	void ReAllocBuffer(unsigned int dataLength);
	unsigned int GetEndIndex() const
	{
		return _endIndex;
	}

	unsigned int GetBeginIndex() const
	{
		return _beginIndex;
	}

	unsigned int GetTotalSize() const
	{
		return _bufferSize;
	}

protected:
	char* _buffer{ nullptr };
	unsigned int _beginIndex{ 0 }; // buffer���� ��ʼλ�����λ
	unsigned int _endIndex{ 0 };

	unsigned int _bufferSize{ 0 }; // �ܳ���
};

