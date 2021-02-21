#ifndef DAILOG_H
#define DAILOG_H

#include <QMainWindow>
#include <QDialog>
#include <QLabel>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <vector>

QT_BEGIN_NAMESPACE
namespace Ui { class dailog; }
QT_END_NAMESPACE

class dailog : public QMainWindow
{
    Q_OBJECT

public:
    dailog(QWidget *parent = nullptr);
    ~dailog();
    void showImage(cv::Mat dest, QLabel* lb);
    void changeContrast(float value);

private slots:
    void on_pushButton_clicked();

    void on_apply_filter_clicked();

    void on_remove_filter_clicked();

    void on_save_img_clicked();

    void on_apply_settings_clicked();

    void on_comboBox_currentIndexChanged(int index);

    void on_remove_settings_clicked();

    void on_low_cons_clicked(bool checked);

    void on_high_cons_clicked(bool checked);

    void on_normal_cons_clicked(bool checked);

private:
    Ui::dailog *ui;
    cv::Mat img;
};
#endif // DAILOG_H
