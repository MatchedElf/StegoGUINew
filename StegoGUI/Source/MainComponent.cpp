#include "../Include/MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
    : Component("MainComponent")
{
    //
    ///*system("echo test > pwdTest.txt");
    /*system("PowerShell > output.txt");
    system("g++ --version > output.txt");*/
    ////system("g++ main.cpp -o main.exe > output.txt");
    //system("pause");
    //std::cout << "Test" << std::endl;
    setLookAndFeel(this);
    //setlocale(LC_ALL, "Russian");
    initGui();
}

MainComponent::~MainComponent()
{
    setLookAndFeel(nullptr);
    deleteAndZero(_openLogo);
    deleteAndZero(_openTitle);
    deleteAndZero(_origTitle);
    deleteAndZero(_diffTitle);
    deleteAndZero(_newTitle);
    deleteAndZero(_orig);
    deleteAndZero(_diff);
    deleteAndZero(_newIm);
    deleteAndZero(_startBut);
    deleteAndZero(_infoBut);
    deleteAndZero(_editBut);
    deleteAndZero(_mainBut);
    deleteAndZero(_hideBut);
    deleteAndZero(_homeBut);
    deleteAndZero(_origLabel);
    deleteAndZero(_decodeLabel);
    deleteAndZero(_textLabel);
    deleteAndZero(_origInfo);
    deleteAndZero(_decodeInfo);
    deleteAndZero(_decodeText);
    deleteAndZero(_menuC);
    deleteAndZero(_helpC);
    deleteAndZero(_error);
    deleteAndZero(_closeErr);
    deleteAndZero(_chooseChecker);
    deleteAndZero(_black);
    //deleteAndZero(loadingGif);
    //deleteAndZero(progress);
    deleteAndZero(_FSizer);
    delete[] _compsList;
}

//==============================================================================
void MainComponent::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
    g.setFont(juce::Font(16.0f));
    g.setColour(juce::Colours::white);
    //g.fillAll(juce::Colour::fromRGB(128, 0, 0));
}

void MainComponent::resized()
{
   if (_startScreen)
   {
      _openTitle->setBounds((int)(getWidth() * 0.3), (int)(getHeight() * 0.22), (int)(getWidth() * 0.4), (int)(getHeight() * 0.08));
      _openLogo->setBounds((int)(getWidth() * 0.3), (int)(getHeight() * 0.3), (int)(getWidth() * 0.4), (int)(getHeight() * 0.4));
      _mainBut->setBounds((int)(getWidth() * 0.45), (int)(getHeight() * 0.75), (int)(getWidth() * 0.1), (int)(getHeight() * 0.1));
   }
   else
   {
      Grid grid;
      //
      using Track = Grid::TrackInfo;
      using Fr = Grid::Fr;
      //
      grid.templateRows = { Track(Fr(1)), Track(Fr(12)), Track(Fr(1)), Track(Fr(4)) };
      grid.templateColumns = { Track(Fr(1)), Track(Fr(1)), Track(Fr(1)) };
      //
      grid.items = { GridItem(_origTitle), GridItem(_diffTitle), GridItem(_newTitle), GridItem(_orig), GridItem(_diff), GridItem(_newIm), GridItem(_origLabel), GridItem(_decodeLabel), GridItem(_textLabel), GridItem(_origInfo), GridItem(_decodeInfo), GridItem(_decodeText) };
      //
      _menuC->setBounds(0, 0, (int)(getWidth() * 0.15) - 10, getHeight());
      //
      /*hideBut->setBounds(0, 0, (int)(getWidth() * 0.05) - 10, (int)(getHeight() * 0.05) - 5);
      startBut->setBounds((int)(getWidth() * 0.05), 0, (int)(getWidth() * 0.05) - 10, (int)(getHeight() * 0.05) - 5);*/
      _FSizer->layOutComponents(_compsList, 5, 0, 0, (int)(getWidth() * 0.15) - 10, (int)(getHeight() * 0.05) - 5, false, true);
      //
      if (_hidden)
      {
         grid.performLayout(juce::Rectangle<int>(0, (int)(getHeight() * 0.05), getWidth(), (int)(getHeight() * 0.94)));
         _error->setBounds((int)(getWidth() * 0.35), (int)(getHeight() * 0.25), (int)(getWidth() * 0.3), (int)(getHeight() * 0.3));
         _closeErr->setBounds((int)(getWidth() * 0.4), (int)(getHeight() * 0.55), (int)(getWidth() * 0.2), (int)(getHeight() * 0.2));
      }
      else
      {
         grid.performLayout(juce::Rectangle<int>((int)(getWidth() * 0.15), 0, (int)(getWidth() * 0.85), (int)(getHeight() * 0.99)));
         _error->setBounds((int)(getWidth() * 0.4), (int)(getHeight() * 0.25), (int)(getWidth() * 0.3), (int)(getHeight() * 0.3));
         _closeErr->setBounds((int)(getWidth() * 0.45), (int)(getHeight() * 0.55), (int)(getWidth() * 0.2), (int)(getHeight() * 0.2));
      }
      _black->setBounds(0, 0, getWidth(), getHeight());
      _helpC->setBounds(0, 0, getWidth(), getHeight());
      //progress->setBounds((int)(getWidth() * 0.25), (int)(getHeight() * 0.45), (int)(getWidth() * 0.5), (int)(getHeight() * 0.1));
   }
}

void MainComponent::buttonClicked(Button* butt)
{
    if (butt == _startBut)
    {
       if ((_menuC->imageName != "-1") && (_menuC->secrName != "-1"))
       {
           startDecode();
          //LoadWindow* processWnd = new LoadWindow(TRANS(std::wstring(L"Load").c_str()),
          //                                             TRANS(std::wstring(L"Load...").c_str()),
          //                                             MessageBoxIconType::NoIcon);
          //   processWnd->setBounds((int)(getWidth() * 0.45), (int)(getHeight() * 0.3), (int)(getWidth() * 0.1), (int)(getHeight() * 0.4));
          //   processWnd->enterModalState(true, nullptr, true);
          //   MessageManager::callAsync([this, processWnd]()
          //      {
          //         startDecode();
          //         if (nullptr != processWnd)
          //         {
          //            processWnd->exitModalState();
          //         }
          //      });
       
       }
       else
       {
          _error->setVisible(true);
          _black->setVisible(true);
          repaint();
          _closeErr->setVisible(true);
          _closeErr->enterModalState(true, nullptr, false);
          resized();
       }
    }
    else if (butt == _mainBut)
    {
       _startScreen = false;
       Array<Component*> tmp = getChildren();
       for (int i = 0; i < tmp.size(); i++)
       {
          tmp[i]->setVisible(true);
       }
       _error->setVisible(false);
       _closeErr->setVisible(false);
       _black->setVisible(false);
       _openLogo->setVisible(false);
       _openTitle->setVisible(false);
       _mainBut->setVisible(false);
       //startBut->setEnabled(true);
       resized();
    }
    else if (butt == _hideBut)
    {
       _menuC->setName("HI");
       _hidden = !_hidden;
       _menuC->setVisible(!(_menuC->isVisible()));
       resized();
    }
    else if (butt == _homeBut)
    {
       _startScreen = true;
       Array<Component*> tmp = getChildren();
       for (int i = 0; i < tmp.size(); i++)
       {
          tmp[i]->setVisible(false);
       }
       _openLogo->setVisible(true);
       _openTitle->setVisible(true);
       _mainBut->setVisible(true);
       _hidden = false;
       resized();
    }
    else if (butt == _closeErr)
    {
       _error->setVisible(false);
       _closeErr->setVisible(false);
       _black->setVisible(false);
       repaint();
       _closeErr->exitModalState();
       resized();
    }
    else if (butt == _infoBut)
    {
       HelpWindow* tmp = new HelpWindow("Help window");
       tmp->enterModalState(true, nullptr, true);
       resized();
       
    }
    else if (butt == _editBut)
    {
       _edited = true;
       EditWindow* tmp = new EditWindow("Edit \"new.bmp\"", L"../../Images/Results/new.bmp");
       tmp->enterModalState(true, nullptr, true);
       resized();

    }
}
//
void MainComponent::componentNameChanged(Component& component)
{
   if (component.getName() == "Load")
   {
      resized();
      repaint();
   }
   else
   {
      paintOrig((_menuC->imageName == "-1"));
   }
}
void MainComponent::initGui()
{
    Font font;
    font.setHeight(25);
    //
    setOpaque(true);
    addMouseListener(this, true);
    //
    _openLogo = new ImageComponent();
    _openLogo->setImage(ImageCache::getFromFile(File::getCurrentWorkingDirectory().getChildFile("logo4.png")));
    addAndMakeVisible(_openLogo);
    //
    _openTitle = new ImageComponent();
    _openTitle->setImage(ImageCache::getFromFile(File::getCurrentWorkingDirectory().getChildFile("StegoGUI.png")));
    addAndMakeVisible(_openTitle);
    //
    _origTitle = new Label();
    _origTitle->setFont(font);
    _origTitle->setText(String((std::wstring(L"Пустой контейнер")).c_str()), dontSendNotification);
    addAndMakeVisible(_origTitle);
    //
    _diffTitle = new Label();
    _diffTitle->setFont(font);
    _diffTitle->setText(String((std::wstring(L"Разностное изображение")).c_str()), dontSendNotification);
    addAndMakeVisible(_diffTitle);
    //
    _newTitle = new Label();
    _newTitle->setFont(font);
    _newTitle->setText(String((std::wstring(L"Заполненный контейнер")).c_str()), dontSendNotification);
    addAndMakeVisible(_newTitle);
    //
    Image startLogo = ImageCache::getFromFile(File::getCurrentWorkingDirectory().getChildFile("play1.png"));
    _startBut = new ImageButton();
    _startBut->addListener(this);
    _startBut->setImages(false, true, true, startLogo, 1.0f, Colours::transparentWhite, startLogo, 0.3f, Colours::transparentWhite, startLogo, 0.1f, Colours::transparentWhite);
    //
    Image infoLogo = ImageCache::getFromFile(File::getCurrentWorkingDirectory().getChildFile("info.png"));
    _infoBut = new ImageButton();
    _infoBut->addListener(this);
    _infoBut->setImages(false, true, true, infoLogo, 1.0f, Colours::transparentWhite, infoLogo, 0.3f, Colours::transparentWhite, infoLogo, 0.1f, Colours::transparentWhite);
    //
    Image editLogo = ImageCache::getFromFile(File::getCurrentWorkingDirectory().getChildFile("edit.png"));
    _editBut = new ImageButton();
    _editBut->addListener(this);
    _editBut->setImages(false, true, true, editLogo, 1.0f, Colours::transparentWhite, editLogo, 0.3f, Colours::transparentWhite, editLogo, 0.1f, Colours::transparentWhite);
    //
    _mainBut = new ImageButton();
    _mainBut->addListener(this);
    _mainBut->setImages(false, true, true, startLogo, 1.0f, Colours::transparentWhite, startLogo, 0.3f, Colours::transparentWhite, startLogo, 0.1f, Colours::transparentWhite);
    //
    Image menuLogo = ImageCache::getFromFile(File::getCurrentWorkingDirectory().getChildFile("menu.png"));
    _hideBut = new ImageButton();
    _hideBut->addListener(this);
    _hideBut->setImages(false, true, true, menuLogo, 1.0f, Colours::transparentWhite, menuLogo, 0.3f, Colours::transparentWhite, menuLogo, 0.1f, Colours::transparentWhite);
    //
    Image homeLogo = ImageCache::getFromFile(File::getCurrentWorkingDirectory().getChildFile("home.png"));
    _homeBut = new ImageButton();
    _homeBut->addListener(this);
    _homeBut->setImages(false, true, true, homeLogo, 1.0f, Colours::white, homeLogo, 0.3f, Colour::fromRGB(128, 128, 128), homeLogo, 0.3f, Colour::fromRGB(96, 96, 96));
    //
    _orig = new ImageComponent();
    _orig->setImage(ImageCache::getFromFile(File::getCurrentWorkingDirectory().getChildFile("empty.png")));
    addAndMakeVisible(_orig);
    //
    _diff = new ImageComponent();
    _diff->setImage(ImageCache::getFromFile(File::getCurrentWorkingDirectory().getChildFile("empty.png")));
    addAndMakeVisible(_diff);
    //
    _newIm = new ImageComponent();
    _newIm->setImage(ImageCache::getFromFile(File::getCurrentWorkingDirectory().getChildFile("empty.png")));
    addAndMakeVisible(_newIm);
    ///
    font.setHeight(25);
    //
    _origLabel = new Label();
    _origLabel->setFont(font);
    _origLabel->setText(String((std::wstring(L"Информация об изображении")).c_str()), dontSendNotification);
    addAndMakeVisible(_origLabel);
    // 
    _decodeLabel = new Label();
    _decodeLabel->setFont(font);
    _decodeLabel->setText(String((std::wstring(L"Информация об извлечении")).c_str()), dontSendNotification);
    addAndMakeVisible(_decodeLabel);
    //
    _textLabel = new Label();
    _textLabel->setFont(font);
    _textLabel->setText(String((std::wstring(L"Извлеченный текст")).c_str()), dontSendNotification);
    addAndMakeVisible(_textLabel);
    //
    _origInfo = new TextEditor();
    _origInfo->setCaretVisible(false);
    _origInfo->setMultiLine(true);
    _origInfo->setReadOnly(true);
    _origInfo->setScrollbarsShown(true);
    _origInfo->setFont(font);
    addAndMakeVisible(_origInfo);
    //
    _decodeInfo = new TextEditor();
    _decodeInfo->setCaretVisible(false);
    _decodeInfo->setMultiLine(true);
    _decodeInfo->setReadOnly(true);
    _decodeInfo->setScrollbarsShown(true);
    _decodeInfo->setFont(font);
    addAndMakeVisible(_decodeInfo);
    //
    _decodeText = new TextEditor();
    _decodeText->setCaretVisible(false);
    _decodeText->setMultiLine(true);
    _decodeText->setReadOnly(true);
    _decodeText->setScrollbarsShown(true);
    _decodeText->setFont(font);
    addAndMakeVisible(_decodeText);
    //
    _chooseChecker = new Component("1");
    _chooseChecker->addComponentListener(this);
    _menuC = new MenuComponent(_chooseChecker);
    _helpC = new HelpComponent();
    //
    addAndMakeVisible(_menuC);
    addAndMakeVisible(_startBut);
    addAndMakeVisible(_infoBut);
    addAndMakeVisible(_editBut);
    addAndMakeVisible(_mainBut);
    addAndMakeVisible(_hideBut);
    addAndMakeVisible(_homeBut);
    //addAndMakeVisible(helpC);
    //
    _error = new ImageComponent();
    _error->setImage(ImageCache::getFromFile(File::getCurrentWorkingDirectory().getChildFile("error.png")));
    //
    _closeErr = new TextButton(String((std::wstring(L"Файлы не выбраны!!!\nНажать сюда для закрытия")).c_str()));
    _closeErr->addListener(this);
    //
    _chooseChecker->addComponentListener(this);
    //
    _black = new BlackComponent();
    addAndMakeVisible(_black);
    //black->setColour(Label::backgroundColourId, juce::Colour::fromRGBA(0, 0, 0, 180));
    //
    addAndMakeVisible(_error);
    _error->setVisible(false);
    addAndMakeVisible(_closeErr);
    _black->setVisible(false);
    _closeErr->setVisible(false);
    _helpC->setVisible(false);
    setSize(600, 400);
    //
    /*loadingGif = new LoadWindow(TRANS(std::wstring(L"Загрузка").c_str()),
       TRANS(std::wstring(L"Процесс идет...").c_str()),
       MessageBoxIconType::NoIcon);
    loadingGif->setBounds((int)(getWidth() * 0.3), (int)(getHeight() * 0.3), (int)(getWidth() * 0.4), (int)(getHeight() * 0.4));*/
    //
    //progress = new ProgressBar(progressStatus, ProgressBar::Style::linear);
    //addAndMakeVisible(progress);
    //progress->setVisible(false);
    //progress->setPercentageDisplay(true);
    //setProgress(100);
    //
    _compsList = new Component * [5];
    //
    _compsList[0] = _hideBut;
    _compsList[1] = _homeBut;
    _compsList[2] = _editBut;
    _compsList[3] = _infoBut;
    _compsList[4] = _startBut;
    //
    _FSizer = new StretchableLayoutManager();
    _FSizer->setItemLayout(0, 1, 100000, -1);
    _FSizer->setItemLayout(1, 1, 100000, -1);
    _FSizer->setItemLayout(2, 1, 100000, -1);
    _FSizer->setItemLayout(3, 1, 100000, -1);
    _FSizer->setItemLayout(4, 1, 100000, -1);
}
//
void MainComponent::paintOrig(bool error)
{
   if (!error)
   {
      //
      int height;
      int width;
      int size;
      String info;
      //
      readFileMono(_menuC->imageFile.getFullPathName().toWideCharPointer(), height, width, size, info);
      _origInfo->setText(info, sendNotification);
      if ((size < 100) || (height < 32) || (height > 5000) || (width < 32) || (width > 5000))
      {
         _orig->setImage(ImageFileFormat::loadFrom(File::getCurrentWorkingDirectory().getChildFile("error.png")), sendNotification);
         _startBut->setEnabled(false);
      }
      else
      {
         //doDecode(_menuC->imageFile.getFullPathName().toWideCharPointer(), "orig.png");
         bmpToPng(_menuC->imageFile.getFullPathName().toStdString(), "orig.png");
         _orig->setImage(ImageFileFormat::loadFrom(File::getCurrentWorkingDirectory().getChildFile("orig.png")), sendNotification);
         _startBut->setEnabled(true);
      }
   }
   else
   {
      _orig->setImage(ImageFileFormat::loadFrom(File::getCurrentWorkingDirectory().getChildFile("empty.png")), sendNotification);
      _origInfo->setText("", sendNotification);
   }
   _diff->setImage(ImageCache::getFromFile(File::getCurrentWorkingDirectory().getChildFile("empty.png")));
   _newIm->setImage(ImageCache::getFromFile(File::getCurrentWorkingDirectory().getChildFile("empty.png")));
   _decodeInfo->setText("", sendNotification);
   _decodeText->setText("", sendNotification);
}
void MainComponent::paintDiffText(string orig, string _new)
{
   _decodeText->setText("");
   //_new = _new.substr(0, 40);
#if 0
   //for (int i = 0; i < _new.size(); i++)
   for (int i = 0; i < 1; i++)
   {
      if (_orig[i] != _new[i])
         decodeText->setColour(TextEditor::textColourId, Colours::red);
      //String tmp = String((std::wstring(_new[i])).c_str());
      //wchar_t* tmpArr = new wchar_t[1];
      //tmpArr[1] = _new[i];
      wchar_t tmpChar = _new[i];
      wstring tmp = L"XXX";
      
      decodeText->insertTextAtCaret(tmp.c_str());
      decodeText->setColour(TextEditor::textColourId, Colours::white);
   }
   /////////////////////////////////////
#else
   int start = 0;
   int count = 0;
   for (int i = 0; i < orig.size(); i++)
   {
      count++;
      if (i >= _new.size())
      {
         _decodeText->insertTextAtCaret(_new.substr(start, count - 1));
         _decodeText->setColour(TextEditor::textColourId, Colours::red);
         for (int j = 0; j < orig.size() - _new.size(); j++)
         {
            _decodeText->insertTextAtCaret("X");
         }
         _decodeText->setColour(TextEditor::textColourId, Colours::white);
         return;
      }
      else if (orig[i] != _new[i])
      {
         _decodeText->insertTextAtCaret(_new.substr(start, count - 1));
         _decodeText->setColour(TextEditor::textColourId, Colours::red);
         _decodeText->insertTextAtCaret(_new.substr(i, 1));
         //decodeText->deleteBackwards(false);
         //decodeText->deleteForwards(true);
         start = i + 1;
         count = 0;
         _decodeText->setColour(TextEditor::textColourId, Colours::white);
      }
   }
   if(count != 0)
      _decodeText->insertTextAtCaret(_new.substr(start, count));
#endif
   // ///////////////////////////
   //decodeText->setText(_new);
   resized();
   //decodeText->setText(_new);
   /*insert(newText, 0, currentFont, findColour(textColourId), nullptr, caretPosition);
   insert(newText, insertIndex, currentFont, findColour(textColourId), getUndoManager(), newCaretPos);*/

}
//
void MainComponent::startDecode()
{
    _progressStatus = 5;
    //setProgress(progressStatus);
    srand((unsigned int)time(NULL));
    _origInfo->setText("In progress", dontSendNotification);
    _decodeInfo->setText("In progress", dontSendNotification);
    int difference = _menuC->getDiff();
    complex<double> differenceComplex(_menuC->getDiff(), 0.0);
    string message1;
    //
    vector<bitset<8>> vect = ReadWord(_menuC->messageFile.getFullPathName().toWideCharPointer(), message1);
    int word_size = (int)message1.length();
    vector<bitset<8>> vectSzhat;
    bitset<16> secr_size(word_size);
    bitset<8> empty;
    vectSzhat.push_back(empty);
    vectSzhat.push_back(empty);
    //
    int height;
    int width;
    int size;
    //
    String info;
    //
    uint8_t** pixels = readFileMono(_menuC->imageFile.getFullPathName().toWideCharPointer(), height, width, size, info);
    uint8_t** pixelsNew;
    uint8_t** pixelsWave = new uint8_t * [width];
    for (int z = 0; z < width; z++) {
        pixelsWave[z] = new uint8_t[width];
    }
    uint8_t** pixelsNewWave = new uint8_t * [width];
    for (int z = 0; z < width; z++) {
        pixelsNewWave[z] = new uint8_t[width];
    }
    //
    _origInfo->setText(info, dontSendNotification);
    //
    string result;
    //
    FILE* newFile;
    FILE* waveFile;
    FILE* waveNewFile;
    vector<int> key;
    key = ReadKey("key.txt", vect);
    if (_menuC->isAttack)
    {
       if (_edited) {
          pixelsNew = readFileMono(L"../../Images/Results/new1.bmp", height, width, size, info);
       }
       else
       {
         pixelsNew = readFileMono(L"../../Images/Results/new.bmp", height, width, size, info);
       }
       
    }
    else
    {
       _edited = false;
       newFile = createFileBmp("../../Images/Results/new.bmp", _menuC->imageFile.getFullPathName().toWideCharPointer());
       
       pixelsNew = readFileMono(_menuC->imageFile.getFullPathName().toWideCharPointer(), height, width, size, info);
    }
    if ((_menuC->selectedTr != Stego::LSB) && !(_menuC->isAttack) && (_menuC->selectedTr != Stego::HAAR))
    {
       if ((vect.size() * 8) >= (size / 64))
       {
          _decodeInfo->setText(String((std::wstring(L"Ошибка! Слишком большое сообщение.")).c_str()), dontSendNotification);
          return;
       }
    }
    if (_menuC->selectedTr == Stego::DCT)
    {
       if (!_menuC->isAttack)
       {
          key = CreateKey("key.txt", size, (int)vect.size(), Stego::DCT);
          encodeDCT(width, pixelsNew, vect, secr_size, difference, key);
          WriteToFileMono(newFile, pixelsNew, height, width);
       }
       result = decodeDCT(height, width, pixels, pixelsNew, vect, vectSzhat, key);
    }
    else if (_menuC->selectedTr == Stego::DFT)
    {
       if (!_menuC->isAttack)
       {
          key = CreateKey("key.txt", size, (int)vect.size(), Stego::DFT);
          encodeDFT(width, pixelsNew, vect, secr_size, differenceComplex, key);
          WriteToFileMono(newFile, pixelsNew, height, width);
       }
       result = decodeDFT(height, width, pixels, pixelsNew, vect, vectSzhat, key);
    }
    else if (_menuC->selectedTr == Stego::LSB)
    {
       if (!_menuC->isAttack)
       {
          key = CreateKey("key.txt", size, (int)vect.size(), Stego::LSB);
          encodeLSB(width, pixelsNew, vect, secr_size);
          WriteToFileMono(newFile, pixelsNew, height, width);
       }
       result = decodeLSB(width, pixelsNew, vect, vectSzhat);
    }
    else if (_menuC->selectedTr == Stego::DCT_KOCH)
    {
       if (!_menuC->isAttack)
       {
          key = CreateKey("key.txt", size, (int)vect.size(), Stego::DCT_KOCH);
          encodeDCTKoch(width, pixelsNew, vect, secr_size, difference, key);
          WriteToFileMono(newFile, pixelsNew, height, width);
       }
       result = decodeDCTKoch(height, width, pixelsNew, vect, vectSzhat, difference, key);
    }
    else if (_menuC->selectedTr == Stego::HAAR)
    {
        waveNewFile = createFileBmp("../../Images/Results/newWave.bmp", _menuC->imageFile.getFullPathName().toWideCharPointer());
        if (!_menuC->isAttack)
        {
            waveFile = createFileBmp("../../Images/Results/origWave.bmp", _menuC->imageFile.getFullPathName().toWideCharPointer());
            key = CreateKey("key.txt", size, (int)vect.size(), Stego::HAAR);
            encodeHaar(width, pixelsNew, pixelsWave, vect, secr_size, difference, key);
            WriteToFileMono(newFile, pixelsNew, height, width);
            WriteToFileMono(waveFile, pixelsWave, height, width);
            fclose(waveFile);
        }
        result = decodeHaar(height, width, pixels, pixelsNew, pixelsNewWave, vect, vectSzhat, key);
        WriteToFileMono(waveNewFile, pixelsNewWave, height, width);
        fclose(waveNewFile);
    }
    else if (_menuC->selectedTr == Stego::HAAR_KOCH)
    {
        waveNewFile = createFileBmp("../../Images/Results/newWave.bmp", _menuC->imageFile.getFullPathName().toWideCharPointer());
        if (!_menuC->isAttack)
        {
            waveFile = createFileBmp("../../Images/Results/origWave.bmp", _menuC->imageFile.getFullPathName().toWideCharPointer());
            //key = CreateKey("key.txt", size, (int)vect.size(), Stego::HAAR);
            encodeHaarKoch(width, pixelsNew, pixelsWave, vect, secr_size, difference, key);
            key = ReadKey("key.txt", vect);
            WriteToFileMono(newFile, pixelsNew, height, width);
            WriteToFileMono(waveFile, pixelsWave, height, width);
            fclose(waveFile);
        }
        result = decodeHaarKoch(height, width, pixels, pixelsNew, pixelsNewWave, vect, vectSzhat, difference, key);
        WriteToFileMono(waveNewFile, pixelsNewWave, height, width);
        fclose(waveNewFile);
    }
    if (!_menuC->isAttack)
    {
       fclose(newFile);
       
    }
    //
    paintDiffText(message1, result);
    _progressStatus = 30;
    //setProgress(progressStatus);
    //repaint();
    long double redP, greenP, blueP;
    long double psnrRes = PSNR(pixels, pixelsNew, height, width);
    long double mseRes = MSE(pixels, pixelsNew, height, width);
    String inf = "";
    inf += String((std::wstring(L"Алгоритм: ")).c_str());
    if (_menuC->selectedTr == Stego::DCT) inf += "DCT\n";
    if (_menuC->selectedTr == Stego::DFT) inf += "DFT\n";
    if (_menuC->selectedTr == Stego::LSB) inf += "LSB\n";
    if (_menuC->selectedTr == Stego::DCT_KOCH) inf += "DCT Koch\n";
    if (_menuC->selectedTr == Stego::HAAR) inf += "Haar\n";
    if (_menuC->selectedTr == Stego::HAAR_KOCH) inf += "Haar Koch\n";
    inf += "PSNR = ";
    inf += String(to_string(psnrRes));
    inf += "\n";
    inf += "MSE = ";
    inf += String(to_string(mseRes));
    inf += "\n";
    //
    inf += String((std::wstring(L"Коэффициент использования = ")).c_str());
    double coef = word_size * 8.0 / (height * width);
    inf += String(to_string(coef));
    inf += "\n";
    //
    inf += String((std::wstring(L"Коэффициент сокрытия = ")).c_str());
    if(_menuC->selectedTr == Stego::LSB)
       coef = 1;
    else
      coef = (height * width) / 64.0 / (height * width);
    inf += String(to_string(coef));
    inf += "\n";
    //
    inf += String((std::wstring(L"Доля испорченных бит = ")).c_str());
    double dolya = DolyaBitov(vect, vectSzhat);
    inf += String(to_string(dolya));
    inf += "\n";
    //
    inf += String((std::wstring(L"Коэффициент корреляции = ")).c_str());
    double corr = corrCoef(pixels, pixelsNew, height, width);
    inf += String(to_string(corr));
    inf += "\n";
    inf += String((std::wstring(L"Сделать гистограммы!!! ")).c_str());
    inf += "\n";
    //
    _progressStatus = 50;
    if(_edited)
       CreateDiffFileMono(_menuC->imageFile.getFullPathName().toWideCharPointer(), L"../../Images/Results/new1.bmp", "diff.bmp");
    else
       CreateDiffFileMono(_menuC->imageFile.getFullPathName().toWideCharPointer(), L"../../Images/Results/new.bmp", "diff.bmp");
    doDecode(L"diff.bmp", "diff.png");
    bmpToPng("diff.bmp", "diff.png");
    //
    _decodeInfo->setText(inf, dontSendNotification);
    _diff->setImage(ImageFileFormat::loadFrom(File::getCurrentWorkingDirectory().getChildFile("diff.png")));
    //diff->repaint();
    if(_edited)
       //doDecode(L"../../Images/Results/new1.bmp", "new.png");
        bmpToPng("../../Images/Results/new1.bmp", "new.png");
    else
       //doDecode(L"../../Images/Results/new.bmp", "new.png");
        bmpToPng("../../Images/Results/new.bmp", "new.png");
    _newIm->setImage(ImageFileFormat::loadFrom(File::getCurrentWorkingDirectory().getChildFile("new.png")));
    _progressStatus = 75;
    //
    for (int i = 0; i < height; i++)
    {
        delete[] pixels[i];
        delete[] pixelsNew[i];
        delete[] pixelsWave[i];
        delete[] pixelsNewWave[i];
    }
    //
    delete[] pixels;
    delete[] pixelsNew;
    delete[] pixelsWave;
    delete[] pixelsNewWave;
    //
    _progressStatus = 100;
    String check = _decodeText->getText();
    return;
    //setProgress(progressStatus);
}
//
Font MainComponent::getTextButtonFont(TextButton&, int buttonHeight)
{
   return { jmin(25.0f, (float)buttonHeight * 0.8f) };
}

//void MainComponent::timerCallback()
//{
//   loadingGif->repaint();
//}
//
//void MainComponent::threadComplete(bool userPressedCancel)
//{
//}
//
//void MainComponent::run()
//{
//   const MessageManagerLock mmLock;
//   startDecode();
//   while (!threadShouldExit())
//   {
//
//   }
//}