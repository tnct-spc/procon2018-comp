#include "qrcode.h"
#include "ui_qrcode.h"



QRCode::QRCode(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QRCode)
{
    ui->setupUi(this);
}

QRCode::~QRCode()
{
    //cvReleaseCapture( &capture );
    delete ui;
}


/*#include "ui_qrcode.h"

using namespace zbar;
using namespace cv;

CvCapture* capture;
QTimer *timer;

QRCode::QRCode(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QRCode)
{
    ui->setupUi(this);
}

QRCode::~QRCode()
{
    cvReleaseCapture( &capture );
    delete ui;
}

void QRCode::on_btnOk_clicked()
{
    this->clearAll();
    //CheckVideoCamera
    if(capture == 0)
    {
        capture = cvCaptureFromCAM(CV_CAP_ANY);
        if(capture)
        {
            cvSetCaptureProperty(capture,CV_CAP_PROP_FRAME_HEIGHT,1600);
            cvSetCaptureProperty(capture,CV_CAP_PROP_FRAME_WIDTH,1600);

            timer = new QTimer(this);

            QObject::connect(timer,SIGNAL(timeout()),this,SLOT(ProcessFrame()));
            timer->start(1000/60);
        } else {
            ui->lineResult->setText("Cannot connect to Camera!");
        }
    }

    return ;
}

void QRCode::ProcessFrame()
{
    IplImage* src = cvQueryFrame(capture);
    QImage img_show = QImage((unsigned char*)(src->imageData),src->width,src->height,QImage::Format_RGB888).rgbSwapped();
    ui->areaCode->setPixmap(QPixmap::fromImage(img_show,Qt::AutoColor).scaledToWidth(300));

    int n = ScanIDCard(src);

    if(n)
    {
        timer->stop();
        cvReleaseCapture(&capture);
    }
}

int QRCode::ScanIDCard(IplImage *src)
{
    IplImage *img = cvCreateImage(cvSize(src->width,src->height), IPL_DEPTH_8U,1);

    cvCvtColor( src, img, CV_RGB2GRAY );

    //UseZbarlibraly

    uchar* raw = (uchar *)img->imageData;
    int width = img->width;
    int height = img->height;

    ImageScanner *scanner=new ImageScanner();

    scanner->set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 1);

    Image *zimg=new Image(width,height,"Y800",raw,width*height);

    int n=scanner->scan(*zimg);

    if(n>0)
    {
        for(Image::SymbolIterator symbol = zimg->symbol_begin(); symbol != zimg->symbol_end(); ++symbol)
        {
            //ShowResult
            this->showresult(QString::fromStdString(symbol->get_data()));
            //InputData
            inputdata(QString::fromStdString(symbol->get_data()));
        }

    }

    else
    {
        this->showresult("Can't Decode!!");
    }

    //ReleaseResorce
    zimg->set_data(NULL, 0);

    return n;
}

void QRCode::use_drawRectangle()
{
      IplImage *src_img = 0, *dst_img;
      IplImage *src_img_gray = 0;
      IplImage *tmp_img;
      CvMemStorage *storage = cvCreateMemStorage (0);
      CvSeq *contours = 0;
      CvBox2D ellipse;
      CvTreeNodeIterator it;
      CvPoint2D32f pt[4];

      IplImage* src = cvQueryFrame(capture);


      src_img=src;

      src_img_gray = cvCreateImage (cvGetSize (src_img), IPL_DEPTH_8U, 1);
      cvCvtColor (src_img, src_img_gray, CV_BGR2GRAY);
      tmp_img = cvCreateImage (cvGetSize (src_img), IPL_DEPTH_8U, 1);
      dst_img = cvCloneImage (src_img);

      cvThreshold (src_img_gray, tmp_img, 100, 255, CV_THRESH_BINARY);
      cvFindContours (tmp_img, storage, &contours, sizeof (CvContour),
                      CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cvPoint (0, 0));

      cvInitTreeNodeIterator (&it, contours, 3);

      while ((contours = (CvSeq *) cvNextTreeNode (&it)) != NULL)
      {
        if (contours->total > 5) {

          ellipse = cvFitEllipse2 (contours);
          ellipse.angle = 90.0 - ellipse.angle;

          cvDrawContours (dst_img, contours, CV_RGB (255, 0, 0), CV_RGB (255, 0, 0), 0, 1, CV_AA, cvPoint (0, 0));
          cvLine (dst_img, cvPointFrom32f (pt[0]), cvPointFrom32f (pt[1]), CV_RGB (0, 255, 0));
          cvLine (dst_img, cvPointFrom32f (pt[1]), cvPointFrom32f (pt[2]), CV_RGB (0, 255, 0));
          cvLine (dst_img, cvPointFrom32f (pt[2]), cvPointFrom32f (pt[3]), CV_RGB (0, 255, 0));
          cvLine (dst_img, cvPointFrom32f (pt[3]), cvPointFrom32f (pt[0]), CV_RGB (0, 255, 0));
        }

      }

      QImage img_show = QImage((unsigned char*)(dst_img->imageData),dst_img->width,dst_img->height,QImage::Format_RGB888).rgbSwapped();
      ui->areaCode->setPixmap(QPixmap::fromImage(img_show,Qt::AutoColor).scaledToWidth(300));
      int n = ScanIDCard(dst_img);

      if(n)
      {
          timer->stop();
          cvReleaseCapture(&capture);
      }

      cvReleaseMemStorage (&storage);

}

void QRCode::showresult(QString code)
{
    if(code != "Can't Decode!!") {
        ui->lineResult->setText(code);
    }
}

void QRCode::inputdata(QString code){
        read_only_data=code.toStdString();
}

std::string QRCode::getdata(){
    return read_only_data;
}

void QRCode::clearAll()
{
    ui->lineResult->clear();
}
*/
