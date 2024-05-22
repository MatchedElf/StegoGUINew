#include "../Include/MenuComponent.h"

MenuComponent::MenuComponent()
{
   setOpaque(true);
   Font font;
   font.setHeight(25);
   //
   compsList = new Component * [10];
   //
   menuTitle = new Label();
   menuTitle->setFont(font);
   menuTitle->setText(String((std::wstring(L"Настройки запуска")).c_str()), dontSendNotification);
   addAndMakeVisible(menuTitle);
   //
   imageTitle = new Label();
   imageTitle->setFont(font);
   imageTitle->setText("Image: " + imageName, dontSendNotification);
   addAndMakeVisible(imageTitle);
   //
   secrTitle = new Label();
   secrTitle->setFont(font);
   secrTitle->setText("Text: " + secrName, dontSendNotification);
   addAndMakeVisible(secrTitle);
   //
   algTitle = new Label();
   algTitle->setFont(font);
   algTitle->setText(String((std::wstring(L"Выбор алгоритма")).c_str()), dontSendNotification);
   addAndMakeVisible(algTitle);
   //
   attackTitle = new Label();
   attackTitle->setFont(font);
   attackTitle->setText(String((std::wstring(L"Проводится ли атака на заполненный контейнер?")).c_str()), dontSendNotification);
   addAndMakeVisible(attackTitle);
   //
   imageCh = new TextButton(String((std::wstring(L"Выбрать изображение")).c_str()));
   addAndMakeVisible(imageCh);
   imageCh->addListener(this);
   //
   secrCh = new TextButton(String((std::wstring(L"Выбрать скрываемое сообщение")).c_str()));
   addAndMakeVisible(secrCh);
   secrCh->addListener(this);
   //
   algCh = new ComboBox();
   algCh->addItem("DCP", 1);
   algCh->addItem("Furie", 2);
   algCh->addItem("LSB", 3);
   algCh->setSelectedId(1);
   addAndMakeVisible(algCh);
   algCh->addListener(this);
   //
   /*startBut = new TextButton("Start decode");
   addAndMakeVisible(startBut);
   startBut->addListener(this);*/
   //
   attack = new ToggleButton("ddd");
   attack->setButtonText("Attack");
   attack->addListener(this);
   addAndMakeVisible(attack);
   //
   compsList[0] = menuTitle;
   //compsList[1] = startBut;
   compsList[1] = imageTitle;
   compsList[2] = imageCh;
   compsList[3] = secrTitle;
   compsList[4] = secrCh;
   compsList[5] = algTitle;
   compsList[6] = algCh;
   compsList[7] = attackTitle;
   compsList[8] = attack;
   //
   FSizer = new StretchableLayoutManager();
   FSizer->setItemLayout(0, 1, 100000, -1);
   FSizer->setItemLayout(1, 1, 100000, -1);
   FSizer->setItemLayout(2, 1, 100000, -1);
   FSizer->setItemLayout(3, 1, 100000, -1);
   FSizer->setItemLayout(4, 1, 100000, -1);
   FSizer->setItemLayout(5, 1, 100000, -1);
   FSizer->setItemLayout(6, 1, 100000, -1);
   FSizer->setItemLayout(7, 1, 100000, -1);
   FSizer->setItemLayout(8, 1, 100000, -1);
   //FSizer->setItemLayout(9, 1, 100000, -1);

}

MenuComponent::~MenuComponent()
{
   deleteAndZero(menuTitle);
   deleteAndZero(imageTitle);
   deleteAndZero(secrTitle);
   deleteAndZero(algTitle);
   deleteAndZero(attackTitle);
   //
   deleteAndZero(imageCh);
   deleteAndZero(secrCh);
   deleteAndZero(algCh);
   //deleteAndZero(startBut);
   deleteAndZero(attack);
   deleteAndZero(FSizer);
   myChooser.reset();
   delete[] compsList;
}

void MenuComponent::paint(juce::Graphics& g)
{
   //g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
   g.fillAll(Colour::fromRGB(50, 50, 50));
   g.setFont(juce::Font(16.0f));
   g.setColour(juce::Colours::white);
}

void MenuComponent::resized()
{
   FSizer->layOutComponents(compsList, 9, 0, 0, getWidth(), getHeight(), true, true);
}

void MenuComponent::buttonClicked(Button* butt)
{
   if (butt == imageCh)
   {
      LoadFile(true);
   }
   else if (butt == secrCh)
   {
      LoadFile(false);
   }
   else if (butt == attack)
   {
      isAttack = butt->getToggleState();
   }
}

void MenuComponent::comboBoxChanged(ComboBox* cb)
{
   if (cb == algCh)
   {
      selectedTr = cb->getSelectedId();
   }
}

void MenuComponent::LoadFile(bool image)
{
   if(image)
      myChooser = std::make_unique<FileChooser>("Please select the file you want to load...",
         File::getCurrentWorkingDirectory(),
         "*.bmp");
   else
      myChooser = std::make_unique<FileChooser>("Please select the file you want to load...",
         File::getCurrentWorkingDirectory(),
         "*.txt");
   

   auto folderChooserFlags = FileBrowserComponent::openMode | FileBrowserComponent::canSelectFiles;
   String ret;
   if (image)
   {
      myChooser->launchAsync(folderChooserFlags, [this](const FileChooser& chooser)
         {
            File choosedFile(chooser.getResult());
            if (choosedFile.getFullPathName().isEmpty()) {
               imageName = "-1";
               imageTitle->setText("Image: not choosed", dontSendNotification);
            }
            else
            {
               auto url = URL(choosedFile);
               imageName = choosedFile.getFullPathName();
               imageTitle->setText("Image: " + choosedFile.getFileName(), dontSendNotification);
            }
         });
   }
   else
   {
      myChooser->launchAsync(folderChooserFlags, [this](const FileChooser& chooser)
         {
            File choosedFile(chooser.getResult());
            if (choosedFile.getFullPathName().isEmpty()) {
               secrName = "-1";
               secrTitle->setText("Text: not choosed", dontSendNotification);
            }
            else
            {
               auto url = URL(choosedFile);
               secrName = choosedFile.getFullPathName();
               secrTitle->setText("Text: " + choosedFile.getFileName(), dontSendNotification);
            }
         });
   }
   repaint();
}

void MenuComponent::startDecode()
{
}
