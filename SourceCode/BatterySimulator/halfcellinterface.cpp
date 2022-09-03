#include "halfcellinterface.h"
#include "ui_halfcellinterface.h"
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

HalfCellInterface::HalfCellInterface(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HalfCellInterface)
{
    ui->setupUi(this);

    //set window title
    QWidget::setWindowTitle ( "BatteryFoam" );

    //set maximum screen length
    ui->terminal_output_window->document()->setMaximumBlockCount(1000);

    //set view button is available if time_voltage exists
    QFile t_v_file(MainWindow::project_path+"/"+MainWindow::project_name+"/CC/time_voltage");
    if(t_v_file.exists()){
        ui->view_result_button->setEnabled(true);
    }


    //set initial value from blockMeshDict
    QFile paramFile(MainWindow::project_path+"/"+MainWindow::project_name+"/CC/system/blockMeshDict");

    if(paramFile.open(QFile::ReadOnly | QFile::Text)){
        QTextStream fileStream(&paramFile);
        QString fileString = fileStream.readAll();

        //set length,width and height
        QRegularExpression baudExpression("[0-9.]{1,}[ ][0-9.]{1,}[ ][0-9.]{1,}[)][/]{2}SS node3");
        QRegularExpressionMatch baudMatch = baudExpression.match(fileString);

        QString lengthWidthHeight;
        if(baudMatch.hasMatch()){
            lengthWidthHeight=baudMatch.capturedRef().toString();
        }

        QString length_original = lengthWidthHeight;
        length_original.replace(QRegExp("[ ][0-9.]{1,}[ ][0-9.]{1,}[)][/]{2}SS node3"),QString(""));
        float temp_length = length_original.toFloat();
        ui->length_lineEdit->setText(length_original);

        QString width_original = lengthWidthHeight;
        width_original.replace(QRegExp("[ ][0-9.]{1,}[)][/]{2}SS node3"),QString(""));
        width_original.replace(QRegExp("[0-9.]{1,}[ ]"),QString(""));
        ui->width_lineEdit->setText(width_original);

        QString height_original = lengthWidthHeight;
        height_original.replace(QRegExp("[)][/]{2}SS node3"),QString(""));
        height_original.replace(QRegExp("[0-9.]{1,}[ ][0-9.]{1,}[ ]"),QString(""));
        ui->height_lineEdit->setText(height_original);

        //set length2
        QRegularExpression baudExpression2("[0-9.]{1,}[ ][0-9.]{1,}[ ][0-9.]{1,}[)][/]{2}SR node3");
        QRegularExpressionMatch baudMatch2 = baudExpression2.match(fileString);

        QString length2Self;
        if(baudMatch2.hasMatch()){
            length2Self=baudMatch2.capturedRef().toString();
        }

        length2Self.replace(QRegExp("[ ][0-9.]{1,}[ ][0-9.]{1,}[)][/]{2}SR node3"),QString(""));
        float temp_length2 = length2Self.toFloat();
        QString length2_original = QString::number(temp_length2-temp_length);
        ui->length2_lineEdit->setText(length2_original);

        //set divition on x,y,z
        QRegularExpression baudExpression3("[0-9]{1,}[ ][0-9]{1,}[ ][0-9]{1,}[)][ ]simpleGrading[ ][(1 )]{1,}[/]{2}anode");
        QRegularExpressionMatch baudMatch3 = baudExpression3.match(fileString);

        QString xyz_divition;
        if(baudMatch3.hasMatch()){
            xyz_divition=baudMatch3.capturedRef().toString();
        }

        QString length_divition = xyz_divition;
        length_divition.replace(QRegExp("[ ][0-9]{1,}[ ][0-9]{1,}[)][ ]simpleGrading[ ][(1 )]{1,}[/]{2}anode"),QString(""));
        ui->x_divide_lineEdit->setText(length_divition);

        QString width_divition = xyz_divition;
        width_divition.replace(QRegExp("[ ][0-9]{1,}[)][ ]simpleGrading[ ][(1 )]{1,}[/]{2}anode"),QString(""));
        width_divition.replace(QRegExp("[0-9]{1,}[ ]"),QString(""));
        ui->y_divide_lineEdit->setText(width_divition);

        QString height_divition = xyz_divition;
        height_divition.replace(QRegExp("[)][ ]simpleGrading[ ][(1 )]{1,}[/]{2}anode"),QString(""));
        height_divition.replace(QRegExp("[0-9]{1,}[ ][0-9]{1,}[ ]"),QString(""));
        ui->z_divide_lineEdit->setText(height_divition);

        //set divition on x2
        QRegularExpression baudExpression4("[0-9]{1,}[ ][0-9]{1,}[ ][0-9]{1,}[)][ ]simpleGrading[ ][(1 )]{1,}[/]{2}separator");
        QRegularExpressionMatch baudMatch4 = baudExpression4.match(fileString);

        QString x2_divition;
        if(baudMatch4.hasMatch()){
            x2_divition=baudMatch4.capturedRef().toString();
        }

        x2_divition.replace(QRegExp("[ ][0-9]{1,}[ ][0-9]{1,}[)][ ]simpleGrading[ ][(1 )]{1,}[/]{2}separator"),QString(""));
        ui->x2_divide_lineEdit->setText(x2_divition);

        //set current unit
        QRegularExpression baudExpression5("convertToMeters[ ]{1,}[1][e][-][0-9]");
        QRegularExpressionMatch baudMatch5 = baudExpression5.match(fileString);

        QString initial_unit;
        if(baudMatch5.hasMatch()){
            initial_unit=baudMatch5.capturedRef().toString();
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

    //set initial value from LiProperties
    QFile paramFile2(MainWindow::project_path+"/"+MainWindow::project_name+"/CC/constant/LiProperties");

    if(paramFile2.open(QFile::ReadOnly | QFile::Text)){
        QTextStream fileStream(&paramFile2);
        QString fileString = fileStream.readAll();

        //por_Ce_WE
        QRegularExpression baudExpression("por_Ce_WE[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}");
        QRegularExpressionMatch baudMatch = baudExpression.match(fileString);

        QString por_Ce_WE;
        if(baudMatch.hasMatch()){
            por_Ce_WE=baudMatch.capturedRef().toString();
        }

        por_Ce_WE.replace(QRegExp("por_Ce_WE[ ][\[][0-9 -]{1,}[]][ ]"),QString(""));
        ui->CeWE_lineEdit->setText(por_Ce_WE);

        //por_Ce_sp
        QRegularExpression baudExpression2("por_Ce_sp[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}");
        QRegularExpressionMatch baudMatch2 = baudExpression2.match(fileString);

        QString por_Ce_sp;
        if(baudMatch2.hasMatch()){
            por_Ce_sp=baudMatch2.capturedRef().toString();
        }

        por_Ce_sp.replace(QRegExp("por_Ce_sp[ ][\[][0-9 -]{1,}[]][ ]"),QString(""));
        ui->Cesp_lineEdit->setText(por_Ce_sp);

        //por_f_WE
        QRegularExpression baudExpression3("por_f_WE[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}");
        QRegularExpressionMatch baudMatch3 = baudExpression3.match(fileString);

        QString por_f_WE;
        if(baudMatch3.hasMatch()){
            por_f_WE=baudMatch3.capturedRef().toString();
        }

        por_f_WE.replace(QRegExp("por_f_WE[ ][\[][0-9 -]{1,}[]][ ]"),QString(""));
        ui->fWE_lineEdit->setText(por_f_WE);

        //Cs_max_WE
        QRegularExpression baudExpression4("Cs_max_WE[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}");
        QRegularExpressionMatch baudMatch4 = baudExpression4.match(fileString);

        QString Cs_max_WE;
        if(baudMatch4.hasMatch()){
            Cs_max_WE=baudMatch4.capturedRef().toString();
        }

        Cs_max_WE.replace(QRegExp("Cs_max_WE[ ][\[][0-9 -]{1,}[]][ ]"),QString(""));
        ui->maxWE_lineEdit->setText(Cs_max_WE);

        //brugg
        QRegularExpression baudExpression5("brugg[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}");
        QRegularExpressionMatch baudMatch5 = baudExpression5.match(fileString);

        QString brugg;
        if(baudMatch5.hasMatch()){
            brugg=baudMatch5.capturedRef().toString();
        }

        brugg.replace(QRegExp("brugg[ ][\[][0-9 -]{1,}[]][ ]"),QString(""));
        ui->brugg_lineEdit->setText(brugg);

        //R
        QRegularExpression baudExpression6("R[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}");
        QRegularExpressionMatch baudMatch6 = baudExpression6.match(fileString);

        QString R;
        if(baudMatch6.hasMatch()){
            R=baudMatch6.capturedRef().toString();
        }

        R.replace(QRegExp("R[ ][\[][0-9 -]{1,}[]][ ]"),QString(""));
        ui->R_lineEdit->setText(R);

        //F
        QRegularExpression baudExpression7("F[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}");
        QRegularExpressionMatch baudMatch7 = baudExpression7.match(fileString);

        QString F;
        if(baudMatch7.hasMatch()){
            F=baudMatch7.capturedRef().toString();
        }

        F.replace(QRegExp("F[ ][\[][0-9 -]{1,}[]][ ]"),QString(""));
        ui->F_lineEdit->setText(F);

        //tNo
        QRegularExpression baudExpression8("tNo[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}");
        QRegularExpressionMatch baudMatch8 = baudExpression8.match(fileString);

        QString tNo;
        if(baudMatch8.hasMatch()){
            tNo=baudMatch8.capturedRef().toString();
        }

        tNo.replace(QRegExp("tNo[ ][\[][0-9 -]{1,}[]][ ]"),QString(""));
        ui->tNo_lineEdit->setText(tNo);

        //k0_fai_s_WE
        QRegularExpression baudExpression9("k0_fai_s_WE[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}");
        QRegularExpressionMatch baudMatch9 = baudExpression9.match(fileString);

        QString k0_fai_s_WE;
        if(baudMatch9.hasMatch()){
            k0_fai_s_WE=baudMatch9.capturedRef().toString();
        }

        k0_fai_s_WE.replace(QRegExp("k0_fai_s_WE[ ][\[][0-9 -]{1,}[]][ ]"),QString(""));
        ui->faisWE_lineEdit->setText(k0_fai_s_WE);

        //D0_Ce
        QRegularExpression baudExpression10("D0_Ce[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}");
        QRegularExpressionMatch baudMatch10 = baudExpression10.match(fileString);

        QString D0_Ce;
        if(baudMatch10.hasMatch()){
            D0_Ce=baudMatch10.capturedRef().toString();
        }

        D0_Ce.replace(QRegExp("D0_Ce[ ][\[][0-9 -]{1,}[]][ ]"),QString(""));
        ui->D0Ce_lineEdit->setText(D0_Ce);

        //r_s
        QRegularExpression baudExpression11("r_s[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}");
        QRegularExpressionMatch baudMatch11 = baudExpression11.match(fileString);

        QString r_s;
        if(baudMatch11.hasMatch()){
            r_s=baudMatch11.capturedRef().toString();
        }

        r_s.replace(QRegExp("r_s[ ][\[][0-9 -]{1,}[]][ ]"),QString(""));
        ui->rs_lineEdit->setText(r_s);

        //Ds_WE
        QRegularExpression baudExpression12("Ds_WE[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}");
        QRegularExpressionMatch baudMatch12 = baudExpression12.match(fileString);

        QString Ds_WE;
        if(baudMatch12.hasMatch()){
            Ds_WE=baudMatch12.capturedRef().toString();
        }

        Ds_WE.replace(QRegExp("Ds_WE[ ][\[][0-9 -]{1,}[]][ ]"),QString(""));
        ui->DsWE_lineEdit->setText(Ds_WE);

        //kReact_WE
        QRegularExpression baudExpression13("kReact_WE[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}");
        QRegularExpressionMatch baudMatch13 = baudExpression13.match(fileString);

        QString kReact_WE;
        if(baudMatch13.hasMatch()){
            kReact_WE=baudMatch13.capturedRef().toString();
        }

        kReact_WE.replace(QRegExp("kReact_WE[ ][\[][0-9 -]{1,}[]][ ]"),QString(""));
        ui->KReact_lineEdit->setText(kReact_WE);

        //kReact_RE
        QRegularExpression baudExpressionRE("kReact_RE[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}");
        QRegularExpressionMatch baudMatchRE = baudExpressionRE.match(fileString);

        QString kReact_RE;
        if(baudMatchRE.hasMatch()){
            kReact_RE=baudMatchRE.capturedRef().toString();
        }

        kReact_RE.replace(QRegExp("kReact_RE[ ][\[][0-9 -]{1,}[]][ ]"),QString(""));
        ui->KReactRE_lineEdit->setText(kReact_RE);

        //alphaA_WE
        QRegularExpression baudExpression14("alphaA_WE[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}");
        QRegularExpressionMatch baudMatch14 = baudExpression14.match(fileString);

        QString alphaA_WE;
        if(baudMatch14.hasMatch()){
            alphaA_WE=baudMatch14.capturedRef().toString();
        }

        alphaA_WE.replace(QRegExp("alphaA_WE[ ][\[][0-9 -]{1,}[]][ ]"),QString(""));
        ui->alphaAWE_lineEdit->setText(alphaA_WE);

        //alphaC_WE
        QRegularExpression baudExpression15("alphaC_WE[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}");
        QRegularExpressionMatch baudMatch15 = baudExpression15.match(fileString);

        QString alphaC_WE;
        if(baudMatch15.hasMatch()){
            alphaC_WE=baudMatch15.capturedRef().toString();
        }

        alphaC_WE.replace(QRegExp("alphaC_WE[ ][\[][0-9 -]{1,}[]][ ]"),QString(""));
        ui->alphaCWE_lineEdit->setText(alphaC_WE);

        //alphaA_RE
        QRegularExpression baudExpressionARE("alphaA_RE[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}");
        QRegularExpressionMatch baudMatchARE = baudExpressionARE.match(fileString);

        QString alphaA_RE;
        if(baudMatchARE.hasMatch()){
            alphaA_RE=baudMatchARE.capturedRef().toString();
        }

        alphaA_RE.replace(QRegExp("alphaA_RE[ ][\[][0-9 -]{1,}[]][ ]"),QString(""));
        ui->alphaARE_lineEdit->setText(alphaA_RE);

        //alphaC_RE
        QRegularExpression baudExpressionCRE("alphaC_RE[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}");
        QRegularExpressionMatch baudMatchCRE = baudExpressionCRE.match(fileString);

        QString alphaC_RE;
        if(baudMatchCRE.hasMatch()){
            alphaC_RE=baudMatchCRE.capturedRef().toString();
        }

        alphaC_RE.replace(QRegExp("alphaC_RE[ ][\[][0-9 -]{1,}[]][ ]"),QString(""));
        ui->alphaCRE_lineEdit->setText(alphaC_RE);


        //IApp
        QRegularExpression baudExpression16("I_app[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}");
        QRegularExpressionMatch baudMatch16 = baudExpression16.match(fileString);

        QString IApp;
        if(baudMatch16.hasMatch()){
            IApp=baudMatch16.capturedRef().toString();
        }

        IApp.replace(QRegExp("I_app[ ][\[][0-9 -]{1,}[]][ ]"),QString(""));
        float I_float = IApp.toFloat();
        if(I_float>=0){
            ui->select_charge->setChecked(true);
        }else{
            ui->select_discharge->setChecked(true);
        }

        float I_float2 = qAbs(I_float);
        QString IApp2 = QString::number(I_float2);
        ui->I_lineEdit->setText(IApp2);
    }
    paramFile2.close();

    //set initial value from solveSolid.H
    QFile paramFileMaterial(MainWindow::project_path+"/"+MainWindow::project_name+"/halfCellFoam/solid/solveSolid.H");

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

    //define an unifrom baudExpress for below cecs
    QRegularExpression baudExpression_csce_uniform("internalField[ ]{1,}uniform[ ]{1,}[0-9.e-]{1,}");
    //set initial value from WE-ce
    QFile paramFile3(MainWindow::project_path+"/"+MainWindow::project_name+"/CC/0/WE/Ce");

    if(paramFile3.open(QFile::ReadOnly | QFile::Text)){
        QTextStream fileStream(&paramFile3);
        QString fileString = fileStream.readAll();

        //set initial ce
        QRegularExpressionMatch baudMatch = baudExpression_csce_uniform.match(fileString);

        QString initial_ce;
        if(baudMatch.hasMatch()){
            initial_ce=baudMatch.capturedRef().toString();
        }

        initial_ce.replace(QRegExp("internalField[ ]{1,}uniform[ ]{1,}"),QString(""));
        ui->WE_ce_lineEdit->setText(initial_ce);
    }
    paramFile3.close();

    //set initial value from WE-cs
    QFile paramFile4(MainWindow::project_path+"/"+MainWindow::project_name+"/CC/0/WE/Cs");

    if(paramFile4.open(QFile::ReadOnly | QFile::Text)){
        QTextStream fileStream(&paramFile4);
        QString fileString = fileStream.readAll();

        //set initial cs
        QRegularExpressionMatch baudMatch = baudExpression_csce_uniform.match(fileString);

        QString initial_cs;
        if(baudMatch.hasMatch()){
            initial_cs=baudMatch.capturedRef().toString();
        }

        initial_cs.replace(QRegExp("internalField[ ]{1,}uniform[ ]{1,}"),QString(""));
        ui->WE_cs_lineEdit->setText(initial_cs);
    }
    paramFile4.close();

    //set initial value from WE-faie
    QFile paramFile5(MainWindow::project_path+"/"+MainWindow::project_name+"/CC/0/WE/fai_e");

    if(paramFile5.open(QFile::ReadOnly | QFile::Text)){
        QTextStream fileStream(&paramFile5);
        QString fileString = fileStream.readAll();

        //set initial cs
        QRegularExpressionMatch baudMatch = baudExpression_csce_uniform.match(fileString);

        QString initial_faie;
        if(baudMatch.hasMatch()){
            initial_faie=baudMatch.capturedRef().toString();
        }

        initial_faie.replace(QRegExp("internalField[ ]{1,}uniform[ ]{1,}"),QString(""));
        ui->WE_faie_lineEdit->setText(initial_faie);
    }
    paramFile5.close();

    //set initial value from WE-fais
    QFile paramFile6(MainWindow::project_path+"/"+MainWindow::project_name+"/CC/0/WE/fai_s");

    if(paramFile6.open(QFile::ReadOnly | QFile::Text)){
        QTextStream fileStream(&paramFile6);
        QString fileString = fileStream.readAll();

        //set initial cs
        QRegularExpressionMatch baudMatch = baudExpression_csce_uniform.match(fileString);

        QString initial_fais;
        if(baudMatch.hasMatch()){
            initial_fais=baudMatch.capturedRef().toString();
        }

        initial_fais.replace(QRegExp("internalField[ ]{1,}uniform[ ]{1,}"),QString(""));
        ui->WE_fais_lineEdit->setText(initial_fais);
    }
    paramFile6.close();

    //set initial value from sep-ce
    QFile paramFile7(MainWindow::project_path+"/"+MainWindow::project_name+"/CC/0/sep/Ce");

    if(paramFile7.open(QFile::ReadOnly | QFile::Text)){
        QTextStream fileStream(&paramFile7);
        QString fileString = fileStream.readAll();

        //set initial cs
        QRegularExpressionMatch baudMatch = baudExpression_csce_uniform.match(fileString);

        QString initial_ce;
        if(baudMatch.hasMatch()){
            initial_ce=baudMatch.capturedRef().toString();
        }

        initial_ce.replace(QRegExp("internalField[ ]{1,}uniform[ ]{1,}"),QString(""));
        ui->sep_ce_lineEdit->setText(initial_ce);
    }
    paramFile7.close();

    //set initial value from sep-faie
    QFile paramFile8(MainWindow::project_path+"/"+MainWindow::project_name+"/CC/0/sep/fai_e");

    if(paramFile8.open(QFile::ReadOnly | QFile::Text)){
        QTextStream fileStream(&paramFile8);
        QString fileString = fileStream.readAll();

        //set initial cs
        QRegularExpressionMatch baudMatch = baudExpression_csce_uniform.match(fileString);

        QString initial_faie;
        if(baudMatch.hasMatch()){
            initial_faie=baudMatch.capturedRef().toString();
        }

        initial_faie.replace(QRegExp("internalField[ ]{1,}uniform[ ]{1,}"),QString(""));
        ui->sep_faie_lineEdit->setText(initial_faie);
    }
    paramFile8.close();

    //set fucntion interface initial value
    QFile paramFile_function(MainWindow::project_path+"/"+MainWindow::project_name+"/CC/system/fvSchemes");

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
    QFile paramFile9(MainWindow::project_path+"/"+MainWindow::project_name+"/CC/system/fvSolution");

    if(paramFile9.open(QFile::ReadOnly | QFile::Text)){
        QTextStream fileStream(&paramFile9);
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
    paramFile9.close();

    //set control(others) interface value
    QFile paramFile10(MainWindow::project_path+"/"+MainWindow::project_name+"/CC/system/controlDict");

    if(paramFile10.open(QFile::ReadOnly | QFile::Text)){
        QTextStream fileStream(&paramFile10);
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
    paramFile10.close();

    //set terminal
    m_process_bash->start("bash");
    m_process_bash->waitForStarted();
    connect(m_process_bash,SIGNAL(readyReadStandardOutput()),this,SLOT(readBashStandardOutputInfo()));
    connect(m_process_bash,SIGNAL(readyReadStandardError()),this,SLOT(readBashStandardErrorInfo()));
}

HalfCellInterface::~HalfCellInterface()
{
    if(m_process_bash)
    {
        m_process_bash->terminate();
        m_process_bash->waitForFinished();
    }
    delete ui;
}


//*******************************function difinition************************//
void HalfCellInterface::readBashStandardOutputInfo()
{
    QByteArray cmdout = m_process_bash->readAllStandardOutput();
    if(!cmdout.isEmpty()){
        ui->terminal_output_window->append(QString::fromLocal8Bit(cmdout));
    }
    QScrollBar* scroll = ui->terminal_output_window->verticalScrollBar();
    scroll->setSliderPosition(scroll->maximum());
}

//This function is to output errors on textEdit
void HalfCellInterface::readBashStandardErrorInfo()
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
void HalfCellInterface::on_change_geometry_button_clicked()
{
    float length = ui->length_lineEdit->text().toFloat();
    float length2 = ui->length2_lineEdit->text().toFloat();
    float lengthTotal = length+length2;

    QString Length = QString::number(length);
    QString Length2 = QString::number(length2);
    QString LengthTotal = QString::number(lengthTotal);
    QString Width = ui->width_lineEdit->text();
    QString Height = ui->height_lineEdit->text();

    //acquire the unit
    QString unit = ui->unit_select_box->currentText();

    //**************************for blockmesh module****************************//
    QFile file(MainWindow::project_path+"/"+MainWindow::project_name+"/CC/system/blockMeshDict");
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
        //interface1
        str.replace(QRegExp("[0-9.]{1,}[ ][0-9.]{1,}[ ][0-9.]{1,}[)][/]{2}WC node2"),
                    QString("0 "+Width+" 0)//WC node2"));

        str.replace(QRegExp("[0-9.]{1,}[ ][0-9.]{1,}[ ][0-9.]{1,}[)][/]{2}WC node3"),
                    QString("0 "+Width+" "+Height+")//WC node3"));

        str.replace(QRegExp("[0-9.]{1,}[ ][0-9.]{1,}[ ][0-9.]{1,}[)][/]{2}WC node4"),
                    QString("0 0 "+Height+")//WC node4"));

        //interface2
        str.replace(QRegExp("[0-9.]{1,}[ ][0-9.]{1,}[ ][0-9.]{1,}[)][/]{2}intface of SiC[/]sep node1"),
                    QString(Length+" 0 0"+")//intface of SiC/sep node1"));

        str.replace(QRegExp("[0-9.]{1,}[ ][0-9.]{1,}[ ][0-9.]{1,}[)][/]{2}SS node2"),
                    QString(Length+" "+Width+" 0)//SS node2"));

        str.replace(QRegExp("[0-9.]{1,}[ ][0-9.]{1,}[ ][0-9.]{1,}[)][/]{2}SS node3"),
                    QString(Length+" "+Width+" "+Height+")//SS node3"));

        str.replace(QRegExp("[0-9.]{1,}[ ][0-9.]{1,}[ ][0-9.]{1,}[)][/]{2}SS node4"),
                    QString(Length+" "+"0"+" "+Height+")//SS node4"));

        //interface3
        str.replace(QRegExp("[0-9.]{1,}[ ][0-9.]{1,}[ ][0-9.]{1,}[)][/]{2}intface of sep[/]RE node1"),
                    QString(LengthTotal+" 0 0"+")//intface of sep/RE node1"));

        str.replace(QRegExp("[0-9.]{1,}[ ][0-9.]{1,}[ ][0-9.]{1,}[)][/]{2}SR node2"),
                    QString(LengthTotal+" "+Width+" 0)//SR node2"));

        str.replace(QRegExp("[0-9.]{1,}[ ][0-9.]{1,}[ ][0-9.]{1,}[)][/]{2}SR node3"),
                    QString(LengthTotal+" "+Width+" "+Height+")//SR node3"));

        str.replace(QRegExp("[0-9.]{1,}[ ][0-9.]{1,}[ ][0-9.]{1,}[)][/]{2}SR node4"),
                    QString(LengthTotal+" "+"0"+" "+Height+")//SR node4"));


        //change divition on x,y,z direction
        str.replace(QRegExp("[0-9]{1,}[ ][0-9]{1,}[ ][0-9]{1,}[)][ ]simpleGrading[ ][(1 )]{1,}[/]{2}anode"),
                    QString(ui->x_divide_lineEdit->text()+" "+ui->y_divide_lineEdit->text()+
                            " "+ui->z_divide_lineEdit->text()+") simpleGrading (1 1 1)//anode"));

        str.replace(QRegExp("[0-9]{1,}[ ][0-9]{1,}[ ][0-9]{1,}[)][ ]simpleGrading[ ][(1 )]{1,}[/]{2}separator"),
                    QString(ui->x2_divide_lineEdit->text()+" "+ui->y_divide_lineEdit->text()+
                            " "+ui->z_divide_lineEdit->text()+") simpleGrading (1 1 1)//separator"));

    }
    file.close();

    QFile::remove(MainWindow::project_path+"/"+MainWindow::project_name+"/CC/system/blockMeshDict");
    if(!file.open(QIODevice::WriteOnly))
    {
         QMessageBox::warning(this,"Error","Cannot open file for Writing");
         return ;
    }

    file.write(str.toUtf8());
    file.close();


    //**************************for toposet file****************************//
    QFile file2(MainWindow::project_path+"/"+MainWindow::project_name+"/CC/system/topoSetDict");
    if(!file2.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this,"Error","Cannot open file for Reading");
        return ;
    }

    QString str2 (file2.readAll());
    if(str2.contains("topoSetDict", Qt::CaseInsensitive)){
        if(unit=="micrometer(um)"){
            str2.replace(QRegExp("[0-9.]{1,}[e][-][0-9][ ]{1,}[0-9.]{1,}[e][-][0-9][ ]{1,}[0-9.]{1,}[e][-][0-9][);/]{1,}WE"),
                         QString(Length+"e-6   "+Width+"e-6  "+Height+"e-6);//WE"));
            str2.replace(QRegExp("[0-9.]{1,}[e][-][0-9][ ]{1,}[0-9.]{1,}[e][-][0-9][ ]{1,}[0-9.]{1,}[e][-][0-9][);/]{1,}separator"),
                         QString(LengthTotal+"e-6   "+Width+"e-6  "+Height+"e-6);//separator"));
            str2.replace(QRegExp("box[ ][(][0-9.]{1,}[e][-][0-9]"),
                         QString("box ("+Length+"e-6"));

        }else if(unit=="millimeter(mm)"){
            str2.replace(QRegExp("[0-9.]{1,}[e][-][0-9][ ]{1,}[0-9.]{1,}[e][-][0-9][ ]{1,}[0-9.]{1,}[e][-][0-9][);/]{1,}WE"),
                         QString(Length+"e-3   "+Width+"e-3  "+Height+"e-3);//WE"));
            str2.replace(QRegExp("[0-9.]{1,}[e][-][0-9][ ]{1,}[0-9.]{1,}[e][-][0-9][ ]{1,}[0-9.]{1,}[e][-][0-9][);/]{1,}separator"),
                         QString(LengthTotal+"e-3   "+Width+"e-3  "+Height+"e-3);//separator"));
            str2.replace(QRegExp("box[ ][(][0-9.]{1,}[e][-][0-9]"),
                         QString("box ("+Length+"e-3"));

        }else if(unit=="meter(m)"){
            str2.replace(QRegExp("[0-9.]{1,}[e][-][0-9][ ]{1,}[0-9.]{1,}[e][-][0-9][ ]{1,}[0-9.]{1,}[e][-][0-9][);/]{1,}WE"),
                         QString(Length+"e-0   "+Width+"e-0  "+Height+"e-0);//WE"));
            str2.replace(QRegExp("[0-9.]{1,}[e][-][0-9][ ]{1,}[0-9.]{1,}[e][-][0-9][ ]{1,}[0-9.]{1,}[e][-][0-9][);/]{1,}separator"),
                         QString(LengthTotal+"e-0   "+Width+"e-0  "+Height+"e-0);//separator"));
            str2.replace(QRegExp("box[ ][(][0-9.]{1,}[e][-][0-9]"),
                         QString("box ("+Length+"e-0"));
        }
    }
    file2.close();

    QFile::remove(MainWindow::project_path+"/"+MainWindow::project_name+"/CC/system/topoSetDict");
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
    QString path =MainWindow::project_path+"/"+MainWindow::project_name+"/"+"CC";

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
void HalfCellInterface::on_run_geometry_button_clicked()
{
    ui->tabWidget->setCurrentIndex(1);
}

//change constant parameters
void HalfCellInterface::on_change_constant_button_clicked()
{
    QFile file(MainWindow::project_path+"/"+MainWindow::project_name+"/CC/constant/LiProperties");
    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this,"Error","Cannot open file for Reading");
        return ;
    }

    QString str (file.readAll());
    if(str.contains("transportProperties", Qt::CaseInsensitive)){
        str.replace(QRegExp("por_Ce_WE[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}"),
                    QString("por_Ce_WE [0 -1 0 0 0 0 0] "+ui->CeWE_lineEdit->text()));

        str.replace(QRegExp("por_Ce_sp[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}"),
                    QString("por_Ce_sp [0 -1 0 0 0 0 0] "+ui->Cesp_lineEdit->text()));

        str.replace(QRegExp("por_f_WE[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}"),
                    QString("por_f_WE [0 -1 0 0 0 0 0] "+ui->fWE_lineEdit->text()));

        str.replace(QRegExp("Cs_max_WE[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}"),
                    QString("Cs_max_WE [0 -1 0 0 0 0 0] "+ui->maxWE_lineEdit->text()));

        str.replace(QRegExp("brugg[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}"),
                    QString("brugg [0 -1 0 0 0 0 0] "+ui->brugg_lineEdit->text()));

        str.replace(QRegExp("R[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}"),
                    QString("R [0 -1 0 0 0 0 0] "+ui->R_lineEdit->text()));

        str.replace(QRegExp("F[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}"),
                    QString("F [0 -1 0 0 0 0 0] "+ui->F_lineEdit->text()));

        str.replace(QRegExp("tNo[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}"),
                    QString("tNo [0 -1 0 0 0 0 0] "+ui->tNo_lineEdit->text()));

        str.replace(QRegExp("k0_fai_s_WE[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}"),
                    QString("k0_fai_s_WE [0 -1 0 0 0 0 0] "+ui->faisWE_lineEdit->text()));

        str.replace(QRegExp("D0_Ce[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}"),
                    QString("D0_Ce [0 -1 0 0 0 0 0] "+ui->D0Ce_lineEdit->text()));

        str.replace(QRegExp("r_s[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}"),
                    QString("r_s [0 -1 0 0 0 0 0] "+ui->rs_lineEdit->text()));

        str.replace(QRegExp("Ds_WE[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}"),
                    QString("Ds_WE [0 -1 0 0 0 0 0] "+ui->DsWE_lineEdit->text()));

        str.replace(QRegExp("kReact_WE[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}"),
                    QString("kReact_WE [0 -1 0 0 0 0 0] "+ui->KReact_lineEdit->text()));

        str.replace(QRegExp("kReact_RE[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}"),
                    QString("kReact_RE [0 -1 0 0 0 0 0] "+ui->KReactRE_lineEdit->text()));

        str.replace(QRegExp("alphaA_WE[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}"),
                    QString("alphaA_WE [0 -1 0 0 0 0 0] "+ui->alphaAWE_lineEdit->text()));

        str.replace(QRegExp("alphaC_WE[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}"),
                    QString("alphaC_WE [0 -1 0 0 0 0 0] "+ui->alphaCWE_lineEdit->text()));

        str.replace(QRegExp("alphaA_RE[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}"),
                    QString("alphaA_RE [0 -1 0 0 0 0 0] "+ui->alphaARE_lineEdit->text()));

        str.replace(QRegExp("alphaC_RE[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}"),
                    QString("alphaC_RE [0 -1 0 0 0 0 0] "+ui->alphaCRE_lineEdit->text()));

        if(ui->select_charge->isChecked()){
            str.replace(QRegExp("I_app[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}"),
                        QString("I_app [0 -1 0 0 0 0 0] "+ui->I_lineEdit->text()));
        }else if(ui->select_discharge->isChecked()){
            str.replace(QRegExp("I_app[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}"),
                        QString("I_app [0 -1 0 0 0 0 0] "+QString("-")+ui->I_lineEdit->text()));
        }

    }
    file.close();

    QFile::remove(MainWindow::project_path+"/"+MainWindow::project_name+"/CC/constant/LiProperties");
    if(!file.open(QIODevice::WriteOnly))
    {
         QMessageBox::warning(this,"Error","Cannot open file for Writing");
         return ;
    }

    file.write(str.toUtf8());
    file.close();


    //change the file in ele and solidphase folder
    if (QFile::exists(MainWindow::project_path+"/"+MainWindow::project_name+"/CC/constant/WE/LiProperties"))
    {
        QFile::remove(MainWindow::project_path+"/"+MainWindow::project_name+"/CC/constant/WE/LiProperties");
    }

    QFile::copy(MainWindow::project_path+"/"+MainWindow::project_name+"/CC/constant/LiProperties",
                MainWindow::project_path+"/"+MainWindow::project_name+"/CC/constant/WE/LiProperties");

    if (QFile::exists(MainWindow::project_path+"/"+MainWindow::project_name+"/CC/constant/sep/LiProperties"))
    {
        QFile::remove(MainWindow::project_path+"/"+MainWindow::project_name+"/CC/constant/sep/LiProperties");
    }

    QFile::copy(MainWindow::project_path+"/"+MainWindow::project_name+"/CC/constant/LiProperties",
                MainWindow::project_path+"/"+MainWindow::project_name+"/CC/constant/sep/LiProperties");

    //Material selection part
    QFile fileMaterial(MainWindow::project_path+"/"+MainWindow::project_name+"/halfCellFoam/solid/solveSolid.H");
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
            strMaterial.replace(QRegExp("//#include [\"]0init_Gr.H[\"]"),
                        QString("#include \"0init_Gr.H\""));
            if(!alreadyCommitSi){
                strMaterial.replace(QRegExp("#include [\"]0init_Si.H[\"]"),
                            QString("//#include \"0init_Si.H\""));
            }
        }else{
            if(!alreadyCommitGr){
                strMaterial.replace(QRegExp("#include [\"]OCV_Gr.H[\"]"),
                            QString("//#include \"OCV_Gr.H\""));
            }
            strMaterial.replace(QRegExp("//#include [\"]OCV_Si.H[\"]"),
                        QString("#include \"OCV_Si.H\""));
            if(!alreadyCommitGr){
                strMaterial.replace(QRegExp("#include [\"]0init_Gr.H[\"]"),
                            QString("//#include \"0init_Gr.H\""));
            }
            strMaterial.replace(QRegExp("//#include [\"]0init_Si.H[\"]"),
                        QString("#include \"0init_Si.H\""));
        }
    }
    fileMaterial.close();

    QFile::remove(MainWindow::project_path+"/"+MainWindow::project_name+"/halfCellFoam/solid/solveSolid.H");
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
void HalfCellInterface::on_help_constant_button_clicked()
{
    QMessageBox::information(this,"hint","por_WE: \tElectrolyte volume fraction in working electrode\n"
                                         "por_SEP: \tElectrolyte volume fraction in separator\n"
                                         "por_f_WE: \tFilter volume fraction in working electrode\n"
                                         "Cs_WE: \tMaximum Li concentration in the solid phase\n"
                                         "brugg: \tBruggmann coefficient\n"
                                         "R: \tUniversal gas constant\n"
                                         "F: \tFaraday's constant\n"
                                         "tNo: \tTransference number\n"
                                         "k0_WE: \tSolid phase conductivity\n"
                                         "D0_e: \tElectrolyte diffusivity\n"
                                         "r: \tSolid particle size\n"
                                         "Ds_WE: \tLithium diffusivity\n"
                                         "kreact_WE: Reaction rate constant in working electrode\n"
                                         "kreact_RE: Reaction rate constant in counter electrode\n"
                                         "alphaA_WE: Anodic transfer coefficient in working electrode\n"
                                         "alphaC_WE: Cathodic transfer coefficient in working electrode\n"
                                         "alphaA_RE: Anodic transfer coefficient in counter electrode\n"
                                         "alphaC_RE: Cathodic transfer coefficient in counter electrode\n"
                                         "Iapp:  \tApplied current density\n");
}

//from constant interface to constant2 interface
void HalfCellInterface::on_run_constant_button_clicked()
{
    ui->tabWidget->setCurrentIndex(2);
}

//from constant2 interface to next page
void HalfCellInterface::on_run_constant2_button_clicked()
{
    ui->tabWidget->setCurrentIndex(3);
}

//change boundary parameters
void HalfCellInterface::on_change_boundary_button_clicked()
{
    //WE-ce
    QFile file(MainWindow::project_path+"/"+MainWindow::project_name+"/CC/0/WE/Ce");
    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this,"Error","Cannot open file for Reading");
        return ;
    }

    QString str (file.readAll());
    if(str.contains("Ce", Qt::CaseInsensitive)){
        str.replace(QRegExp("internalField[ ]{1,}uniform[ ]{1,}[0-9.e-]{1,}"),
                    QString("internalField   uniform   "+ui->WE_ce_lineEdit->text()));
    }
    file.close();

    QFile::remove(MainWindow::project_path+"/"+MainWindow::project_name+"/CC/0/WE/Ce");
    if(!file.open(QIODevice::WriteOnly))
    {
         QMessageBox::warning(this,"Error","Cannot open file for Writing");
         return ;
    }

    file.write(str.toUtf8());
    file.close();

    //WE-cs
    QFile file2(MainWindow::project_path+"/"+MainWindow::project_name+"/CC/0/WE/Cs");
    if(!file2.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this,"Error","Cannot open file for Reading");
        return ;
    }

    QString str2 (file2.readAll());
    if(str2.contains("Cs", Qt::CaseInsensitive)){
        str2.replace(QRegExp("internalField[ ]{1,}uniform[ ]{1,}[0-9.e-]{1,}"),
                    QString("internalField   uniform   "+ui->WE_cs_lineEdit->text()));
    }
    file2.close();

    QFile::remove(MainWindow::project_path+"/"+MainWindow::project_name+"/CC/0/WE/Cs");
    if(!file2.open(QIODevice::WriteOnly))
    {
         QMessageBox::warning(this,"Error","Cannot open file for Writing");
         return ;
    }

    file2.write(str2.toUtf8());
    file2.close();

    //WE-fai-e
    QFile file3(MainWindow::project_path+"/"+MainWindow::project_name+"/CC/0/WE/fai_e");
    if(!file3.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this,"Error","Cannot open file for Reading");
        return ;
    }

    QString str3 (file3.readAll());
    if(str3.contains("fai_e", Qt::CaseInsensitive)){
        str3.replace(QRegExp("internalField[ ]{1,}uniform[ ]{1,}[0-9.e-]{1,}"),
                    QString("internalField   uniform   "+ui->WE_faie_lineEdit->text()));
    }
    file3.close();

    QFile::remove(MainWindow::project_path+"/"+MainWindow::project_name+"/CC/0/WE/fai_e");
    if(!file3.open(QIODevice::WriteOnly))
    {
         QMessageBox::warning(this,"Error","Cannot open file for Writing");
         return ;
    }

    file3.write(str3.toUtf8());
    file3.close();

    //WE-fai-s
    QFile file4(MainWindow::project_path+"/"+MainWindow::project_name+"/CC/0/WE/fai_s");
    if(!file4.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this,"Error","Cannot open file for Reading");
        return ;
    }

    QString str4 (file4.readAll());
    if(str4.contains("fai_s", Qt::CaseInsensitive)){
        str4.replace(QRegExp("internalField[ ]{1,}uniform[ ]{1,}[0-9.e-]{1,}"),
                    QString("internalField   uniform   "+ui->WE_fais_lineEdit->text()));
    }
    file4.close();

    QFile::remove(MainWindow::project_path+"/"+MainWindow::project_name+"/CC/0/WE/fai_s");
    if(!file4.open(QIODevice::WriteOnly))
    {
         QMessageBox::warning(this,"Error","Cannot open file for Writing");
         return ;
    }

    file4.write(str4.toUtf8());
    file4.close();

    //sep-ce
    QFile file5(MainWindow::project_path+"/"+MainWindow::project_name+"/CC/0/sep/Ce");
    if(!file5.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this,"Error","Cannot open file for Reading");
        return ;
    }

    QString str5 (file5.readAll());
    if(str5.contains("Ce", Qt::CaseInsensitive)){
        str5.replace(QRegExp("internalField[ ]{1,}uniform[ ]{1,}[0-9.e-]{1,}"),
                    QString("internalField   uniform   "+ui->sep_ce_lineEdit->text()));
    }
    file5.close();

    QFile::remove(MainWindow::project_path+"/"+MainWindow::project_name+"/CC/0/sep/Ce");
    if(!file5.open(QIODevice::WriteOnly))
    {
         QMessageBox::warning(this,"Error","Cannot open file for Writing");
         return ;
    }

    file5.write(str5.toUtf8());
    file5.close();

    //sep-fai-e
    QFile file6(MainWindow::project_path+"/"+MainWindow::project_name+"/CC/0/sep/fai_e");
    if(!file6.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this,"Error","Cannot open file for Reading");
        return ;
    }

    QString str6 (file6.readAll());
    if(str6.contains("fai_e", Qt::CaseInsensitive)){
        str6.replace(QRegExp("internalField[ ]{1,}uniform[ ]{1,}[0-9.e-]{1,}"),
                    QString("internalField   uniform   "+ui->sep_faie_lineEdit->text()));
    }
    file6.close();

    QFile::remove(MainWindow::project_path+"/"+MainWindow::project_name+"/CC/0/sep/fai_e");
    if(!file6.open(QIODevice::WriteOnly))
    {
         QMessageBox::warning(this,"Error","Cannot open file for Writing");
         return ;
    }

    file6.write(str6.toUtf8());
    file6.close();


    QMessageBox::information(this,"title","Change successfully");
}

//from boundary interface to next page
void HalfCellInterface::on_run_boundary_button_clicked()
{
    ui->tabWidget->setCurrentIndex(4);
}

//change function parameters
void HalfCellInterface::on_change_function_button_clicked()
{
    QFile file(MainWindow::project_path+"/"+MainWindow::project_name+"/CC/system/fvSchemes");
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

    QFile::remove(MainWindow::project_path+"/"+MainWindow::project_name+"/CC/system/fvSchemes");
    if(!file.open(QIODevice::WriteOnly))
    {
         QMessageBox::warning(this,"Error","Cannot open file for Writing");
         return ;
    }

    file.write(str.toUtf8());
    file.close();

    //change the file in ele and solidphase folder
    if (QFile::exists(MainWindow::project_path+"/"+MainWindow::project_name+"/CC/system/WE/fvSchemes"))
    {
        QFile::remove(MainWindow::project_path+"/"+MainWindow::project_name+"/CC/system/WE/fvSchemes");
    }

    QFile::copy(MainWindow::project_path+"/"+MainWindow::project_name+"/CC/system/fvSchemes",
                MainWindow::project_path+"/"+MainWindow::project_name+"/CC/system/WE/fvSchemes");

    if (QFile::exists(MainWindow::project_path+"/"+MainWindow::project_name+"/CC/system/sep/fvSchemes"))
    {
        QFile::remove(MainWindow::project_path+"/"+MainWindow::project_name+"/CC/system/sep/fvSchemes");
    }

    QFile::copy(MainWindow::project_path+"/"+MainWindow::project_name+"/CC/system/fvSchemes",
                MainWindow::project_path+"/"+MainWindow::project_name+"/CC/system/sep/fvSchemes");

    QMessageBox::information(this,"title","Change successfully");
}

//from function interface to next page
void HalfCellInterface::on_run_function_button_clicked()
{
    ui->tabWidget->setCurrentIndex(5);
}

//change tolerance and other control parameters
void HalfCellInterface::on_change_control_button_clicked()
{
     //change fvsolution file in sep and WE folder
    QFile file(MainWindow::project_path+"/"+MainWindow::project_name+"/CC/system/fvSolution");
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

    QFile::remove(MainWindow::project_path+"/"+MainWindow::project_name+"/CC/system/fvSolution");
    if(!file.open(QIODevice::WriteOnly))
    {
         QMessageBox::warning(this,"Error","Cannot open file for Writing");
         return ;
    }

    file.write(str.toUtf8());
    file.close();


    if (QFile::exists(MainWindow::project_path+"/"+MainWindow::project_name+"/CC/system/WE/fvSolution"))
    {
        QFile::remove(MainWindow::project_path+"/"+MainWindow::project_name+"/CC/system/WE/fvSolution");
    }

    QFile::copy(MainWindow::project_path+"/"+MainWindow::project_name+"/CC/system/fvSolution",
                MainWindow::project_path+"/"+MainWindow::project_name+"/CC/system/WE/fvSolution");

    if (QFile::exists(MainWindow::project_path+"/"+MainWindow::project_name+"/CC/system/spe/fvSolution"))
    {
        QFile::remove(MainWindow::project_path+"/"+MainWindow::project_name+"/CC/system/spe/fvSolution");
    }

    QFile::copy(MainWindow::project_path+"/"+MainWindow::project_name+"/CC/system/fvSolution",
                MainWindow::project_path+"/"+MainWindow::project_name+"/CC/system/spe/fvSolution");


    //change control parameters
    QFile file2(MainWindow::project_path+"/"+MainWindow::project_name+"/CC/system/controlDict");
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

    QFile::remove(MainWindow::project_path+"/"+MainWindow::project_name+"/CC/system/controlDict");
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
void HalfCellInterface::on_run_button_clicked()
{
    if(delete_flag==1){
        //remove folders
        QString folderPath=MainWindow::project_path+"/"+MainWindow::project_name+"/CC";

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
        QFileInfo file(MainWindow::project_path+"/"+MainWindow::project_name+"/CC/time_voltage");
        if(file.exists()){
            QFile::remove(MainWindow::project_path+"/"+MainWindow::project_name+"/CC/time_voltage");
        }

        QFileInfo file2(MainWindow::project_path+"/"+MainWindow::project_name+"/CC/time_current");
        if(file2.exists()){
            QFile::remove(MainWindow::project_path+"/"+MainWindow::project_name+"/CC/time_current");
        }

        QFileInfo file3(MainWindow::project_path+"/"+MainWindow::project_name+"/CC/time_species");
        if(file3.exists()){
            QFile::remove(MainWindow::project_path+"/"+MainWindow::project_name+"/CC/time_species");
        }

        QFileInfo file4(MainWindow::project_path+"/"+MainWindow::project_name+"/CC/time_RE");
        if(file4.exists()){
            QFile::remove(MainWindow::project_path+"/"+MainWindow::project_name+"/CC/time_RE");
        }

        QEventLoop eventloop2;
        QTimer::singleShot(1000, &eventloop2, SLOT(quit()));
        eventloop2.exec();

        //wclean and wmake
        QString path =MainWindow::project_path+"/"+MainWindow::project_name+"/"+"halfCellFoam";

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


    QString path2 =MainWindow::project_path+"/"+MainWindow::project_name+"/"+"CC";

    ui->command_input_lineEdit->setText("cd /"+path2+" && "+MainWindow::project_name);
    m_process_bash->write(ui->command_input_lineEdit->text().toLocal8Bit()+'\n');
    ui->command_input_lineEdit->clear();

    ui->view_result_button->setEnabled(true);
    delete_flag=1;
}

//pause the run process
void HalfCellInterface::on_pause_run_button_clicked()
{
    //stop the program
    QString str ="pkill -f "+QString(MainWindow::project_name);
    QByteArray byteArray = str.toUtf8();
    char* data = byteArray.data();

    system(data);
    delete_flag=0;
}

//go back to the main window
void HalfCellInterface::on_half_return_Button_clicked()
{
    emit ExitWin();
    close();
}

//command test button
void HalfCellInterface::on_pushButton_clicked()
{
    QString strCmd = ui->command_input_lineEdit->text();
    ui->terminal_output_window->append("Linux:~$ "+strCmd);
    m_process_bash->write(ui->command_input_lineEdit->text().toLocal8Bit()+'\n');
    ui->command_input_lineEdit->clear();
}

//paraview
void HalfCellInterface::on_open_paraview_Button_clicked()
{
    QString path =MainWindow::project_path+"/"+MainWindow::project_name+"/"+"CC";

    ui->command_input_lineEdit->setText("cd /"+path+" && "+"paraview &");
    ui->terminal_output_window->append("Linux:~$ paraview &");
    m_process_bash->write(ui->command_input_lineEdit->text().toLocal8Bit()+'\n');
    ui->command_input_lineEdit->clear();
}

//view the results
void HalfCellInterface::on_view_result_button_clicked()
{
    resultInterface = new ResultInterface(this);
    resultInterface->show();
}

