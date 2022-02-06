/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "MixFeel.h"
#include "TapeReel.h"
#include "PresetMenu.h"
#include "LFOSliders.h"
//==============================================================================
/**
*/
class FlangerAudioProcessorEditor  : public juce::AudioProcessorEditor, juce::Timer, public juce::Slider::Listener, juce::Button::Listener
{
public:
    FlangerAudioProcessorEditor (FlangerAudioProcessor&);
    ~FlangerAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void timerCallback() override;
    void sliderValueChanged(juce::Slider* s) override;
    void buttonClicked(juce::Button* b) override;

    void initColors();
    void initButtons();
   
private:
    juce::OwnedArray<TapeReel> tapeReels;
    float tapeRadius = 100.f;
    Point<float> center1, center2;

    MixFeel mixFeel;

    std::unique_ptr<LFOSliders> lfo1;
	std::unique_ptr<LFOSliders> lfo2;

    std::unique_ptr<Label>  mainLabel;

    std::unique_ptr<juce::TextButton> buttonLow1;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> lowAttach1;

    std::unique_ptr<juce::TextButton> buttonLow2;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> lowAttach2;

    std::unique_ptr<juce::TextButton> buttonMid1;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> midAttach1;

    std::unique_ptr<juce::TextButton> buttonMid2;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> midAttach2;

    std::unique_ptr<juce::TextButton> buttonHigh1;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> highAttach1;

    std::unique_ptr<juce::TextButton> buttonHigh2;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> highAttach2;


    std::unique_ptr<juce::TextButton> menuButton;
    std::unique_ptr<PresetMenu> presetMenu;

    std::unique_ptr<juce::Colour> lavender;
    std::unique_ptr<juce::Colour> pink;
    std::unique_ptr<juce::Colour> yellow;
    std::unique_ptr<juce::Colour> babyBlue;
    std::unique_ptr<juce::Colour> blue;

    FlangerAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FlangerAudioProcessorEditor)
};
