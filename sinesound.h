#pragma once
#include<math.h>
#include<stdint.h>
#include<iostream>
#include<string>
template <typename T>
bool WriteWaveFile(char const *szFileName, float *pRawData, int32_t nNumSamples, int16_t nNumChannels, int32_t nSampleRate);
class sinesound
{
private:
    char* name;
    int nSeconds = 0;
    float fPhase = 0;
    float fFrequency = 0;
    float fSampleRate = 0;
    int nNumSamples = 0;
    int nNumChannels = 1;
    float* pData;
public:
float AdvanceOscilator_Sine(float &fPhase, float fFrequency, float fSampleRate)
    {
        fPhase += 2 * (float)M_PI * fFrequency/fSampleRate;

        while(fPhase >= 2 * (float)M_PI)
            fPhase -= 2 * (float)M_PI;

        while(fPhase < 0)
            fPhase += 2 * (float)M_PI;

        return sin(fPhase);
    }
    sinesound(int nSeconds,float fFrequency, float fSampleRate)
    {
        this->nSeconds = nSeconds;
        this->fFrequency = fFrequency;
        this->fSampleRate = fSampleRate;
        this->nNumSamples = fSampleRate * nNumChannels * nSeconds;
        pData = new float[nNumSamples];
    }
    void generatewav(float fPhase, char *fileName)
    {
        this->name = fileName;
        this->fPhase = fPhase;
        for(int index = 0;index < nNumSamples;index++)
        {
            pData[index] = AdvanceOscilator_Sine(fPhase, fFrequency,fSampleRate) * 1.4f;
            std::cout<<"Sample "<<pData[index]<<" written\n";
        }
        WriteWaveFile<int16_t>(name, pData, nNumSamples, 1, fSampleRate);
        std::cout<<"Sine wave file "<<name<<" written\n";
    }
    ~sinesound()
    {}
    
};