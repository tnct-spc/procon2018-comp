#include "qrcode.h"
#include "ui_qrcode.h"

QRCode::QRCode(QWidget *parent):
    QWidget(parent),
    ui(new Ui::QRCode){
    ui->setupUi(this);
    ui->lineResult->setText("Push Start button.");
    QRCode::decodeQRcode();
}

QRCode::~QRCode(){
    delete ui;
}



void QRCode::on_Start_clicked(){
    this->clearAll();
    QRCode::decodeQRcode();
}

std::string QRCode::decodeQRcode(){
    VideoCapture cap(0);
    if(!cap.isOpened()){
        std::cout<<"Can not connect to cammera."<<std::endl;
        ui->lineResult->setText("Can not connect to cammera.");
        while(!cap.isOpened()){}
    }

    int resizedw;
    std::string code = {"."};
    std::string type = {""};

    ImageScanner scanner;
    scanner.set_config(ZBAR_NONE,ZBAR_CFG_ENABLE,1);

    double width = cap.get(CV_CAP_PROP_FRAME_WIDTH);
    double height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);

    std::cout<<"Size::"<<width<<"x"<<height<<std::endl;
    ui->lineWidth->setText(QString::fromStdString(std::to_string(width)));
    ui->lineHeight->setText(QString::fromStdString(std::to_string(height)));

    resizedw = (width-height)/2;

    while(1){
        Mat frame;

        bool success = cap.read(frame);

        if(!success){
            std::cout<<"Can not read a frame from video stream."<<std::endl;
            ui->lineResult->setText("Can not read a frame from video stream.");
            break;
        }
        else ui->lineResult->setText("Ok");

        Mat zframe(frame,Rect(resizedw,0,height,height));
        Mat gray;
        cvtColor(zframe,gray,CV_BGR2GRAY);

        unsigned int zwidth = zframe.cols;
        unsigned int zheight = zframe.cols;
        uchar *raw = (uchar *)gray.data;

        Image image(zwidth,zheight,"Y800",raw,zwidth*zheight);
        int n = scanner.scan(image);

        for(Image::SymbolIterator symbol = image.symbol_begin();symbol != image.symbol_end();++symbol){
            std::vector<Point> vp;

            code = symbol->get_data();
            type = symbol->get_type_name();
            std::cout<<symbol->get_type_name()<<" \""<<symbol->get_data()<<'"'<<std::endl;
            ui->lineType->setText(QString::fromStdString(symbol->get_type_name()));

            int n = symbol->get_location_size();
            for(int i=0;i<n;i++)vp.push_back(Point(symbol->get_location_x(i),symbol->get_location_y(i)));
            RotatedRect r = minAreaRect(vp);
            Point2f pts[4];
            r.points(pts);
            for(int i=0;i<4;i++)line(zframe,pts[i],pts[(i+1)%4],Scalar(255,0,0),3);
            std::cout<<"Angle:"<<r.angle<<std::endl;
            read_only_data = code;
            ui->lineCode->setText(QString::fromStdString(read_only_data));
        }
        imshow("Video Stream",zframe);
        waitKey(1);
        if(code != "."){
            ConvertCsvToField(read_only_data);
            ui->lineResult->setText("decoded");
            std::cout<<"decoded"<<std::endl;
            destroyWindow("Video Stream");
            break;
        }
    }
    return code;
}

void  QRCode::on_Exit_clicked(){
    exit(0);
}

void QRCode::clearAll(){
    ui->lineWidth->clear();
    ui->lineHeight->clear();
    ui->lineType->clear();
    ui->lineResult->clear();
    ui->lineCode->clear();
}
