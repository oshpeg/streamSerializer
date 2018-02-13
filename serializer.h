#ifndef _SERIALIZER_H_
#define _SERIALIZER_H_

#include <vector>
#include <complex>

typedef struct { float Re; float Im; }  compexFloat;

typedef void(*SERIALIZER_CALLBACK)(compexFloat *, unsigned int);

class CSerializer
{
public:

	// constructor with initialization
	CSerializer									(	int						innerFixedSizeBufferLen,
													SERIALIZER_CALLBACK     pCallback);
	~CSerializer								();

	// reset internal paremeters
	void 					Reset				();

	// take input buffer of variable length and give away buffer with fixed length
	void 					Process				(	compexFloat				*inputBufferOfVariableLength,
													unsigned int			inputBufferLength);

private:

	int						m_innerBufferLen;
	int						m_innerBufferPos;
	compexFloat				*m_pInnerBuffer;

	// callback not for multithreading
	SERIALIZER_CALLBACK		m_pCallback;

	void					runSomeFunction		(	compexFloat				*inputBufferOfFixedSize,
													unsigned int			inputBufferLength);
};


#endif