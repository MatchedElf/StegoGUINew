/*
  ==============================================================================

    LoadWindow.cpp
    Created: 28 May 2024 1:29:47pm
    Author:  fomin

  ==============================================================================
*/

#include "../Include/LoadWindow.h"

LoadWindow::LoadWindow(const String& title, const String& message, MessageBoxIconType iconType, Component* associatedComponent)
   : AlertWindow(title,  message, iconType)
{
   //setOpaque(true);
   setSize(360, 670);
   startTimer(100);
}

LoadWindow::~LoadWindow()
{
}

void LoadWindow::timerCallback()
{
   repaint();
}

void LoadWindow::paint(juce::Graphics& g)
{
   String file = "clock" + status + ".png";
   Image clock = ImageFileFormat::loadFrom(File::getCurrentWorkingDirectory().getChildFile(file));
   juce::Rectangle<float> rec(getWidth(), getHeight());
   g.drawImage(clock, rec);
   status = (status == "0") ? "1" : "0";
}
