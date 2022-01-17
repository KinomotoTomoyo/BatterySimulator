#include "fullcellfoam.h"
#include "ui_fullcellfoam.h"
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

FullCellFoam::FullCellFoam(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FullCellFoam)
{
    ui->setupUi(this);

    //set window title
    QWidget::setWindowTitle ( "BatteryFoam" );

    //set maximum screen length
    ui->terminal_output_window->document()->setMaximumBlockCount(1000);

    //set view button is available if time_voltage exists
    QFile t_v_file(MainWindow::project_path+"/"+MainWindow::project_name+"/case/time_voltage");
    if(t_v_file.exists()){
        ui->view_result_button->setEnabled(true);
    }


    //set initial value from blockMeshDict
    QFile paramFile(MainWindow::project_path+"/"+MainWindow::project_name+"/case/system/blockMeshDict");

    if(paramFile.open(QFile::ReadOnly | QFile::Text)){
        QTextStream fileStream(&paramFile);
        QString fileString = fileStream.readAll();

        //set length,width and height
        QRegularExpression baudExpression("[0-9.]{1,}[ ][0-9.]{1,}[ ][0-9.]{1,}[)][/]{2}sl node3");
        QRegularExpressionMatch baudMatch = baudExpression.match(fileString);

        QString lengthWidthHeight;
        if(baudMatch.hasMatch()){
            lengthWidthHeight=baudMatch.capturedRef().toString();
        }

        QString length_original = lengthWidthHeight;
        length_original.replace(QRegExp("[ ][0-9.]{1,}[ ][0-9.]{1,}[)][/]{2}sl node3"),QString(""));
        float part1_length = length_original.toFloat();
        ui->length_lineEdit->setText(length_original);

        QString width_original = lengthWidthHeight;
        width_original.replace(QRegExp("[ ][0-9.]{1,}[)][/]{2}sl node3"),QString(""));
        width_original.replace(QRegExp("[0-9.]{1,}[ ]"),QString(""));
        ui->width_lineEdit->setText(width_original);

        QString height_original = lengthWidthHeight;
        height_original.replace(QRegExp("[)][/]{2}sl node3"),QString(""));
        height_original.replace(QRegExp("[0-9.]{1,}[ ][0-9.]{1,}[ ]"),QString(""));
        ui->height_lineEdit->setText(height_original);

        //set length2
        QRegularExpression baudExpression2("[0-9.]{1,}[ ][0-9.]{1,}[ ][0-9.]{1,}[)][/]{2}sr node3");
        QRegularExpressionMatch baudMatch2 = baudExpression2.match(fileString);

        QString length2Self;
        if(baudMatch2.hasMatch()){
            length2Self=baudMatch2.capturedRef().toString();
        }

        length2Self.replace(QRegExp("[ ][0-9.]{1,}[ ][0-9.]{1,}[)][/]{2}sr node3"),QString(""));
        float part2_length = length2Self.toFloat();
        QString length2_original = QString::number(part2_length-part1_length);
        ui->length2_lineEdit->setText(length2_original);

        //set length3
        QRegularExpression baudExpression3("[0-9.]{1,}[ ][0-9.]{1,}[ ][0-9.]{1,}[)][/]{2}cr node3");
        QRegularExpressionMatch baudMatch3 = baudExpression3.match(fileString);

        QString length3Self;
        if(baudMatch3.hasMatch()){
            length3Self=baudMatch3.capturedRef().toString();
        }

        length3Self.replace(QRegExp("[ ][0-9.]{1,}[ ][0-9.]{1,}[)][/]{2}cr node3"),QString(""));
        float part3_length = length3Self.toFloat();
        QString length3_original = QString::number(part3_length-part2_length);
        ui->length3_lineEdit->setText(length3_original);

        //set divition on x,y,z
        QRegularExpression baudExpression4("[0-9]{1,}[ ][0-9]{1,}[ ][0-9]{1,}[)][ ]simpleGrading[ ][(1 )]{1,}[/]{2}anode");
        QRegularExpressionMatch baudMatch4 = baudExpression4.match(fileString);

        QString xyz_divition;
        if(baudMatch4.hasMatch()){
            xyz_divition=baudMatch4.capturedRef().toString();
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
        QRegularExpression baudExpression5("[0-9]{1,}[ ][0-9]{1,}[ ][0-9]{1,}[)][ ]simpleGrading[ ][(1 )]{1,}[/]{2}separator");
        QRegularExpressionMatch baudMatch5 = baudExpression5.match(fileString);

        QString x2_divition;
        if(baudMatch5.hasMatch()){
            x2_divition=baudMatch5.capturedRef().toString();
        }

        x2_divition.replace(QRegExp("[ ][0-9]{1,}[ ][0-9]{1,}[)][ ]simpleGrading[ ][(1 )]{1,}[/]{2}separator"),QString(""));
        ui->x2_divide_lineEdit->setText(x2_divition);

        //set divition on x3
        QRegularExpression baudExpression6("[0-9]{1,}[ ][0-9]{1,}[ ][0-9]{1,}[)][ ]simpleGrading[ ][(1 )]{1,}[/]{2}cathode");
        QRegularExpressionMatch baudMatch6 = baudExpression6.match(fileString);

        QString x3_divition;
        if(baudMatch6.hasMatch()){
            x3_divition=baudMatch6.capturedRef().toString();
        }

        x3_divition.replace(QRegExp("[ ][0-9]{1,}[ ][0-9]{1,}[)][ ]simpleGrading[ ][(1 )]{1,}[/]{2}cathode"),QString(""));
        ui->x3_divide_lineEdit->setText(x3_divition);

        //set current unit
        QRegularExpression baudExpression7("convertToMeters[ ]{1,}[1][e][-][0-9]");
        QRegularExpressionMatch baudMatch7 = baudExpression7.match(fileString);

        QString initial_unit;
        if(baudMatch7.hasMatch()){
            initial_unit=baudMatch7.capturedRef().toString();
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
    QFile paramFile2(MainWindow::project_path+"/"+MainWindow::project_name+"/case/constant/LiProperties");

    if(paramFile2.open(QFile::ReadOnly | QFile::Text)){
        QTextStream fileStream(&paramFile2);
        QString fileString = fileStream.readAll();

        QRegularExpressionMatch baudMatch;
        QString one_constant;

        //Cs_max_a
        QRegularExpression baudExpression("Cs_max_a[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}");
        baudMatch = baudExpression.match(fileString);

        if(baudMatch.hasMatch()){
            one_constant=baudMatch.capturedRef().toString();
        }

        one_constant.replace(QRegExp("Cs_max_a[ ][\[][0-9 -]{1,}[]][ ]"),QString(""));
        ui->csmaxa_lineEdit->setText(one_constant);

        //Cs_max_c
        QRegularExpression baudExpression2("Cs_max_c[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}");
        baudMatch = baudExpression2.match(fileString);

        if(baudMatch.hasMatch()){
            one_constant=baudMatch.capturedRef().toString();
        }

        one_constant.replace(QRegExp("Cs_max_c[ ][\[][0-9 -]{1,}[]][ ]"),QString(""));
        ui->csmaxc_lineEdit->setText(one_constant);

        //kReact_a
        QRegularExpression baudExpression3("kReact_a[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}");
        baudMatch = baudExpression3.match(fileString);

        if(baudMatch.hasMatch()){
            one_constant=baudMatch.capturedRef().toString();
        }

        one_constant.replace(QRegExp("kReact_a[ ][\[][0-9 -]{1,}[]][ ]"),QString(""));
        ui->kreacta_lineEdit->setText(one_constant);

        //kReact_c
        QRegularExpression baudExpression4("kReact_c[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}");
        baudMatch = baudExpression4.match(fileString);

        if(baudMatch.hasMatch()){
            one_constant=baudMatch.capturedRef().toString();
        }

        one_constant.replace(QRegExp("kReact_c[ ][\[][0-9 -]{1,}[]][ ]"),QString(""));
        ui->kreactc_lineEdit->setText(one_constant);

        //alphaA_a
        QRegularExpression baudExpression5("alphaA_a[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}");
        baudMatch = baudExpression5.match(fileString);

        if(baudMatch.hasMatch()){
            one_constant=baudMatch.capturedRef().toString();
        }

        one_constant.replace(QRegExp("alphaA_a[ ][\[][0-9 -]{1,}[]][ ]"),QString(""));
        ui->alphaaa_lineEdit->setText(one_constant);

        //alphaC_a
        QRegularExpression baudExpression6("alphaC_a[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}");
        baudMatch = baudExpression6.match(fileString);

        if(baudMatch.hasMatch()){
            one_constant=baudMatch.capturedRef().toString();
        }

        one_constant.replace(QRegExp("alphaC_a[ ][\[][0-9 -]{1,}[]][ ]"),QString(""));
        ui->alphaca_lineEdit->setText(one_constant);

        //alphaA_c
        QRegularExpression baudExpression7("alphaA_c[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}");
        baudMatch = baudExpression7.match(fileString);

        if(baudMatch.hasMatch()){
            one_constant=baudMatch.capturedRef().toString();
        }

        one_constant.replace(QRegExp("alphaA_c[ ][\[][0-9 -]{1,}[]][ ]"),QString(""));
        ui->alphaac_lineEdit->setText(one_constant);

        //alphaC_c
        QRegularExpression baudExpression8("alphaC_c[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}");
        baudMatch = baudExpression8.match(fileString);

        if(baudMatch.hasMatch()){
            one_constant=baudMatch.capturedRef().toString();
        }

        one_constant.replace(QRegExp("alphaC_c[ ][\[][0-9 -]{1,}[]][ ]"),QString(""));
        ui->alphacc_lineEdit->setText(one_constant);

        //R
        QRegularExpression baudExpression9("R[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}");
        baudMatch = baudExpression9.match(fileString);

        if(baudMatch.hasMatch()){
            one_constant=baudMatch.capturedRef().toString();
        }

        one_constant.replace(QRegExp("R[ ][\[][0-9 -]{1,}[]][ ]"),QString(""));
        ui->r_lineEdit->setText(one_constant);

        //F
        QRegularExpression baudExpression10("F[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}");
        baudMatch = baudExpression10.match(fileString);

        if(baudMatch.hasMatch()){
            one_constant=baudMatch.capturedRef().toString();
        }

        one_constant.replace(QRegExp("F[ ][\[][0-9 -]{1,}[]][ ]"),QString(""));
        ui->f_lineEdit->setText(one_constant);

        //por_Ce_a
        QRegularExpression baudExpression11("por_Ce_a[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}");
        baudMatch = baudExpression11.match(fileString);

        if(baudMatch.hasMatch()){
            one_constant=baudMatch.capturedRef().toString();
        }

        one_constant.replace(QRegExp("por_Ce_a[ ][\[][0-9 -]{1,}[]][ ]"),QString(""));
        ui->porcea_lineEdit->setText(one_constant);

        //por_Ce_sp
        QRegularExpression baudExpression12("por_Ce_sp[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}");
        baudMatch = baudExpression12.match(fileString);

        if(baudMatch.hasMatch()){
            one_constant=baudMatch.capturedRef().toString();
        }

        one_constant.replace(QRegExp("por_Ce_sp[ ][\[][0-9 -]{1,}[]][ ]"),QString(""));
        ui->porcesp_lineEdit->setText(one_constant);

        //por_Ce_c
        QRegularExpression baudExpression13("por_Ce_c[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}");
        baudMatch = baudExpression13.match(fileString);

        if(baudMatch.hasMatch()){
            one_constant=baudMatch.capturedRef().toString();
        }

        one_constant.replace(QRegExp("por_Ce_c[ ][\[][0-9 -]{1,}[]][ ]"),QString(""));
        ui->porcec_lineEdit->setText(one_constant);

        //por_f_a
        QRegularExpression baudExpression14("por_f_a[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}");
        baudMatch = baudExpression14.match(fileString);

        if(baudMatch.hasMatch()){
            one_constant=baudMatch.capturedRef().toString();
        }

        one_constant.replace(QRegExp("por_f_a[ ][\[][0-9 -]{1,}[]][ ]"),QString(""));
        ui->porfa_lineEdit->setText(one_constant);

        //por_f_c
        QRegularExpression baudExpression15("por_f_c[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}");
        baudMatch = baudExpression15.match(fileString);

        if(baudMatch.hasMatch()){
            one_constant=baudMatch.capturedRef().toString();
        }

        one_constant.replace(QRegExp("por_f_c[ ][\[][0-9 -]{1,}[]][ ]"),QString(""));
        ui->porfc_lineEdit->setText(one_constant);

        //brugg
        QRegularExpression baudExpression16("brugg[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}");
        baudMatch = baudExpression16.match(fileString);

        if(baudMatch.hasMatch()){
            one_constant=baudMatch.capturedRef().toString();
        }

        one_constant.replace(QRegExp("brugg[ ][\[][0-9 -]{1,}[]][ ]"),QString(""));
        ui->brugg_lineEdit->setText(one_constant);

        //k0_fai_s_a
        QRegularExpression baudExpression17("k0_fai_s_a[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}");
        baudMatch = baudExpression17.match(fileString);

        if(baudMatch.hasMatch()){
            one_constant=baudMatch.capturedRef().toString();
        }

        one_constant.replace(QRegExp("k0_fai_s_a[ ][\[][0-9 -]{1,}[]][ ]"),QString(""));
        ui->k0faisa_lineEdit->setText(one_constant);

        //k0_fai_s_c
        QRegularExpression baudExpression18("k0_fai_s_c[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}");
        baudMatch = baudExpression18.match(fileString);

        if(baudMatch.hasMatch()){
            one_constant=baudMatch.capturedRef().toString();
        }

        one_constant.replace(QRegExp("k0_fai_s_c[ ][\[][0-9 -]{1,}[]][ ]"),QString(""));
        ui->k0faisc_lineEdit->setText(one_constant);

        //D0_Ce
        QRegularExpression baudExpression19("D0_Ce[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}");
        baudMatch = baudExpression19.match(fileString);

        if(baudMatch.hasMatch()){
            one_constant=baudMatch.capturedRef().toString();
        }

        one_constant.replace(QRegExp("D0_Ce[ ][\[][0-9 -]{1,}[]][ ]"),QString(""));
        ui->d0ce_lineEdit->setText(one_constant);

        //tNo
        QRegularExpression baudExpression20("tNo[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}");
        baudMatch = baudExpression20.match(fileString);

        if(baudMatch.hasMatch()){
            one_constant=baudMatch.capturedRef().toString();
        }

        one_constant.replace(QRegExp("tNo[ ][\[][0-9 -]{1,}[]][ ]"),QString(""));
        ui->tno_lineEdit->setText(one_constant);

        //Ds_a
        QRegularExpression baudExpression21("Ds_a[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}");
        baudMatch = baudExpression21.match(fileString);

        if(baudMatch.hasMatch()){
            one_constant=baudMatch.capturedRef().toString();
        }

        one_constant.replace(QRegExp("Ds_a[ ][\[][0-9 -]{1,}[]][ ]"),QString(""));
        ui->dsa_lineEdit->setText(one_constant);

        //Ds_c
        QRegularExpression baudExpression22("Ds_c[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}");
        baudMatch = baudExpression22.match(fileString);

        if(baudMatch.hasMatch()){
            one_constant=baudMatch.capturedRef().toString();
        }

        one_constant.replace(QRegExp("Ds_c[ ][\[][0-9 -]{1,}[]][ ]"),QString(""));
        ui->dsc_lineEdit->setText(one_constant);

        //r_s_a
        QRegularExpression baudExpression23("r_s_a[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}");
        baudMatch = baudExpression23.match(fileString);

        if(baudMatch.hasMatch()){
            one_constant=baudMatch.capturedRef().toString();
        }

        one_constant.replace(QRegExp("r_s_a[ ][\[][0-9 -]{1,}[]][ ]"),QString(""));
        ui->rsa_lineEdit->setText(one_constant);

        //r_s_c
        QRegularExpression baudExpression24("r_s_c[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}");
        baudMatch = baudExpression24.match(fileString);

        if(baudMatch.hasMatch()){
            one_constant=baudMatch.capturedRef().toString();
        }

        one_constant.replace(QRegExp("r_s_c[ ][\[][0-9 -]{1,}[]][ ]"),QString(""));
        ui->rsc_lineEdit->setText(one_constant);

        //IApp
        QRegularExpression baudExpression25("I_app[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}");
        baudMatch = baudExpression25.match(fileString);

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
        one_constant = QString::number(I_float2);
        ui->I_lineEdit->setText(one_constant);

    }
    paramFile2.close();

    //set initial value from 0init_anode.H
    QFile paramFileMaterial(MainWindow::project_path+"/"+MainWindow::project_name+"/fullCellFoam/solid/0init_anode.H");

    if(paramFileMaterial.open(QFile::ReadOnly | QFile::Text)){
        QTextStream fileStream(&paramFileMaterial);
        QString fileString = fileStream.readAll();


        //set initial material
        QRegularExpression baudExpression("//#include [\"]0init_Gr.H[\"]");
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

    //set initial value from anode-ce
    QFile paramFile3(MainWindow::project_path+"/"+MainWindow::project_name+"/case/0/anode/Ce");

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
        ui->anode_ce_lineEdit->setText(initial_ce);
    }
    paramFile3.close();

    //set initial value from anode-cs
    QFile paramFile4(MainWindow::project_path+"/"+MainWindow::project_name+"/case/0/anode/Cs");

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
        ui->anode_cs_lineEdit->setText(initial_cs);
    }
    paramFile4.close();

    //set initial value from anode-faie
    QFile paramFile5(MainWindow::project_path+"/"+MainWindow::project_name+"/case/0/anode/fai_e");

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
        ui->anode_faie_lineEdit->setText(initial_faie);
    }
    paramFile5.close();

    //set initial value from WE-fais
    QFile paramFile6(MainWindow::project_path+"/"+MainWindow::project_name+"/case/0/anode/fai_s");

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
        ui->anode_fais_lineEdit->setText(initial_fais);
    }
    paramFile6.close();

    //set initial value from cathode-ce
    QFile paramFile7(MainWindow::project_path+"/"+MainWindow::project_name+"/case/0/cathode/Ce");

    if(paramFile7.open(QFile::ReadOnly | QFile::Text)){
        QTextStream fileStream(&paramFile7);
        QString fileString = fileStream.readAll();

        //set initial ce
        QRegularExpressionMatch baudMatch = baudExpression_csce_uniform.match(fileString);

        QString initial_ce;
        if(baudMatch.hasMatch()){
            initial_ce=baudMatch.capturedRef().toString();
        }

        initial_ce.replace(QRegExp("internalField[ ]{1,}uniform[ ]{1,}"),QString(""));
        ui->cathode_ce_lineEdit->setText(initial_ce);
    }
    paramFile7.close();

    //set initial value from cathode-cs
    QFile paramFile8(MainWindow::project_path+"/"+MainWindow::project_name+"/case/0/cathode/Cs");

    if(paramFile8.open(QFile::ReadOnly | QFile::Text)){
        QTextStream fileStream(&paramFile8);
        QString fileString = fileStream.readAll();

        //set initial cs
        QRegularExpressionMatch baudMatch = baudExpression_csce_uniform.match(fileString);

        QString initial_cs;
        if(baudMatch.hasMatch()){
            initial_cs=baudMatch.capturedRef().toString();
        }

        initial_cs.replace(QRegExp("internalField[ ]{1,}uniform[ ]{1,}"),QString(""));
        ui->cathode_cs_lineEdit->setText(initial_cs);
    }
    paramFile8.close();

    //set initial value from cathode-faie
    QFile paramFile9(MainWindow::project_path+"/"+MainWindow::project_name+"/case/0/cathode/fai_e");

    if(paramFile9.open(QFile::ReadOnly | QFile::Text)){
        QTextStream fileStream(&paramFile9);
        QString fileString = fileStream.readAll();

        //set initial cs
        QRegularExpressionMatch baudMatch = baudExpression_csce_uniform.match(fileString);

        QString initial_faie;
        if(baudMatch.hasMatch()){
            initial_faie=baudMatch.capturedRef().toString();
        }

        initial_faie.replace(QRegExp("internalField[ ]{1,}uniform[ ]{1,}"),QString(""));
        ui->cathode_faie_lineEdit->setText(initial_faie);
    }
    paramFile9.close();

    //set initial value from WE-fais
    QFile paramFile10(MainWindow::project_path+"/"+MainWindow::project_name+"/case/0/cathode/fai_s");

    if(paramFile10.open(QFile::ReadOnly | QFile::Text)){
        QTextStream fileStream(&paramFile10);
        QString fileString = fileStream.readAll();

        //set initial cs
        QRegularExpressionMatch baudMatch = baudExpression_csce_uniform.match(fileString);

        QString initial_fais;
        if(baudMatch.hasMatch()){
            initial_fais=baudMatch.capturedRef().toString();
        }

        initial_fais.replace(QRegExp("internalField[ ]{1,}uniform[ ]{1,}"),QString(""));
        ui->cathode_fais_lineEdit->setText(initial_fais);
    }
    paramFile10.close();

    //set initial value from sep-ce
    QFile paramFile11(MainWindow::project_path+"/"+MainWindow::project_name+"/case/0/seperator/Ce");

    if(paramFile11.open(QFile::ReadOnly | QFile::Text)){
        QTextStream fileStream(&paramFile11);
        QString fileString = fileStream.readAll();

        //set initial sep-ce
        QRegularExpressionMatch baudMatch = baudExpression_csce_uniform.match(fileString);

        QString initial_ce;
        if(baudMatch.hasMatch()){
            initial_ce=baudMatch.capturedRef().toString();
        }

        initial_ce.replace(QRegExp("internalField[ ]{1,}uniform[ ]{1,}"),QString(""));
        ui->sep_ce_lineEdit->setText(initial_ce);
    }
    paramFile11.close();

    //set initial value from sep-faie
    QFile paramFile12(MainWindow::project_path+"/"+MainWindow::project_name+"/case/0/seperator/fai_e");

    if(paramFile12.open(QFile::ReadOnly | QFile::Text)){
        QTextStream fileStream(&paramFile12);
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
    paramFile12.close();

    //set fucntion interface initial value
    QFile paramFile_function(MainWindow::project_path+"/"+MainWindow::project_name+"/case/system/fvSchemes");

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
    QFile paramFile13(MainWindow::project_path+"/"+MainWindow::project_name+"/case/system/fvSolution");

    if(paramFile13.open(QFile::ReadOnly | QFile::Text)){
        QTextStream fileStream(&paramFile13);
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
    paramFile13.close();

    //set control(others) interface value
    QFile paramFile14(MainWindow::project_path+"/"+MainWindow::project_name+"/case/system/controlDict");

    if(paramFile14.open(QFile::ReadOnly | QFile::Text)){
        QTextStream fileStream(&paramFile14);
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

FullCellFoam::~FullCellFoam()
{
    if(m_process_bash)
    {
        m_process_bash->terminate();
        m_process_bash->waitForFinished();
    }
    delete ui;
}

//*******************************function difinition************************//
void FullCellFoam::readBashStandardOutputInfo()
{
    QByteArray cmdout = m_process_bash->readAllStandardOutput();
    if(!cmdout.isEmpty()){
        ui->terminal_output_window->append(QString::fromLocal8Bit(cmdout));
    }
    QScrollBar* scroll = ui->terminal_output_window->verticalScrollBar();
    scroll->setSliderPosition(scroll->maximum());
}

//This function is to output errors on textEdit
void FullCellFoam::readBashStandardErrorInfo()
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
void FullCellFoam::on_change_geometry_button_clicked()
{
    float length = ui->length_lineEdit->text().toFloat();
    float length2 = ui->length2_lineEdit->text().toFloat();
    float length3 = ui->length3_lineEdit->text().toFloat();
    float lengthTwoParts = length+length2;
    float lengthThreeParts = length+length2+length3;

    QString Length = QString::number(length);
    QString Length2 = QString::number(length2);
    QString Length3 = QString::number(length3);
    QString LengthTwoParts = QString::number(lengthTwoParts);
    QString LengthThreeParts = QString::number(lengthThreeParts);
    QString Width = ui->width_lineEdit->text();
    QString Height = ui->height_lineEdit->text();

    //acquire the unit
    QString unit = ui->unit_select_box->currentText();

    //**************************for blockmesh module****************************//
    QFile file(MainWindow::project_path+"/"+MainWindow::project_name+"/case/system/blockMeshDict");
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
        str.replace(QRegExp("[0-9.]{1,}[ ][0-9.]{1,}[ ][0-9.]{1,}[)][/]{2}al node2"),
                    QString("0 "+Width+" 0)//al node2"));

        str.replace(QRegExp("[0-9.]{1,}[ ][0-9.]{1,}[ ][0-9.]{1,}[)][/]{2}al node3"),
                    QString("0 "+Width+" "+Height+")//al node3"));

        str.replace(QRegExp("[0-9.]{1,}[ ][0-9.]{1,}[ ][0-9.]{1,}[)][/]{2}al node4"),
                    QString("0 0 "+Height+")//al node4"));

        //interface2
        str.replace(QRegExp("[0-9.]{1,}[ ][0-9.]{1,}[ ][0-9.]{1,}[)][/]{2}separator_left node1"),
                    QString(Length+" 0 0"+")//separator_left node1"));

        str.replace(QRegExp("[0-9.]{1,}[ ][0-9.]{1,}[ ][0-9.]{1,}[)][/]{2}sl node2"),
                    QString(Length+" "+Width+" 0)//sl node2"));

        str.replace(QRegExp("[0-9.]{1,}[ ][0-9.]{1,}[ ][0-9.]{1,}[)][/]{2}sl node3"),
                    QString(Length+" "+Width+" "+Height+")//sl node3"));

        str.replace(QRegExp("[0-9.]{1,}[ ][0-9.]{1,}[ ][0-9.]{1,}[)][/]{2}sl node4"),
                    QString(Length+" "+"0"+" "+Height+")//sl node4"));

        //interface3
        str.replace(QRegExp("[0-9.]{1,}[ ][0-9.]{1,}[ ][0-9.]{1,}[)][/]{2}separator_right node1"),
                    QString(LengthTwoParts+" 0 0"+")//separator_right node1"));

        str.replace(QRegExp("[0-9.]{1,}[ ][0-9.]{1,}[ ][0-9.]{1,}[)][/]{2}sr node2"),
                    QString(LengthTwoParts+" "+Width+" 0)//sr node2"));

        str.replace(QRegExp("[0-9.]{1,}[ ][0-9.]{1,}[ ][0-9.]{1,}[)][/]{2}sr node3"),
                    QString(LengthTwoParts+" "+Width+" "+Height+")//sr node3"));

        str.replace(QRegExp("[0-9.]{1,}[ ][0-9.]{1,}[ ][0-9.]{1,}[)][/]{2}sr node4"),
                    QString(LengthTwoParts+" "+"0"+" "+Height+")//sr node4"));

        //interface4
        str.replace(QRegExp("[0-9.]{1,}[ ][0-9.]{1,}[ ][0-9.]{1,}[)][/]{2}cathode_right node1"),
                    QString(LengthThreeParts+" 0 0"+")//cathode_right node1"));

        str.replace(QRegExp("[0-9.]{1,}[ ][0-9.]{1,}[ ][0-9.]{1,}[)][/]{2}cr node2"),
                    QString(LengthThreeParts+" "+Width+" 0)//cr node2"));

        str.replace(QRegExp("[0-9.]{1,}[ ][0-9.]{1,}[ ][0-9.]{1,}[)][/]{2}cr node3"),
                    QString(LengthThreeParts+" "+Width+" "+Height+")//cr node3"));

        str.replace(QRegExp("[0-9.]{1,}[ ][0-9.]{1,}[ ][0-9.]{1,}[)][/]{2}cr node4"),
                    QString(LengthThreeParts+" "+"0"+" "+Height+")//cr node4"));

        //change divition on x,y,z direction
        str.replace(QRegExp("[0-9]{1,}[ ][0-9]{1,}[ ][0-9]{1,}[)][ ]simpleGrading[ ][(1 )]{1,}[/]{2}anode"),
                    QString(ui->x_divide_lineEdit->text()+" "+ui->y_divide_lineEdit->text()+
                            " "+ui->z_divide_lineEdit->text()+") simpleGrading (1 1 1)//anode"));

        str.replace(QRegExp("[0-9]{1,}[ ][0-9]{1,}[ ][0-9]{1,}[)][ ]simpleGrading[ ][(1 )]{1,}[/]{2}separator"),
                    QString(ui->x2_divide_lineEdit->text()+" "+ui->y_divide_lineEdit->text()+
                            " "+ui->z_divide_lineEdit->text()+") simpleGrading (1 1 1)//separator"));

        str.replace(QRegExp("[0-9]{1,}[ ][0-9]{1,}[ ][0-9]{1,}[)][ ]simpleGrading[ ][(1 )]{1,}[/]{2}cathode"),
                    QString(ui->x3_divide_lineEdit->text()+" "+ui->y_divide_lineEdit->text()+
                            " "+ui->z_divide_lineEdit->text()+") simpleGrading (1 1 1)//cathode"));

    }
    file.close();

    QFile::remove(MainWindow::project_path+"/"+MainWindow::project_name+"/case/system/blockMeshDict");
    if(!file.open(QIODevice::WriteOnly))
    {
         QMessageBox::warning(this,"Error","Cannot open file for Writing");
         return ;
    }

    file.write(str.toUtf8());
    file.close();

    //**************************for toposet file****************************//
    QFile file2(MainWindow::project_path+"/"+MainWindow::project_name+"/case/system/topoSetDict");
    if(!file2.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this,"Error","Cannot open file for Reading");
        return ;
    }

    QString str2 (file2.readAll());
    if(str2.contains("topoSetDict", Qt::CaseInsensitive))
    {
        if(unit=="micrometer(um)"){
            str2.replace(QRegExp("[0-9.]{1,}[e][-][0-9][ ]{1,}[0-9.]{1,}[e][-][0-9][ ]{1,}[0-9.]{1,}[e][-][0-9][);/]{1,}anode"),
                         QString(Length+"e-6   "+Width+"e-6  "+Height+"e-6);//anode"));
            str2.replace(QRegExp("[0-9.]{1,}[e][-][0-9][ ]{1,}[0][ ]{1,}[0][ ]{0,}[)][(][0-9.]{1,}[e][-][0-9][ ]{1,}[0-9.]{1,}[e][-][0-9][ ]{1,}[0-9.]{1,}[e][-][0-9][);/]{1,}separator"),
                         QString(Length+"e-6 0 0)("+LengthTwoParts+"e-6   "+Width+"e-6  "+Height+"e-6);//separator"));
            str2.replace(QRegExp("[0-9.]{1,}[e][-][0-9][ ]{1,}[0][ ]{1,}[0][ ]{0,}[)][(][0-9.]{1,}[e][-][0-9][ ]{1,}[0-9.]{1,}[e][-][0-9][ ]{1,}[0-9.]{1,}[e][-][0-9][);/]{1,}cathode"),
                         QString(LengthTwoParts+"e-6 0 0)("+LengthThreeParts+"e-6   "+Width+"e-6  "+Height+"e-6);//cathode"));

        }else if(unit=="millimeter(mm)"){
            str2.replace(QRegExp("[0-9.]{1,}[e][-][0-9][ ]{1,}[0-9.]{1,}[e][-][0-9][ ]{1,}[0-9.]{1,}[e][-][0-9][);/]{1,}anode"),
                         QString(Length+"e-3   "+Width+"e-3  "+Height+"e-3);//anode"));
            str2.replace(QRegExp("[0-9.]{1,}[e][-][0-9][ ]{1,}[0][ ]{1,}[0][ ]{0,}[)][(][0-9.]{1,}[e][-][0-9][ ]{1,}[0-9.]{1,}[e][-][0-9][ ]{1,}[0-9.]{1,}[e][-][0-9][);/]{1,}separator"),
                         QString(Length+"e-3 0 0)("+LengthTwoParts+"e-3   "+Width+"e-3  "+Height+"e-3);//separator"));
            str2.replace(QRegExp("[0-9.]{1,}[e][-][0-9][ ]{1,}[0][ ]{1,}[0][ ]{0,}[)][(][0-9.]{1,}[e][-][0-9][ ]{1,}[0-9.]{1,}[e][-][0-9][ ]{1,}[0-9.]{1,}[e][-][0-9][);/]{1,}cathode"),
                         QString(LengthTwoParts+"e-3 0 0)("+LengthThreeParts+"e-3   "+Width+"e-3  "+Height+"e-3);//cathode"));


        }else if(unit=="meter(m)"){
            str2.replace(QRegExp("[0-9.]{1,}[e][-][0-9][ ]{1,}[0-9.]{1,}[e][-][0-9][ ]{1,}[0-9.]{1,}[e][-][0-9][);/]{1,}anode"),
                         QString(Length+"e-0   "+Width+"e-0  "+Height+"e-0);//anode"));
            str2.replace(QRegExp("[0-9.]{1,}[e][-][0-9][ ]{1,}[0][ ]{1,}[0][ ]{0,}[)][(][0-9.]{1,}[e][-][0-9][ ]{1,}[0-9.]{1,}[e][-][0-9][ ]{1,}[0-9.]{1,}[e][-][0-9][);/]{1,}separator"),
                         QString(LengthTwoParts+"e-0 0 0)("+LengthTwoParts+"e-0   "+Width+"e-0  "+Height+"e-0);//separator"));
            str2.replace(QRegExp("[0-9.]{1,}[e][-][0-9][ ]{1,}[0][ ]{1,}[0][ ]{0,}[)][(][0-9.]{1,}[e][-][0-9][ ]{1,}[0-9.]{1,}[e][-][0-9][ ]{1,}[0-9.]{1,}[e][-][0-9][);/]{1,}cathode"),
                         QString(LengthTwoParts+"e-0 0 0)("+LengthThreeParts+"e-0   "+Width+"e-0  "+Height+"e-0);//cathode"));

        }
    }
    file2.close();

    QFile::remove(MainWindow::project_path+"/"+MainWindow::project_name+"/case/system/topoSetDict");
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
    QString path =MainWindow::project_path+"/"+MainWindow::project_name+"/"+"case";

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
void FullCellFoam::on_run_geometry_button_clicked()
{
    ui->tabWidget->setCurrentIndex(1);
}

// constant help button
void FullCellFoam::on_help_constant_button_clicked()
{
    QMessageBox::information(this,"hint","Csa_max: \tMaximum li concentration in anode materials\n"
                                         "Csc_max: \tMaximum li concentration in cathode materials\n"
                                         "kreact_a: \tReaction rate constant in anode\n"
                                         "kreact_c: \tReaction rate constant in cathode\n"
                                         "alphaA_a: \tAnodic transference coefficient in the anode\n"
                                         "alphaC_a: \tCathodic transference coefficient in the anode\n"
                                         "alphaA_c: \tAnodic transference coefficient in the cathode\n"
                                         "alphaC_c: \tCathodic transference coefficient in the cathode\n"
                                         "R: \tUniversal gas constant\n"
                                         "F: \tFaraday constant \n"
                                         "por_a: \tPorosity in anode\n"
                                         "por_sep: \tPorosity in seperator\n"
                                         "por_c: \tPorosity in cathode\n"
                                         "por_f_a: \tFiller fraction in anode\n"
                                         "por_f_c: \tFiller fraction in cathode\n"
                                         "brugg: \tBruggman coefficient\n"
                                         "k0_a: \tIntrinsic solid-phase conductivity in anode\n"
                                         "k0_c: \tIntrinsic solid-phase conductivity in cathode\n"
                                         "D0_e: \tIntrinsic electrolyte diffusion coefficient\n"
                                         "tNo: \tTransference number\n"
                                         "Ds_a: \tDiffusivity of Li in solid-phase of anode\n"
                                         "Ds_c: \tDiffusivity of Li in solid-phase of cathode\n"
                                         "r_a: \tParticle radius in anode\n"
                                         "r_c: \tParticle radius in cathode\n"
                                         "I_app:  \tApplied current density\n");
}

//continue the constant edit
void FullCellFoam::on_constant_cont_button_clicked()
{
    ui->tabWidget->setCurrentIndex(2);
}

//change constant parameters
void FullCellFoam::on_change_constant_button_clicked()
{
    QFile file(MainWindow::project_path+"/"+MainWindow::project_name+"/case/constant/LiProperties");
    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this,"Error","Cannot open file for Reading");
        return ;
    }

    QString str (file.readAll());
    if(str.contains("transportProperties", Qt::CaseInsensitive)){
        str.replace(QRegExp("Cs_max_a[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}"),
                    QString("Cs_max_a [0 -1 0 0 0 0 0] "+ui->csmaxa_lineEdit->text()));

        str.replace(QRegExp("Cs_max_c[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}"),
                    QString("Cs_max_c [0 -1 0 0 0 0 0] "+ui->csmaxc_lineEdit->text()));

        str.replace(QRegExp("kReact_a[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}"),
                    QString("kReact_a [0 -1 0 0 0 0 0] "+ui->kreacta_lineEdit->text()));

        str.replace(QRegExp("kReact_c[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}"),
                    QString("kReact_c [0 -1 0 0 0 0 0] "+ui->kreactc_lineEdit->text()));

        str.replace(QRegExp("alphaA_a[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}"),
                    QString("alphaA_a [0 -1 0 0 0 0 0] "+ui->alphaaa_lineEdit->text()));

        str.replace(QRegExp("alphaC_a[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}"),
                    QString("alphaC_a [0 -1 0 0 0 0 0] "+ui->alphaca_lineEdit->text()));

        str.replace(QRegExp("alphaA_c[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}"),
                    QString("alphaA_c [0 -1 0 0 0 0 0] "+ui->alphaac_lineEdit->text()));

        str.replace(QRegExp("alphaC_c[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}"),
                    QString("alphaC_c [0 -1 0 0 0 0 0] "+ui->alphacc_lineEdit->text()));

        str.replace(QRegExp("R[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}"),
                    QString("R [0 -1 0 0 0 0 0] "+ui->r_lineEdit->text()));

        str.replace(QRegExp("F[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}"),
                    QString("F [0 -1 0 0 0 0 0] "+ui->f_lineEdit->text()));

        str.replace(QRegExp("por_Ce_a[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}"),
                    QString("por_Ce_a [0 -1 0 0 0 0 0] "+ui->porcea_lineEdit->text()));

        str.replace(QRegExp("por_Ce_sp[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}"),
                    QString("por_Ce_sp [0 -1 0 0 0 0 0] "+ui->porcesp_lineEdit->text()));

        str.replace(QRegExp("por_Ce_c[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}"),
                    QString("por_Ce_c [0 -1 0 0 0 0 0] "+ui->porcec_lineEdit->text()));

        str.replace(QRegExp("por_f_a[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}"),
                    QString("por_f_a [0 -1 0 0 0 0 0] "+ui->porfa_lineEdit->text()));

        str.replace(QRegExp("por_f_c[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}"),
                    QString("por_f_c [0 -1 0 0 0 0 0] "+ui->porfc_lineEdit->text()));

        str.replace(QRegExp("brugg[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}"),
                    QString("brugg [0 -1 0 0 0 0 0] "+ui->brugg_lineEdit->text()));

        str.replace(QRegExp("k0_fai_s_a[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}"),
                    QString("k0_fai_s_a [0 -1 0 0 0 0 0] "+ui->k0faisa_lineEdit->text()));

        str.replace(QRegExp("k0_fai_s_c[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}"),
                    QString("k0_fai_s_c [0 -1 0 0 0 0 0] "+ui->k0faisc_lineEdit->text()));

        str.replace(QRegExp("D0_Ce[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}"),
                    QString("D0_Ce [0 -1 0 0 0 0 0] "+ui->d0ce_lineEdit->text()));

        str.replace(QRegExp("tNo[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}"),
                    QString("tNo [0 -1 0 0 0 0 0] "+ui->tno_lineEdit->text()));

        str.replace(QRegExp("Ds_a[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}"),
                    QString("Ds_a [0 -1 0 0 0 0 0] "+ui->dsa_lineEdit->text()));

        str.replace(QRegExp("Ds_c[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}"),
                    QString("Ds_c [0 -1 0 0 0 0 0] "+ui->dsc_lineEdit->text()));

        str.replace(QRegExp("r_s_a[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}"),
                    QString("r_s_a [0 -1 0 0 0 0 0] "+ui->rsa_lineEdit->text()));

        str.replace(QRegExp("r_s_c[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}"),
                    QString("r_s_c [0 -1 0 0 0 0 0] "+ui->rsc_lineEdit->text()));

        if(ui->select_charge->isChecked()){
            str.replace(QRegExp("I_app[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}"),
                        QString("I_app [0 -1 0 0 0 0 0] "+ui->I_lineEdit->text()));
        }else if(ui->select_discharge->isChecked()){
            str.replace(QRegExp("I_app[ ][\[][0-9 -]{1,}[]][ ][0-9.e-]{1,}"),
                        QString("I_app [0 -1 0 0 0 0 0] "+QString("-")+ui->I_lineEdit->text()));
        }
    }
    file.close();

    QFile::remove(MainWindow::project_path+"/"+MainWindow::project_name+"/case/constant/LiProperties");
    if(!file.open(QIODevice::WriteOnly))
    {
         QMessageBox::warning(this,"Error","Cannot open file for Writing");
         return ;
    }

    file.write(str.toUtf8());
    file.close();

    //change the file in 3 parts folder
    if (QFile::exists(MainWindow::project_path+"/"+MainWindow::project_name+"/case/constant/anode/LiProperties"))
    {
        QFile::remove(MainWindow::project_path+"/"+MainWindow::project_name+"/case/constant/anode/LiProperties");
    }

    QFile::copy(MainWindow::project_path+"/"+MainWindow::project_name+"/case/constant/LiProperties",
                MainWindow::project_path+"/"+MainWindow::project_name+"/case/constant/anode/LiProperties");

    if (QFile::exists(MainWindow::project_path+"/"+MainWindow::project_name+"/case/constant/cathode/LiProperties"))
    {
        QFile::remove(MainWindow::project_path+"/"+MainWindow::project_name+"/case/constant/cathode/LiProperties");
    }

    QFile::copy(MainWindow::project_path+"/"+MainWindow::project_name+"/case/constant/LiProperties",
                MainWindow::project_path+"/"+MainWindow::project_name+"/case/constant/cathode/LiProperties");

    if (QFile::exists(MainWindow::project_path+"/"+MainWindow::project_name+"/case/constant/seperator/LiProperties"))
    {
        QFile::remove(MainWindow::project_path+"/"+MainWindow::project_name+"/case/constant/seperator/LiProperties");
    }

    QFile::copy(MainWindow::project_path+"/"+MainWindow::project_name+"/case/constant/LiProperties",
                MainWindow::project_path+"/"+MainWindow::project_name+"/case/constant/seperator/LiProperties");

    //Material selection part for OCV_anode.H
    QFile fileMaterial(MainWindow::project_path+"/"+MainWindow::project_name+"/fullCellFoam/solid/OCV_anode.H");
    if(!fileMaterial.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this,"Error","Cannot open file for Reading");
        return ;
    }

    QString strMaterial (fileMaterial.readAll());
    if(strMaterial.contains("OCV_Si.H", Qt::CaseInsensitive)){
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

    QFile::remove(MainWindow::project_path+"/"+MainWindow::project_name+"/fullCellFoam/solid/OCV_anode.H");
    if(!fileMaterial.open(QIODevice::WriteOnly))
    {
         QMessageBox::warning(this,"Error","Cannot open file for Writing");
         return ;
    }

    fileMaterial.write(strMaterial.toUtf8());
    fileMaterial.close();

    //Material selection part for 0init_anode.H
    QFile fileMaterial2(MainWindow::project_path+"/"+MainWindow::project_name+"/fullCellFoam/solid/0init_anode.H");
    if(!fileMaterial2.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this,"Error","Cannot open file for Reading");
        return ;
    }

    QString strMaterial2 (fileMaterial2.readAll());
    if(strMaterial2.contains("0init_Si.H", Qt::CaseInsensitive)){
        QRegularExpression baudExpression1("//#include [\"]0init_Gr.H[\"]");
        QRegularExpressionMatch baudMatch1 = baudExpression1.match(strMaterial2);
        bool alreadyCommitGr= baudMatch1.hasMatch();

        QRegularExpression baudExpression2("//#include [\"]0init_Si.H[\"]");
        QRegularExpressionMatch baudMatch2 = baudExpression2.match(strMaterial2);
        bool alreadyCommitSi= baudMatch2.hasMatch();

        if(ui->select_carbon->isChecked()){
            strMaterial2.replace(QRegExp("//#include [\"]0init_Gr.H[\"]"),
                        QString("#include \"0init_Gr.H\""));
            if(!alreadyCommitSi){
                strMaterial2.replace(QRegExp("#include [\"]0init_Si.H[\"]"),
                            QString("//#include \"0init_Si.H\""));
            }
        }else{
            if(!alreadyCommitGr){
                strMaterial2.replace(QRegExp("#include [\"]0init_Gr.H[\"]"),
                            QString("//#include \"0init_Gr.H\""));
            }
            strMaterial2.replace(QRegExp("//#include [\"]0init_Si.H[\"]"),
                        QString("#include \"0init_Si.H\""));
        }
    }
    fileMaterial2.close();

    QFile::remove(MainWindow::project_path+"/"+MainWindow::project_name+"/fullCellFoam/solid/0init_anode.H");
    if(!fileMaterial2.open(QIODevice::WriteOnly))
    {
         QMessageBox::warning(this,"Error","Cannot open file for Writing");
         return ;
    }

    fileMaterial2.write(strMaterial2.toUtf8());
    fileMaterial2.close();

    QMessageBox::information(this,"title","Change successfully");

}

//from constant interface to next page
void FullCellFoam::on_run_constant_button_clicked()
{
    ui->tabWidget->setCurrentIndex(3);
}

//change boundary parameters
void FullCellFoam::on_change_boundary_button_clicked()
{
    //anode-ce
    QFile file(MainWindow::project_path+"/"+MainWindow::project_name+"/case/0/anode/Ce");
    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this,"Error","Cannot open file for Reading");
        return ;
    }

    QString str (file.readAll());
    if(str.contains("Ce", Qt::CaseInsensitive)){
        str.replace(QRegExp("internalField[ ]{1,}uniform[ ]{1,}[0-9.e-]{1,}"),
                    QString("internalField   uniform   "+ui->anode_ce_lineEdit->text()));
    }
    file.close();

    QFile::remove(MainWindow::project_path+"/"+MainWindow::project_name+"/case/0/anode/Ce");
    if(!file.open(QIODevice::WriteOnly))
    {
         QMessageBox::warning(this,"Error","Cannot open file for Writing");
         return ;
    }

    file.write(str.toUtf8());
    file.close();

    //anode-cs
    QFile file2(MainWindow::project_path+"/"+MainWindow::project_name+"/case/0/anode/Cs");
    if(!file2.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this,"Error","Cannot open file for Reading");
        return ;
    }

    QString str2 (file2.readAll());
    if(str2.contains("Cs", Qt::CaseInsensitive)){
        str2.replace(QRegExp("internalField[ ]{1,}uniform[ ]{1,}[0-9.e-]{1,}"),
                    QString("internalField   uniform   "+ui->anode_cs_lineEdit->text()));
    }
    file2.close();

    QFile::remove(MainWindow::project_path+"/"+MainWindow::project_name+"/case/0/anode/Cs");
    if(!file2.open(QIODevice::WriteOnly))
    {
         QMessageBox::warning(this,"Error","Cannot open file for Writing");
         return ;
    }

    file2.write(str2.toUtf8());
    file2.close();

    //anode-fai-e
    QFile file3(MainWindow::project_path+"/"+MainWindow::project_name+"/case/0/anode/fai_e");
    if(!file3.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this,"Error","Cannot open file for Reading");
        return ;
    }

    QString str3 (file3.readAll());
    if(str3.contains("fai_e", Qt::CaseInsensitive)){
        str3.replace(QRegExp("internalField[ ]{1,}uniform[ ]{1,}[0-9.e-]{1,}"),
                    QString("internalField   uniform   "+ui->anode_faie_lineEdit->text()));
    }
    file3.close();

    QFile::remove(MainWindow::project_path+"/"+MainWindow::project_name+"/case/0/anode/fai_e");
    if(!file3.open(QIODevice::WriteOnly))
    {
         QMessageBox::warning(this,"Error","Cannot open file for Writing");
         return ;
    }

    file3.write(str3.toUtf8());
    file3.close();

    //anode-fai-s
    QFile file4(MainWindow::project_path+"/"+MainWindow::project_name+"/case/0/anode/fai_s");
    if(!file4.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this,"Error","Cannot open file for Reading");
        return ;
    }

    QString str4 (file4.readAll());
    if(str4.contains("fai_s", Qt::CaseInsensitive)){
        str4.replace(QRegExp("internalField[ ]{1,}uniform[ ]{1,}[0-9.e-]{1,}"),
                    QString("internalField   uniform   "+ui->anode_fais_lineEdit->text()));
    }
    file4.close();

    QFile::remove(MainWindow::project_path+"/"+MainWindow::project_name+"/case/0/anode/fai_s");
    if(!file4.open(QIODevice::WriteOnly))
    {
         QMessageBox::warning(this,"Error","Cannot open file for Writing");
         return ;
    }

    file4.write(str4.toUtf8());
    file4.close();

    //cathode-ce
    QFile file5(MainWindow::project_path+"/"+MainWindow::project_name+"/case/0/cathode/Ce");
    if(!file5.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this,"Error","Cannot open file for Reading");
        return ;
    }

    QString str5 (file5.readAll());
    if(str5.contains("Ce", Qt::CaseInsensitive)){
        str5.replace(QRegExp("internalField[ ]{1,}uniform[ ]{1,}[0-9.e-]{1,}"),
                    QString("internalField   uniform   "+ui->cathode_ce_lineEdit->text()));
    }
    file5.close();

    QFile::remove(MainWindow::project_path+"/"+MainWindow::project_name+"/case/0/cathode/Ce");
    if(!file5.open(QIODevice::WriteOnly))
    {
         QMessageBox::warning(this,"Error","Cannot open file for Writing");
         return ;
    }

    file5.write(str5.toUtf8());
    file5.close();

    //cathode-cs
    QFile file6(MainWindow::project_path+"/"+MainWindow::project_name+"/case/0/cathode/Cs");
    if(!file6.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this,"Error","Cannot open file for Reading");
        return ;
    }

    QString str6 (file6.readAll());
    if(str6.contains("Cs", Qt::CaseInsensitive)){
        str6.replace(QRegExp("internalField[ ]{1,}uniform[ ]{1,}[0-9.e-]{1,}"),
                    QString("internalField   uniform   "+ui->cathode_cs_lineEdit->text()));
    }
    file6.close();

    QFile::remove(MainWindow::project_path+"/"+MainWindow::project_name+"/case/0/cathode/Cs");
    if(!file2.open(QIODevice::WriteOnly))
    {
         QMessageBox::warning(this,"Error","Cannot open file for Writing");
         return ;
    }

    file6.write(str2.toUtf8());
    file6.close();

    //cathode-fai-e
    QFile file7(MainWindow::project_path+"/"+MainWindow::project_name+"/case/0/cathode/fai_e");
    if(!file7.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this,"Error","Cannot open file for Reading");
        return ;
    }

    QString str7 (file7.readAll());
    if(str7.contains("fai_e", Qt::CaseInsensitive)){
        str7.replace(QRegExp("internalField[ ]{1,}uniform[ ]{1,}[0-9.e-]{1,}"),
                    QString("internalField   uniform   "+ui->cathode_faie_lineEdit->text()));
    }
    file7.close();

    QFile::remove(MainWindow::project_path+"/"+MainWindow::project_name+"/case/0/cathode/fai_e");
    if(!file7.open(QIODevice::WriteOnly))
    {
         QMessageBox::warning(this,"Error","Cannot open file for Writing");
         return ;
    }

    file7.write(str7.toUtf8());
    file7.close();

    //anode-fai-s
    QFile file8(MainWindow::project_path+"/"+MainWindow::project_name+"/case/0/cathode/fai_s");
    if(!file8.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this,"Error","Cannot open file for Reading");
        return ;
    }

    QString str8 (file8.readAll());
    if(str8.contains("fai_s", Qt::CaseInsensitive)){
        str8.replace(QRegExp("internalField[ ]{1,}uniform[ ]{1,}[0-9.e-]{1,}"),
                    QString("internalField   uniform   "+ui->cathode_fais_lineEdit->text()));
    }
    file8.close();

    QFile::remove(MainWindow::project_path+"/"+MainWindow::project_name+"/case/0/cathode/fai_s");
    if(!file8.open(QIODevice::WriteOnly))
    {
         QMessageBox::warning(this,"Error","Cannot open file for Writing");
         return ;
    }

    file8.write(str8.toUtf8());
    file8.close();

    //sep-ce
    QFile file9(MainWindow::project_path+"/"+MainWindow::project_name+"/case/0/seperator/Ce");
    if(!file5.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this,"Error","Cannot open file for Reading");
        return ;
    }

    QString str9 (file9.readAll());
    if(str9.contains("Ce", Qt::CaseInsensitive)){
        str9.replace(QRegExp("internalField[ ]{1,}uniform[ ]{1,}[0-9.e-]{1,}"),
                    QString("internalField   uniform   "+ui->sep_ce_lineEdit->text()));
    }
    file9.close();

    QFile::remove(MainWindow::project_path+"/"+MainWindow::project_name+"/case/0/seperator/Ce");
    if(!file9.open(QIODevice::WriteOnly))
    {
         QMessageBox::warning(this,"Error","Cannot open file for Writing");
         return ;
    }

    file9.write(str9.toUtf8());
    file9.close();

    //sep-fai-e
    QFile file10(MainWindow::project_path+"/"+MainWindow::project_name+"/case/0/seperator/fai_e");
    if(!file10.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this,"Error","Cannot open file for Reading");
        return ;
    }

    QString str10 (file10.readAll());
    if(str10.contains("fai_e", Qt::CaseInsensitive)){
        str10.replace(QRegExp("internalField[ ]{1,}uniform[ ]{1,}[0-9.e-]{1,}"),
                    QString("internalField   uniform   "+ui->sep_faie_lineEdit->text()));
    }
    file10.close();

    QFile::remove(MainWindow::project_path+"/"+MainWindow::project_name+"/case/0/seperatpr/fai_e");
    if(!file10.open(QIODevice::WriteOnly))
    {
         QMessageBox::warning(this,"Error","Cannot open file for Writing");
         return ;
    }

    file10.write(str10.toUtf8());
    file10.close();

    QMessageBox::information(this,"title","Change successfully");
}

//from boundary interface to next page
void FullCellFoam::on_run_boundary_button_clicked()
{
    ui->tabWidget->setCurrentIndex(4);
}

//change function parameters
void FullCellFoam::on_change_function_button_clicked()
{
    QFile file(MainWindow::project_path+"/"+MainWindow::project_name+"/case/system/fvSchemes");
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

    QFile::remove(MainWindow::project_path+"/"+MainWindow::project_name+"/case/system/fvSchemes");
    if(!file.open(QIODevice::WriteOnly))
    {
         QMessageBox::warning(this,"Error","Cannot open file for Writing");
         return ;
    }

    file.write(str.toUtf8());
    file.close();

    //change the file in 3 parts folder
    if (QFile::exists(MainWindow::project_path+"/"+MainWindow::project_name+"/case/system/anode/fvSchemes"))
    {
        QFile::remove(MainWindow::project_path+"/"+MainWindow::project_name+"/case/system/anode/fvSchemes");
    }

    QFile::copy(MainWindow::project_path+"/"+MainWindow::project_name+"/case/system/fvSchemes",
                MainWindow::project_path+"/"+MainWindow::project_name+"/case/system/anode/fvSchemes");

    if (QFile::exists(MainWindow::project_path+"/"+MainWindow::project_name+"/case/system/cathode/fvSchemes"))
    {
        QFile::remove(MainWindow::project_path+"/"+MainWindow::project_name+"/case/system/cathode/fvSchemes");
    }

    QFile::copy(MainWindow::project_path+"/"+MainWindow::project_name+"/case/system/fvSchemes",
                MainWindow::project_path+"/"+MainWindow::project_name+"/case/system/cathode/fvSchemes");

    if (QFile::exists(MainWindow::project_path+"/"+MainWindow::project_name+"/case/system/seperator/fvSchemes"))
    {
        QFile::remove(MainWindow::project_path+"/"+MainWindow::project_name+"/case/system/seperator/fvSchemes");
    }

    QFile::copy(MainWindow::project_path+"/"+MainWindow::project_name+"/case/system/fvSchemes",
                MainWindow::project_path+"/"+MainWindow::project_name+"/case/system/seperator/fvSchemes");

    QMessageBox::information(this,"title","Change successfully");

}

//from function interface to next page
void FullCellFoam::on_run_function_button_clicked()
{
    ui->tabWidget->setCurrentIndex(5);
}

//change tolerance and other control parameters
void FullCellFoam::on_change_control_button_clicked()
{
    //change fvsolution file in sep and WE folder
   QFile file(MainWindow::project_path+"/"+MainWindow::project_name+"/case/system/fvSolution");
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

   QFile::remove(MainWindow::project_path+"/"+MainWindow::project_name+"/case/system/fvSolution");
   if(!file.open(QIODevice::WriteOnly))
   {
        QMessageBox::warning(this,"Error","Cannot open file for Writing");
        return ;
   }

   file.write(str.toUtf8());
   file.close();


   if (QFile::exists(MainWindow::project_path+"/"+MainWindow::project_name+"/case/system/anode/fvSolution"))
   {
       QFile::remove(MainWindow::project_path+"/"+MainWindow::project_name+"/case/system/anode/fvSolution");
   }

   QFile::copy(MainWindow::project_path+"/"+MainWindow::project_name+"/case/system/fvSolution",
               MainWindow::project_path+"/"+MainWindow::project_name+"/case/system/anode/fvSolution");

   if (QFile::exists(MainWindow::project_path+"/"+MainWindow::project_name+"/case/system/cathode/fvSolution"))
   {
       QFile::remove(MainWindow::project_path+"/"+MainWindow::project_name+"/case/system/cathode/fvSolution");
   }

   QFile::copy(MainWindow::project_path+"/"+MainWindow::project_name+"/case/system/fvSolution",
               MainWindow::project_path+"/"+MainWindow::project_name+"/case/system/cathode/fvSolution");

   if (QFile::exists(MainWindow::project_path+"/"+MainWindow::project_name+"/case/system/seperator/fvSolution"))
   {
       QFile::remove(MainWindow::project_path+"/"+MainWindow::project_name+"/case/system/seperator/fvSolution");
   }

   QFile::copy(MainWindow::project_path+"/"+MainWindow::project_name+"/case/system/fvSolution",
               MainWindow::project_path+"/"+MainWindow::project_name+"/case/system/seperator/fvSolution");

   //change control parameters
   QFile file2(MainWindow::project_path+"/"+MainWindow::project_name+"/case/system/controlDict");
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

   QFile::remove(MainWindow::project_path+"/"+MainWindow::project_name+"/case/system/controlDict");
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
void FullCellFoam::on_run_button_clicked()
{
    if(delete_flag==1){
        //remove folders
        QString folderPath=MainWindow::project_path+"/"+MainWindow::project_name+"/case";

        QDir dir(folderPath);
        QStringList names = dir.entryList(QDir::Dirs);
        QDir dir2;

        names.removeOne(".");
        names.removeOne("..");

        QStringList valid_list;
        valid_list<<"0"<<"constant"<<"system"<<"0_CV"<<"0_org";

        for(QString s: names){
            if(!valid_list.contains(s) && QDir(folderPath+"/"+s).exists()){
                dir2.setPath(folderPath+"/"+s);
                dir2.removeRecursively();
            }
        }

        //remove file(s)
        QFileInfo file(MainWindow::project_path+"/"+MainWindow::project_name+"/case/time_voltage");
        if(file.exists()){
            QFile::remove(MainWindow::project_path+"/"+MainWindow::project_name+"/case/time_voltage");
        }

        QEventLoop eventloop2;
        QTimer::singleShot(1000, &eventloop2, SLOT(quit()));
        eventloop2.exec();

        //wclean and wmake
        QString path =MainWindow::project_path+"/"+MainWindow::project_name+"/"+"fullCellFoam";

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


    QString path2 =MainWindow::project_path+"/"+MainWindow::project_name+"/"+"case";

    ui->command_input_lineEdit->setText("cd /"+path2+" && "+MainWindow::project_name);
    m_process_bash->write(ui->command_input_lineEdit->text().toLocal8Bit()+'\n');
    ui->command_input_lineEdit->clear();

    ui->view_result_button->setEnabled(true);
    delete_flag=1;
}

//pause the run process
void FullCellFoam::on_pause_run_button_clicked()
{
    //stop the program
    QString str ="pkill -f "+QString(MainWindow::project_name);
    QByteArray byteArray = str.toUtf8();
    char* data = byteArray.data();

    system(data);
    delete_flag=0;
}

//go back to the main window
void FullCellFoam::on_full_return_Button_clicked()
{
    emit ExitWin();
    close();
}

//command test button
void FullCellFoam::on_pushButton_clicked()
{
     QString strCmd = ui->command_input_lineEdit->text();
     ui->terminal_output_window->append("Linux:~$ "+strCmd);
     m_process_bash->write(ui->command_input_lineEdit->text().toLocal8Bit()+'\n');
     ui->command_input_lineEdit->clear();
}

//paraview
void FullCellFoam::on_open_paraview_Button_clicked()
{
    QString path =MainWindow::project_path+"/"+MainWindow::project_name+"/"+"case";

    ui->command_input_lineEdit->setText("cd /"+path+" && "+"paraview &");
    ui->terminal_output_window->append("Linux:~$ paraview &");
    m_process_bash->write(ui->command_input_lineEdit->text().toLocal8Bit()+'\n');
    ui->command_input_lineEdit->clear();
}

//view the results
void FullCellFoam::on_view_result_button_clicked()
{
    resultInterface = new ResultInterface(this);
    resultInterface->show();
}
