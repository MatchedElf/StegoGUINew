#pragma once

#include <JuceHeader.h>
//#include "RGB.h"
#include "Stego.h"
//
using namespace juce;
//
class EditComponent : public Component, public Button::Listener
{
public:
   EditComponent(const wchar_t* _filename);
   ~EditComponent() override;
   void paint(juce::Graphics&) override;
   void resized() override;
   void buttonClicked(Button* butt) override;
   void mouseDown(const MouseEvent& event) override;
   void mouseDrag(const MouseEvent& event) override;
private:
   //
   class newPoint : public Point<int>
   {
   public:
      bool operator<(newPoint t1) const;
      newPoint(int x, int y);
   };
   class EditMenuComponent : public Component, public LookAndFeel_V4
   {
   public:
      EditMenuComponent();
      ~EditMenuComponent();
      void paint(juce::Graphics&) override;
      void resized() override;

      void drawTickBox(Graphics&, Component&,
         float x, float y, float w, float h,
         bool ticked, bool isEnabled,
         bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;
      Slider* thickSlider;
      ColourSelector* colourSelect;
      ImageButton* undoBut;
      ToggleButton* eraseBut;
   private:
      StretchableLayoutManager* FSizer;
      Component** compsList;
      //
      //EditComponent& parent;
   };
   //
   bool hided = true;
   int height, width;
   RGB** pixels;
   RGB** pixelsNew;
   FILE* newFile;
   //vector<paintData> paintPoints;
   map<newPoint, Colour> paintMap;
   vector<map<newPoint, Colour>> paintHistory;
   int thickness = 10;
   EditMenuComponent* menu;
   ImageButton* hideBut;
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