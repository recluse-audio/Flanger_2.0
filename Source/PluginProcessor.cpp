/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FlangerAudioProcessor::FlangerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), apvts(*this, nullptr, "Parameters", createParameters())
#endif
{
    apvts.state.addListener(this);

    for (int i = 0; i < 2; i++)
    {
        delayLines.add(new DelayBase());
    }

    updateParameters();
}

FlangerAudioProcessor::~FlangerAudioProcessor()
{
}

//==============================================================================
const juce::String FlangerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool FlangerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool FlangerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool FlangerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double FlangerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int FlangerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int FlangerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void FlangerAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String FlangerAudioProcessor::getProgramName (int index)
{
    return {};
}

void FlangerAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void FlangerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    for (int i = 0; i < delayLines.size(); i++)
        delayLines[i]->prepare(sampleRate, samplesPerBlock);
}

void FlangerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool FlangerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else

    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void FlangerAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    sampleLoop(buffer);
}

void FlangerAudioProcessor::sampleLoop(juce::AudioBuffer<float>& buffer)
{
    for (int sampleIndex = 0; sampleIndex < buffer.getNumSamples(); sampleIndex++)
    {
        if (mustUpdateParameters)
            updateParameters();

        for (int i = 0; i < delayLines.size(); i++)
            delayLines[i]->increment();

        channelLoop(buffer, sampleIndex);
    }
}

void FlangerAudioProcessor::channelLoop(juce::AudioBuffer<float>& buffer, int sampleIndex)
{
    auto buffRead = buffer.getArrayOfReadPointers();
    auto buffWrite = buffer.getArrayOfWritePointers();

   // mustDebug = true;

    // writing and reading split in two loops... don't remember why
    for (int channel = 0; channel < buffer.getNumChannels(); channel++)
    {
        auto drySample = buffRead[channel][sampleIndex];

        for (int i = 0; i < delayLines.size(); i++)
        {
            auto samp = drySample + lastOutput[channel];
            delayLines[i]->pushSample(channel, samp);
        }
    }

    for (int channel = 0; channel < buffer.getNumChannels(); channel++)
    {
       // float fbSample = 0.f;

        for (int i = 0; i < delayLines.size(); i++)
        {
            auto wSample = delayLines[i]->popSample(channel);
            buffWrite[channel][sampleIndex] += wSample;
        }
    }
}

//==============================================================================
bool FlangerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* FlangerAudioProcessor::createEditor()
{
    return new FlangerAudioProcessorEditor (*this);
}

//==============================================================================
void FlangerAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    juce::ValueTree copyState = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml = copyState.createXml();
    copyXmlToBinary(*xml.get(), destData);
}

void FlangerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xml = getXmlFromBinary(data, sizeInBytes);
    juce::ValueTree copyState = juce::ValueTree::fromXml(*xml.get());
    apvts.replaceState(copyState);
}

void FlangerAudioProcessor::savePreset(StringRef presetName)
{
    auto presetPath = userPresetPath + presetName;
    MemoryBlock block;
    auto file = juce::File(presetPath);
    getStateInformation(block); // copies xml state to this 'block' a binary blob
    file.replaceWithData(block.getData(), block.getSize());
}

void FlangerAudioProcessor::loadPreset(StringRef presetPath)
{
    MemoryBlock block;
    auto file = juce::File(presetPath);
    file.loadFileAsData(block); // fills block with files data.  See setStateInformation() in Audio Processor Base class
    setStateInformation(block.getData(), (int)block.getSize());
    mustUpdateParameters = true;
}

juce::AudioProcessorValueTreeState::ParameterLayout FlangerAudioProcessor::createParameters()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> parameters;

    parameters.push_back(std::make_unique<juce::AudioParameterFloat>("LOW MODE 1", "Low Mode 1",
        juce::NormalisableRange<float>(0.0f, 1.f, 1.f, 1.f), 1.f));

    parameters.push_back(std::make_unique<juce::AudioParameterFloat>("LOW MODE 2", "Low Mode 2",
        juce::NormalisableRange<float>(0.0f, 1.f, 1.f, 1.f), 1.f));

    parameters.push_back(std::make_unique<juce::AudioParameterFloat>("MID MODE 1", "Mid Mode 1",
        juce::NormalisableRange<float>(0.0f, 1.f, 1.f, 1.f), 0.f));

    parameters.push_back(std::make_unique<juce::AudioParameterFloat>("MID MODE 2", "Mid Mode 2",
        juce::NormalisableRange<float>(0.0f, 1.f, 1.f, 1.f), 0.f));

    parameters.push_back(std::make_unique<juce::AudioParameterFloat>("HIGH MODE 1", "High Mode 1",
        juce::NormalisableRange<float>(0.0f, 1.f, 1.f, 1.f), 0.f));

    parameters.push_back(std::make_unique<juce::AudioParameterFloat>("HIGH MODE 2", "High Mode 2",
        juce::NormalisableRange<float>(0.0f, 1.f, 1.f, 1.f), 0.f));


    parameters.push_back(std::make_unique<juce::AudioParameterFloat>("LFO DEPTH 1", "LFO Depth 1",
        juce::NormalisableRange<float>(0.0f, 1.f, 0.001f, 1.0f), 0.f));

    parameters.push_back(std::make_unique<juce::AudioParameterFloat>("LFO DEPTH 2", "LFO Depth 2",
        juce::NormalisableRange<float>(0.0f, 1.f, 0.001f, 1.0f), 0.f));

    parameters.push_back(std::make_unique<juce::AudioParameterFloat>("LFO RATE 1", "LFO Rate 1",
        juce::NormalisableRange<float>(0.0f, 5.f, 0.01f, 1.0f), 0.5f));

    parameters.push_back(std::make_unique<juce::AudioParameterFloat>("LFO RATE 2", "LFO Rate 2",
        juce::NormalisableRange<float>(0.0f, 5.f, 0.01f, 1.0f), 0.5f));

    parameters.push_back(std::make_unique<juce::AudioParameterFloat>("FEEDBACK 1", "FEEDBACK 1",
        juce::NormalisableRange<float>(0.0f, 1.f, 0.01f, 1.0f), 0.2f));

    parameters.push_back(std::make_unique<juce::AudioParameterFloat>("FEEDBACK 2", "FEEDBACK 2",
        juce::NormalisableRange<float>(0.0f, 1.f, 0.01f, 1.0f), 0.2f));


    return { parameters.begin(), parameters.end() };
}



void FlangerAudioProcessor::updateParameters()
{
    mustUpdateParameters = false;
    auto lfoAmp1 = apvts.getRawParameterValue("LFO DEPTH 1");
    auto lfoAmp2 = apvts.getRawParameterValue("LFO DEPTH 2");
    delayLines[0]->setLFO(*lfoAmp1);
    delayLines[1]->setLFO(*lfoAmp2);
    auto lfoFreq1 = apvts.getRawParameterValue("LFO RATE 1");
    auto lfoFreq2 = apvts.getRawParameterValue("LFO RATE 2");
    delayLines[0]->setLFOFreq(*lfoFreq1);
    delayLines[1]->setLFOFreq(*lfoFreq2);
    auto feedback1 = apvts.getRawParameterValue("FEEDBACK 1");
    auto feedback2 = apvts.getRawParameterValue("FEEDBACK 2");
    delayLines[0]->setFeedback(*feedback1);
    delayLines[1]->setFeedback(*feedback2);

  //  smoothDelay.setTargetValue(*apvts.getRawParameterValue("Delay Time"));
}

juce::String& FlangerAudioProcessor::getParamName()
{
    return juce::String("");
}

void FlangerAudioProcessor::flangePress(float normalizedPressure, int reelIndex)
{
    auto pressureDelay = normalizedPressure * 75.f; // 75 corresponds with a max of 100ms delay caused by manual flanging
    delayLines[reelIndex]->setDelayTime(pressureDelay);
}

void FlangerAudioProcessor::flangeRelease(int reelIndex)
{
    delayLines[reelIndex]->setDelayTime(0.f);
}

void FlangerAudioProcessor::setLFOMode(int reelIndex, int mode)
{
    delayLines[reelIndex]->setLFOMode(mode);
}



//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FlangerAudioProcessor();
}
