#include "windowlogin.h"
#include "./ui_windowlogin.h"

#include <QMessageBox>
#include "windowadmin.h"
#include "windowregister.h"
#include "windowuser.h"

// EXTRAS
#include "globales.h"
#include "funciones.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    Func::backupSerializacion();
    seguridad_blockchain.exportBlocks();
    delete ui;
}

void MainWindow::on_loginButton_clicked()
{
    QString email = ui->emailLineEdit->text();
    QString password = ui->passwordLineEdit->text();
    if(email.isEmpty() || password.isEmpty()){
        QMessageBox::warning(nullptr, "Iniciar Sesion", "No puede dejar campos vacios");
    }else{
        Func::IniciarSesion(email.toStdString(), password.toStdString());
        if (usuario_logeado && usuario_logeado->rol=="admin") {
            AdminWindow *admin = new AdminWindow;
            admin->show();
            this->close();
        }else if (usuario_logeado && usuario_logeado->rol=="user") {
            UserWindow *user= new UserWindow;
            user->show();
            this->close();
        }else{
            QMessageBox::warning(nullptr, "Iniciar Sesion", "Datos incorrectos.");
        }
    }
}


void MainWindow::on_passwordLineEdit_returnPressed()
{
    on_loginButton_clicked();
}


void MainWindow::on_emailLineEdit_returnPressed()
{
    on_loginButton_clicked();
}

void MainWindow::on_RegisterButton_clicked()
{
    WindowRegister *reg = new WindowRegister;
    reg->show();
    this->close();
}

