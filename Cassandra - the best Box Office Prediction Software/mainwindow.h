#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSvg>

#include "BOP_manager.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void actorsDirectorsScore();
    string getGenre();
    string getAgeGroup();

private slots:
    void on_startButton_clicked();

    void on_selectFile_clicked();

    void on_goBack_clicked();

    void on_about_clicked();

    void on_goBackFromAbout_clicked();

    void on_goBackFromScore_clicked();

    void on_searchActor_textChanged(const QString &arg1);

    void on_BOPButton_clicked();

    void on_buttonGoBackFromCreateNewMovie_clicked();

    void on_buttonDone_clicked();

    void on_buttonAddMoreMovies_clicked();

    void on_searchDirector_textChanged(const QString &arg1);

    void on_actionOpen_triggered();

    void on_buttonComplete_clicked();

    void on_pushButton_clicked();

    void on_testButton_clicked();

    void on_timeSchedulingButton_clicked();

    void on_goToNextDay_clicked();

    void on_goToPreviousDay_clicked();

private:
    Ui::MainWindow *ui;
    Manager *manager;
};

#endif // MAINWINDOW_H
