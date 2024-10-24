#include "dialogmodificar.h"
#include "funciones.h"
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
    QString contrasena = ui->contrasenaLineEdit->text();
    std::string temppass = "";
    if(ui->contrasenaLineEdit->text().isEmpty() || contrasena.toStdString() == usuario_logeado->contrasena){
        temppass = usuario_logeado->contrasena;
    }else{
        temppass = ui->contrasenaLineEdit->text().toStdString();
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
    if (Func::ModificarUsuario(nombres,apellidos,fecha,correo,temppass, rol)) {
        QMessageBox::information(nullptr,"Modificar Usuario", "Usuario modificado exitosamente");
    }else{
        QMessageBox::warning(nullptr,"Modificar Usuario", "No se pudo actualizar el usuario");
    }
}

