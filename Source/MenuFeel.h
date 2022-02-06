/*
  ==============================================================================

    MenuFeel.h
    Created: 5 Feb 2022 7:54:25am
    Author:  ryand

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class MenuFeel : public juce::LookAndFeel_V4
{
public:
    MenuFeel();
    ~MenuFeel();

    void drawLabel(Graphics& g, Label& l) override;

    void drawButtonBackground(juce::Graphics& g, juce::Button& button, const juce::Colour& backgroundColour,
        bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;

    void drawPopupMenuBackground(juce::Graphics& g, int width, int height) override;

    juce::Font getPopupMenuFont() override;
    juce::Font getLabelFont(juce::Label&) override;
    Font getTextButtonFont(TextButton&, int buttonHeight) override { return getFont().withHeight(buttonHeight * 0.7f); }

private:
    juce::Font getFont();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MenuFeel)

};