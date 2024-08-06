/*
  ==============================================================================

    LoadWindow.cpp
    Created: 28 May 2024 1:29:47pm
    Author:  fomin

  ==============================================================================
*/

#include "../Include/LoadWindow.h"

LoadWindow::LoadWindow(const String& title, const String& message, MessageBoxIconType iconType, Component*)
   : AlertWindow(title,  message, iconType)
{
   /*for (int i = 0; i < 2; i++)
   {
      String file = "Load/loading" + String(std::to_string(i)) + ".png";
      Image clockIm = ImageFileFormat::loadFrom(File::getCurrentWorkingDirectory().getChildFile(file));
      vect.push_back(clockIm);
   }*/
   setOpaque(false);
   setSize(600, 600);
   startTimer(100);
   setSize(500, 500);
   clock = new ImageComponent();
   //addAndMakeVisible(clock);
   clock->setImage(ImageFileFormat::loadFrom(File::getCurrentWorkingDirectory().getChildFile("clock0.png")));
   clock->setBounds(0, 0, 500, 500);
   //anim = new ComponentAnimator();
   juce::Rectangle<int> finalBounds(1000, 1000, 200, 200);
   //anim->animateComponent(clock, finalBounds, 1.0f, 5000, true, 1.0f, 1.0f);
   //Desktop::getInstance().getAnimator().animateComponent(clock, finalBounds, 1.0f, 15000, true, 1.0f, 1.0f);
}
//
LoadWindow::~LoadWindow()
{
   deleteAndZero(clock);
   //deleteAndZero(anim);
}
//
void LoadWindow::timerCallback()
{
   repaint();
}
//
void LoadWindow::paint(juce::Graphics& g)
{
   juce::Rectangle<float> rec((float)getWidth(), (float)getHeight());
   //String file = "loading0r.gif";
   String file = "Clock" + status + ".png";
   Image clockIm = ImageFileFormat::loadFrom(File::getCurrentWorkingDirectory().getChildFile(file));
   
   //Image clockIm = ImageFileFormat::loadFrom(File::getCurrentWorkingDirectory().getChildFile(file));*/
   //counter++;
   g.drawImage(clockIm, rec);
   status = (status == "0") ? "1" : "0";
   //
   //Rectangle<float> r = Rectangle<float>(10, 10, 100, 100);
   //rotate = (rotate + 0.5) % 1000;
   //rotate += 0.1;
   //AffineTransform t = AffineTransform::rotation(rotate);
   //g.drawImageTransformed(clockIm, t);
}
