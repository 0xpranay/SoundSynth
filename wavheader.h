#pragma once
#include<cstdint>
#include<string.h>
#include<iostream>
#include<math.h>
#define CLAMP(value,min,max) {if(value < min) { value = min; } else if(value > max) { value = max; }}
struct SMinimalWaveFileHeader
{
	//the main chunk
	unsigned char m_szChunkID[4];
	uint32_t		  m_nChunkSize;
	unsigned char m_szFormat[4];

	//sub chunk 1 "fmt "
	unsigned char m_szSubChunk1ID[4];
	uint32_t		  m_nSubChunk1Size;
	uint16_t		  m_nAudioFormat;
	uint16_t	      m_nNumChannels;
	uint32_t		  m_nSampleRate;
	uint32_t		  m_nByteRate;
	uint16_t		  m_nBlockAlign;
	uint16_t		  m_nBitsPerSample;

	//sub chunk 2 "data"
	unsigned char m_szSubChunk2ID[4];
	uint32_t		  m_nSubChunk2Size;

	//then comes the data!
};
float CalcFrequency(float fOctave,float fNote)
/*
	Calculate the frequency of any note!
	frequency = 440×(2^(n/12))
	N=0 is A4
	N=1 is A#4
	etc...
	notes go like so...
	0  = A
	1  = A#
	2  = B
	3  = C
	4  = C#
	5  = D
	6  = D#
	7  = E
	8  = F
	9  = F#
	10 = G
	11 = G#
*/
{
	return (float)(440*pow(2.0,((double)((fOctave-4)*12+fNote))/12.0));
}
void ConvertFloatToAudioSample(float fFloat, uint8_t &nOut)
{
	fFloat = (fFloat + 1.0f) * 127.5f;
	CLAMP(fFloat,0.0f,255.0f);
	nOut = (uint8_t)fFloat;
}
void ConvertFloatToAudioSample(float fFloat, int16_t &nOut)
{
	fFloat *= 32767.0f;
	CLAMP(fFloat,-32768.0f,32767.0f);
	nOut = (int16_t)fFloat;
}
void ConvertFloatToAudioSample(float fFloat, int32_t &nOut)
{
	double dDouble = (double)fFloat;
	dDouble *= 2147483647.0;
	CLAMP(dDouble,-2147483648.0,2147483647.0);
	nOut = (int32_t)dDouble;
}
template <typename T>
bool WriteWaveFile(char const *szFileName, float *pRawData, int32_t nNumSamples, int16_t nNumChannels, int32_t nSampleRate)
{
	//open the file if we can
	FILE *File = fopen(szFileName,"w+b");
	if(!File)
	{
		return false;
	}

	//calculate bits per sample and the data size
	int32_t nBitsPerSample = sizeof(T) * 8;
	int nDataSize = nNumSamples * sizeof(T);

	SMinimalWaveFileHeader waveHeader;

	//fill out the main chunk
	memcpy(waveHeader.m_szChunkID,"RIFF",4);
	waveHeader.m_nChunkSize = nDataSize + 36;
	memcpy(waveHeader.m_szFormat,"WAVE",4);

	//fill out sub chunk 1 "fmt "
	memcpy(waveHeader.m_szSubChunk1ID,"fmt ",4);
	waveHeader.m_nSubChunk1Size = 16;
	waveHeader.m_nAudioFormat = 1;
	waveHeader.m_nNumChannels = nNumChannels;
	waveHeader.m_nSampleRate = nSampleRate;
	waveHeader.m_nByteRate = nSampleRate * nNumChannels * nBitsPerSample / 8;
	waveHeader.m_nBlockAlign = nNumChannels * nBitsPerSample / 8;
	waveHeader.m_nBitsPerSample = nBitsPerSample;

	//fill out sub chunk 2 "data"
	memcpy(waveHeader.m_szSubChunk2ID,"data",4);
	waveHeader.m_nSubChunk2Size = nDataSize;
    //write the header
	fwrite(&waveHeader,sizeof(SMinimalWaveFileHeader),1,File);

	//write the wave data itself, converting it from float to the type specified
	T *pData = new T[nNumSamples];
	for(int nIndex = 0; nIndex < nNumSamples; ++nIndex)
		ConvertFloatToAudioSample(pRawData[nIndex],pData[nIndex]);
	fwrite(pData,nDataSize,1,File);
	delete[] pData;

	//close the file and return success
	fclose(File);
	return true;
}