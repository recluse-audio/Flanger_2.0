/*
  ==============================================================================

    LFOSliders.h
    Created: 5 Feb 2022 11:15:36am
    Author:  ryand

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
//==============================================================================
/*
*/
class LFOSliders  : public juce::Component
{
public:
    LFOSliders(FlangerAudioProcessor&, StringArray);
    ~LFOSliders() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void rotateSliders();
private:
    std::unique_ptr<Slider> depth, rate, feedback;
    std::unique_ptr<Label> depthLabel, rateLabel, fbLabel;

	using Attach = juce::AudioProcessorValueTreeState::SliderAttachment;
    std::unique_ptr<Attach> depthAttach, rateAttach, fbAttach;

    FlangerAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LFOSliders)
};
