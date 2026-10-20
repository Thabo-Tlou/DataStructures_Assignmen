#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QRegularExpression>
#include <QStringList>

#include "hashsetopen.h"
#include "hashsetclosed.h"
#include "dijkstra.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void showMenu();
    void showOpenPage();
    void showClosedPage();
    void showDijkstraHardPage();
    void showDijkstraFilePage();
    void showHelpPage();

private slots:
    void runOpenDemo();
    void runClosedDemo();
    void runDijkstraHard();
    void browseFile();
    void runDijkstraFile();

private:
    QStackedWidget *stack;
    QWidget* buildMenuPage();
    QWidget* buildOpenPage();
    QWidget* buildClosedPage();
    QWidget* buildDijkstraHardPage();
    QWidget* buildDijkstraFilePage();
    QWidget* buildHelpPage();

    QTextEdit* openOutput;
    QTextEdit* closedOutput;
    QTextEdit* hardOutput;
    QTextEdit* fileOutput;
    QLineEdit* filePathEdit;
    QLineEdit* targetEdit;
};
#endif