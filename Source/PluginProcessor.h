/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DelayBase.h"
//==============================================================================
/**
*/
class FlangerAudioProcessor  : public juce::AudioProcessor, public juce::ValueTree::Listener
{
public:
    //==============================================================================
    FlangerAudioProcessor();
    ~FlangerAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    void sampleLoop(juce::AudioBuffer<float>&);
    void channelLoop(juce::AudioBuffer<float>&, int sampleIndex);
    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    juce::AudioProcessorValueTreeState apvts;
    juce::AudioProcessorValueTreeState::ParameterLayout createParameters();
    void savePreset(StringRef presetPath);
    void loadPreset(StringRef presetPath);

    void updateParameters();
    juce::String& getParamName();

    float getDelay(int reelIndex) 
    { 
        return delayLines[reelIndex]->getDelayTime(); 
    }

    void flangePress(float pressure, int reelIndex);
    void flangeRelease(int reelIndex);

    void debug(float value) 
    {
        if (mustDebug) 
        { 
            mustDebug = false;
        }
    }

    void setLFOMode(int reelIndex, int mode);

private:
    juce::OwnedArray<DelayBase> delayLines;
    String userPresetPath{ "C:/ProgramData/Recluse-Audio/Rompler/Presets/User Presets/" };

    std::vector<float> lastOutput   { 0.f, 0.f };

    bool mustUpdateParameters = false;
    bool mustDebug = false;

    void valueTreePropertyChanged(juce::ValueTree& tree, const juce::Identifier& property) override
    {
        mustUpdateParameters = true;
    }
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FlangerAudioProcessor)
};
