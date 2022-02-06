/*
  ==============================================================================

    PresetMenu.cpp
    Created: 4 Feb 2022 11:12:14am
    Author:  ryand

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PresetMenu.h"

//==============================================================================
PresetMenu::PresetMenu(FlangerAudioProcessor& p) : audioProcessor(p)
{
    prepareMenu();

    setLookAndFeel(&menuFeel);

    presetNameInput = std::make_unique<Label>();
    presetNameInput->setText("Preset Name", NotificationType::dontSendNotification);

    addChildComponent(presetNameInput.get());
    presetNameInput->setEditable(true);
    presetNameInput->onTextChange = [this] { createNewPreset(presetNameInput->getText()); };

    savePresetButton = std::make_unique<TextButton>("Save Preset");
    savePresetButton->addListener(this);
    addChildComponent(savePresetButton.get());
}

PresetMenu::~PresetMenu()
{
}

void PresetMenu::paint (juce::Graphics& g)
{
}

void PresetMenu::resized()
{
    presetNameInput->setBoundsRelative(0.2f, 0.2f, 0.4f, 0.2f);
    savePresetButton->setBoundsRelative(0.2f, 0.4f, 0.4f, 0.2f);
}

void PresetMenu::prepareMenu()
{
    menu.clear();
    menu.setLookAndFeel(&menuFeel);
    menu.addSectionHeader("SAVE");

    int itemIndex = 1;

    auto saveMenu = new PopupMenu();
    saveMenu->addItem(itemIndex, "Save Preset");
    itemIndex++;

    auto presetMenu = new PopupMenu();
    auto presetFiles = File("C:/ProgramData/Recluse-Audio/Flanger/Presets/Stock Presets/").findChildFiles(File::findFiles, false);

    for (int i = 0; i < presetFiles.size(); i++)
    {
        presetMenu->addItem(itemIndex, presetFiles[i].getFileNameWithoutExtension());
        presetPaths.set(itemIndex, presetFiles[i].getFullPathName());
        itemIndex++;
    }

    auto userPresetMenu = new PopupMenu();
    auto userPresetFiles = juce::File("C:/ProgramData/Recluse-Audio/Flanger/Presets/User Presets/").findChildFiles(File::findFiles, false);
    for (int j = 0; j < userPresetFiles.size(); j++)
    {
        userPresetMenu->addItem(itemIndex, userPresetFiles[j].getFileNameWithoutExtension());
        presetPaths.set(itemIndex, userPresetFiles[j].getFullPathName());
        itemIndex++;
    }

    menu.addSubMenu("SAVE", *saveMenu);
    menu.addSeparator();
    menu.addSectionHeader("LOAD");
    menu.addSubMenu("Presets", *presetMenu);
    menu.addSubMenu("User Presets", *userPresetMenu);
}

void PresetMenu::runMenu()
{
    auto menuArea = Rectangle<int>(getScreenX(), getScreenY() - getParentHeight(), getParentWidth(), getParentHeight());
    int selection = menu.showMenu(PopupMenu::Options().withTargetScreenArea(menuArea));

    switch (selection)
    {
        case 0:
        {
            menu.dismissAllActiveMenus();
            presetNameInput->setVisible(false);
            savePresetButton->setVisible(false);
            setVisible(false);
        } break;
        case 1:
        {
            presetNameInput->setVisible(true);
            savePresetButton->setVisible(true);
        } break;
    }

    if (selection > 1)
    {
        audioProcessor.loadPreset(presetPaths[selection]);
		setVisible(false);
    }

    repaint();
}

void PresetMenu::buttonClicked(Button* b)
{
    if (b == savePresetButton.get())
    {
        presetNameInput->setText("Preset Name", NotificationType::dontSendNotification);
        presetToProcessor();
    }
}

void PresetMenu::presetToProcessor()
{
    audioProcessor.savePreset(presetName);

    presetNameInput->hideEditor(true);
    presetNameInput->setVisible(false);
	savePresetButton->setVisible(false);

    prepareMenu();

    setVisible(false);
}

void PresetMenu::createNewPreset(StringRef preName)
{
    presetName = preName;
    presetNameInput->setText("Preset Name: " + presetName, 
        NotificationType::dontSendNotification);
}

