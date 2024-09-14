#include "dialogmodificar.h"
#include "ui_dialogmodificar.h"
#include "Structs.h"
#include "globales.h"
#include <QMessageBox>

DialogModificar::DialogModificar(std::string correo, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogModificar)
{
    ui->setupUi(this);
    llenarDatos(correo);
}

DialogModificar::~DialogModificar()
{
    delete ui;
}

void DialogModificar::llenarDatos(const std::string correo){
    Structs::Usuario *user = lista_usuarios.buscar(correo);
    if (user) {
        ui->nombresLineEdit->setText(QString::fromStdString(user->nombres));
        ui->apellidosLineEdit->setText(QString::fromStdString(user->apellidos));
        ui->correoLineEdit->setText(QString::fromStdString(user->correo));
        ui->fechaDeNacimientoLineEdit->setText(QString::fromStdString(user->fechaNacimiento));
        ui->contrasenaLineEdit->setText(QString::fromStdString(user->contrasena));
        if (user->rol == "admin") {
            ui->rolComboBox->setCurrentIndex(0);
        }else if (user->rol == "user") {
            ui->rolComboBox->setCurrentIndex(1);
        }
    }
}

void DialogModificar::setCorreo(const string correo){
    llenarDatos(correo);
}

void DialogModificar::on_buttonBox_accepted()
{
    QString nombres = ui->nombresLineEdit->text();
    QString apellidos = ui->apellidosLineEdit->text();
    QString correo = ui->correoLineEdit->text();
    QString fecha = ui->fechaDeNacimientoLineEdit->text();
    string contrasena = ui->contrasenaLineEdit->text().toStdString();
    if(ui->contrasenaLineEdit->text().isEmpty() || contrasena == usuario_logeado->contrasena){
        contrasena = usuario_logeado->contrasena;
    }else{
        contrasena = ui->contrasenaLineEdit->text().toStdString();
    }
    int _rol = ui->rolComboBox->currentIndex();
    string rol = "";
    if (_rol==0) {
        rol="admin";
    }else if (_rol==1) {
        rol="user";
    }

    if (correo.toStdString() == usuario_logeado->correo && rol=="user") {
        QMessageBox::warning(nullptr,"Modificar Usuario", "No puedes cambiarte a usuario");
        return;
    }
    lista_usuarios.modificar(correo.toStdString(),nombres.toStdString(),apellidos.toStdString(),fecha.toStdString(),contrasena,rol);
}

