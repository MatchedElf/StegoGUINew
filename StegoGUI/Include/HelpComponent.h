#pragma once

#include <JuceHeader.h>
//#include <iostream>
//
using namespace juce;
//
class HelpComponent : public Component
{
public:
   HelpComponent();
   ~HelpComponent() override;
   void paint(juce::Graphics&) override;
   void resized() override;
private:
   TextEditor* text;
   ImageComponent* emoji;
   StretchableLayoutManager* FSizer;
   Component** compsList;
};
//
class HelpWindow : public DocumentWindow
{
public:
   HelpWindow(String name)
      : DocumentWindow(name,
         Desktop::getInstance().getDefaultLookAndFeel()
         .findColour(ResizableWindow::backgroundColourId),
         DocumentWindow::minimiseButton | closeButton)
   {
      setUsingNativeTitleBar(false);
      setContentOwned(new HelpComponent(), true);
      setResizable(true, true);
      setFullScreen(false);
      setVisible(true);
      //setResizeLimits(100, 100, 500, 500);
      //setSize(400, 400);
      centreWithSize(getWidth(), getHeight());
   }
   ~HelpWindow() override;
   void closeButtonPressed() override;
   //void paint(juce::Graphics&) override;
   //void resized() override;
private:
   
};
