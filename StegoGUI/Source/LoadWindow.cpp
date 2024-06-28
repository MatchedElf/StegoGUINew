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
   //setSize(600, 600);
   startTimer(100);
   /*clock = new ImageComponent();
   addAndMakeVisible(clock);
   clock->setImage(GIFImageFormat::loadFrom(File::getCurrentWorkingDirectory().getChildFile("loading.gif")));
   clock->setBounds((int)(getWidth() * 0.35), (int)(getHeight() * 0.25), (int)(getWidth() * 0.3), (int)(getHeight() * 0.3));*/
}
//
LoadWindow::~LoadWindow()
{
   //deleteAndZero(clock);
}
//
void LoadWindow::timerCallback()
{
   repaint();
}
//
void LoadWindow::paint(juce::Graphics& g)
{
   //Path p;
   //juce::Rectangle<float> rec((float)getWidth(), (float)getHeight());
   String file = "loading0r.gif";
   Image clockIm = ImageFileFormat::loadFrom(File::getCurrentWorkingDirectory().getChildFile(file));
   
   ///*String file = "Load/loading" + String(std::to_string(counter % 2)) + ".png";
   //Image clockIm = ImageFileFormat::loadFrom(File::getCurrentWorkingDirectory().getChildFile(file));*/
   //counter++;
   //status = (status == "0") ? "1" : "0";
   //
   //Rectangle<float> r = Rectangle<float>(10, 10, 100, 100);
   //rotate = (rotate + 0.5) % 1000;
   rotate += 0.1;
   AffineTransform t = AffineTransform::rotation(rotate);

   //Path p;
   //p.addRectangle(r);
   //g.strokePath(p, PathStrokeType(1), t);
   //g.addTransform(t);
   //g.drawImage(clockIm, rec, true);
   g.drawImageTransformed(clockIm, t);
}
