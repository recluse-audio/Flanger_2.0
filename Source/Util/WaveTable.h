/*
  ==============================================================================

    WaveTable.h
    Created: 27 May 2021 7:42:15am
    Author:  ryand

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class WaveTable
{
public:
    /*
        TO DO: run this as a []() function*
    */
    WaveTable(int lengthInSamples = 2048) : waveBuffer(1, lengthInSamples)
    {
        tableSize = lengthInSamples;
    }



    ~WaveTable() {}

    void prepare(double sampleRate)
    {
        mSampleRate = sampleRate;
    }

    // used for defaults, lfo's... not involved in loading new tables
    void createSineTable()
    {
        waveBuffer.setSize(1, (int)tableSize);
        waveBuffer.clear();

        auto* buffWrite = waveBuffer.getWritePointer(0);

        auto angleDelta = juce::MathConstants<double>::twoPi / (double)(tableSize - 1);
        auto pi = juce::MathConstants<double>::pi;
        double currentAngle = -pi;

        for (unsigned int i = 0; i < tableSize; ++i)
        {
            float sample;
            sample = std::sin(currentAngle);

            buffWrite[i] += sample;
            currentAngle += angleDelta;
        }


        buffWrite[tableSize] = buffWrite[0];
    }

    float getNextSample()
    {
        auto index0 = (unsigned int)currentIndex;

        auto frac = currentIndex - (float)index0;
        auto* table = waveBuffer.getReadPointer(0);

        auto value0 = table[index0];
        auto value1 = table[index0 + 1];

        currentSample = value0 + (frac * (value1 - value0));

        currentSample *= gain;

        currentIndex += tableDelta;

        if (currentIndex > (float)waveBuffer.getNumSamples())
        {
            currentIndex = 0;
        }

        return currentSample;
    }

    // way of getting sample without increment
    float getCurrentSample()
    {
        return currentSample;
    }

    void setFrequency(float freq)
    {
        auto tableSizeOverSampleRate = (float)waveBuffer.getNumSamples() / mSampleRate;
       // auto tableScale = // scaling according to how 
        tableDelta = freq * tableSizeOverSampleRate;
    }

    AudioBuffer<float>& getBuffer()
    {
        return waveBuffer;
    }

    // specifically for visualizer (exists in case user dropped in a wavetable of a different size)
    AudioBuffer<float>& getMappedBuffer()
    {
        if (waveBuffer.getNumSamples() != tableSize)
        {

        }
    }

    // passes new buffer to wavetable, handles conversion to size of 2048
    void passBuffer(AudioBuffer<float>& newTable) // coming in at length of period
    {

        auto buffRead = newTable.getArrayOfReadPointers();
        auto buffWrite = waveBuffer.getArrayOfWritePointers();
        float sizeRatio = (float)newTable.getNumSamples() / (float)tableSize;

        for (int i = 0; i < tableSize; i++)
        {
            auto waveIndex = i; // for our nice 2048 sample sized wavetable
                
            // downsampling / interpolation algorithm to make new table into 2048
            auto readIndex = i * sizeRatio;
            float frac = readIndex - (int)readIndex; 

            float readSample0 = buffRead[0][(int)readIndex] * (1 - frac);
            float readSample1 = buffRead[0][(int)readIndex + 1] * frac;
            float readSample = readSample0 + readSample1;
              
            buffWrite[0][i] = readSample;

        }

    }

    void setGain(float gainVal)
    {
        gain = gainVal;
    }
private:
    juce::AudioBuffer<float> waveBuffer;
    int tableSize = 2048;
    double mSampleRate = 48000;
    float tableDelta = 0.f, currentIndex = 0.f, currentSample = 0.f;
    float gain = 1.f;
};