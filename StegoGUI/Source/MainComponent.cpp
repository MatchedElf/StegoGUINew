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
    Font font;
    font.setHeight(25);
    //
    setOpaque(true);
    addMouseListener(this, true);
    //
    openLogo = new ImageComponent();
    openLogo->setImage(ImageCache::getFromFile(File::getCurrentWorkingDirectory().getChildFile("logo4.png")));
    addAndMakeVisible(openLogo);
    //
    openTitle = new ImageComponent();
    openTitle->setImage(ImageCache::getFromFile(File::getCurrentWorkingDirectory().getChildFile("StegoGUI.png")));
    addAndMakeVisible(openTitle);
    //
    origTitle = new Label();
    origTitle->setFont(font);
    origTitle->setText(String((std::wstring(L"Пустой контейнер")).c_str()), dontSendNotification);
    addAndMakeVisible(origTitle);
    //
    diffTitle = new Label();
    diffTitle->setFont(font);
    diffTitle->setText(String((std::wstring(L"Разностное изображение")).c_str()), dontSendNotification);
    addAndMakeVisible(diffTitle);
    //
    newTitle = new Label();
    newTitle->setFont(font);
    newTitle->setText(String((std::wstring(L"Заполненный контейнер")).c_str()), dontSendNotification);
    addAndMakeVisible(newTitle);
    //
    Image startLogo = ImageCache::getFromFile(File::getCurrentWorkingDirectory().getChildFile("play1.png"));
    startBut = new ImageButton();
    startBut->addListener(this);
    startBut->setImages(false, true, true, startLogo, 1.0f, Colours::transparentWhite, startLogo, 0.3f, Colours::transparentWhite, startLogo, 0.1f, Colours::transparentWhite);
    //
    Image infoLogo = ImageCache::getFromFile(File::getCurrentWorkingDirectory().getChildFile("info.png"));
    infoBut = new ImageButton();
    infoBut->addListener(this);
    infoBut->setImages(false, true, true, infoLogo, 1.0f, Colours::transparentWhite, infoLogo, 0.3f, Colours::transparentWhite, infoLogo, 0.1f, Colours::transparentWhite);
    //
    mainBut = new ImageButton();
    mainBut->addListener(this);
    mainBut->setImages(false, true, true, startLogo, 1.0f, Colours::transparentWhite, startLogo, 0.3f, Colours::transparentWhite, startLogo, 0.1f, Colours::transparentWhite);
    //
    Image menuLogo = ImageCache::getFromFile(File::getCurrentWorkingDirectory().getChildFile("menu.png"));
    hideBut = new ImageButton();
    hideBut->addListener(this);
    hideBut->setImages(false, true, true, menuLogo, 1.0f, Colours::transparentWhite, menuLogo, 0.3f, Colours::transparentWhite, menuLogo, 0.1f, Colours::transparentWhite);
    //
    Image homeLogo = ImageCache::getFromFile(File::getCurrentWorkingDirectory().getChildFile("home.png"));
    homeBut = new ImageButton();
    homeBut->addListener(this);
    homeBut->setImages(false, true, true, homeLogo, 1.0f, Colours::white, homeLogo, 0.3f, Colour::fromRGB(128, 128, 128), homeLogo, 0.3f, Colour::fromRGB(96, 96, 96));
    //
    orig = new ImageComponent();
    orig->setImage(ImageCache::getFromFile(File::getCurrentWorkingDirectory().getChildFile("empty.png")));
    addAndMakeVisible(orig);
    //
    diff = new ImageComponent();
    diff->setImage(ImageCache::getFromFile(File::getCurrentWorkingDirectory().getChildFile("empty.png")));
    addAndMakeVisible(diff);
    //
    newIm = new ImageComponent();
    newIm->setImage(ImageCache::getFromFile(File::getCurrentWorkingDirectory().getChildFile("empty.png")));
    addAndMakeVisible(newIm);
    ///
    font.setHeight(25);
    //
    origLabel = new Label();
    origLabel->setFont(font);
    origLabel->setText(String((std::wstring(L"Информация об изображении")).c_str()), dontSendNotification);
    addAndMakeVisible(origLabel);
    // 
    decodeLabel = new Label();
    decodeLabel->setFont(font);
    decodeLabel->setText(String((std::wstring(L"Информация об извлечении")).c_str()), dontSendNotification);
    addAndMakeVisible(decodeLabel);
    //
    textLabel = new Label();
    textLabel->setFont(font);
    textLabel->setText(String((std::wstring(L"Извлеченный текст")).c_str()), dontSendNotification);
    addAndMakeVisible(textLabel);
    //
    origInfo = new TextEditor();
    origInfo->setCaretVisible(false);
    origInfo->setMultiLine(true);
    origInfo->setReadOnly(true);
    origInfo->setScrollbarsShown(true);
    origInfo->setFont(font);
    addAndMakeVisible(origInfo);
    //
    decodeInfo = new TextEditor();
    decodeInfo->setCaretVisible(false);
    decodeInfo->setMultiLine(true);
    decodeInfo->setReadOnly(true);
    decodeInfo->setScrollbarsShown(true);
    decodeInfo->setFont(font);
    addAndMakeVisible(decodeInfo);
    //
    decodeText = new TextEditor();
    decodeText->setCaretVisible(false);
    decodeText->setMultiLine(true);
    decodeText->setReadOnly(true);
    decodeText->setScrollbarsShown(true);
    decodeText->setFont(font);
    addAndMakeVisible(decodeText);
    //
    chooseChecker = new Component("1");
    chooseChecker->addComponentListener(this);
    menuC = new MenuComponent(chooseChecker);
    helpC = new HelpComponent();
    //
    addAndMakeVisible(menuC);
    addAndMakeVisible(startBut);
    addAndMakeVisible(infoBut);
    addAndMakeVisible(mainBut);
    addAndMakeVisible(hideBut);
    addAndMakeVisible(homeBut);
    //addAndMakeVisible(helpC);
    //
    error = new ImageComponent();
    error->setImage(ImageCache::getFromFile(File::getCurrentWorkingDirectory().getChildFile("error.png")));
    //
    closeErr = new TextButton(String((std::wstring(L"Файлы не выбраны!!!\nНажать сюда для закрытия")).c_str()));
    closeErr->addListener(this);
    //
    chooseChecker->addComponentListener(this);
    //
    black = new BlackComponent();
    addAndMakeVisible(black);
    //black->setColour(Label::backgroundColourId, juce::Colour::fromRGBA(0, 0, 0, 180));
    //
    addAndMakeVisible(error);
    error->setVisible(false);
    addAndMakeVisible(closeErr);
    black->setVisible(false);
    closeErr->setVisible(false);
    helpC->setVisible(false);
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
    compsList = new Component * [4];
    //
    compsList[0] = hideBut;
    compsList[1] = homeBut;
    compsList[2] = infoBut;
    compsList[3] = startBut;
    //
    FSizer = new StretchableLayoutManager();
    FSizer->setItemLayout(0, 1, 100000, -1);
    FSizer->setItemLayout(1, 1, 100000, -1);
    FSizer->setItemLayout(2, 1, 100000, -1);
    FSizer->setItemLayout(3, 1, 100000, -1);
}

MainComponent::~MainComponent()
{
    setLookAndFeel(nullptr);
    deleteAndZero(openLogo);
    deleteAndZero(openTitle);
    deleteAndZero(origTitle);
    deleteAndZero(diffTitle);
    deleteAndZero(newTitle);
    deleteAndZero(orig);
    deleteAndZero(diff);
    deleteAndZero(newIm);
    deleteAndZero(startBut);
    deleteAndZero(infoBut);
    deleteAndZero(mainBut);
    deleteAndZero(hideBut);
    deleteAndZero(homeBut);
    deleteAndZero(origLabel);
    deleteAndZero(decodeLabel);
    deleteAndZero(textLabel);
    deleteAndZero(origInfo);
    deleteAndZero(decodeInfo);
    deleteAndZero(decodeText);
    deleteAndZero(menuC);
    deleteAndZero(helpC);
    deleteAndZero(error);
    deleteAndZero(closeErr);
    deleteAndZero(chooseChecker);
    deleteAndZero(black);
    //deleteAndZero(loadingGif);
    //deleteAndZero(progress);
    deleteAndZero(FSizer);
    delete[] compsList;
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
   if (startScreen)
   {
      openTitle->setBounds((int)(getWidth() * 0.3), (int)(getHeight() * 0.22), (int)(getWidth() * 0.4), (int)(getHeight() * 0.08));
      openLogo->setBounds((int)(getWidth() * 0.3), (int)(getHeight() * 0.3), (int)(getWidth() * 0.4), (int)(getHeight() * 0.4));
      mainBut->setBounds((int)(getWidth() * 0.45), (int)(getHeight() * 0.75), (int)(getWidth() * 0.1), (int)(getHeight() * 0.1));
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
      grid.items = { GridItem(origTitle), GridItem(diffTitle), GridItem(newTitle), GridItem(orig), GridItem(diff), GridItem(newIm), GridItem(origLabel), GridItem(decodeLabel), GridItem(textLabel), GridItem(origInfo), GridItem(decodeInfo), GridItem(decodeText) };
      //
      menuC->setBounds(0, 0, (int)(getWidth() * 0.1) - 10, getHeight());
      //
      /*hideBut->setBounds(0, 0, (int)(getWidth() * 0.05) - 10, (int)(getHeight() * 0.05) - 5);
      startBut->setBounds((int)(getWidth() * 0.05), 0, (int)(getWidth() * 0.05) - 10, (int)(getHeight() * 0.05) - 5);*/
      FSizer->layOutComponents(compsList, 4, 0, 0, (int)(getWidth() * 0.1) - 10, (int)(getHeight() * 0.05) - 5, false, true);
      //
      if (hidden)
      {
         grid.performLayout(juce::Rectangle<int>(0, (int)(getHeight() * 0.05), getWidth(), (int)(getHeight() * 0.94)));
         error->setBounds((int)(getWidth() * 0.35), (int)(getHeight() * 0.25), (int)(getWidth() * 0.3), (int)(getHeight() * 0.3));
         closeErr->setBounds((int)(getWidth() * 0.4), (int)(getHeight() * 0.55), (int)(getWidth() * 0.2), (int)(getHeight() * 0.2));
      }
      else
      {
         grid.performLayout(juce::Rectangle<int>((int)(getWidth() * 0.1), 0, (int)(getWidth() * 0.9), (int)(getHeight() * 0.99)));
         error->setBounds((int)(getWidth() * 0.4), (int)(getHeight() * 0.25), (int)(getWidth() * 0.3), (int)(getHeight() * 0.3));
         closeErr->setBounds((int)(getWidth() * 0.45), (int)(getHeight() * 0.55), (int)(getWidth() * 0.2), (int)(getHeight() * 0.2));
      }
      black->setBounds(0, 0, getWidth(), getHeight());
      helpC->setBounds(0, 0, getWidth(), getHeight());
      //progress->setBounds((int)(getWidth() * 0.25), (int)(getHeight() * 0.45), (int)(getWidth() * 0.5), (int)(getHeight() * 0.1));
   }
}

void MainComponent::buttonClicked(Button* butt)
{
    if (butt == startBut)
    {
       if ((menuC->imageName != "-1") && (menuC->secrName != "-1"))
       {
          LoadWindow* processWnd = new LoadWindow(TRANS(std::wstring(L"Load").c_str()),
                                                       TRANS(std::wstring(L"Load...").c_str()),
                                                       MessageBoxIconType::NoIcon);
             processWnd->setBounds((int)(getWidth() * 0.45), (int)(getHeight() * 0.3), (int)(getWidth() * 0.1), (int)(getHeight() * 0.4));
             processWnd->enterModalState(true, nullptr, true);
             MessageManager::callAsync([this, processWnd]()
                {
                   startDecode();
                   if (nullptr != processWnd)
                   {
                      processWnd->exitModalState();
                   }
                });
       }
       else
       {
          error->setVisible(true);
          black->setVisible(true);
          repaint();
          closeErr->setVisible(true);
          closeErr->enterModalState(true, nullptr, false);
          resized();
       }
    }
    else if (butt == mainBut)
    {
       startScreen = false;
       Array<Component*> tmp = getChildren();
       for (int i = 0; i < tmp.size(); i++)
       {
          tmp[i]->setVisible(true);
       }
       error->setVisible(false);
       closeErr->setVisible(false);
       black->setVisible(false);
       openLogo->setVisible(false);
       openTitle->setVisible(false);
       mainBut->setVisible(false);
       //startBut->setEnabled(true);
       resized();
    }
    else if (butt == hideBut)
    {
       /*menuC->setName("HI");
       hidden = !hidden;
       menuC->setVisible(!(menuC->isVisible()));
       resized();*/
       //EditWindow* tmp = new EditWindow("Edit \"" + menuC->imageFile.getFileName() + "\"", L"../../Images/Results/new.bmp");
       edited = true;
       EditWindow* tmp = new EditWindow("Edit \"new.bmp\"", L"../../Images/Results/new.bmp");
       tmp->enterModalState(true, nullptr, true);
       resized();
    }
    else if (butt == homeBut)
    {
       startScreen = true;
       Array<Component*> tmp = getChildren();
       for (int i = 0; i < tmp.size(); i++)
       {
          tmp[i]->setVisible(false);
       }
       openLogo->setVisible(true);
       openTitle->setVisible(true);
       mainBut->setVisible(true);
       hidden = false;
       resized();
    }
    else if (butt == closeErr)
    {
       error->setVisible(false);
       closeErr->setVisible(false);
       black->setVisible(false);
       repaint();
       closeErr->exitModalState();
       resized();
    }
    else if (butt == infoBut)
    {
       HelpWindow* tmp = new HelpWindow("Help window");
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
      paintOrig((menuC->imageName == "-1"));
   }
}
//
void MainComponent::paintOrig(bool _error)
{
   if (!_error)
   {
      //
      int height;
      int width;
      int size;
      String info;
      //
      ReadFile(menuC->imageFile.getFullPathName().toWideCharPointer(), height, width, size, info);
      origInfo->setText(info, sendNotification);
      if ((size < 100) || (height < 32) || (height > 5000) || (width < 32) || (width > 5000))
      {
         orig->setImage(ImageFileFormat::loadFrom(File::getCurrentWorkingDirectory().getChildFile("error.png")), sendNotification);
         startBut->setEnabled(false);
      }
      else
      {
         doDecode(menuC->imageFile.getFullPathName().toWideCharPointer(), "orig.png");
         orig->setImage(ImageFileFormat::loadFrom(File::getCurrentWorkingDirectory().getChildFile("orig.png")), sendNotification);
         startBut->setEnabled(true);
      }
   }
   else
   {
      orig->setImage(ImageFileFormat::loadFrom(File::getCurrentWorkingDirectory().getChildFile("empty.png")), sendNotification);
      origInfo->setText("", sendNotification);
   }
   diff->setImage(ImageCache::getFromFile(File::getCurrentWorkingDirectory().getChildFile("empty.png")));
   newIm->setImage(ImageCache::getFromFile(File::getCurrentWorkingDirectory().getChildFile("empty.png")));
   decodeInfo->setText("", sendNotification);
   decodeText->setText("", sendNotification);
}
void MainComponent::paintDiffText(string _orig, string _new)
{
   decodeText->setText("");
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
   for (int i = 0; i < _orig.size(); i++)
   {
      count++;
      if (i >= _new.size())
      {
         decodeText->insertTextAtCaret(_new.substr(start, count - 1));
         decodeText->setColour(TextEditor::textColourId, Colours::red);
         for (int j = 0; j < _orig.size() - _new.size(); j++)
         {
            decodeText->insertTextAtCaret("X");
         }
         decodeText->setColour(TextEditor::textColourId, Colours::white);
         return;
      }
      else if (_orig[i] != _new[i])
      {
         decodeText->insertTextAtCaret(_new.substr(start, count - 1));
         decodeText->setColour(TextEditor::textColourId, Colours::red);
         decodeText->insertTextAtCaret(_new.substr(i, 1));
         //decodeText->deleteBackwards(false);
         //decodeText->deleteForwards(true);
         start = i + 1;
         count = 0;
         decodeText->setColour(TextEditor::textColourId, Colours::white);
      }
   }
   if(count != 0)
      decodeText->insertTextAtCaret(_new.substr(start, count));
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
    progressStatus = 5;
    //setProgress(progressStatus);
    srand((unsigned int)time(NULL));
    origInfo->setText("In progress", dontSendNotification);
    decodeInfo->setText("In progress", dontSendNotification);
    int difference = 8;
    complex<double> differenceComplex(2.0, 0.0);
    string message1;
    //
    vector<bitset<8>> vect = ReadWord(menuC->messageFile.getFullPathName().toWideCharPointer(), message1);
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
    RGB** pixels = ReadFile(menuC->imageFile.getFullPathName().toWideCharPointer(), height, width, size, info);
    RGB** pixelsNew;
    //
    origInfo->setText(info, dontSendNotification);
    //
    string result;
    //
    FILE* newFile;
    vector<int> key;
    if (menuC->isAttack)
    {
       if (edited) {
          pixelsNew = ReadFile(L"../../Images/Results/new1.bmp", height, width, size, info);
       }
       else
       {
         pixelsNew = ReadFile(L"../../Images/Results/new.bmp", height, width, size, info);
       }
       key = ReadKey("key.txt", vect);
    }
    else
    {
       edited = false;
       newFile = Create_File("../../Images/Results/new.bmp", menuC->imageFile.getFullPathName().toWideCharPointer());
       pixelsNew = ReadFile(menuC->imageFile.getFullPathName().toWideCharPointer(), height, width, size, info);
    }
    if ((menuC->selectedTr != 3) && !(menuC->isAttack))
    {
       if ((vect.size() * 8) >= (size / 64))
       {
          decodeInfo->setText(String((std::wstring(L"Ошибка! Слишком большое сообщение.")).c_str()), dontSendNotification);
          return;
       }
    }
    if (menuC->selectedTr == 1)
    {
       if (!menuC->isAttack)
       {
          key = CreateKey("key.txt", size, (int)vect.size(), true);
          encodeDCT(width, pixelsNew, vect, secr_size, difference, key);
          WriteToFile(newFile, pixelsNew, height, width);
       }
       result = decodeDCT(height, width, pixels, pixelsNew, vect, vectSzhat, key);
    }
    else if (menuC->selectedTr == 2)
    {
       if (!menuC->isAttack)
       {
          key = CreateKey("key.txt", size, (int)vect.size(), true);
          encodeDFT(width, pixelsNew, vect, secr_size, differenceComplex, key);
          WriteToFile(newFile, pixelsNew, height, width);
       }
       result = decodeDFT(height, width, pixels, pixelsNew, vect, vectSzhat, key);
    }
    else if (menuC->selectedTr == 3)
    {
       if (!menuC->isAttack)
       {
          key = CreateKey("key.txt", size, (int)vect.size(), false);
          encodeLSB(width, pixelsNew, vect, secr_size);
          WriteToFile(newFile, pixelsNew, height, width);
       }
       result = decodeLSB(width, pixelsNew, vect, vectSzhat);
    }
    else if (menuC->selectedTr == 4)
    {
       if (!menuC->isAttack)
       {
          key = CreateKey("key.txt", size, (int)vect.size(), true);
          encodeDCTKoch(width, pixelsNew, vect, secr_size, difference, key);
          WriteToFile(newFile, pixelsNew, height, width);
       }
       result = decodeDCTKoch(height, width, pixelsNew, vect, vectSzhat, difference, key);
    }
    if (!menuC->isAttack)
    {
       fclose(newFile);
       
    }
    //
    paintDiffText(message1, result);
    progressStatus = 30;
    //setProgress(progressStatus);
    //repaint();
    long double redP, greenP, blueP;
    PSNR(pixels, pixelsNew, redP, greenP, blueP, height, width);
    String inf = "";
    inf += String((std::wstring(L"Алгоритм: ")).c_str());
    if (menuC->selectedTr == 1) inf += "DCT\n";
    if (menuC->selectedTr == 2) inf += "DFT\n";
    if (menuC->selectedTr == 3) inf += "LSB\n";
    if (menuC->selectedTr == 4) inf += "DCT Koch\n";
    inf += "PSNR = ";
    inf += String(to_string(blueP));
    inf += "\n";
    //
    inf += String((std::wstring(L"Коэффициент использования = ")).c_str());
    double coef = word_size * 8.0 / (height * width);
    inf += String(to_string(coef));
    inf += "\n";
    //
    inf += String((std::wstring(L"Коэффициент сокрытия = ")).c_str());
    if(menuC->selectedTr == 3) 
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
    double corr = CorrCoef(pixels, pixelsNew, height, width);
    inf += String(to_string(corr));
    inf += "\n";
    //
    progressStatus = 50;
    if(edited)
       CreateDiffFile(menuC->imageFile.getFullPathName().toWideCharPointer(), L"../../Images/Results/new1.bmp", "diff.bmp");
    else
       CreateDiffFile(menuC->imageFile.getFullPathName().toWideCharPointer(), L"../../Images/Results/new.bmp", "diff.bmp");
    doDecode(L"diff.bmp", "diff.png");
    //
    decodeInfo->setText(inf, dontSendNotification);
    diff->setImage(ImageFileFormat::loadFrom(File::getCurrentWorkingDirectory().getChildFile("diff.png")));
    //diff->repaint();
    if(edited)
       doDecode(L"../../Images/Results/new1.bmp", "new.png");
    else
       doDecode(L"../../Images/Results/new.bmp", "new.png");
    newIm->setImage(ImageFileFormat::loadFrom(File::getCurrentWorkingDirectory().getChildFile("new.png")));
    progressStatus = 75;
    //
    for (int i = 0; i < height + 2; i++)
    {
        delete[] pixels[i];
        delete[] pixelsNew[i];
    }
    //
    delete[] pixels;
    delete[] pixelsNew;
    //
    progressStatus = 100;
    String check = decodeText->getText();
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