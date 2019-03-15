#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QFile>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QFile styleFile(":/Light.qss");
    styleFile.open(QFile::ReadOnly);
    QString style = QString::fromUtf8(styleFile.readAll());
    setStyleSheet(style);
    styleFile.close();

    // set property
    ui->primary->setProperty("variable", "primary");
    ui->success->setProperty("variable", "success");
    ui->warning->setProperty("variable", "warning");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_primary_clicked()
{
    if(ui->primary->property("variable").toString() == "primary") {
        ui->primary->setProperty("variable", "success");
    } else {
        ui->primary->setProperty("variable", "primary");
    }
    ui->primary->style()->unpolish(ui->primary);
    ui->primary->style()->polish(ui->primary);
}
