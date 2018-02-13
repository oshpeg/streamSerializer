#include "serializer.h"

CSerializer::CSerializer(			int						innerFixedSizeBufferLen,
									SERIALIZER_CALLBACK     pCallback)
{
	m_innerBufferLen = innerFixedSizeBufferLen;

	m_pInnerBuffer = new compexFloat[m_innerBufferLen * 2]; // 2 is for stock

	m_pCallback = pCallback;

	Reset();
}

CSerializer::~CSerializer()
{
	delete[] m_pInnerBuffer;
}

void CSerializer::Reset()
{
	m_innerBufferPos = 0;
	memset(m_pInnerBuffer, 0, 2 * m_innerBufferLen * sizeof(compexFloat));
}

void CSerializer::Process(			compexFloat				*inputBufferOfFixedSize,
									unsigned int			inputBufferLength)
{
	// read not more than inputBufferLength
	int readSubBlockLen = inputBufferLength < m_innerBufferLen ? inputBufferLength : m_innerBufferLen;

	for (int bufPos = 0; bufPos < inputBufferLength; bufPos += readSubBlockLen) 
	{
		int bufPosReminder	= inputBufferLength - bufPos;
		int bufNumRead		= readSubBlockLen < bufPosReminder ? readSubBlockLen : bufPosReminder;

		memcpy(&m_pInnerBuffer[m_innerBufferPos], &inputBufferOfFixedSize[bufPos], bufNumRead * sizeof(compexFloat));

		m_innerBufferPos += bufNumRead;

		if (m_innerBufferPos >= m_innerBufferLen)
		{
			// here we are free to do anything with m_pInnerBuffer of fixed size
			runSomeFunction(&m_pInnerBuffer[0], m_innerBufferLen);

			// save remainder after buffer extraction
			m_innerBufferPos -= m_innerBufferLen;
			memcpy(&m_pInnerBuffer[0], &m_pInnerBuffer[m_innerBufferLen], m_innerBufferPos*sizeof(compexFloat));
		}

	}

}

void CSerializer::runSomeFunction(	compexFloat				*inputBufferOfFixedSize,
									unsigned int			inputBufferLength)
{
	if (m_pCallback)
		m_pCallback(inputBufferOfFixedSize, inputBufferLength);
}