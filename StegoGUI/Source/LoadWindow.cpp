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
   /*for (int i = 0; i < 2; i++)
   {
      String file = "Load/loading" + String(std::to_string(i)) + ".png";
      Image clockIm = ImageFileFormat::loadFrom(File::getCurrentWorkingDirectory().getChildFile(file));
      vect.push_back(clockIm);
   }*/
   setOpaque(false);
   setSize(360, 670);
   startTimer(100);
   /*clock = new ImageComponent();
   addAndMakeVisible(clock);
   clock->setImage(GIFImageFormat::loadFrom(File::getCurrentWorkingDirectory().getChildFile("loading.gif")));
   clock->setBounds((int)(getWidth() * 0.35), (int)(getHeight() * 0.25), (int)(getWidth() * 0.3), (int)(getHeight() * 0.3));*/
}

LoadWindow::~LoadWindow()
{
   //deleteAndZero(clock);
}

void LoadWindow::timerCallback()
{
   repaint();
}
//
void LoadWindow::paint(juce::Graphics& g)
{
   juce::Rectangle<float> rec((float)getWidth(), (float)getHeight());
   String file = "clock" + status + ".png";
   Image clockIm = ImageFileFormat::loadFrom(File::getCurrentWorkingDirectory().getChildFile(file));
   /*String file = "Load/loading" + String(std::to_string(counter % 2)) + ".png";
   Image clockIm = ImageFileFormat::loadFrom(File::getCurrentWorkingDirectory().getChildFile(file));*/
   g.drawImage(clockIm, rec, true);
   counter++;
   status = (status == "0") ? "1" : "0";
}
