#ifndef RESULTINTERFACE_H
#define RESULTINTERFACE_H

#include <QDialog>

namespace Ui {
class ResultInterface;
}

class ResultInterface : public QDialog
{
    Q_OBJECT

public:
    explicit ResultInterface(QWidget *parent = nullptr);
    ~ResultInterface();


private slots:
    void on_voltage_button_clicked();

    void on_choose_file_button_clicked();

    void on_view_another_button_clicked();

private:
    Ui::ResultInterface *ui;
    QString moudle_case;

    float floatEndTime=0;
    int intXNumber=0;

    QString myfile;
};

#endif // RESULTINTERFACE_H
