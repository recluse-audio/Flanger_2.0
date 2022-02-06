/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FlangerAudioProcessorEditor::FlangerAudioProcessorEditor(FlangerAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    startTimerHz(60);
    setLookAndFeel(&mixFeel);
    
    initColors();

    auto lfoParams1 = StringArray({ "LFO DEPTH 1", "LFO RATE 1", "FEEDBACK 1" });
    lfo1 = std::make_unique<LFOSliders>(audioProcessor, lfoParams1);
    addAndMakeVisible(*lfo1);

	auto lfoParams2 = StringArray({ "LFO DEPTH 2", "LFO RATE 2", "FEEDBACK 2" });
	lfo2 = std::make_unique<LFOSliders>(audioProcessor, lfoParams2);
	addAndMakeVisible(*lfo2);

    for (int i = 0; i < 2; i++)
    {
        tapeReels.add(new TapeReel(p, i));
        addAndMakeVisible(tapeReels[i]);
    }
    tapeReels[0]->setBounds(100, 150, 125, 125);
    tapeReels[1]->setBounds(100, 150, 125, 125);

    initButtons();
    
    mainLabel = std::make_unique<Label>();
    mainLabel->setText(" F L A N G E R ", NotificationType::dontSendNotification);
    mainLabel->setJustificationType(Justification::centred);
    addAndMakeVisible(*mainLabel);

    setResizable(true, true);
    setSize(500, 300);

}

FlangerAudioProcessorEditor::~FlangerAudioProcessorEditor()
{
}

//==============================================================================
void FlangerAudioProcessorEditor::paint (juce::Graphics& g)
{

    g.fillAll(juce::Colour (5, 0, 5));

    g.setColour(*babyBlue);
    auto frame = juce::Rectangle<float>(getWidth() * 0.15f, getHeight() * 0.2f, getWidth() * 0.7f, getHeight() * 0.6f);
    g.drawRoundedRectangle(frame, 3.f, 3.f);

    g.drawRect(0, 0, getWidth(), getHeight(), 2);

  //  g.setColour(babyBlue->withAlpha(0.8f));
    g.drawLine(getWidth() * 0.15f, getHeight() * 0.2f, 0, 0, 2.f);
    g.drawLine(frame.getBottomLeft().getX(), frame.getBottomLeft().getY(), 0, getHeight(), 2.f);
    g.drawLine(frame.getTopRight().getX(), frame.getTopRight().getY(), getWidth(), 0, 2.f);
    g.drawLine(frame.getBottomRight().getX(), frame.getBottomRight().getY(), getWidth(), getHeight(), 2.f);

    g.setColour(juce::Colour (15, 0, 15));
    g.fillRoundedRectangle(frame, 3.f);


    int numCombs1 = audioProcessor.getDelay(0) / 4.f;

    for (int comb = 1; comb < numCombs1; comb++)
    {
        g.setColour(yellow->withAlpha(0.7f));

        float spacing = pow((double) comb, 2);
        auto bounds = juce::Rectangle<float>(tapeReels[0]->getX(), tapeReels[0]->getY(), tapeRadius + spacing, tapeRadius + spacing);
        
		bounds.setCentre(center1);

        g.drawEllipse(bounds, 2.f);
    }


    int numCombs2 = audioProcessor.getDelay(1) / 4.f;

    for (int comb = 1; comb < numCombs2; comb++)
    {
        g.setColour(babyBlue->withAlpha(0.7f));

        float spacing = pow((double)comb, 2);
        auto bounds = juce::Rectangle<float>(tapeReels[1]->getX(), tapeReels[1]->getY(), tapeRadius + spacing, tapeRadius + spacing);

        bounds.setCentre(center2);
        g.drawEllipse(bounds, 2.f);
    }

}

void FlangerAudioProcessorEditor::resized()
{
    mainLabel->setBoundsRelative(0.35f, 0.02f, 0.3f, 0.1f);

    presetMenu->setBoundsRelative(0.1f, 0.2f, 0.8f, 0.8f);
    menuButton->setBoundsRelative(0.4f, 0.85f, 0.2f, 0.1f);

    lfo1->setBoundsRelative(0.02f, 0.3f, 0.1f, 0.5f);
    lfo2->setBoundsRelative(0.88f, 0.3f, 0.1f, 0.5f);

    buttonHigh1->setBoundsRelative(0.02f, 0.125f, 0.05f, 0.04f);
	buttonMid1->setBoundsRelative(0.02f, 0.175f, 0.075f, 0.04f);
    buttonLow1->setBoundsRelative(0.02f, 0.225f, 0.11f, 0.04f);

	buttonHigh2->setBoundsRelative(0.93f, 0.125f, 0.05f, 0.04f);
	buttonMid2->setBoundsRelative(0.905f, 0.175f, 0.075f, 0.04f);
	buttonLow2->setBoundsRelative(0.87f, 0.225f, 0.11f, 0.04f);

    tapeRadius = (getWidth() > getHeight()) ? getWidth() * 0.25f : getHeight() * 0.25f;
	tapeReels[0]->setBounds(0, 0, tapeRadius, tapeRadius);
	tapeReels[1]->setBounds(0, 0, tapeRadius, tapeRadius);

    center1.setXY(getWidth() * 0.33f, getHeight() * 0.5f);
	center2.setXY(getWidth() * 0.67f, getHeight() * 0.5f);

	tapeReels[0]->setCentreRelative(0.33f, 0.5f);
	tapeReels[1]->setCentreRelative(0.67f, 0.5f);


}

void FlangerAudioProcessorEditor::timerCallback()
{
    for (int i = 0; i < tapeReels.size(); i++)
        tapeReels[i]->incrementPhase();

    tapeReels[0]->setCentreRelative(0.33f, 0.5f);
    tapeReels[1]->setCentreRelative(0.67f, 0.5f);
    repaint();
}

void FlangerAudioProcessorEditor::sliderValueChanged(juce::Slider* s)
{

}

void FlangerAudioProcessorEditor::buttonClicked(juce::Button* b)
{
    if (b == buttonHigh1.get())
        audioProcessor.setLFOMode(0, 0);
    if (b == buttonHigh2.get())
        audioProcessor.setLFOMode(1, 0);
    if (b == buttonMid1.get())
        audioProcessor.setLFOMode(0, 1);
    if (b == buttonMid2.get())
        audioProcessor.setLFOMode(1, 1);
    if (b == buttonLow1.get())
        audioProcessor.setLFOMode(0, 2);
    if (b == buttonLow2.get())
        audioProcessor.setLFOMode(1, 2);

    if (b == menuButton.get())
    {
        presetMenu->setVisible(true);
        presetMenu->runMenu();
    }
}

void FlangerAudioProcessorEditor::initColors()
{
    lavender = std::make_unique<juce::Colour>(119, 0, 166);
    pink = std::make_unique<juce::Colour>(254, 0, 254);
    yellow = std::make_unique<juce::Colour>(222, 254, 71);
    babyBlue = std::make_unique<juce::Colour>(115, 255, 254);
}

void FlangerAudioProcessorEditor::initButtons()
{
    menuButton = std::make_unique<TextButton>("Save / Load");
    menuButton->addListener(this);
    addAndMakeVisible(menuButton.get());

    presetMenu = std::make_unique<PresetMenu>(audioProcessor);
    addChildComponent(presetMenu.get());

    buttonLow1 = std::make_unique<juce::TextButton>("Ultra-Low");
    buttonLow1->setBounds(10, 67, 35, 12);
    buttonLow1->setClickingTogglesState(true);
    buttonLow1->addListener(this);
    buttonLow1->setRadioGroupId(1);
   // buttonLow1->setToggleState(true, NotificationType::dontSendNotification);
    addAndMakeVisible(buttonLow1.get());

    lowAttach1 = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>
        (audioProcessor.apvts, "LOW MODE 1", *buttonLow1);

    buttonLow2 = std::make_unique<juce::TextButton>("Ultra-Low");
    buttonLow2->setBounds(455, 67, 35, 12);
    buttonLow2->setClickingTogglesState(true);
    buttonLow2->addListener(this);
    buttonLow2->setRadioGroupId(2);
   // buttonLow2->setToggleState(true, NotificationType::dontSendNotification);
    addAndMakeVisible(buttonLow2.get());

    lowAttach2 = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>
        (audioProcessor.apvts, "LOW MODE 2", *buttonLow2);

    buttonMid1 = std::make_unique<juce::TextButton>("LFO");
    buttonMid1->setBounds(10, 52, 30, 12);
    buttonMid1->setClickingTogglesState(true);
    buttonMid1->addListener(this);
    buttonMid1->setRadioGroupId(1);
    addAndMakeVisible(buttonMid1.get());

    midAttach1 = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>
        (audioProcessor.apvts, "MID MODE 1", *buttonMid1);

    buttonMid2 = std::make_unique<juce::TextButton>("LFO");
    buttonMid2->setBounds(460, 52, 30, 12);
    buttonMid2->setClickingTogglesState(true);
    buttonMid2->addListener(this);
    buttonMid2->setRadioGroupId(2);
    addAndMakeVisible(buttonMid2.get());

    midAttach2 = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>
        (audioProcessor.apvts, "MID MODE 2", *buttonMid2);

    buttonHigh1 = std::make_unique<juce::TextButton>("FM");
    buttonHigh1->setBounds(10, 37, 25, 12);
    buttonHigh1->setClickingTogglesState(true);
    buttonHigh1->addListener(this);
    buttonHigh1->setRadioGroupId(1);
    addAndMakeVisible(buttonHigh1.get());

    highAttach1 = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>
        (audioProcessor.apvts, "HIGH MODE 1", *buttonHigh1);

    buttonHigh2 = std::make_unique<juce::TextButton>("FM");
    buttonHigh2->setBounds(465, 37, 25, 12);
    buttonHigh2->setClickingTogglesState(true);
    buttonHigh2->addListener(this);
    buttonHigh2->setRadioGroupId(2);
    addAndMakeVisible(buttonHigh2.get());

    highAttach2 = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>
        (audioProcessor.apvts, "HIGH MODE 2", *buttonHigh2);
}




