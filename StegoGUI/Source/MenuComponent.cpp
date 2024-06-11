#include "../Include/MenuComponent.h"

MenuComponent::MenuComponent(Component* _costyl)
{
   setOpaque(true);
   Font font;
   font.setHeight(25);
   //
   compsList = new Component * [10];
   //
   addComponentListener(this);
   //
   menuTitle = new Label();
   font.setUnderline(true);
   menuTitle->setFont(font);
   menuTitle->setText(String((std::wstring(L"Настройки запуска")).c_str()), dontSendNotification);
   addAndMakeVisible(menuTitle);
   //
   font.setUnderline(false);
   imageTitle = new Label();
   imageTitle->setFont(font);
   imageTitle->setText(String((std::wstring(L"Изображение:\nне выбрано")).c_str()), dontSendNotification);
   imageTitle->setColour(imageTitle->textColourId, Colours::red);
   addAndMakeVisible(imageTitle);
   //
   secrTitle = new Label();
   secrTitle->setFont(font);
   secrTitle->setText(String((std::wstring(L"Сообщение:\nне выбрано")).c_str()), dontSendNotification);
   secrTitle->setColour(secrTitle->textColourId, Colours::red);
   addAndMakeVisible(secrTitle);
   //
   algTitle = new Label();
   algTitle->setFont(font);
   algTitle->setText(String((std::wstring(L"Выбор алгоритма")).c_str()), dontSendNotification);
   addAndMakeVisible(algTitle);
   //
   attackTitle = new Label();
   attackTitle->setFont(font);
   attackTitle->setText(String((std::wstring(L"Проводится ли атака\nна контейнер?")).c_str()), dontSendNotification);
   addAndMakeVisible(attackTitle);
   //
   imageCh = new TextButton(String((std::wstring(L"Выбрать изображение")).c_str()));
   addAndMakeVisible(imageCh);
   imageCh->addListener(this);
   //
   secrCh = new TextButton(String((std::wstring(L"Выбрать\nскрываемое сообщение")).c_str()));
   addAndMakeVisible(secrCh);
   secrCh->addListener(this);
   //
   algCh = new ComboBox();
   algCh->addItem("DCT", 1);
   algCh->addItem("DFT", 2);
   algCh->addItem("LSB", 3);
   algCh->setSelectedId(1);
   addAndMakeVisible(algCh);
   algCh->addListener(this);
   //
   attack = new ToggleButton("ddd");
   attack->setButtonText(String((std::wstring(L"Только извлечение")).c_str()));
   attack->addListener(this);
   
   addAndMakeVisible(attack);
   //
   compsList[0] = menuTitle;
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
   FSizer->setItemLayout(2, 1, 100000, 600);
   FSizer->setItemLayout(3, 1, 100000, -1);
   FSizer->setItemLayout(4, 1, 100000, 600);
   FSizer->setItemLayout(5, 1, 100000, -1);
   FSizer->setItemLayout(6, 1, 100000, 400);
   FSizer->setItemLayout(7, 1, 100000, -1);
   FSizer->setItemLayout(8, 1, 100000, -1);
   costyl = _costyl;
}

MenuComponent::~MenuComponent()
{
   deleteAndZero(menuTitle);
   deleteAndZero(imageTitle);
   deleteAndZero(secrTitle);
   deleteAndZero(algTitle);
   deleteAndZero(attackTitle);
   deleteAndZero(imageCh);
   deleteAndZero(secrCh);
   deleteAndZero(algCh);
   deleteAndZero(attack);
   deleteAndZero(FSizer);
   myChooser.reset();
   delete[] compsList;
}

void MenuComponent::paint(juce::Graphics& g)
{
   g.fillAll(Colour::fromRGB(50, 50, 50));
   g.setFont(juce::Font(16.0f));
   g.setColour(juce::Colours::white);
}

void MenuComponent::resized()
{
   FSizer->layOutComponents(compsList, 9, 0, getHeight() * 0.05, getWidth(), getHeight() * 0.95, true, true);
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
      setName("HIIIIIIIII");
   }
}

void MenuComponent::comboBoxChanged(ComboBox* cb)
{
   if (cb == algCh)
   {
      selectedTr = cb->getSelectedId();
   }
}

void MenuComponent::componentNameChanged(Component& component)
{
   int t = 1;
}

void MenuComponent::LoadFile(bool image)
{
   if(image)
      myChooser = std::make_unique<FileChooser>(String((std::wstring(L"Выберите нужный файл")).c_str()),
         File(File::getCurrentWorkingDirectory().getFullPathName() + "/../../Images"),
         "*.bmp");
   else
      myChooser = std::make_unique<FileChooser>(String((std::wstring(L"Выберите нужный файл")).c_str()),
         File(File::getCurrentWorkingDirectory().getFullPathName() +"/../../Messages"),
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
               imageTitle->setText(String((std::wstring(L"Изображение:\nне выбрано")).c_str()), dontSendNotification);
               imageTitle->setColour(imageTitle->textColourId, Colours::red);
            }
            else
            {
               auto url = URL(choosedFile);
               imageName = String((std::wstring(choosedFile.getFullPathName().toWideCharPointer())).c_str());
               imageTitle->setText(String((std::wstring(L"Изображение:\n")).c_str()) + choosedFile.getFileName(), dontSendNotification);
               imageTitle->setColour(imageTitle->textColourId, Colours::green);
            }
            costyl->setName(String(rand()));
         });
   }
   else
   {
      myChooser->launchAsync(folderChooserFlags, [this](const FileChooser& chooser)
         {
            File choosedFile(chooser.getResult());
            if (choosedFile.getFullPathName().isEmpty()) {
               secrName = "-1";
               secrTitle->setText(String((std::wstring(L"Сообщение:\nне выбрано")).c_str()), dontSendNotification);
               secrTitle->setColour(secrTitle->textColourId, Colours::red);
            }
            else
            {
               auto url = URL(choosedFile);
               secrName = String((std::wstring(choosedFile.getFullPathName().toWideCharPointer())).c_str());
               secrTitle->setText(String((std::wstring(L"Сообщение:\n")).c_str()) + choosedFile.getFileName(), dontSendNotification);
               secrTitle->setColour(secrTitle->textColourId, Colours::green);
            }
         });
   }
   repaint();
   changed = true;
}

void MenuComponent::startDecode()
{
}
