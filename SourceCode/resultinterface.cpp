#include "resultinterface.h"
#include "ui_resultinterface.h"
#include "mainwindow.h"
#include <QMessageBox>
#include <QFile>
#include <QFileInfo>
#include <QDebug>
#include <QScrollBar>
#include <QVector>

ResultInterface::ResultInterface(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ResultInterface)
{
    ui->setupUi(this);

    //set window title
    QWidget::setWindowTitle ( "BatteryFoam" );

    //to determine which moudle is

    QFile fileC(MainWindow::project_path+"/"+MainWindow::project_name+"/SPMFoam");
    QFile fileH(MainWindow::project_path+"/"+MainWindow::project_name+"/halfCellFoam");
    QFile fileF(MainWindow::project_path+"/"+MainWindow::project_name+"/fullCellFoam");

    if(fileC.exists()){
        moudle_case="Case";
    }else if(fileH.exists()){
        moudle_case="CC";
    }else{
        moudle_case="case";
    }

    //set endtime and the number of X
    QFile paramFile(MainWindow::project_path+"/"+MainWindow::project_name+"/"+moudle_case+"/system/controlDict");

    if(paramFile.open(QFile::ReadOnly | QFile::Text)){
        QTextStream fileStream(&paramFile);
        QString fileString = fileStream.readAll();

        //set endtime
        QRegularExpression baudExpression("endTime[ ]{1,}[0-9.e-]{1,}");
        QRegularExpressionMatch baudMatch = baudExpression.match(fileString);

        QString endtime;
        if(baudMatch.hasMatch()){
            endtime=baudMatch.capturedRef().toString();
        }

        endtime.replace(QRegExp("endTime[ ]{1,}"),QString(""));
        floatEndTime = endtime.toFloat();

        //set how many numbers
        QRegularExpression baudExpression2("deltaT[ ]{1,}[0-9.e-]{1,}");
        QRegularExpressionMatch baudMatch2 = baudExpression2.match(fileString);

        QString timestep;
        if(baudMatch2.hasMatch()){
            timestep=baudMatch2.capturedRef().toString();
        }

        timestep.replace(QRegExp("deltaT[ ]{1,}"),QString(""));
        intXNumber =floatEndTime/(timestep.toFloat()*10);

    }
    paramFile.close();
}

ResultInterface::~ResultInterface()
{
    delete ui;
}

void ResultInterface::on_voltage_button_clicked()
{
    QString path =MainWindow::project_path+"/"+MainWindow::project_name+"/"+moudle_case+"/time_voltage";

    QFile file(path);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    //get data from file and stored as text stream
    QTextStream in(&file);

    int i=0;
    int time=0;

    if(floatEndTime!=0&&intXNumber!=0){
        QVector<double> x(intXNumber), y(intXNumber);

        while (!in.atEnd()) {
              //prevent the user from modifing but not running, in this case, exists overflow risk
              if(i==intXNumber){
                  QMessageBox::warning(this,"Warning","It seems that the current 'Endtime' and/or 'Timestep' doesn't match your time_voltage file"
                                                      ", please check!");
                  break;
              }

              QString line = in.readLine();// read first line and so on
              QStringList fields = line.split(' ');// split the string

              //two conditions: 1.the value didn't occur before 2.this is not the title line
              if(time!=0){
                if(!x.contains(fields.at(0).toFloat()) && fields.at(0).toFloat()!=0){
                  x[i] =  fields.at(0).toFloat(); // take first value and stored into x
                  y[i] =  fields.at(1).toFloat(); // take second value and stored into y

                  i++;
                }
              }else{
                  time++;
              }

        }

        //prevent the user from modifing but not running, in this case, controlDict endTime > time_voltage endTIme
        if(x[i-1]!=floatEndTime){
            QMessageBox::warning(this,"Warning","It seems that the current 'Endtime' and/or 'Timestep' doesn't match your time_voltage file"
                                                ", please check!");
        }

        auto max = std::max_element(std::begin(y), std::end(y));
        auto min = std::min_element(std::begin(y), std::end(y));

        double extraBoundry;
        extraBoundry=(*max-*min)*0.2;

        // create graph and assign data to it:
        ui->customPlot->addGraph();
        ui->customPlot->graph(0)->setData(x, y);

        // give the axes some labels:
        ui->customPlot->xAxis->setLabel("time(s)");
        ui->customPlot->yAxis->setLabel("voltage(V)");

        // set axes ranges, so we see all data:
        ui->customPlot->xAxis->setRange(0, floatEndTime);
        if(*min!=*max){
            ui->customPlot->yAxis->setRange(*min-extraBoundry, *max+extraBoundry);
        }else{
            ui->customPlot->yAxis->setRange(*min*0.9, *max*1.1);
        }

        // replot graph
        ui->customPlot->replot();

        // allow users to rescale the graph
        ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

    }

}

//choose another results file
void ResultInterface::on_choose_file_button_clicked()
{
    myfile = QFileDialog::getOpenFileName(this,"Open file",MainWindow::project_path+"/"+MainWindow::project_name+"/"+moudle_case);
    ui->file_path_label->setText(myfile);

    if(ui->file_path_label->text().isEmpty()){
        QMessageBox::information(this,"hint","Path should not be empty");
        ui->view_another_button->setEnabled(false);
    }else{
        ui->view_another_button->setEnabled(true);

        //below are for set comboxBox
        QFile file(myfile);
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
            QMessageBox::warning(this,"Error","Cannot open the file.");
            return;
        }

        //get data from file and stored as text stream
        QTextStream in(&file);

        QString line = in.readLine();// read first line and so on
        QStringList fields = line.split(' ');// split the string

        ui->comboBox_x->clear();
        ui->comboBox_y->clear();

        for(int variable=0;variable<fields.size();variable++){
            ui->comboBox_x->addItem(fields.at(variable));
            ui->comboBox_y->addItem(fields.at(variable));
        }
    }
}

//generate the new graph according to the choose
void ResultInterface::on_view_another_button_clicked()
{
    QFile file(myfile);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    //get data from file and stored as text stream
    QTextStream in(&file);

    int i=0;
    int time=0;

    QString x_name;
    QString y_name;

    if(floatEndTime!=0&&intXNumber!=0){
        QVector<double> x(intXNumber), y(intXNumber);

        while (!in.atEnd()) {
              QString line = in.readLine();// read first line and so on
              QStringList fields = line.split(' ');// split the string

              //prevent the user from modifing but not running, in this case, exists overflow risk
              if(i==intXNumber){
                  QMessageBox::warning(this,"Warning","It seems that the current 'Endtime' and/or 'Timestep' doesn't match your time_voltage file"
                                                      ", please check!");
                  break;
              }

              if(time!=0){
                if(!x.contains(fields.at(ui->comboBox_x->currentIndex()).toFloat()) && fields.at(ui->comboBox_x->currentIndex()).toFloat()!=0){
                    x[i] =  fields.at(ui->comboBox_x->currentIndex()).toFloat(); // take first value and stored into x
                    y[i] =  fields.at(ui->comboBox_y->currentIndex()).toFloat(); // take second value and stored into y

                    i++;
                }
              }else{
                  x_name=fields.at(ui->comboBox_x->currentIndex());
                  y_name=fields.at(ui->comboBox_y->currentIndex());
                  time++;
              }

        }

        auto max = std::max_element(std::begin(y), std::end(y));
        auto min = std::min_element(std::begin(y), std::end(y));

        auto max_x = std::max_element(std::begin(x), std::end(x));
        auto min_x = std::min_element(std::begin(x), std::end(x));

        double extraBoundry;
        extraBoundry=(*max-*min)*0.2;

        // create graph and assign data to it:
        ui->customPlot->addGraph();
        ui->customPlot->graph(0)->setData(x, y);

        // give the axes some labels:
        ui->customPlot->xAxis->setLabel(x_name);
        ui->customPlot->yAxis->setLabel(y_name);

        // set axes ranges, so we see all data:
        ui->customPlot->xAxis->setRange(*min_x, *max_x);
        if(*min!=*max){
            ui->customPlot->yAxis->setRange(*min-extraBoundry, *max+extraBoundry);
        }else{
            ui->customPlot->yAxis->setRange(*min*0.9, *max*1.1);
        }

        // replot graph
        ui->customPlot->replot();

        // allow users to rescale the graph
        ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

    }
}
