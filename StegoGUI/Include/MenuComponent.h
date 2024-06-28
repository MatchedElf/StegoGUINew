#pragma once

#include <JuceHeader.h>
using namespace juce;
//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MenuComponent : public Component, public Button::Listener, public ComboBox::Listener, public ComponentListener, public LookAndFeel_V4
{
public:
   //==============================================================================
   MenuComponent(Component* _costyl);
   ~MenuComponent() override;

   //==============================================================================
   void paint(juce::Graphics&) override;
   void resized() override;
   void buttonClicked(Button* butt) override;
   void comboBoxChanged(ComboBox* cb) override;
   void LoadFile(bool image);
   void startDecode();
   void drawToggleButton(Graphics& g, ToggleButton& button,
                         bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;
   //
   int selectedTr = 1;
   bool isAttack = false;
   bool changed = false;
   String imageName = "-1";
   String secrName = "-1";
   File imageFile;
   File messageFile;
   TextButton* imageCh;
private:
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
   TextButton* secrCh;
   ComboBox* algCh;
   ToggleButton* attack;
   //
   Component* costyl;
   //
   JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MenuComponent)
};