/*
  ==============================================================================

    TapeReel.cpp
    Created: 22 Jul 2021 2:34:23pm
    Author:  ryand

  ==============================================================================
*/

#include <JuceHeader.h>
#include "TapeReel.h"

//==============================================================================
TapeReel::TapeReel(FlangerAudioProcessor& p, int index) : audioProcessor(p), reelIndex(index)
{
    image = juce::ImageFileFormat::loadFrom(juce::File("C:/ProgramData/Recluse-Audio/Flanger/Images/FlangerTape.png"));
    phaseIncrement = juce::MathConstants<float>::twoPi / 180.f;
}

TapeReel::~TapeReel()
{
}

void TapeReel::paint (juce::Graphics& g)
{
    g.drawImageWithin(image, 0, 0, getWidth()-2, getHeight()-2, juce::RectanglePlacement::centred);
    g.setColour(juce::Colours::transparentBlack.withAlpha(0.f));
    g.fillAll();
}

void TapeReel::resized()
{
    
}

void TapeReel::mouseDown(const juce::MouseEvent& event)
{
    isFlanging = true;
}

void TapeReel::mouseUp(const juce::MouseEvent& event)
{
    audioProcessor.flangeRelease(reelIndex);
}

void TapeReel::mouseDrag(const juce::MouseEvent& event)
{
    normalizedPressure = ((float) event.getMouseDownScreenY() - (float) event.getScreenY()) / (float) event.getMouseDownScreenY(); // 1.0 at very top of screen, 0.0 at bottom
    normalizedPressure = juce::jlimit(0.0f, 1.0f, normalizedPressure);

    audioProcessor.flangePress(normalizedPressure, reelIndex);
}

float TapeReel::getPhaseDelay()
{
    auto scaledPi = juce::jmap(normalizedPressure, 0.f, juce::MathConstants<float>::pi);
    return scaledPi;
}

void TapeReel::incrementPhase()
{
    phase += phaseIncrement;

    if (phase >= juce::MathConstants<float>::twoPi)
        phase = phase - juce::MathConstants<float>::twoPi;

    auto x = (float)getWidth();// / 2.f;
    auto y = (float)getHeight();// / 2.f;

    auto scaledDelay = juce::jmap(audioProcessor.getDelay(reelIndex) / 180.f, 0.f, juce::MathConstants<float>::pi);
    this->setTransform(juce::AffineTransform::rotation(phase - scaledDelay));
    //repaint();
}
