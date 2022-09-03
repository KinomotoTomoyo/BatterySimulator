#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QFile>
#include <QFileDialog>
#include <QDir>
#include <QDebug>
#include <QDirIterator>
#include <QMessageBox>
#include <QRegExpValidator>
#include <QCoreApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //set window title
    QWidget::setWindowTitle ( "BatteryFOAM" );

    //disable border of the introduction frame
    ui->intro_browser->setFrameStyle(QFrame::NoFrame);

    //choose carbon module by default
    ui->carbon_button->setChecked(true);

    //set the left battery picture
    QPixmap pix(":/resources/img/lithium-ion-battery-vertical.png");
    int w = ui->label_pic_vertical->width();
    int h = ui->label_pic_vertical->height();
    ui->label_pic_vertical->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));

    //constrain the project name form
    QRegExp regx("^[a-zA-Z0-9_]+$");
    ui->pro_name_editline->setValidator( new QRegExpValidator(regx, this) );
}

MainWindow::~MainWindow()
{
    delete ui;
}

//variable definition
QString MainWindow::project_path;

QString MainWindow::project_name;

//fucntion definition
//copy function
void MainWindow::copyAndReplaceFolderContents(const QString &fromDir, const QString &toDir, bool copyAndRemove) {
    QDirIterator it(fromDir, QDirIterator::Subdirectories);
    QDir dir(fromDir);
    const int absSourcePathLength = dir.absoluteFilePath(fromDir).length();

    while (it.hasNext()){
        it.next();
        const auto fileInfo = it.fileInfo();
        if(!fileInfo.isHidden()) { //filters dot and dotdot
            const QString subPathStructure = fileInfo.absoluteFilePath().mid(absSourcePathLength);
            const QString constructedAbsolutePath = toDir + subPathStructure;

            if(fileInfo.isDir()){
                //Create directory in target folder
                dir.mkpath(constructedAbsolutePath);
            } else if(fileInfo.isFile()) {
                //Copy File to target directory

                //Remove file at target location, if it exists, or QFile::copy will fail
                QFile::remove(constructedAbsolutePath);
                QFile::copy(fileInfo.absoluteFilePath(), constructedAbsolutePath);
            }
        }
    }

    if(copyAndRemove)
        dir.removeRecursively();
}

//change folder name
void MainWindow::changeFolderName(const QString &Name1, const QString &Name2){
    QFile::rename(Name1,Name2);
}

//change make-file
void MainWindow::changeMakeFile(const QString &suffix, const QString &module){

    QString file_path = project_path+suffix;
    QFile file(file_path);

    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this,"Error","Cannot open file for Reading");
        return ;
    }

    QString str (file.readAll());
    if(str.contains("fluid", Qt::CaseInsensitive))
    {
        if(module=="SPM"){
            str.replace(QRegExp("/SPMFoam_OF6"),QString("/")+QString(project_name));
        }else if(module=="halfCell"){
            str.replace(QRegExp("/halfCellFoam_OF6"),QString("/")+QString(project_name));
        }else if(module=="fullCell"){
            str.replace(QRegExp("/fullCellFoam_OF6"),QString("/")+QString(project_name));
        }
    }
    file.close();

    QFile::remove(file_path);
    if(!file.open(QIODevice::WriteOnly))
    {
         QMessageBox::warning(this,"Error","Cannot open file for Writing");
         return ;
    }

    file.write(str.toUtf8());
    file.close();
    QMessageBox::information(this,"Info","Create successfully");
}

//***********************New interface*******************************//
void MainWindow::on_main_path_button_clicked()
{
    project_path = QFileDialog::getExistingDirectory(
                this, "Choose a position",
                "/home");
    ui->main_path_label->setText(project_path);
    if(ui->main_path_label->text().isEmpty()){
        QMessageBox::information(this,"hint","Path should not be empty");
        ui->main_next_button->setEnabled(false);
    }else{
        ui->main_next_button->setEnabled(true);
        ui->main_next_button_2->setEnabled(false);
    }
}

void MainWindow::on_main_next_button_clicked()
{
    project_name =ui->pro_name_editline->text();
    if(ui->pro_name_editline->text().isEmpty()){
        QMessageBox::information(this,"hint","Project name should not be empty");
        return ;
    }

    QString complete_project_info=project_path+"/"+project_name;

    QString current_path = QCoreApplication::applicationDirPath();

    QString recent_file_path = QCoreApplication::applicationDirPath()+"/OpenfoamModule/most_recent_file";

    QFile file2(recent_file_path);

    QFile::remove(recent_file_path);
    if(!file2.open(QIODevice::WriteOnly))
    {
         QMessageBox::warning(this,"Error","Cannot open file for Writing");
         return ;
    }

    file2.write(complete_project_info.toUtf8());
    file2.close();


    QFile file(complete_project_info);

    if(file.exists())
    {
      QMessageBox::warning(this,"BatteryFOAM","Cannot create the folder, because a file or folder with that name already exists");
    }else{
        if(ui->carbon_button->isChecked()){
            copyAndReplaceFolderContents(current_path+"/OpenfoamModule/SPMFoam",project_path,false);
            changeFolderName(project_path+"/SPMFoam",project_path+"/"+project_name);
            changeMakeFile("/"+project_name+"/SPMFoam/Make/files","SPM");

            this->hide();
            carbonInterface = new CarbonInterface(this);
            carbonInterface->show();
            connect(carbonInterface,SIGNAL(ExitWin()),this,SLOT(show()));
        }else if(ui->halfCell_button->isChecked()){
            copyAndReplaceFolderContents(current_path+"/OpenfoamModule/halfCellFoam",project_path,false);
            changeFolderName(project_path+"/halfCellFoam",project_path+"/"+project_name);
            changeMakeFile("/"+project_name+"/halfCellFoam/Make/files","halfCell");

            this->hide();
            halfCellInterface = new HalfCellInterface(this);
            halfCellInterface->show();
            connect(halfCellInterface,SIGNAL(ExitWin()),this,SLOT(show()));
        }else if(ui->fullCell_button->isChecked()){
            copyAndReplaceFolderContents(current_path+"/OpenfoamModule/fullCellFoam",project_path,false);
            changeFolderName(project_path+"/fullCellFoam",project_path+"/"+project_name);
            changeMakeFile("/"+project_name+"/fullCellFoam/Make/files","fullCell");

            this->hide();
            fullCellInterface = new FullCellFoam(this);
            fullCellInterface->show();
            connect(fullCellInterface,SIGNAL(ExitWin()),this,SLOT(show()));
        }
    }
}

void MainWindow::on_main_name_hint_clicked()
{
    QMessageBox::information(this,"hint","Only supports upper&lower case letter,number and underscore\n"
                             "Once confirmed,do not change the name arbitrarily");
}

//**************************Open interface**********************************//
void MainWindow::on_main_path_button_2_clicked()
{
    project_path = QFileDialog::getExistingDirectory(
                this, "Choose a project",
                "/home");

    ui->main_path_label_2->setText(project_path);

    //acquire project_name
    QFileInfo fileInfo(project_path);
    project_name = fileInfo.fileName();

    //acquire real project_path
    project_path.replace(QRegExp("[/][a-zA-Z_0-9]{1,}$"),"");

    if(ui->main_path_label_2->text().isEmpty()){
        QMessageBox::information(this,"hint","Path should not be empty");
        ui->main_next_button_2->setEnabled(false);
    }else{
        ui->main_next_button->setEnabled(false);
        ui->main_next_button_2->setEnabled(true);
    }

}

void MainWindow::on_recent_path_button_clicked()
{
    QString current_path = QCoreApplication::applicationDirPath()+"/OpenfoamModule/most_recent_file";

    QFile file(current_path);
    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this,"Error","Cannot open file for Reading");
        return ;
    }

    QString str (file.readAll());
    if(str!=""){
        ui->recent_path_label->setText(str);

        //acquire project_name
        QFileInfo fileInfo(str);
        project_name = fileInfo.fileName();

        //acquire real project_path
        str.replace(QRegExp("[/][a-zA-Z_0-9]{1,}$"),"");
        project_path=str;

        ui->main_next_button->setEnabled(false);
        ui->main_next_button_2->setEnabled(true);
    }
    file.close();
}

void MainWindow::on_main_next_button_2_clicked()
{
    QString current_path = QCoreApplication::applicationDirPath()+"/OpenfoamModule/most_recent_file";

    QFile file(current_path);
    QString str=project_path+"/"+project_name;

    QFile::remove(current_path);
    if(!file.open(QIODevice::WriteOnly))
    {
         QMessageBox::warning(this,"Error","Cannot open file for Writing");
         return ;
    }

    file.write(str.toUtf8());
    file.close();

    //comfirm which module is
    QFile fileC(project_path+"/"+project_name+"/SPMFoam");
    QFile fileH(project_path+"/"+project_name+"/halfCellFoam");
    QFile fileF(project_path+"/"+project_name+"/fullCellFoam");

    if(fileC.exists()){
        this->hide();
        carbonInterface = new CarbonInterface(this);
        carbonInterface->show();
        connect(carbonInterface,SIGNAL(ExitWin()),this,SLOT(show()));
    }else if(fileH.exists()){
        this->hide();
        halfCellInterface = new HalfCellInterface(this);
        halfCellInterface->show();
        connect(halfCellInterface,SIGNAL(ExitWin()),this,SLOT(show()));
    }else if(fileF.exists()){
        this->hide();
        fullCellInterface = new FullCellFoam(this);
        fullCellInterface->show();
        connect(fullCellInterface,SIGNAL(ExitWin()),this,SLOT(show()));
    }else{
        QMessageBox::information(this,"hint","The folder you chose is invalid.");
    }

    ui->recent_path_label->clear();
    ui->main_path_label_2->clear();

}
