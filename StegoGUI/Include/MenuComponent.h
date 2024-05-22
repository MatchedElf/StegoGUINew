#pragma once

#include <JuceHeader.h>

using namespace juce;
//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MenuComponent : public Component, public Button::Listener, public ComboBox::Listener
{
public:
   //==============================================================================
   MenuComponent();
   ~MenuComponent() override;

   //==============================================================================
   void paint(juce::Graphics&) override;
   void resized() override;
   void buttonClicked(Button* butt) override;
   void comboBoxChanged(ComboBox* cb) override;
   void LoadFile(bool image);
   void startDecode();
   //
   int selectedTr = 1;
   bool isAttack = false;
   String imageName = "orig.bmp";
   String secrName = "secr.txt";
private:
   //==============================================================================
   // Your private member variables go here...
   //StretchableLayoutManager* FSizer;
   //StretchableLayoutManager* imagesSizer;
   //Grid* grid1;
   /*Label* origTitle;
   Label* diffTitle;
   Label* newTitle;*/
   //
   std::unique_ptr<FileChooser> myChooser;
   //
   Component** compsList;
   StretchableLayoutManager* FSizer;
   //
   Label* menuTitle;
   Label* imageTitle;
   Label* secrTitle;
   Label* algTitle;
   Label* attackTitle;
   //
   TextButton* imageCh;
   TextButton* secrCh;
   ComboBox* algCh;
   //TextButton* startBut;
   ToggleButton* attack;
   //
   JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MenuComponent)
};