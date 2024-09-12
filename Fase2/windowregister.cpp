#include "windowregister.h"
#include "Structs.h"
#include "globales.h"
#include "ui_windowregister.h"
#include "windowmain.h"
#include <QMessageBox>

WindowRegister::WindowRegister(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::WindowRegister)
{
    ui->setupUi(this);
}

WindowRegister::~WindowRegister()
{
    delete ui;
}

void WindowRegister::on_registrarButton_clicked()
{
    QString nombres = ui->nombresLineEdit->text();
    QString apellidos = ui->apellidosLineEdit->text();
    QString correo = ui->correoLineEdit->text();
    QString fechaNacimiento = ui->fechaDeNacimientoDateEdit->text();
    QString contrasena = ui->contrasenaLineEdit->text();
    QString confirmarcontrasena = ui->confirmarContrasenaLineEdit->text();
    if (nombres.isEmpty() || apellidos.isEmpty() || correo.isEmpty() || fechaNacimiento.isEmpty() || contrasena.isEmpty() || confirmarcontrasena.isEmpty()) {
        QMessageBox::warning(nullptr,"Registrar usuario", "No debe dejar ningun campo vacio");
    }else{
        if (contrasena == confirmarcontrasena) {
            int id = lista_usuarios.obtenerId()+1;
            Structs::Usuario nuevo(id,nombres.toStdString(),apellidos.toStdString(),fechaNacimiento.toStdString(),correo.toStdString(),contrasena.toStdString());
            lista_usuarios.insertar(nuevo);
            QMessageBox::information(nullptr,"Registrar usuario", "Usuario registrado");
            MainWindow *login = new MainWindow;
            login->show();
            this->close();
        }else{
            QMessageBox::warning(nullptr,"Registrar usuario", "Las contraseñas no coinciden");
        }
    }
}


void WindowRegister::on_confirmarContrasenaLineEdit_returnPressed()
{
    on_registrarButton_clicked();
}


void WindowRegister::on_contrasenaLineEdit_returnPressed()
{
    on_registrarButton_clicked();
}


void WindowRegister::on_correoLineEdit_returnPressed()
{
    on_registrarButton_clicked();
}


void WindowRegister::on_apellidosLineEdit_returnPressed()
{
    on_registrarButton_clicked();
}


void WindowRegister::on_nombresLineEdit_returnPressed()
{
    on_registrarButton_clicked();
}


void WindowRegister::on_backButton_clicked()
{
    MainWindow *login = new MainWindow;
    login->show();
    this->close();
}

