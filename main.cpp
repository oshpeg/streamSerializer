#include <stdio.h>
#include <stdlib.h>
#include "serializer.h"

/* TESTBENCH in runTestbench()
	
	1) at each iteration random value sets buffer size (dynamic memory allocation)
	2) each complex value of buffer is set by random generator and is printed in a file "testInput" thus we can view all data in a file
	3) buffer is fed to Serialier object where data is cut to buffers of fixed length 
	4) Serialier object has a callback which prints serialesed data to a file "testOutput"
	5) after execution you have to manually compare two files where onle last fractional part of input data will be missed in "testOutput"

*/

void runTestbench(void);
void serializerCallback(compexFloat *outputBuffer, unsigned int outputBufferLen);

FILE *pFileTest_Input;
FILE *pFileTest_Output;

int main(int argc, char* argv[])
{
	pFileTest_Input = fopen("testInput", "w");
	pFileTest_Output = fopen("testOutput", "w");

	runTestbench();

	fclose(pFileTest_Input);
	fclose(pFileTest_Output);
}

void runTestbench(void)
{
	int fixedSizeLength = 7;
	CSerializer oSerialier(fixedSizeLength, serializerCallback);
	oSerialier.Reset();

	int iterationsNumber = 100;

	compexFloat *p_testInputBuffer;

	for (int itCnt = 0; itCnt < iterationsNumber; itCnt++)
	{
		int blockLenRandom = rand() & 0xFF; // not more than 256
		
		p_testInputBuffer = new compexFloat[blockLenRandom];

		for (int sampleCnt = 0; sampleCnt < blockLenRandom; sampleCnt++)
		{
			p_testInputBuffer[sampleCnt].Re = (float)rand();
			p_testInputBuffer[sampleCnt].Im = (float)rand();

			fprintf(pFileTest_Input, "%f %f\n", p_testInputBuffer[sampleCnt].Re, p_testInputBuffer[sampleCnt].Im);
		}

		oSerialier.Process(p_testInputBuffer, blockLenRandom);

		delete[] p_testInputBuffer;
	}
	
}

void serializerCallback(compexFloat *outputBuffer, unsigned int outputBufferLen)
{
	for (int sampleCnt = 0; sampleCnt < outputBufferLen; sampleCnt++)
	{
		fprintf(pFileTest_Output, "%f %f\n", outputBuffer[sampleCnt].Re, outputBuffer[sampleCnt].Im);
	}

}