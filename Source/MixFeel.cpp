
/*
 ==============================================================================
 
 LookAndFeel.cpp
 Created: 12 Jul 2020 8:49:21am
 Author:  Billie (Govnah) Jean
 
 ==============================================================================
 */

#include "MixFeel.h"

MixFeel::MixFeel()
{
    setColour(juce::Slider::backgroundColourId, juce::Colours::grey);
    setColour(juce::Slider::trackColourId, juce::Colours::darkred);
    setColour(juce::TextButton::textColourOnId, juce::Colours::antiquewhite);
    setColour(juce::TextButton::textColourOffId, juce::Colours::antiquewhite);
    setColour(juce::TextButton::buttonColourId, juce::Colours::lavender);
    setColour(juce::TextButton::buttonOnColourId, juce::Colours::yellow);
    
}

MixFeel::~MixFeel()
{
    
}



/*=================================================================================*/
/*----------------------------- drawRotarySlider ----------------------------------*/
/*=================================================================================*/
void MixFeel::drawLinearSlider (juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, const juce::Slider::SliderStyle, juce::Slider& slider)
{
    
    if(slider.getSliderStyle() == juce::Slider::SliderStyle::LinearBarVertical)
    {
        
        juce::Rectangle<float> bgFrame(x, y, width, height);
        g.setColour(juce::Colours::whitesmoke);
        g.drawRoundedRectangle(bgFrame, 1.f, 2.f);

        g.setColour(juce::Colours::transparentBlack.withAlpha(0.f));
        g.fillAll();

       
        juce::Rectangle<float> centerRect(x, sliderPos, width, 2);
        g.setColour(juce::Colours::orange);
        g.fillRoundedRectangle(centerRect, 1.f);

    }

    if(slider.getSliderStyle() == juce::Slider::SliderStyle::LinearVertical)
    {
        juce::Rectangle<int> bgRect;
        bgRect.setX(x);
        bgRect.setY(y);
        bgRect.setSize(width, height);
        g.setColour(juce::Colours::whitesmoke);
        g.drawRoundedRectangle(bgRect.toFloat(), 1.f, 1.f);
        
        
        juce::Rectangle<int> centerTick(x, height/2, width, 2);
        
        
        auto pos = sliderPos;
        if (pos >= height - 3)
            pos = height - 3;
        
        juce::Rectangle<int> thumb(0, pos, width, 2);
        g.setColour(juce::Colours::yellow);
        g.fillRoundedRectangle(thumb.toFloat(), 5.f);
        
        
        g.setColour(juce::Colours::whitesmoke.darker());
        g.fillRoundedRectangle(centerTick.toFloat(), 3.f);
    }

    
    if (slider.getSliderStyle() == juce::Slider::SliderStyle::LinearHorizontal)
    {
        juce::Rectangle<int> bgRect;
        bgRect.setX(x);
        bgRect.setY(y);
        bgRect.setSize(width, height);
        g.setColour(juce::Colours::whitesmoke);
        g.drawRoundedRectangle(bgRect.toFloat(), 1.f, 1.f);

        juce::Rectangle<int> centerTick(width/2, y, 1, height);
        
        g.setColour(juce::Colours::transparentBlack.withAlpha(0.f));
        g.fillAll();
        
        auto pos = sliderPos;
        if (pos >= width - 3)
            pos = width - 3;
        
        juce::Rectangle<int> thumb(pos, 0, 2, 20);
        g.setColour(juce::Colours::yellow);
        g.fillRoundedRectangle(thumb.toFloat(), 2.f);
        
        g.setColour(juce::Colours::whitesmoke.darker());
        g.fillRoundedRectangle(centerTick.toFloat(), 2.f);
    }
    
    if(slider.getSliderStyle() == juce::Slider::ThreeValueVertical)
    {
        juce::Rectangle<float> bgFrame(x, y, width, height);
        g.setColour(juce::Colours::white);
        g.drawRoundedRectangle(bgFrame, 1.f, 2.f);
        
        juce::Rectangle<float> fillRect(x, maxSliderPos - 2, width, minSliderPos-maxSliderPos);
        juce::ColourGradient gradient(juce::Colours::mediumvioletred.darker(), width/2, height, juce::Colours::mediumvioletred.withHue(0.5f), width/2, 0, false);
        g.setGradientFill(gradient);
        g.fillRect(fillRect);
    }
    
    if(slider.getSliderStyle() == juce::Slider::ThreeValueHorizontal)
    {
        juce::Rectangle<float> bgFrame(x, y, width, height);
        g.setColour(juce::Colours::whitesmoke);
        g.drawRoundedRectangle(bgFrame, 1.f, 2.f);
        
        juce::Rectangle<float> fillRect(minSliderPos + 2, y, maxSliderPos-minSliderPos, height);
        juce::ColourGradient gradient(juce::Colours::mediumvioletred.darker(), 0, height/2, juce::Colours::mediumvioletred.withHue(0.5f), width, height/2, false);
        g.setGradientFill(gradient);
        g.fillRect(fillRect);
    }
    
    
}

void MixFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& s)
{
    auto radius = juce::jmin(width / 2, height / 2) - 2.0f;
    auto centreX = x + width * 0.5f;
    auto centreY = y + height * 0.5f;
    auto rx = centreX - radius;
    auto ry = centreY - radius;
    auto rw = radius * 2.0f;
    auto angle = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);
    auto isMouseOver = s.isMouseOverOrDragging() && s.isEnabled();
    auto pi = juce::MathConstants<float>::pi;

    g.setColour(juce::Colours::floralwhite.brighter());
    juce::Path bg;
    bg.addPieSegment(rx, ry, rw, rw, rotaryStartAngle, rotaryEndAngle, 0.8f);
    g.strokePath(bg, juce::PathStrokeType(1.f));

    g.setColour(s.findColour(juce::Slider::rotarySliderFillColourId));
    juce::Path arc;
    arc.addPieSegment(rx, ry, rw, rw, rotaryStartAngle, angle, 0.8f);
    g.fillPath(arc);
    
}

juce::Slider::SliderLayout MixFeel::getSliderLayout(juce::Slider& slider)
{
    juce::Slider::SliderLayout layout;
    auto bounds = slider.getLocalBounds();
    
    juce::Rectangle<int> textBox(0, bounds.getHeight()*0.7, bounds.getWidth(), 15);
    
    layout.textBoxBounds = textBox;
    layout.sliderBounds  = bounds;

    return layout;
}


juce::Label* MixFeel::createSliderTextBox(juce::Slider& slider)
{
    juce::Label* l = LookAndFeel_V4::createSliderTextBox(slider);
    // make sure editor text is black (so it shows on white background)
    l->setColour(juce::Label::outlineColourId, juce::Colours::white.withAlpha(0.0f));
    l->setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::hotpink.withAlpha(0.f));
    l->setColour(juce::Label::textColourId, juce::Colours::white);

    return l;
}

/*=================================== FONT =========================================*/

juce::Font MixFeel::getLabelFont(juce::Label& label)
{
   return getFont().withHeight(label.getHeight() * 0.9f); 
}

void MixFeel::drawButtonBackground(juce::Graphics& g, juce::Button& button, const juce::Colour& backgroundColour,
                                    bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
    juce::Rectangle<int> bounds = button.getLocalBounds();
    
    if(button.getToggleState())
    {
        g.setColour(button.findColour(juce::TextButton::buttonOnColourId).withAlpha(1.f));
        g.drawRoundedRectangle(bounds.toFloat(), 2.f, 2.f);
        
    } else
    {
        g.setColour(button.findColour(juce::TextButton::buttonOnColourId).withAlpha(0.3f));
        g.drawRoundedRectangle(bounds.toFloat(), 2.f, 2.f);
    }

    if (shouldDrawButtonAsDown)
    {
        g.setColour(button.findColour(TextButton::buttonOnColourId));
		g.drawRoundedRectangle(bounds.toFloat(), 2.f, 2.f);
    }
}

juce::Font MixFeel::getTextButtonFont(juce::TextButton& button, int buttonHeight)
{
    return juce::Font("Consolas", "Regular", 10.f);
}







