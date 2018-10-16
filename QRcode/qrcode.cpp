#include "qrcode.h"

QRCode::QRCode(){
}

QRCode::~QRCode(){
}

std::string QRCode::decodeQRcode(){
    cv::VideoCapture cap(2);
    if(!cap.isOpened()){
        std::cout<<"Can not connect to cammera."<<std::endl;
        cap = cv::VideoCapture(0);
        while(!cap.isOpened()){}
    }

    int resizedw;
    std::string code = {"."};

    zbar::ImageScanner scanner;
    scanner.set_config(zbar::ZBAR_NONE,zbar::ZBAR_CFG_ENABLE,0);
    scanner.set_config(zbar::ZBAR_QRCODE,zbar::ZBAR_CFG_ENABLE,1);

    double width = cap.get(CV_CAP_PROP_FRAME_WIDTH);
    double height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);

    resizedw = (width-height)/2;

    while(1){
        cv::Mat frame;
        bool success = cap.read(frame);

        if(!success){
            std::cout<<"Can not read a frame from video stream."<<std::endl;
            break;
        }

        cv::Mat zframe(frame,cv::Rect(resizedw,0,height,height));
        cv::Mat gray;
        cvtColor(zframe,gray,CV_BGR2GRAY);

        unsigned int zwidth = zframe.cols;
        unsigned int zheight = zframe.cols;
        uchar *raw = (uchar *)gray.data;

        zbar::Image image(zwidth,zheight,"Y800",raw,zwidth*zheight);
        int n = scanner.scan(image);

        for(zbar::Image::SymbolIterator symbol = image.symbol_begin();symbol != image.symbol_end();++symbol)code = symbol->get_data();

        imshow("Video Stream",zframe);
        cv::waitKey(1);
        if(code != "."){
            cv::destroyWindow("Video Stream");
            break;
        }

    }
    return code;
}
