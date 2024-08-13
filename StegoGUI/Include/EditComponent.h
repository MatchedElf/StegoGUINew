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
   struct paintData {
      Point<int> pointPos;
      int thick;
      Colour colour;
      paintData(Point<int> _pos, int _thick, Colour _colour) {
         pointPos = _pos;
         thick = _thick;
         colour = _colour;
      }
      bool operator==(paintData t1) {
         if ((pointPos == t1.pointPos) && (thick == t1.thick) && (colour == t1.colour)) return true;
         else return false;
      }

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
   vector<paintData> paintPoints;
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