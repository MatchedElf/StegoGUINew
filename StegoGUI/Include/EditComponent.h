#pragma once

#include <JuceHeader.h>
//#include "RGB.h"
#include "Stego.h"
//
using namespace juce;
//
class EditComponent : public Component
{
public:
   EditComponent(const wchar_t* _filename);
   ~EditComponent() override;
   void paint(juce::Graphics&) override;
   void resized() override;
   void mouseDown(const MouseEvent& event) override;
private:
   ImageComponent* image;
   int height, width;
   RGB** pixels;
   FILE* newFile;
   Point<int> coord;
};
//
class EditWindow : public DocumentWindow
{
public:
   EditWindow(String name, const wchar_t* _filename)
      : DocumentWindow(name,
         Desktop::getInstance().getDefaultLookAndFeel()
         .findColour(ResizableWindow::backgroundColourId),
         DocumentWindow::minimiseButton | closeButton)
   {
      setUsingNativeTitleBar(false);
      setContentOwned(new EditComponent(_filename), true);
      setResizable(false, false);
      setFullScreen(false);
      setVisible(true);
      //setResizeLimits(100, 100, 500, 500);
      //setSize(400, 400);
      centreWithSize(getWidth(), getHeight());
   }
   ~EditWindow() override;
   void closeButtonPressed() override;
   //void paint(juce::Graphics&) override;
   //void resized() override;
private:

};