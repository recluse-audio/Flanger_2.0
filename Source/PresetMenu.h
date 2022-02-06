/*
  ==============================================================================

    PresetMenu.h
    Created: 4 Feb 2022 11:12:14am
    Author:  ryand

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "MenuFeel.h"

//==============================================================================
/*
*/
class PresetMenu  : public juce::Component, Button::Listener
{
public:
    PresetMenu(FlangerAudioProcessor&);
    ~PresetMenu() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void prepareMenu();

    void runMenu();

    void buttonClicked(Button* b) override;

    void presetToProcessor();

    void createNewPreset(StringRef);

private:
    String presetName = { "" };
    StringArray presetPaths = { "", "" }; // nasty way of offsetting because menu starts referencing these at index 3

    std::unique_ptr<Label> presetNameInput;
    std::unique_ptr<TextButton> savePresetButton;

    FlangerAudioProcessor& audioProcessor;

    MenuFeel menuFeel;

    PopupMenu menu;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PresetMenu)
};
