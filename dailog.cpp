#include "dailog.h"
#include "ui_dailog.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <iostream>

void errorMessage(QString message);
void Mat2Image(cv::Mat isrc, QImage& dst) // Okunan resmin kanal sayısına göre donusum islemi
{
    QImage::Format f;
    static cv::Mat src;
    isrc.copyTo(src);

    if(src.channels() == 3) // eger resim renkli ise (3 kanalli)
    {
        cv::cvtColor(src,src,cv::COLOR_BGR2RGB); // resim RGB formatina donusturulur.
        f = QImage::Format_RGB888;
    }
    else if(src.channels() == 1) // eger resim tek kanalliysa yani gri ise
    {
        f = QImage::Format_Indexed8;
    }
    else // eger kanal sayisi 1 veya 3 disinda ise hatalidir.
    {
        errorMessage("Görüntü kanal sayısı uyumsuz !");
        return;
    }
    dst = QImage((const unsigned char*)(src.data),src.cols,src.rows,src.step,f); // goruntu QImage objesine yazilir.
}

void dailog::changeContrast(float value)
{
    cv::Mat copy;
    this->img.copyTo(copy);
    this->img.convertTo(copy,-1,value,0);
    showImage(copy,ui->cons_area);
}

void errorMessage(QString message) // hata mesajlari icin bir metot. # refactor
{
    QMessageBox msg;
    msg.setText(message);
    msg.exec();
}

dailog::dailog(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::dailog)
{
    ui->setupUi(this);
}

dailog::~dailog()
{
    delete ui;
}

void dailog::showImage(cv::Mat dest, QLabel *lb) // secilen gorselin QLabel nesnesinde goruntulenmesini saglar
{
    QImage img;
    Mat2Image(dest,img);
    lb->setPixmap(QPixmap::fromImage(img).scaled(lb->size(),Qt::KeepAspectRatio));
}

void dailog::on_pushButton_clicked() // gorselin bilgisayardan alinmasi ve ekranda gosterilmesini saglayan metod.
{
    QString file_name = QFileDialog::getOpenFileName(this, "Open a file","C://"); // dosya yolunu alir
    std::string path = file_name.toLocal8Bit().constData(); // QString'den std::string tipine donusturme.
    ui->lineEdit->setText(file_name); // lineEdit uzerine dosya yolunun yazilmasi
    cv::Mat image = cv::imread(path); // gorselin ilgili dosya yolundan okunmasi

    if(image.data != NULL) // eger gorsel basarili bir sekilde okunduysa
    {
        showImage(image,ui->img_area);
        this->img = image.clone();
    }

    else
        errorMessage("Lütfen bir resim dosyasını seçin !");
}

void dailog::on_apply_filter_clicked() // filtrelerin uygulanmasını saglayan click sinyali
{
    int red_track_value = ui->red_slider->value(); // trackbarlardan alinan degerler.
    int green_track_value = ui->green_slider->value();
    int blue_track_value = ui->blue_slider->value();

    if(this->img.data != NULL) // eger img nesnesi bos degilse (goruntu okunduysa)
    {
        this->img = cv::imread(ui->lineEdit->text().toLocal8Bit().constData()); // img nesnesi eski haline getirilir.
        this->img = this->img + cv::Scalar(blue_track_value,green_track_value,red_track_value); // ilgili renk degerleri img nesnesine eklenir.
        showImage(this->img,ui->img_area); // her iki QLabel alanında gosterilir.
        showImage(this->img,ui->cons_area);
    }
    else
        errorMessage("Filtreyi uygulamadan önce lütfen bir resim seçin !");

}

void dailog::on_remove_filter_clicked() // filtlerin kaldirilmasi.
{
    if(this->img.data != NULL)
    {
        this->img = cv::imread(ui->lineEdit->text().toLocal8Bit().constData()); //img nesnesi gorselin ilk haline getirilir.
        showImage(this->img,ui->img_area);
        ui->red_slider->setValue(0); // track barlarin degerleri sifirlanir.
        ui->green_slider->setValue(0);
        ui->blue_slider->setValue(0);
    }
    else
        errorMessage("Filtreyi temizleden önce lütfen bir resim seçin !");
}

void dailog::on_save_img_clicked() // gorselin kayit edilmesi
{
    if(this->img.data != NULL) // eger gorsel okunmussa
    {
        std::vector<int> quality;
        quality.push_back(cv::IMWRITE_JPEG_QUALITY);
        quality.push_back(100); // max quality

        cv::imwrite("new_image.jpg",this->img,quality); // gorsel ilgili konuma yazilir.
        errorMessage("Resim basariyla kaydedildi ...");
    }
    else
        errorMessage("Resim kaydedilemedi !");
}

void dailog::on_apply_settings_clicked()
{
    if(this->img.data != NULL)
    {
        if(ui->high_cons->isChecked()) // yuksek kontrast degeri secilmise
            this->img.convertTo(this->img,-1,1.5,0); // 1.5 oraninda (1 normal hali) kontrast artilir.
        else if(ui->low_cons->isChecked()) // dusuk kontrast degeri secilmisse
            this->img.convertTo(this->img,-1,0.5,0); // 0.5 oranında kontast atrilir.(normal hali * 0.5 olarak dusunun yani yarı yarıya azaldi).
        else
            this->img.convertTo(this->img,-1,1,0); // normal kontrast degeri.
    }
    else
        errorMessage("Lütfen bir resim seçin !");

}

void dailog::on_comboBox_currentIndexChanged(int index) // renk tipleri secildiginde olusan durumlar
{
    int color_array[] = {cv::COLOR_BGR2RGB,
                         cv::COLOR_BGR2XYZ,
                         cv::COLOR_BGR2YCrCb,
                         cv::COLOR_BGR2HSV,
                         cv::COLOR_BGR2HLS,
                         cv::COLOR_BGR2Lab,
                         cv::COLOR_BGR2Luv,
                         cv::COLOR_BGR2YUV}; // renk uzaylari
    if(this->img.data != NULL) // eger resim okunduysa
    {
        cv::cvtColor(this->img,this->img,color_array[index]); // secilen resim uzayına convert islemi yapilir.
        std::vector<cv::Mat> channels; // ilgili gorselin kanallarını tutacak bir vektor.
        cv::split(this->img,channels); // gorselin kanallarinin ayrilmasi
        cv::equalizeHist(channels[0],channels[0]); // histogram esitleme
        cv::merge(channels,this->img); // tekrardan kanallari tek bir Matriste birlestirme
        showImage(this->img,ui->cons_area);
    }
    else
        errorMessage("Lütfen bir resim seçin !");

}

void dailog::on_remove_settings_clicked() // ayarlarin temizlenmesi
{
    if(this->img.data != NULL)
    {
        this->img = cv::imread(ui->lineEdit->text().toLocal8Bit().constData()); // img nesnesi eski haline getirilir.
        this->img = this->img + cv::Scalar(ui->blue_slider->value(),ui->green_slider->value(),ui->red_slider->value()); // ilgili renk degerleri img nesnesine eklenir.
        showImage(this->img,ui->cons_area);
    }
    else
        errorMessage("Lütfen bir resim seçin !");

}

void dailog::on_low_cons_clicked(bool checked)
{
    if(checked)
        changeContrast(0.5);
}

void dailog::on_high_cons_clicked(bool checked)
{
    if(checked)
        changeContrast(1.5);
}

void dailog::on_normal_cons_clicked(bool checked)
{
    if(checked)
        changeContrast(1);
}
