/*
  ==============================================================================

    MenuFeel.cpp
    Created: 5 Feb 2022 7:54:25am
    Author:  ryand

  ==============================================================================
*/

#include "MenuFeel.h"

MenuFeel::MenuFeel()
{
	setColour(TextButton::textColourOnId, Colours::antiquewhite);
	setColour(TextButton::textColourOffId, Colours::antiquewhite);
	setColour(TextButton::buttonColourId, Colours::black);
	setColour(TextButton::buttonOnColourId, Colours::yellow.darker());

	setColour(PopupMenu::backgroundColourId, Colours::black);

    setColour(Label::backgroundColourId, Colours::black);
    setColour(Label::backgroundWhenEditingColourId, Colours::black);
}

MenuFeel::~MenuFeel()
{

}

void MenuFeel::drawLabel(Graphics& g, Label& label)
{
	g.setColour(label.findColour(Label::backgroundColourId));
	g.fillAll();

	g.setColour(Colours::white);
	g.drawRoundedRectangle(label.getLocalBounds().toFloat(), 2.f, 2.f);

	g.setFont(getFont()); // not calling label.setFont(), but rather on the graphics context directly
	g.setColour(label.findColour(Label::textColourId));
	g.drawFittedText(label.getText(), 0, 0, label.getWidth(), label.getHeight(), Justification::centred, 2);
}

void MenuFeel::drawButtonBackground(Graphics& g, Button& button, const Colour& backgroundColour, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
	Rectangle<int> bounds = button.getLocalBounds();

	g.setColour(findColour(TextButton::textColourOnId));
	g.drawRoundedRectangle(bounds.toFloat(), 3.f, 2.f);

	g.setColour(button.findColour(TextButton::buttonColourId));
	g.fillAll();

	if (button.isDown())
	{
		g.setColour(button.findColour(TextButton::buttonOnColourId));
		g.fillRoundedRectangle(bounds.toFloat(), 5.f);
	}
	if (shouldDrawButtonAsHighlighted)
	{
		g.setColour(button.findColour(TextButton::buttonOnColourId));
		g.fillRoundedRectangle(bounds.toFloat(), 5.f);
	}
	if (button.getToggleState())
	{
		g.setColour(button.findColour(TextButton::buttonOnColourId));
		g.fillRoundedRectangle(bounds.toFloat(), 5.f);
	}
}

void MenuFeel::drawPopupMenuBackground(Graphics& g, int width, int height)
{
	g.setColour(Colours::black);
	g.fillAll();

	g.setColour(Colours::white);
	g.drawRoundedRectangle(0, 0, width, height, 5.f, 2.f);
}

Font MenuFeel::getPopupMenuFont()
{
	return getFont().withHeight(16.f);
}

Font MenuFeel::getLabelFont(Label& label)
{
	return getFont().withHeight(label.getHeight() * 0.3f);
}

Font MenuFeel::getFont()
{
	return Font("Consolas", "Regular", 15.f);
}
