/*
  ==============================================================================

    TapeReel.h
    Created: 3 Feb 2022 3:11:45pm
    Author:  ryand

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/*
*/
class TapeReel  : public juce::Component, juce::MouseListener
{
public:
    TapeReel(FlangerAudioProcessor& p, int index);
    ~TapeReel() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void mouseDown(const juce::MouseEvent& event);
    void mouseUp(const juce::MouseEvent& event);
    void mouseDrag(const juce::MouseEvent& event);

    float getPhaseDelay();
    int getReelIndex() { return reelIndex; }
    bool isCurrentlyFlanging() { return isFlanging;  }

    void incrementPhase();
private:
    juce::Image image;
    float normalizedPressure = 0.f;
    bool isFlanging = false;

    float phaseIncrement = 0;
    float phase = 0;
    double dTime = 0;
    float feedback = 0.f;
    int reelIndex = 0;

    FlangerAudioProcessor& audioProcessor;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TapeReel)
};
