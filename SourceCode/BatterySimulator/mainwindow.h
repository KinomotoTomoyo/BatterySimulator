#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "carboninterface.h"
#include "halfcellinterface.h"
#include "fullcellfoam.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    static QString project_path;
    static QString project_name;
    ~MainWindow();

    //interfaces
    CarbonInterface *carbonInterface;
    HalfCellInterface *halfCellInterface;
    FullCellFoam *fullCellInterface;

    //function
    void copyAndReplaceFolderContents(const QString &fromDir, const QString &toDir, bool copyAndRemove);
    void changeMakeFile(const QString &suffix, const QString &module);
    void changeFolderName(const QString &Name1, const QString &Name2);

private slots:
    void on_main_path_button_clicked();

    void on_main_next_button_clicked();

    void on_main_path_button_2_clicked();

    void on_main_name_hint_clicked();

    void on_main_next_button_2_clicked();

    void on_recent_path_button_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
