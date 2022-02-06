/*
  ==============================================================================

    LFOSliders.cpp
    Created: 5 Feb 2022 11:15:36am
    Author:  ryand

  ==============================================================================
*/

#include <JuceHeader.h>
#include "LFOSliders.h"

//==============================================================================
LFOSliders::LFOSliders(FlangerAudioProcessor& p, StringArray params) : audioProcessor(p)
{
	depth = std::make_unique<Slider>(Slider::SliderStyle::RotaryHorizontalVerticalDrag, Slider::NoTextBox);
	depth->setColour(Slider::rotarySliderFillColourId, Colour(115, 255, 254));
	addAndMakeVisible(depth.get());

	depthLabel = std::make_unique<Label>("Depth");
	depthLabel->setText("Depth", NotificationType::dontSendNotification);
	depthLabel->setJustificationType(Justification::centred);
	addAndMakeVisible(depthLabel.get());


	rate = std::make_unique<Slider>(Slider::SliderStyle::RotaryHorizontalVerticalDrag, Slider::NoTextBox);
	rate->setColour(Slider::rotarySliderFillColourId, Colour(222, 254, 71));
	addAndMakeVisible(rate.get());

	rateLabel = std::make_unique<Label>("Rate");
	rateLabel->setText("Rate", NotificationType::dontSendNotification);
	rateLabel->setJustificationType(Justification::centred);
	addAndMakeVisible(rateLabel.get());


	feedback = std::make_unique<Slider>(Slider::SliderStyle::RotaryHorizontalVerticalDrag, Slider::NoTextBox);
	feedback->setColour(Slider::rotarySliderFillColourId, Colour(254, 0, 254));
	addAndMakeVisible(feedback.get());

	fbLabel = std::make_unique<Label>("Feedback");
	fbLabel->setText("Feedback", NotificationType::dontSendNotification);
	fbLabel->setJustificationType(Justification::centred);
	addAndMakeVisible(fbLabel.get());


	depthAttach = std::make_unique<Attach>(audioProcessor.apvts, params[0], *depth);
	rateAttach = std::make_unique<Attach>(audioProcessor.apvts, params[1], *rate);
	fbAttach = std::make_unique<Attach>(audioProcessor.apvts, params[2], *feedback);
}

LFOSliders::~LFOSliders()
{
}

void LFOSliders::paint (Graphics& g)
{

}

void LFOSliders::resized()
{
	depth->setBoundsRelative		(0.f, 0.f,  1.f,  0.25f);
	depthLabel->setBoundsRelative	(0.f, 0.25f, 1.f, 0.07f);

	rate->setBoundsRelative			(0.f, 0.35f, 1.f, 0.25f);
	rateLabel->setBoundsRelative	(0.f, 0.6f, 1.f, 0.07f);

	feedback->setBoundsRelative		(0.f, 0.7f, 1.f,  0.25f);
	fbLabel->setBoundsRelative		(0.f, 0.95f, 1.f, 0.07f);
}

void LFOSliders::rotateSliders()
{

}
