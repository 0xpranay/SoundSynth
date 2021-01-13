#pragma once
#include<iostream>
#include "wavheader.h"
class sawtooth
{
private:
    int nSampleRate = 44100;
    int nNumSeconds = 4;
    int nNumChannels = 1;
    int nNumSamples;
    int32_t* data;
public:
    sawtooth(/* args */)
    {
        std::cout<<"Default sawtooth\n";
    }
    ~sawtooth()
    {
        delete[] data;
    }
    sawtooth(int nSampleRate, int nNumSeconds, int nNumChannels)
    {
        std::cout<<"User sawtooth\n";
        this->nSampleRate = nSampleRate;
        this->nNumSeconds = nNumSeconds;
        this->nNumChannels = nNumChannels;
        nNumSamples= nSampleRate * nNumChannels * nNumSeconds;
        data = new int32_t[nNumSamples];
        int32_t nValue = 0;
        for(int index = 0;index < nNumSamples;index++)
        {
            nValue += 8000000;
            data[index] = nValue;
        }
        WriteWaveFile("Sawtoothmono.wav", data, nNumSamples * sizeof(int32_t), nNumChannels, nSampleRate, sizeof(int32_t)* 8);
    }
};
