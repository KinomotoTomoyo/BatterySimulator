#include "carboninterface.h"
#include "ui_carboninterface.h"
#include "mainwindow.h"
#include <QMessageBox>
#include <QFile>
#include <QFileInfo>
#include <QDebug>
#include <QProcess>
#include <QScrollBar>
#include <QTimer>
#include <QThread>
#include <QDir>

CarbonInterface::CarbonInterface(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CarbonInterface)
{
    ui->setupUi(this);

    //set window title
    QWidget::setWindowTitle ( "BatteryFoam" );

    //set maximum screen length
    ui->terminal_output_window->document()->setMaximumBlockCount(1000);

    //set view button is available if time_voltage exists
    QFile t_v_file(MainWindow::project_path+"/"+MainWindow::project_name+"/Case/time_voltage");
    if(t_v_file.exists()){
        ui->view_result_button->setEnabled(true);
    }

    //set initial value from blockMeshDict
    QFile paramFile(MainWindow::project_path+"/"+MainWindow::project_name+"/Case/system/blockMeshDict");

    if(paramFile.open(QFile::ReadOnly | QFile::Text)){
        QTextStream fileStream(&paramFile);
        QString fileString = fileStream.readAll();

        //set length,width and height
        QRegularExpression baudExpression("[-][0-9.]{1,}[ ]{1,}[-][0-9.]{1,}[ ]{1,}[-][0-9.]{1,}");
        QRegularExpressionMatch baudMatch = baudExpression.match(fileString);

        QString lengthWidthHeight;
        if(baudMatch.hasMatch()){
            lengthWidthHeight=baudMatch.capturedRef().toString();
        }

        QString length_original = lengthWidthHeight;
        length_original.replace(QRegExp("[ ]{1,}[-][0-9.]{1,}[ ]{1,}[-][0-9.]{1,}"),QString(""));
        float real_length = length_original.toFloat()*(-2);
        QString real_Length = QString::number(real_length);
        ui->length_lineEdit->setText(real_Length);

        QString width_original = lengthWidthHeight;
        width_original.replace(QRegExp("^[-][0-9.]{1,}[ ]{1,}"),QString(""));
        width_original.replace(QRegExp("[ ]{1,}[-][0-9.]{1,}"),QString(""));
        float real_width = width_original.toFloat()*(-2);
        QString real_Width = QString::number(real_width);
        ui->width_lineEdit->setText(real_Width);

        QString height_original = lengthWidthHeight;
        height_original.replace(QRegExp("[-][0-9.]{1,}[ ]{1,}[-][0-9.]{1,}[ ]{1,}"),QString(""));
        float real_height = height_original.toFloat()*(-2);
        QString real_Height = QString::number(real_height);
        ui->height_lineEdit->setText(real_Height);

        //set divition on x,y,z
        QRegularExpression baudExpression2("[0-9]{1,}[ ][0-9]{1,}[ ][0-9]{1,}[)][ ]simpleGrading");
        QRegularExpressionMatch baudMatch2 = baudExpression2.match(fileString);

        QString xyz_divition;
        if(baudMatch2.hasMatch()){
            xyz_divition=baudMatch2.capturedRef().toString();
        }

        QString x_original = xyz_divition;
        x_original.replace(QRegExp("[ ][0-9]{1,}[ ][0-9]{1,}[)][ ]simpleGrading"),QString(""));
        int real_x = x_original.toInt();
        QString real_X = QString::number(real_x);
        ui->x_divide_lineEdit->setText(real_X);

        QString y_original = xyz_divition;
        y_original.replace(QRegExp("^[0-9]{1,}[ ]"),QString(""));
        y_original.replace(QRegExp("[ ][0-9]{1,}[)][ ]simpleGrading"),QString(""));
        int real_y = y_original.toInt();
        QString real_Y = QString::number(real_y);
        ui->y_divide_lineEdit->setText(real_Y);

        QString z_original = xyz_divition;
        z_original.replace(QRegExp("[0-9]{1,}[ ][0-9]{1,}[ ]"),QString(""));
        z_original.replace(QRegExp("[)][ ]simpleGrading"),QString(""));
        int real_z = z_original.toInt();
        QString real_Z = QString::number(real_z);
        ui->z_divide_lineEdit->setText(real_Z);

        //set current unit
        QRegularExpression baudExpression3("convertToMeters[ ]{1,}[1][e][-][036]");
        QRegularExpressionMatch baudMatch3 = baudExpression3.match(fileString);

        QString initial_unit;
        if(baudMatch3.hasMatch()){
            initial_unit=baudMatch3.capturedRef().toString();
        }

        initial_unit.replace(QRegExp("convertToMeters[ ]{1,}"),QString(""));

        if(initial_unit=="1e-0"){
            ui->unit_select_box->setCurrentIndex(2);
        }else if(initial_unit=="1e-3"){
            ui->unit_select_box->setCurrentIndex(1);
        }else if(initial_unit=="1e-6"){
            ui->unit_select_box->setCurrentIndex(0);
        }
    }
    paramFile.close();

    //set initial value from topoSetDict
    QFile paramFile2(MainWindow::project_path+"/"+MainWindow::project_name+"/Case/system/topoSetDict");

    if(paramFile2.open(QFile::ReadOnly | QFile::Text)){
        QTextStream fileStream(&paramFile2);
        QString fileString = fileStream.readAll();

        //set radius
        QRegularExpression baudExpression("radius[ ]{1,}[0-9.]{1,}");
        QRegularExpressionMatch baudMatch = baudExpression.match(fileString);

        QString radius;
        if(baudMatch.hasMatch()){
            radius=baudMatch.capturedRef().toString();
        }

        QString radius_original =radius;
        radius_original.replace(QRegExp("radius[ ]{1,}"),QString(""));
        float real_radius = radius_original.toFloat();
        QString real_Radius = QString::number(real_radius);
        ui->radius_lineEdit->setText(real_Radius);

    }
    paramFile2.close();

    //set initial value from LiProperties
    QFile paramFile3(MainWindow::project_path+"/"+MainWindow::project_name+"/Case/constant/LiProperties");

    if(paramFile3.open(QFile::ReadOnly | QFile::Text)){
        QTextStream fileStream(&paramFile3);
        QString fileString = fileStream.readAll();

        QRegularExpressionMatch baudMatch;
        QString one_constant;

        //set parameter1 DS
        QRegularExpression baudExpression("Ds_value[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}");
        baudMatch = baudExpression.match(fileString);

        if(baudMatch.hasMatch()){
            one_constant=baudMatch.capturedRef().toString();
        }

        one_constant.replace(QRegExp("Ds_value[ ][\[][0-9 -]{1,}[]][ ]"),QString(""));
        ui->DS_lineEdit->setText(one_constant);

        //set parameter2 CS
        QRegularExpression baudExpression2("Cs_max[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}");
        baudMatch = baudExpression2.match(fileString);

        if(baudMatch.hasMatch()){
            one_constant=baudMatch.capturedRef().toString();
        }

        one_constant.replace(QRegExp("Cs_max[ ][\[][0-9 -]{1,}[]][ ]"),QString(""));
        ui->CS_lineEdit->setText(one_constant);

        //set parameter3 KReact
        QRegularExpression baudExpression3("kReact[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}");
        baudMatch = baudExpression3.match(fileString);

        if(baudMatch.hasMatch()){
            one_constant=baudMatch.capturedRef().toString();
        }

        one_constant.replace(QRegExp("kReact[ ][\[][0-9 -]{1,}[]][ ]"),QString(""));
        ui->KReact_lineEdit->setText(one_constant);

        //set parameter4 R
        QRegularExpression baudExpression4("R[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}");
        baudMatch = baudExpression4.match(fileString);

        if(baudMatch.hasMatch()){
            one_constant=baudMatch.capturedRef().toString();
        }

        one_constant.replace(QRegExp("R[ ][\[][0-9 -]{1,}[]][ ]"),QString(""));
        ui->R_lineEdit->setText(one_constant);

        //set parameter5 F
        QRegularExpression baudExpression5("F[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}");
        baudMatch = baudExpression5.match(fileString);

        if(baudMatch.hasMatch()){
            one_constant=baudMatch.capturedRef().toString();
        }

        one_constant.replace(QRegExp("F[ ][\[][0-9 -]{1,}[]][ ]"),QString(""));
        ui->F_lineEdit->setText(one_constant);

        //set parameter6 Ce
        QRegularExpression baudExpression6("Ce[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}");
        baudMatch = baudExpression6.match(fileString);

        if(baudMatch.hasMatch()){
            one_constant=baudMatch.capturedRef().toString();
        }

        one_constant.replace(QRegExp("Ce[ ][\[][0-9 -]{1,}[]][ ]"),QString(""));
        ui->Ce_lineEdit->setText(one_constant);

        //set parameter7 alphaA
        QRegularExpression baudExpression7("alphaA[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}");
        baudMatch = baudExpression7.match(fileString);

        if(baudMatch.hasMatch()){
            one_constant=baudMatch.capturedRef().toString();
        }

        one_constant.replace(QRegExp("alphaA[ ][\[][0-9 -]{1,}[]][ ]"),QString(""));
        ui->alphaA_lineEdit->setText(one_constant);

        //set parameter8 alphaC
        QRegularExpression baudExpression8("alphaC[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}");
        baudMatch = baudExpression8.match(fileString);

        if(baudMatch.hasMatch()){
            one_constant=baudMatch.capturedRef().toString();
        }

        one_constant.replace(QRegExp("alphaC[ ][\[][0-9 -]{1,}[]][ ]"),QString(""));
        ui->alphaC_lineEdit->setText(one_constant);

        //set parameter9 Temp
        QRegularExpression baudExpression9("T_temp[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}");
        baudMatch = baudExpression9.match(fileString);

        if(baudMatch.hasMatch()){
            one_constant=baudMatch.capturedRef().toString();
        }

        one_constant.replace(QRegExp("T_temp[ ][\[][0-9 -]{1,}[]][ ]"),QString(""));
        ui->Temp_lineEdit->setText(one_constant);

        //set parameter10 IApp
        QRegularExpression baudExpression10("I_app[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}");
        baudMatch = baudExpression10.match(fileString);

        if(baudMatch.hasMatch()){
            one_constant=baudMatch.capturedRef().toString();
        }

        one_constant.replace(QRegExp("I_app[ ][\[][0-9 -]{1,}[]][ ]"),QString(""));
        float I_float = one_constant.toFloat();
        if(I_float>=0){
            ui->select_charge->setChecked(true);
        }else{
            ui->select_discharge->setChecked(true);
        }

        float I_float2 = qAbs(I_float);
        QString IApp2 = QString::number(I_float2);
        ui->I_lineEdit->setText(IApp2);
    }
    paramFile3.close();

    //set initial value from solveSolid.H
    QFile paramFileMaterial(MainWindow::project_path+"/"+MainWindow::project_name+"/SPMFoam/solid/solveSolid.H");

    if(paramFileMaterial.open(QFile::ReadOnly | QFile::Text)){
        QTextStream fileStream(&paramFileMaterial);
        QString fileString = fileStream.readAll();


        //set initial material
        QRegularExpression baudExpression("//#include [\"]OCV_Gr.H[\"]");
        QRegularExpressionMatch baudMatch = baudExpression.match(fileString);

        if(baudMatch.hasMatch()){
            ui->select_silicon->setChecked(true);
        }else{
            ui->select_carbon->setChecked(true);
        }
    }
    paramFileMaterial.close();

    //set initial value from Cs
    QFile paramFile4(MainWindow::project_path+"/"+MainWindow::project_name+"/Case/0/solidPhase/Cs");

    if(paramFile4.open(QFile::ReadOnly | QFile::Text)){
        QTextStream fileStream(&paramFile4);
        QString fileString = fileStream.readAll();

        //set initial cs
        QRegularExpression baudExpression("internalField[ ]{1,}uniform[ ]{1,}[0-9.e-]{1,}");
        QRegularExpressionMatch baudMatch = baudExpression.match(fileString);

        QString initial_cs;
        if(baudMatch.hasMatch()){
            initial_cs=baudMatch.capturedRef().toString();
        }

        initial_cs.replace(QRegExp("internalField[ ]{1,}uniform[ ]{1,}"),QString(""));
        ui->initial_cs_lineEdit->setText(initial_cs);
    }
    paramFile4.close();

    //set fucntion interface initial value
    QFile paramFile_function(MainWindow::project_path+"/"+MainWindow::project_name+"/Case/system/fvSchemes");

    if(paramFile_function.open(QFile::ReadOnly | QFile::Text)){
        QTextStream fileStream(&paramFile_function);
        QString fileString = fileStream.readAll();

        QString one_function;
        QRegularExpressionMatch baudMatch;

        //set ddtSchemes
        QRegularExpression baudExpression("ddtSchemes[\n][{][\n][ ]{1,}default[ ]{1,}[a-zA-Z ]{1,}");
        baudMatch = baudExpression.match(fileString);

        if(baudMatch.hasMatch()){
            one_function=baudMatch.capturedRef().toString();
        }

        one_function.replace(QRegExp("ddtSchemes[\n][{][\n][ ]{1,}default[ ]{1,}"),QString(""));

        if(one_function=="Euler"){
            ui->derivative_comboBox->setCurrentIndex(0);
        }else if(one_function=="backward"){
            ui->derivative_comboBox->setCurrentIndex(1);
        }else if(one_function=="localEuler"){
            ui->derivative_comboBox->setCurrentIndex(2);
        }else if(one_function=="steadyState"){
            ui->derivative_comboBox->setCurrentIndex(3);
        }else if(one_function=="none"){
            ui->derivative_comboBox->setCurrentIndex(4);
        }

        //set gradSchemes
        QRegularExpression baudExpression2("gradSchemes[\n][{][\n][ ]{1,}default[ ]{1,}[a-zA-Z ]{1,}");
        baudMatch = baudExpression2.match(fileString);

        if(baudMatch.hasMatch()){
            one_function=baudMatch.capturedRef().toString();
        }

        one_function.replace(QRegExp("gradSchemes[\n][{][\n][ ]{1,}default[ ]{1,}"),QString(""));

        if(one_function=="Gauss linear"){
            ui->gardient_comboBox->setCurrentIndex(0);
        }else if(one_function=="Gauss cubic"){
            ui->gardient_comboBox->setCurrentIndex(1);
        }else if(one_function=="leastSquares"){
            ui->gardient_comboBox->setCurrentIndex(2);
        }else if(one_function=="none"){
            ui->gardient_comboBox->setCurrentIndex(3);
        }

        //set divSchemes
        QRegularExpression baudExpressionExtra("divSchemes[\n][{][\n][ ]{1,}default[ ]{1,}[a-zA-Z ]{1,}");
        baudMatch = baudExpressionExtra.match(fileString);

        if(baudMatch.hasMatch()){
            one_function=baudMatch.capturedRef().toString();
        }

        one_function.replace(QRegExp("divSchemes[\n][{][\n][ ]{1,}default[ ]{1,}"),QString(""));

        if(one_function=="bounded Gauss upwind"){
            ui->divergence_comboBox->setCurrentIndex(0);
        }else if(one_function=="none"){
            ui->divergence_comboBox->setCurrentIndex(1);
        }

        //set laplacianSchemes
        QRegularExpression baudExpression3("laplacianSchemes[\n][{][\n][ ]{1,}default[ ]{1,}[a-zA-Z ]{1,}");
        baudMatch = baudExpression3.match(fileString);

        if(baudMatch.hasMatch()){
            one_function=baudMatch.capturedRef().toString();
        }

        one_function.replace(QRegExp("laplacianSchemes[\n][{][\n][ ]{1,}default[ ]{1,}"),QString(""));

        if(one_function=="Gauss linear uncorrected"){
            ui->laplacian_comboBox->setCurrentIndex(0);
        }else if(one_function=="Gauss linear corrected"){
            ui->laplacian_comboBox->setCurrentIndex(1);
        }else if(one_function=="Gauss linear orthogonal"){
            ui->laplacian_comboBox->setCurrentIndex(2);
        }else if(one_function=="none"){
            ui->laplacian_comboBox->setCurrentIndex(3);
        }

        //set interpolationSchemes
        QRegularExpression baudExpression4("interpolationSchemes[\n][{][\n][ ]{1,}default[ ]{1,}[a-zA-Z ]{1,}");
        baudMatch = baudExpression4.match(fileString);

        if(baudMatch.hasMatch()){
            one_function=baudMatch.capturedRef().toString();
        }

        one_function.replace(QRegExp("interpolationSchemes[\n][{][\n][ ]{1,}default[ ]{1,}"),QString(""));

        if(one_function=="linear"){
            ui->interpolation_comboBox->setCurrentIndex(0);
        }else if(one_function=="cubic"){
            ui->interpolation_comboBox->setCurrentIndex(1);
        }else if(one_function=="none"){
            ui->interpolation_comboBox->setCurrentIndex(2);
        }

    }
    paramFile_function.close();

    //set control(tolerance) interface value
    QFile paramFile5(MainWindow::project_path+"/"+MainWindow::project_name+"/Case/system/fvSolution");

    if(paramFile5.open(QFile::ReadOnly | QFile::Text)){
        QTextStream fileStream(&paramFile5);
        QString fileString = fileStream.readAll();

        //set initial tolerance
        QRegularExpression baudExpression("tolerance[ ]{1,}[0-9e.-]{1,}");
        QRegularExpressionMatch baudMatch = baudExpression.match(fileString);

        QString tolerance;
        if(baudMatch.hasMatch()){
            tolerance=baudMatch.capturedRef().toString();
        }

        tolerance.replace(QRegExp("tolerance[ ]{1,}"),QString(""));
        ui->tolerance_lineEdit->setText(tolerance);
    }
    paramFile5.close();

    //set control(others) interface value
    QFile paramFile6(MainWindow::project_path+"/"+MainWindow::project_name+"/Case/system/controlDict");

    if(paramFile6.open(QFile::ReadOnly | QFile::Text)){
        QTextStream fileStream(&paramFile6);
        QString fileString = fileStream.readAll();

        //set initial endtime
        QRegularExpression baudExpression("endTime[ ]{1,}[0-9.e-]{1,}");
        QRegularExpressionMatch baudMatch = baudExpression.match(fileString);

        QString endtime;
        if(baudMatch.hasMatch()){
            endtime=baudMatch.capturedRef().toString();
        }

        endtime.replace(QRegExp("endTime[ ]{1,}"),QString(""));
        ui->endtime_lineEdit->setText(endtime);

        //set initial delta
        QRegularExpression baudExpression2("deltaT[ ]{1,}[0-9.e-]{1,}");
        QRegularExpressionMatch baudMatch2 = baudExpression2.match(fileString);

        QString timestep;
        if(baudMatch2.hasMatch()){
            timestep=baudMatch2.capturedRef().toString();
        }

        timestep.replace(QRegExp("deltaT[ ]{1,}"),QString(""));
        ui->timestep_lineEdit->setText(timestep);

        //set initial interval
        QRegularExpression baudExpression3("writeInterval[ ]{1,}[0-9.e-]{1,}");
        QRegularExpressionMatch baudMatch3 = baudExpression3.match(fileString);

        QString interval;
        if(baudMatch3.hasMatch()){
            interval=baudMatch3.capturedRef().toString();
        }

        interval.replace(QRegExp("writeInterval[ ]{1,}"),QString(""));
        ui->interval_lineEdit->setText(interval);
    }
    paramFile6.close();

    //set terminal
    m_process_bash->start("bash");
    m_process_bash->waitForStarted();
    connect(m_process_bash,SIGNAL(readyReadStandardOutput()),this,SLOT(readBashStandardOutputInfo()));
    connect(m_process_bash,SIGNAL(readyReadStandardError()),this,SLOT(readBashStandardErrorInfo()));
}

CarbonInterface::~CarbonInterface()
{
    if(m_process_bash)
    {
        m_process_bash->terminate();
        m_process_bash->waitForFinished();
    }
    delete ui;
}

//*******************************function difinition************************//
//This function is to output normal results on textEdit
void CarbonInterface::readBashStandardOutputInfo()
{
    QByteArray cmdout = m_process_bash->readAllStandardOutput();
    if(!cmdout.isEmpty()){
        ui->terminal_output_window->append(QString::fromLocal8Bit(cmdout));
    }
    QScrollBar* scroll = ui->terminal_output_window->verticalScrollBar();
    scroll->setSliderPosition(scroll->maximum());
}

//This function is to output errors on textEdit
void CarbonInterface::readBashStandardErrorInfo()
{
    QByteArray cmdout = m_process_bash->readAllStandardError();
    if(!cmdout.isEmpty()){
        ui->terminal_output_window->append(QString::fromLocal8Bit(cmdout));
    }
    QScrollBar* scroll = ui->terminal_output_window->verticalScrollBar();
    scroll->setSliderPosition(scroll->maximum());
}

//**************************button action definition********************//
//change geometry parameters
void CarbonInterface::on_change_geometry_button_clicked()
{
    //acquire the length,width and height
    float length = ui->length_lineEdit->text().toFloat()/2;
    float width = ui->width_lineEdit->text().toFloat()/2;
    float height = ui->height_lineEdit->text().toFloat()/2;

    QString Length = QString::number(length);
    QString Width = QString::number(width);
    QString Height = QString::number(height);

    float radius = ui->radius_lineEdit->text().toFloat();

    //detect if the radius is bigger than L/W/H
    if(radius>length||radius>width||radius>height){
        QMessageBox::information(this,"Error","The radius should be smaller than the half of length & width & height");
        return;
    }

    //acquire the unit
    QString unit = ui->unit_select_box->currentText();

    //**************************for rectangle module****************************//
    QFile file(MainWindow::project_path+"/"+MainWindow::project_name+"/Case/system/blockMeshDict");
    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this,"Error","Cannot open file for Reading");
        return ;
    }

    QString str (file.readAll());
    if(str.contains("convertToMeters", Qt::CaseInsensitive))
    {
        //change the unit
        if(unit =="micrometer(um)"){
            str.replace(QRegExp("convertToMeters 1e-[0-9];[/]{2}[a-z]+"),QString("convertToMeters 1e-6;//micrometer"));
        }else if(unit =="millimeter(mm)"){;
            str.replace(QRegExp("convertToMeters 1e-[0-9];[/]{2}[a-z]+"),QString("convertToMeters 1e-3;//millimeter"));
        }else if(unit =="meter(m)"){
            str.replace(QRegExp("convertToMeters 1e-[0-9];[/]{2}[a-z]+"),QString("convertToMeters 1e-0;//meter"));
        }

        //change length,width and height
        str.replace(QRegExp("[(][-][0-9.]{1,}[ ]{1,}[-][0-9.]{1,}[ ]{1,}[-][0-9.]{1,}"),
                    QString(QString("(")+"-"+Length+" "+"-"+Width+" "+"-"+Height));

        str.replace(QRegExp("[(][-][0-9.]{1,}[ ]{1,}[0-9.]{1,}[ ]{1,}[-][0-9.]{1,}"),
                    QString(QString("(")+"-"+Length+" "+Width+" "+"-"+Height));

        str.replace(QRegExp("[(][-][0-9.]{1,}[ ]{1,}[0-9.]{1,}[ ]{1,}[0-9.]{1,}"),
                    QString(QString("(")+"-"+Length+" "+Width+" "+Height));

        str.replace(QRegExp("[(][-][0-9.]{1,}[ ]{1,}[-][0-9.]{1,}[ ]{1,}[0-9.]{1,}"),
                    QString(QString("(")+"-"+Length+" "+"-"+Width+" "+Height));

        str.replace(QRegExp("[(][0-9.]{1,}[ ]{1,}[-][0-9.]{1,}[ ]{1,}[-][0-9.]{1,}"),
                    QString(QString("(")+Length+" "+"-"+Width+" "+"-"+Height));

        str.replace(QRegExp("[(][0-9.]{1,}[ ]{1,}[0-9.]{1,}[ ]{1,}[-][0-9.]{1,}"),
                    QString(QString("(")+Length+" "+Width+" "+"-"+Height));

        str.replace(QRegExp("[ ]{4}[(][0-9.]{1,}[ ]{1,}[0-9.]{1,}[ ]{1,}[0-9.]{1,}[)]"),
                    QString("    "+QString("(")+Length+" "+Width+" "+Height+QString(")")));

        str.replace(QRegExp("[(][0-9.]{1,}[ ]{1,}[-][0-9.]{1,}[ ]{1,}[0-9.]{1,}"),
                    QString(QString("(")+Length+" "+"-"+Width+" "+Height));

        //change divition on x,y,z direction
        str.replace(QRegExp("[(][0-9]{1,}[ ][0-9]{1,}[ ][0-9]{1,}[)][ ]simpleGrading"),
                    QString(QString("(")+ui->x_divide_lineEdit->text()+" "+ui->y_divide_lineEdit->text()+
                            " "+ui->z_divide_lineEdit->text()+QString(")")+" "+"simpleGrading"));

    }
    file.close();

    QFile::remove(MainWindow::project_path+"/"+MainWindow::project_name+"/Case/system/blockMeshDict");
    if(!file.open(QIODevice::WriteOnly))
    {
         QMessageBox::warning(this,"Error","Cannot open file for Writing");
         return ;
    }

    file.write(str.toUtf8());
    file.close();

    //**************************for sphere module****************************//
    QFile file2(MainWindow::project_path+"/"+MainWindow::project_name+"/Case/system/topoSetDict");
    if(!file2.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this,"Error","Cannot open file for Reading");
        return ;
    }

    QString str2 (file2.readAll());
    if(str2.contains("radius", Qt::CaseInsensitive))
    {
        //change radius and electrolyte
        if(unit =="millimeter(mm)"){
            str2.replace(QRegExp("[0-9.]{1,}e-[0][0-9]"),QString(ui->radius_lineEdit->text()+"e-03"));
            str2.replace(QRegExp("[-][0-9.]{1,}[e][-][0-9][ ]{1,}[-][0-9.]{1,}[e][-][0-9][ ]{1,}[-][0-9.]{1,}[e][-][0-9][ ]{0,}[)]"
                                 "[(][0-9.]{1,}[e][-][0-9][ ]{1,}[0-9.]{1,}[e][-][0-9][ ]{1,}[0-9.]{1,}[e][-][0-9][ ]{0,}"),
                         QString("-"+Length+"e-3"+"   "+"-"+Width+"e-3"+"   "+"-"+Height+"e-3"
                        +")("+Length+"e-3"+"   "+Width+"e-3"+"   "+Height+"e-3"));
        }else if(unit =="micrometer(um)"){
            str2.replace(QRegExp("[0-9.]{1,}e-[0][0-9]"),QString(ui->radius_lineEdit->text()+"e-06"));
            str2.replace(QRegExp("[-][0-9.]{1,}[e][-][0-9][ ]{1,}[-][0-9.]{1,}[e][-][0-9][ ]{1,}[-][0-9.]{1,}[e][-][0-9][ ]{0,}[)]"
                                 "[(][0-9.]{1,}[e][-][0-9][ ]{1,}[0-9.]{1,}[e][-][0-9][ ]{1,}[0-9.]{1,}[e][-][0-9][ ]{0,}"),
                         QString("-"+Length+"e-6"+"   "+"-"+Width+"e-6"+"   "+"-"+Height+"e-6"
                        +")("+Length+"e-6"+"   "+Width+"e-6"+"   "+Height+"e-6"));
        }else if(unit =="meter(m)"){
            str2.replace(QRegExp("[0-9.]{1,}e-[0][0-9]"),QString(ui->radius_lineEdit->text()+"e-00"));
            str2.replace(QRegExp("[-][0-9.]{1,}[e][-][0-9][ ]{1,}[-][0-9.]{1,}[e][-][0-9][ ]{1,}[-][0-9.]{1,}[e][-][0-9][ ]{0,}[)]"
                                 "[(][0-9.]{1,}[e][-][0-9][ ]{1,}[0-9.]{1,}[e][-][0-9][ ]{1,}[0-9.]{1,}[e][-][0-9][ ]{0,}"),
                         QString("-"+Length+"e-0"+"   "+"-"+Width+"e-0"+"   "+"-"+Height+"e-0"
                        +")("+Length+"e-0"+"   "+Width+"e-0"+"   "+Height+"e-0"));
        }

    }
    file2.close();

    QFile::remove(MainWindow::project_path+"/"+MainWindow::project_name+"/Case/system/topoSetDict");
    if(!file2.open(QIODevice::WriteOnly))
    {
         QMessageBox::warning(this,"Error","Cannot open file for Writing");
         return ;
    }

    file2.write(str2.toUtf8());
    file2.close();

    //stop the program for 2 seconds
    QEventLoop eventloop;
    QTimer::singleShot(2000, &eventloop, SLOT(quit()));
    eventloop.exec();

    //generate relative files
    QString path =MainWindow::project_path+"/"+MainWindow::project_name+"/"+"Case";

    ui->command_input_lineEdit->setText("cd "+path+" && blockMesh");
    m_process_bash->write(ui->command_input_lineEdit->text().toLocal8Bit()+'\n');
    ui->command_input_lineEdit->clear();


    ui->command_input_lineEdit->setText("topoSet");
    m_process_bash->write(ui->command_input_lineEdit->text().toLocal8Bit()+'\n');
    ui->command_input_lineEdit->clear();

    ui->command_input_lineEdit->setText("splitMeshRegions -cellZones -overwrite");
    m_process_bash->write(ui->command_input_lineEdit->text().toLocal8Bit()+'\n');
    ui->command_input_lineEdit->clear();

    ui->command_input_lineEdit->setText("paraFoam -touchAll");
    m_process_bash->write(ui->command_input_lineEdit->text().toLocal8Bit()+'\n');
    ui->command_input_lineEdit->clear();

    QMessageBox::information(this,"hint","Modify successfully");
}

//from geometry interface to next page
void CarbonInterface::on_run_geometry_button_clicked()
{
    ui->tabWidget->setCurrentIndex(1);
}

//change constant parameters
void CarbonInterface::on_change_constant_button_clicked()
{
    QFile file(MainWindow::project_path+"/"+MainWindow::project_name+"/Case/constant/LiProperties");
    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this,"Error","Cannot open file for Reading");
        return ;
    }

    QString str (file.readAll());
    if(str.contains("transportProperties", Qt::CaseInsensitive)){
        str.replace(QRegExp("Ds_value[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}"),
                    QString("Ds_value [0 -1 0 0 0 0 0] "+ui->DS_lineEdit->text()));

        str.replace(QRegExp("Cs_max[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}"),
                    QString("Cs_max [0 -1 0 0 0 0 0] "+ui->CS_lineEdit->text()));

        str.replace(QRegExp("kReact[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}"),
                    QString("kReact [0 -1 0 0 0 0 0] "+ui->KReact_lineEdit->text()));

        str.replace(QRegExp("R[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}"),
                    QString("R [0 -1 0 0 0 0 0] "+ui->R_lineEdit->text()));

        str.replace(QRegExp("F[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}"),
                    QString("F [0 -1 0 0 0 0 0] "+ui->F_lineEdit->text()));

        str.replace(QRegExp("Ce[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}"),
                    QString("Ce [0 -1 0 0 0 0 0] "+ui->Ce_lineEdit->text()));

        str.replace(QRegExp("alphaA[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}"),
                    QString("alphaA [0 -1 0 0 0 0 0] "+ui->alphaA_lineEdit->text()));

        str.replace(QRegExp("alphaC[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}"),
                    QString("alphaC [0 -1 0 0 0 0 0] "+ui->alphaC_lineEdit->text()));

        str.replace(QRegExp("T_temp[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}"),
                    QString("T_temp [0 -1 0 0 0 0 0] "+ui->Temp_lineEdit->text()));

        if(ui->select_charge->isChecked()){
            str.replace(QRegExp("I_app[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}"),
                        QString("I_app [0 -1 0 0 0 0 0] "+ui->I_lineEdit->text()));
        }else if(ui->select_discharge->isChecked()){
            str.replace(QRegExp("I_app[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}"),
                        QString("I_app [0 -1 0 0 0 0 0] "+QString("-")+ui->I_lineEdit->text()));
        }

    }
    file.close();

    QFile::remove(MainWindow::project_path+"/"+MainWindow::project_name+"/Case/constant/LiProperties");
    if(!file.open(QIODevice::WriteOnly))
    {
         QMessageBox::warning(this,"Error","Cannot open file for Writing");
         return ;
    }

    file.write(str.toUtf8());
    file.close();

    //change the file in ele and solidphase folder
    if (QFile::exists(MainWindow::project_path+"/"+MainWindow::project_name+"/Case/constant/ele/LiProperties"))
    {
        QFile::remove(MainWindow::project_path+"/"+MainWindow::project_name+"/Case/constant/ele/LiProperties");
    }

    QFile::copy(MainWindow::project_path+"/"+MainWindow::project_name+"/Case/constant/LiProperties",
                MainWindow::project_path+"/"+MainWindow::project_name+"/Case/constant/ele/LiProperties");

    if (QFile::exists(MainWindow::project_path+"/"+MainWindow::project_name+"/Case/constant/solidPhase/LiProperties"))
    {
        QFile::remove(MainWindow::project_path+"/"+MainWindow::project_name+"/Case/constant/solidPhase/LiProperties");
    }

    QFile::copy(MainWindow::project_path+"/"+MainWindow::project_name+"/Case/constant/LiProperties",
                MainWindow::project_path+"/"+MainWindow::project_name+"/Case/constant/solidPhase/LiProperties");


    //Material selection part
    QFile fileMaterial(MainWindow::project_path+"/"+MainWindow::project_name+"/SPMFoam/solid/solveSolid.H");
    if(!fileMaterial.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this,"Error","Cannot open file for Reading");
        return ;
    }

    QString strMaterial (fileMaterial.readAll());
    if(strMaterial.contains("correctNonOrthogonal", Qt::CaseInsensitive)){
        QRegularExpression baudExpression1("//#include [\"]OCV_Gr.H[\"]");
        QRegularExpressionMatch baudMatch1 = baudExpression1.match(strMaterial);
        bool alreadyCommitGr= baudMatch1.hasMatch();

        QRegularExpression baudExpression2("//#include [\"]OCV_Si.H[\"]");
        QRegularExpressionMatch baudMatch2 = baudExpression2.match(strMaterial);
        bool alreadyCommitSi= baudMatch2.hasMatch();

        if(ui->select_carbon->isChecked()){
            strMaterial.replace(QRegExp("//#include [\"]OCV_Gr.H[\"]"),
                        QString("#include \"OCV_Gr.H\""));
            if(!alreadyCommitSi){
                strMaterial.replace(QRegExp("#include [\"]OCV_Si.H[\"]"),
                            QString("//#include \"OCV_Si.H\""));
            }
        }else{
            if(!alreadyCommitGr){
                strMaterial.replace(QRegExp("#include [\"]OCV_Gr.H[\"]"),
                            QString("//#include \"OCV_Gr.H\""));
            }
            strMaterial.replace(QRegExp("//#include [\"]OCV_Si.H[\"]"),
                        QString("#include \"OCV_Si.H\""));
        }
    }
    fileMaterial.close();

    QFile::remove(MainWindow::project_path+"/"+MainWindow::project_name+"/SPMFoam/solid/solveSolid.H");
    if(!fileMaterial.open(QIODevice::WriteOnly))
    {
         QMessageBox::warning(this,"Error","Cannot open file for Writing");
         return ;
    }

    fileMaterial.write(strMaterial.toUtf8());
    fileMaterial.close();


    QMessageBox::information(this,"title","Change successfully");
}

// constant help button
void CarbonInterface::on_help_constant_button_clicked()
{
    QMessageBox::information(this,"hint","DS value \t\tLi Intrinsic diffusivity in material\n"
                                         "CS max \t\tMaximum Li concentration in material\n"
                                         "kreact  \t\tReaction rate constant\n"
                                         "R \t\tUniversal gas constant\n"
                                         "F \t\tFaraday's constant\n"
                                         "Ce \t\tTransfer coefficien\n"
                                         "alphaA \t\tAnodic\n"
                                         "alphaC \t\tCathodic\n"
                                         "T \t\tTemperature\n"
                                         "Iapp  \t\tApplied current density\n");
}

//from constant interface to next page
void CarbonInterface::on_run_constant_button_clicked()
{
    ui->tabWidget->setCurrentIndex(2);
}

//change boundary parameters
void CarbonInterface::on_change_boundary_button_clicked()
{
    QFile file(MainWindow::project_path+"/"+MainWindow::project_name+"/Case/0/solidPhase/Cs");
    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this,"Error","Cannot open file for Reading");
        return ;
    }

    QString str (file.readAll());
    if(str.contains("volScalarField", Qt::CaseInsensitive)){
        str.replace(QRegExp("internalField[ ]{1,}uniform[ ]{1,}[0-9.e-]{1,}"),
                    QString("internalField   uniform   "+ui->initial_cs_lineEdit->text()));
    }
    file.close();

    QFile::remove(MainWindow::project_path+"/"+MainWindow::project_name+"/Case/0/solidPhase/Cs");
    if(!file.open(QIODevice::WriteOnly))
    {
         QMessageBox::warning(this,"Error","Cannot open file for Writing");
         return ;
    }

    file.write(str.toUtf8());
    file.close();

    QMessageBox::information(this,"title","Change successfully");
}

//from boundary interface to next page
void CarbonInterface::on_run_boundary_button_clicked()
{
    ui->tabWidget->setCurrentIndex(3);
}

//change function parameters
void CarbonInterface::on_change_function_button_clicked()
{
    QFile file(MainWindow::project_path+"/"+MainWindow::project_name+"/Case/system/fvSchemes");
    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this,"Error","Cannot open file for Reading");
        return ;
    }

    QString str (file.readAll());
    if(str.contains("fvSchemes", Qt::CaseInsensitive)){
        str.replace(QRegExp("ddtSchemes[\n][{][\n][ ]{1,}default[ ]{1,}[a-zA-Z ]{1,}"),
                    QString("ddtSchemes\n{\n    default         "+ui->derivative_comboBox->currentText()));
        str.replace(QRegExp("gradSchemes[\n][{][\n][ ]{1,}default[ ]{1,}[a-zA-Z ]{1,}"),
                    QString("gradSchemes\n{\n    default         "+ui->gardient_comboBox->currentText()));
        str.replace(QRegExp("divSchemes[\n][{][\n][ ]{1,}default[ ]{1,}[a-zA-Z ]{1,}"),
                    QString("divSchemes\n{\n    default         "+ui->divergence_comboBox->currentText()));
        str.replace(QRegExp("laplacianSchemes[\n][{][\n][ ]{1,}default[ ]{1,}[a-zA-Z ]{1,}"),
                    QString("laplacianSchemes\n{\n    default         "+ui->laplacian_comboBox->currentText()));
        str.replace(QRegExp("interpolationSchemes[\n][{][\n][ ]{1,}default[ ]{1,}[a-zA-Z ]{1,}"),
                    QString("interpolationSchemes\n{\n    default         "+ui->interpolation_comboBox->currentText()));
    }
    file.close();

    QFile::remove(MainWindow::project_path+"/"+MainWindow::project_name+"/Case/system/fvSchemes");
    if(!file.open(QIODevice::WriteOnly))
    {
         QMessageBox::warning(this,"Error","Cannot open file for Writing");
         return ;
    }

    file.write(str.toUtf8());
    file.close();

    //change the file in ele and solidphase folder
    if (QFile::exists(MainWindow::project_path+"/"+MainWindow::project_name+"/Case/system/ele/fvSchemes"))
    {
        QFile::remove(MainWindow::project_path+"/"+MainWindow::project_name+"/Case/system/ele/fvSchemes");
    }

    QFile::copy(MainWindow::project_path+"/"+MainWindow::project_name+"/Case/system/fvSchemes",
                MainWindow::project_path+"/"+MainWindow::project_name+"/Case/system/ele/fvSchemes");

    if (QFile::exists(MainWindow::project_path+"/"+MainWindow::project_name+"/Case/system/solidPhase/fvSchemes"))
    {
        QFile::remove(MainWindow::project_path+"/"+MainWindow::project_name+"/Case/system/solidPhase/fvSchemes");
    }

    QFile::copy(MainWindow::project_path+"/"+MainWindow::project_name+"/Case/system/fvSchemes",
                MainWindow::project_path+"/"+MainWindow::project_name+"/Case/system/solidPhase/fvSchemes");

    QMessageBox::information(this,"title","Change successfully");


}

//from function interface to next page
void CarbonInterface::on_run_function_button_clicked()
{
    ui->tabWidget->setCurrentIndex(4);
}

//change tolerance and other control parameters
void CarbonInterface::on_change_control_button_clicked()
{
    QFile file(MainWindow::project_path+"/"+MainWindow::project_name+"/Case/system/fvSolution");
    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this,"Error","Cannot open file for Reading");
        return ;
    }

    QString str (file.readAll());
    if(str.contains("fvSolution", Qt::CaseInsensitive)){
        str.replace(QRegExp("tolerance[ ]{1,}[0-9e.-]{1,}"),
                    QString("tolerance       "+ui->tolerance_lineEdit->text()));
    }
    file.close();

    QFile::remove(MainWindow::project_path+"/"+MainWindow::project_name+"/Case/system/fvSolution");
    if(!file.open(QIODevice::WriteOnly))
    {
         QMessageBox::warning(this,"Error","Cannot open file for Writing");
         return ;
    }

    file.write(str.toUtf8());
    file.close();


    //change fvsolution file in ele and solidphase folder
    if (QFile::exists(MainWindow::project_path+"/"+MainWindow::project_name+"/Case/system/ele/fvSolution"))
    {
        QFile::remove(MainWindow::project_path+"/"+MainWindow::project_name+"/Case/system/ele/fvSolution");
    }

    QFile::copy(MainWindow::project_path+"/"+MainWindow::project_name+"/Case/system/fvSolution",
                MainWindow::project_path+"/"+MainWindow::project_name+"/Case/system/ele/fvSolution");

    if (QFile::exists(MainWindow::project_path+"/"+MainWindow::project_name+"/Case/system/solidPhase/fvSolution"))
    {
        QFile::remove(MainWindow::project_path+"/"+MainWindow::project_name+"/Case/system/solidPhase/fvSolution");
    }

    QFile::copy(MainWindow::project_path+"/"+MainWindow::project_name+"/Case/system/fvSolution",
                MainWindow::project_path+"/"+MainWindow::project_name+"/Case/system/solidPhase/fvSolution");


    //change control parameters
    QFile file2(MainWindow::project_path+"/"+MainWindow::project_name+"/Case/system/controlDict");
    if(!file2.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this,"Error","Cannot open file for Reading");
        return ;
    }

    QString str2 (file2.readAll());
    if(str2.contains("controlDict", Qt::CaseInsensitive)){
        str2.replace(QRegExp("endTime[ ]{1,}[0-9.e-]{1,}"),
                    QString("endTime         "+ui->endtime_lineEdit->text()));
        str2.replace(QRegExp("deltaT[ ]{1,}[0-9.e-]{1,}"),
                    QString("deltaT          "+ui->timestep_lineEdit->text()));
        str2.replace(QRegExp("writeInterval[ ]{1,}[0-9.e-]{1,}"),
                    QString("writeInterval   "+ui->interval_lineEdit->text()));
    }
    file2.close();

    QFile::remove(MainWindow::project_path+"/"+MainWindow::project_name+"/Case/system/controlDict");
    if(!file2.open(QIODevice::WriteOnly))
    {
         QMessageBox::warning(this,"Error","Cannot open file for Writing");
         return ;
    }

    file2.write(str2.toUtf8());
    file2.close();

    QMessageBox::information(this,"title","Change successfully");
    delete_flag=1;
}

//run the program button
void CarbonInterface::on_run_button_clicked()
{
    if(delete_flag==1){
        //remove folders
        QString folderPath=MainWindow::project_path+"/"+MainWindow::project_name+"/Case";

        QDir dir(folderPath);
        QStringList names = dir.entryList(QDir::Dirs);
        QDir dir2;

        names.removeOne(".");
        names.removeOne("..");

        QStringList valid_list;
        valid_list<<"0"<<"constant"<<"system";

        for(QString s: names){
            if(!valid_list.contains(s) && QDir(folderPath+"/"+s).exists()){
                dir2.setPath(folderPath+"/"+s);
                dir2.removeRecursively();
            }
        }

        //remove file(s)
        QFileInfo file(MainWindow::project_path+"/"+MainWindow::project_name+"/Case/time_voltage");
        if(file.exists()){
            QFile::remove(MainWindow::project_path+"/"+MainWindow::project_name+"/Case/time_voltage");
        }

        QEventLoop eventloop2;
        QTimer::singleShot(1000, &eventloop2, SLOT(quit()));
        eventloop2.exec();

        //wclean and wmake
        QString path =MainWindow::project_path+"/"+MainWindow::project_name+"/"+"SPMFoam";

        ui->command_input_lineEdit->setText("cd /"+path+" && wclean");
        m_process_bash->write(ui->command_input_lineEdit->text().toLocal8Bit()+'\n');
        ui->command_input_lineEdit->clear();

        ui->command_input_lineEdit->setText("wmake");
        m_process_bash->write(ui->command_input_lineEdit->text().toLocal8Bit()+'\n');
        ui->command_input_lineEdit->clear();

        QEventLoop eventloop;
        QTimer::singleShot(2000, &eventloop, SLOT(quit()));
        eventloop.exec();

    }

    QString path2 =MainWindow::project_path+"/"+MainWindow::project_name+"/"+"Case";

    ui->command_input_lineEdit->setText("cd /"+path2+" && "+MainWindow::project_name);
    m_process_bash->write(ui->command_input_lineEdit->text().toLocal8Bit()+'\n');
    ui->command_input_lineEdit->clear();

    ui->view_result_button->setEnabled(true);
    delete_flag=1;

}

//pause the run process
void CarbonInterface::on_pause_run_button_clicked()
{
    //stop the program
    QString str ="pkill -f "+QString(MainWindow::project_name);
    QByteArray byteArray = str.toUtf8();
    char* data = byteArray.data();

    system(data);

    delete_flag=0;
}

//go back to the mainwindow
void CarbonInterface::on_c_back_Button_clicked()
{
    emit ExitWin();
    close();
}

//command test button
void CarbonInterface::on_pushButton_clicked()
{
    QString strCmd = ui->command_input_lineEdit->text();
    ui->terminal_output_window->append("Linux:~$ "+strCmd);
    m_process_bash->write(ui->command_input_lineEdit->text().toLocal8Bit()+'\n');
    ui->command_input_lineEdit->clear();
}

//paraview
void CarbonInterface::on_open_paraview_Button_clicked()
{
    QString path =MainWindow::project_path+"/"+MainWindow::project_name+"/"+"Case";

    ui->command_input_lineEdit->setText("cd /"+path+" && "+"paraview &");
    ui->terminal_output_window->append("Linux:~$ paraview &");
    m_process_bash->write(ui->command_input_lineEdit->text().toLocal8Bit()+'\n');
    ui->command_input_lineEdit->clear();
}

//view the results
void CarbonInterface::on_view_result_button_clicked()
{
    resultInterface = new ResultInterface(this);
    resultInterface->show();
}
