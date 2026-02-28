#include "mainframe.h"
#include "ui_mainframe.h"
#include "imageform.h"
#include "kfc.h"
#include "edge.h"
#include "corner.h"
#include "circleHough.h"

#include <QFileDialog>
#include <QPainter>
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

MainFrame::MainFrame(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainFrame)
{
    ui->setupUi(this);

    _q_pFormFocused     = 0;

    //객체 맴버의 초기화


    //리스트 출력창을 안보이게    
    ui->listWidget->setVisible(false);
    this->adjustSize();

    //UI 활성화 갱신
    UpdateUI();
}

MainFrame::~MainFrame()
{ 
    delete ui;         

    for(auto& item : _lImageForm)
        delete item;

}

void MainFrame::CloseImageForm(ImageForm *pForm)
{
    //ImageForm 포인터 삭제
    unsigned int idx = std::find(_lImageForm.begin(), _lImageForm.end(), pForm) - _lImageForm.begin();
    if(idx != _lImageForm.size())
    {
        delete _lImageForm[idx];
        _lImageForm.erase(_lImageForm.begin() + idx);
    }

    //활성화 ImageForm 초기화
    _q_pFormFocused     = 0;

    //관련 객체 삭제

    //UI 활성화 갱신
    UpdateUI();
}

void MainFrame::UpdateUI()
{    
    if(ui->tabWidget->currentIndex() == 0)
    {
    }
    else if(ui->tabWidget->currentIndex() == 1)
    {  
    }
    else if(ui->tabWidget->currentIndex() == 2)
    {        
    }
    else if(ui->tabWidget->currentIndex() == 3)
    {
    }
}

void MainFrame::OnMousePos(const int &nX, const int &nY, ImageForm* q_pForm)
{
}

void MainFrame::focusInEvent(QFocusEvent * event)
{
    Q_UNUSED(event) ;

    UpdateUI();
}

void MainFrame::closeEvent(QCloseEvent* event)
{
    //생성된 ImageForm을 닫는다.
    for(int i=0; i< _lImageForm.size(); i++)
        delete _lImageForm[i];

    //리스트에서 삭제한다.
    _lImageForm.clear();
}


void MainFrame::on_buttonOpen_clicked()
{
    //이미지 파일 선택
    QFileDialog::Options    q_Options   =  QFileDialog::DontResolveSymlinks  | QFileDialog::DontUseNativeDialog; // | QFileDialog::ShowDirsOnly
    QString                 q_stFile    =  QFileDialog::getOpenFileName(this, tr("Select a Image File"),  "./data", "Image Files(*.bmp *.ppm *.pgm *.png)",0, q_Options);

    if(q_stFile.length() == 0)
        return;

    //이미지 출력을 위한 ImageForm 생성    
    ImageForm*              q_pForm   = new ImageForm(q_stFile, "OPEN", this);

    _lImageForm.push_back(q_pForm);
    q_pForm->show();

    //UI 활성화 갱신
    UpdateUI();
}

void MainFrame::on_buttonDeleteContents_clicked()
{
    //생성된 ImageForm을 닫는다.
    for(int i=_lImageForm.size()-1; i>=0; i--)
        delete _lImageForm[i];

    //리스트에서 삭제한다.
    _lImageForm.clear();

    //객체 삭제


    ui->listWidget->clear();
}

void MainFrame::on_tabWidget_currentChanged(int index)
{
    static int nOld = -1;

    if(nOld == 0)
    {

    }
    else if(nOld == 1)
    {

    }
    nOld = index;

    //UI 활성화 갱신
    UpdateUI();
}

void MainFrame::on_buttonShowList_clicked()
{
    static int nWidthOld = ui->tabWidget->width();

    if(ui->listWidget->isVisible())
    {
        nWidthOld = ui->listWidget->width();
        ui->listWidget->hide();
        this->adjustSize();
    }
    else
    {        
        ui->listWidget->show();
        QRect q_rcWin = this->geometry();

        this->setGeometry(q_rcWin.left(), q_rcWin.top(), q_rcWin.width()+nWidthOld, q_rcWin.height());
    }
}




void MainFrame::on_Contrast_Transform_clicked()
{
    KImageColor icmain;
    if(_q_pFormFocused != 0 && _q_pFormFocused->ImageColor().Address() != 0 && _q_pFormFocused->ID() == "OPEN")
        icmain = _q_pFormFocused->ImageColor();
    else
        return;

    int a1 = ui->spinA1->value();
    int b1 = ui->spinB1->value();
    int a2 = ui->spinA2->value();
    int b2 = ui->spinB2->value();

    KImageColor igImg(icmain.Row(), icmain.Col());

    for (unsigned int i=0; i<icmain.Row(); i++)
    {
        for(unsigned int j=0; j<icmain.Col(); j++)
        {
            int red = icmain[i][j].r;
            int green = icmain[i][j].g;
            int blue = icmain[i][j].b;

            int R = ((b2-a2)/(b1-a1))*(red  -a1)+a2;
            int G = ((b2-a2)/(b1-a1))*(green-a1)+a2;
            int B = ((b2-a2)/(b1-a1))*(blue -a1)+a2;

            igImg[i][j].r = (red<a1)?a2:((red>b1)?b2:R);
            igImg[i][j].g = (green<a1)?a2:((green>b1)?b2:G);
            igImg[i][j].b = (blue<a1)?a2:((blue>b1)?b2:B);
        }
    }

    ImageForm* q_pForm = 0;
    for(auto item : _lImageForm)
        if(item->ID() == "Contrast TransForm" && item->size() == _q_pFormFocused->size())
        {
            q_pForm = item;
            break;
        }

    if(q_pForm)
        q_pForm->Update(igImg);

    else
    {
        q_pForm = new ImageForm(igImg, "Contrast TransForm", this);
        q_pForm->show();
        _lImageForm.push_back(q_pForm);
    }

    UpdateUI();
}

void MainFrame::on_Image_Dilation_mask3_clicked()
{
    KImageGray icMain;

    if(_q_pFormFocused != 0 && _q_pFormFocused->ImageGray().Address() != 0 && _q_pFormFocused->ID() == "OPEN")
        icMain = _q_pFormFocused->ImageGray();
    else
        return;

    KImageGray igImg(icMain.Row(), icMain.Col());

    igImg = icMain.Thresholded(ui->spinOstu->value()).BinaryDilate(_FOREGROUND);

    ImageForm* q_pForm = 0;
    for(auto item : _lImageForm)
        if(item->ID() == "Image Dilation(mask 3*3)" && item->size() == _q_pFormFocused->size())
        {
            q_pForm = item;
            break;
        }

    if(q_pForm)
        q_pForm->Update(igImg);

    else
    {
        q_pForm = new ImageForm(igImg, "Image Dilation(mask 3*3)", this);
        q_pForm->show();
        _lImageForm.push_back(q_pForm);
    }

    UpdateUI();


}


void MainFrame::on_Image_Erosion_mask3_clicked()
{
    KImageGray icMain;

    if(_q_pFormFocused != 0 && _q_pFormFocused->ImageGray().Address() != 0 && _q_pFormFocused->ID() == "OPEN")
        icMain = _q_pFormFocused->ImageGray();
    else
        return;

    KImageGray igImg(icMain.Row(), icMain.Col());

    igImg = icMain.Thresholded(ui->spinOstu->value()).BinaryErode(_FOREGROUND);

    ImageForm* q_pForm = 0;
    for(auto item : _lImageForm)
        if(item->ID() == "Image Erosion(mask 3*3)" && item->size() == _q_pFormFocused->size())
        {
            q_pForm = item;
            break;
        }

    if(q_pForm)
        q_pForm->Update(igImg);

    else
    {
        q_pForm = new ImageForm(igImg, "Image Erosion(mask 3*3)", this);
        q_pForm->show();
        _lImageForm.push_back(q_pForm);
    }

    UpdateUI();


}


void MainFrame::on_Image_Dilation_mask5_clicked()
{
    KImageGray igMain;

    if(_q_pFormFocused != 0 && _q_pFormFocused->ImageGray().Address() != 0 && _q_pFormFocused->ID() == "OPEN")
        igMain = _q_pFormFocused->ImageGray();
    else
        return;

    KImageGray igImg(igMain.Row(), igMain.Col());

    igImg = igMain.Thresholded(ui->spinOstu->value()).BinaryDilate2(_FOREGROUND);

    ImageForm* q_pForm = 0;
    for(auto item : _lImageForm)
        if(item->ID() == "Image Dilation(mask 5*5)" && item->size() == _q_pFormFocused->size())
        {
            q_pForm = item;
            break;
        }

    if(q_pForm)
        q_pForm->Update(igImg);

    else
    {
        q_pForm = new ImageForm(igImg, "Image Dilation(mask 5*5)", this);
        q_pForm->show();
        _lImageForm.push_back(q_pForm);
    }

    UpdateUI();


}


void MainFrame::on_Image_Erosion_mask5_clicked()
{
    KImageGray igMain;

    if(_q_pFormFocused != 0 && _q_pFormFocused->ImageGray().Address() != 0 && _q_pFormFocused->ID() == "OPEN")
        igMain = _q_pFormFocused->ImageGray();
    else
        return;

    KImageGray igImg(igMain.Row(), igMain.Col());

    igImg = igMain.Thresholded(ui->spinOstu->value()).BinaryErode2(_FOREGROUND);

    ImageForm* q_pForm = 0;
    for(auto item : _lImageForm)
        if(item->ID() == "Image Erosion(mask 5*5)" && item->size() == _q_pFormFocused->size())
        {
            q_pForm = item;
            break;
        }

    if(q_pForm)
        q_pForm->Update(igImg);

    else
    {
        q_pForm = new ImageForm(igImg, "Image Erosion(mask 5*5)", this);
        q_pForm->show();
        _lImageForm.push_back(q_pForm);
    }

    UpdateUI();


}



void MainFrame::on_ImageLabelingN4_clicked()
{
    KImageGray igMain;

    igMain = _q_pFormFocused->ImageGray();


    KImageColor icImg(igMain.Row(), igMain.Col());

    icImg = igMain.Thresholded(ui->spinOstu->value()).ImageLabelingN4();

    ImageForm* q_pForm = 0;
    for(auto item : _lImageForm)
        if(item->ID() == "Image Labeling" && item->size() == _q_pFormFocused->size())
        {
            q_pForm = item;
            break;
        }

    if(q_pForm)
        q_pForm->Update(icImg);

    else
    {
        q_pForm = new ImageForm(icImg, "Image Labeling(4-N)", this);
        q_pForm->show();
        _lImageForm.push_back(q_pForm);
    }

    UpdateUI();
}

void MainFrame::on_ImageLabelingN8_clicked()
{
    KImageGray igMain;

    igMain = _q_pFormFocused->ImageGray();


    KImageColor icImg(igMain.Row(), igMain.Col());

    //여기에 labeling code 추가

    icImg = igMain.Thresholded(ui->spinOstu->value()).ImageLabelingN8();

    ImageForm* q_pForm = 0;
    for(auto item : _lImageForm)
        if(item->ID() == "Image Labeling" && item->size() == _q_pFormFocused->size())
        {
            q_pForm = item;
            break;
        }

    if(q_pForm)
        q_pForm->Update(icImg);

    else
    {
        q_pForm = new ImageForm(icImg, "Image Labeling(8-N))", this);
        q_pForm->show();
        _lImageForm.push_back(q_pForm);
    }

    UpdateUI();
}

void MainFrame::on_Boundary_clicked()
{
    KImageGray igMain;
    int mask = ui->comboMask->currentIndex();
    int ground = ui->comboGround->currentIndex();

    igMain = _q_pFormFocused->ImageGray();

    KImageGray igImg(igMain.Row(), igMain.Col());

    igImg = igMain.Thresholded(131).Boundary(mask, ground);

    ImageForm* q_pForm = 0;
    for(auto item : _lImageForm)
        if(item->ID() == "Boundary" && item->size() == _q_pFormFocused->size())
        {
            q_pForm = item;
            break;
        }

    if(q_pForm)
        q_pForm->Update(igImg);

    else
    {
        q_pForm = new ImageForm(igImg, "Boundary", this);
        q_pForm->show();
        _lImageForm.push_back(q_pForm);
    }

    UpdateUI();
}

void MainFrame::on_pushOpening_clicked()
{
    KImageGray igMain;
    int size = ui->comboSize->currentIndex();

    igMain = _q_pFormFocused->ImageGray();
    KImageGray igImg(igMain.Row(), igMain.Col());

    switch(size){
    case 0:
        igImg = igMain.Thresholded(131).BinaryErode(_FOREGROUND).BinaryDilate(_FOREGROUND);
        break;
    case 1:
        igImg = igMain.Thresholded(131).BinaryErode2(_FOREGROUND).BinaryDilate2(_FOREGROUND);
        break;
    }


    ImageForm* q_pForm = 0;
    for(auto item : _lImageForm)
        if(item->ID() == "Opening" && item->size() == _q_pFormFocused->size())
        {
            q_pForm = item;
            break;
        }

    if(q_pForm)
        q_pForm->Update(igImg);

    else
    {
        q_pForm = new ImageForm(igImg, "Opening", this);
        q_pForm->show();
        _lImageForm.push_back(q_pForm);
    }

    UpdateUI();
}

void MainFrame::on_pushClosing_clicked()
{
    KImageGray igMain;
    int size = ui->comboSize->currentIndex();

    igMain = _q_pFormFocused->ImageGray();
    KImageGray igImg(igMain.Row(), igMain.Col());

    switch(size){
    case 0:
        igImg = igMain.Thresholded(131).BinaryDilate(_FOREGROUND).BinaryErode(_FOREGROUND);
        break;
    case 1:
        igImg = igMain.Thresholded(131).BinaryDilate2(_FOREGROUND).BinaryErode2(_FOREGROUND);
        break;
    }


    ImageForm* q_pForm = 0;
    for(auto item : _lImageForm)
        if(item->ID() == "Closing" && item->size() == _q_pFormFocused->size())
        {
            q_pForm = item;
            break;
        }

    if(q_pForm)
        q_pForm->Update(igImg);

    else
    {
        q_pForm = new ImageForm(igImg, "Closing", this);
        q_pForm->show();
        _lImageForm.push_back(q_pForm);
    }

    UpdateUI();
}

void MainFrame::on_pushHistoEqualization_clicked()
{
    KImageColor icMain;
    if(_q_pFormFocused != 0 && _q_pFormFocused->ImageColor().Address() != 0 && _q_pFormFocused->ID() == "OPEN")
        icMain = _q_pFormFocused->ImageColor();
    else
        return;

    KArray<KHSI> arHSI;
    icMain.RGBtoHSI(arHSI);

    icMain.ColorHistoEqualization(arHSI);

//    SaveHisto(arHSI, "C:/Users/jhwoo/수업/3학년/2학기/컴퓨터비전/과제/555/Histo.csv");

    icMain.FromHSI(arHSI);

    ImageForm* q_pForm = 0;
    for(auto item : _lImageForm)
        if(item->ID() == "Histo Equalization" && item->size() == _q_pFormFocused->size())
        {
            q_pForm = item;
            break;
        }

    if(q_pForm)
        q_pForm->Update(icMain);

    else
    {
        q_pForm = new ImageForm(icMain, "Histo Equalization", this);
        q_pForm->show();
        _lImageForm.push_back(q_pForm);
    }

    UpdateUI();
}

 void MainFrame::on_pushHistoMatching_clicked()
{
    QFileDialog :: Options q_Options = QFileDialog :: DontResolveSymlinks |QFileDialog :: DontUseNativeDialog;
    QString q_stFile = QFileDialog :: getOpenFileName(this, tr("Target Image"), "./data", "Image Files(*.ppm *.pgm *.tif)", 0, q_Options);

    if(q_stFile.length()==0)
        return;

    ImageForm * q_pFormT = new ImageForm(q_stFile, "Target Image", this);
    KImageColor icTarget = q_pFormT -> ImageColor();

    _lImageForm.push_back(q_pFormT);
    q_pFormT -> show();

    q_stFile = QFileDialog :: getOpenFileName(this, tr("Main Image"), "./data", "Image Files(*.bmp *.ppm *pgm *tif)", 0, q_Options);

    if(q_stFile.length() == 0)
        return;

    ImageForm * q_pFormS = new ImageForm(q_stFile, "Source Image", this);
    KImageColor icMain = q_pFormS -> ImageColor();

    _lImageForm.push_back(q_pFormS);
    q_pFormS -> show();


    icMain.ColorHistoMatching(icMain, icTarget);


    // print the image
    ImageForm* q_pForm = 0;
    for(auto item : _lImageForm)
        if(item->ID() == "Histo Matching" && item->size() == _q_pFormFocused->size())
        {
            q_pForm = item;
            break;
        }

    if(q_pForm)
        q_pForm->Update(icMain);

    else
    {
        q_pForm = new ImageForm(icMain, "Histo Matching", this);
        q_pForm->show();
        _lImageForm.push_back(q_pForm);
    }

    UpdateUI();
 }

void MainFrame::on_pushHoughTransform_clicked()
{
    KImageGray          igMain = _q_pFormFocused->ImageGray();
    KCIRCLEHOUGH_INFO   oInfo;

    oInfo.nLeft         = ui->editMinR->text().toInt();
    oInfo.nTop          = ui->editMinR->text().toInt();
    oInfo.nRight        = igMain.Col() - ui->editMinR->text().toInt();
    oInfo.nBottom       = igMain.Row() - ui->editMinR->text().toInt();
    oInfo.nMinR         = ui->editMinR->text().toInt();
    oInfo.nMaxR         = ui->editMaxR->text().toInt();
    oInfo.nNumGridCx    = (int)((oInfo.nRight - oInfo.nLeft + 1)/ui->editResolution->text().toDouble());
    oInfo.nNumGridCy    = (int)((oInfo.nBottom - oInfo.nTop + 1)/ui->editResolution->text().toDouble());
    oInfo.nNumGridR     = (int)((oInfo.nMaxR - oInfo.nMinR + 1)/ui->editResolution->text().toDouble());
    oInfo.nDirEdge      = _OUTWARD_EDGE;
    oInfo.nThreshVote   = ui->editThreshVote->text().toInt();

    //
    KEdge               oEdge(ui->editSigmaCanny->text().toDouble());
    KImageGray          igEdge;
    oEdge.Canny(ui->editLow->text().toInt(), ui->editHigh->text().toInt(), igMain,igEdge);

    KPGM(igEdge).Save("./output/edge4Hough.pgm");

    //
    KCircleHough        oCircleHough1, oCircleHough2, oCircleHough3; // 가상의 circle hough 객체 생성
    KCircle*            opCircle1;
    KCircle*            opCircle2;
    KCircle*            opCircle3; // 가상의 circle 객체 생성


    oInfo.nMinR = 20;
    oInfo.nMaxR = 25;
    oCircleHough1.Init((void*)&oInfo); // 입력한 oInfo 값으로 circle hough 객체 형성
    opCircle1 = (KCircle*)oCircleHough1.Run((void*)&oEdge); // circle hough 객체로 opcircle 형성

    oInfo.nMinR = 25;
    oInfo.nMaxR = 30;
    oCircleHough2.Init((void*)&oInfo); // 입력한 oInfo 값으로 circle hough 객체 형성
    opCircle2 = (KCircle*)oCircleHough2.Run((void*)&oEdge); // circle hough 객체로 opcircle 형성

    oInfo.nMinR = 30;
    oInfo.nMaxR = 35;
    oCircleHough3.Init((void*)&oInfo); // 입력한 oInfo 값으로 circle hough 객체 형성
    opCircle3 = (KCircle*)oCircleHough3.Run((void*)&oEdge); // circle hough 객체로 opcircle 형성


    if(opCircle1 == 0)
    {
        //결과 표시
        if(ui->listWidget->isVisible() == false)
            on_buttonShowList_clicked();
        ui->listWidget->insertItem(0,QString("There is no circle detected!"));

        return;
    }

    //출력창 결정
    ImageForm* q_pForm = 0;
    for(auto item : _lImageForm)
        if(item->ID() == "Circle Localization" && item->size() == _q_pFormFocused->size())
        {
            q_pForm = item;
            q_pForm->Update(igMain.GrayToRGB());
            break;
        }
    if(q_pForm == 0)
    {
        q_pForm = new ImageForm(igMain.GrayToRGB(), "Circle Localization", this);
        _lImageForm.push_back(q_pForm);
    }

    q_pForm->DrawEllipse(QPoint((int)(opCircle1->_dCx),(int)(opCircle1->_dCy)),
                         (int)(opCircle1->_dRadius),(int)(opCircle1->_dRadius),QColor(255,0,0),2);
    q_pForm->DrawEllipse(QPoint((int)(opCircle2->_dCx),(int)(opCircle2->_dCy)),
                         (int)(opCircle2->_dRadius),(int)(opCircle2->_dRadius),QColor(255,0,0),2);
    q_pForm->DrawEllipse(QPoint((int)(opCircle3->_dCx),(int)(opCircle3->_dCy)),
                         (int)(opCircle3->_dRadius),(int)(opCircle3->_dRadius),QColor(255,0,0),2);
    q_pForm->update();
    q_pForm->show();

    if(ui->listWidget->isVisible()==false)
        on_buttonShowList_clicked();

    ui->listWidget->insertItem(0,QString(">> detected circle --> radius = %1").arg(opCircle1->_dRadius));
    ui->listWidget->insertItem(0,QString(">> detected circle --> radius = %1").arg(opCircle2->_dRadius));
    ui->listWidget->insertItem(0,QString(">> detected circle --> radius = %1").arg(opCircle3->_dRadius));

    UpdateUI();

}

void MainFrame::on_pushGeneralizedHoughTransform_clicked()
{

    // 파일 열기
    ifstream inputFile("C:/Users/jhwoo/maxk.txt");

    if (!inputFile.is_open()) {
        std::cerr << "Unable to open the file." << std::endl;
    }

    // 첫 번째 줄 읽기: 좌표의 개수
    int numPoints;
    inputFile >> numPoints;

    // KPoint 배열 동적 할당
    std::vector<KPoint> points;
    points.reserve(numPoints);

    // 두 번째 줄부터 좌표 읽어와 배열에 저장
    for (int i = 0; i < numPoints; ++i) {
        int nX, nY;
        inputFile >> nX >> nY;
        points.emplace_back(nX, nY);
    }

    // 파일 닫기
    inputFile.close();

//    // 배열에 저장된 좌표 출력 (예시)
//    for (const auto& point : points) {
//        std::cout << "x: " << point._dX << ", y: " << point._dY << std::endl;
//    }

    // 템플릿으로 사용할 도형의 좌표 생성 (원의 경우)
    std::vector<KPoint> edgePoints;
    for (int angle = 0; angle < 360; angle += 10) {
        int x = static_cast<int>(50 * std::cos(angle * M_PI / 180.0));
        int y = static_cast<int>(50 * std::sin(angle * M_PI / 180.0));
        edgePoints.emplace_back(x, y);
    }

    // Generalized Hough Transform 수행
    GeneralizedHoughTransform ght(points);
    ght.detectCircle(edgePoints, 50); // radiusThreshold는 원을 검출할 때 얼마나 강한 신뢰로 간주할지 결정하는 임계값

}
