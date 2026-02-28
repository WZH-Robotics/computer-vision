#include "mainframe.h"
#include "ui_mainframe.h"
#include "imageform.h"

#include <QFileDialog>
#include <QPainter>
#include <sstream>
#include <fstream>

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
