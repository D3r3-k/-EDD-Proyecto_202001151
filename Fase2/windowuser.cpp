#include "windowuser.h"
#include "globales.h"
#include "windowadmin.h"
#include "windowmain.h"
#include "ui_windowuser.h"
#include <QMessageBox>

UserWindow::UserWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::UserWindow)
{
    ui->setupUi(this);
    // MenuBar
    ui->menubar->setStyleSheet("QMenu { min-width: 120px; }");
    if(usuario_logeado->rol=="user"){
        ui->actionPanel_Administrador->setVisible(false);
    }
    // TabMenu
    ui->tabWidget->setCurrentIndex(0);
    // MenuBar
    QString user = QString::fromStdString(usuario_logeado->nombres);
    ui->menuBienvenido->setTitle("Bienvenido: "+user);
    // Tab Solicitudes
    ui->tableUsuarios->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableSolicitudesRec->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableSolicitudesEnv->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Tab Perfil
    ui->nombresLogLineEdit->setText(QString::fromStdString(usuario_logeado->nombres));
    ui->apellidosLogLineEdit->setText(QString::fromStdString(usuario_logeado->apellidos));
    ui->correoLogLineEdit->setText(QString::fromStdString(usuario_logeado->correo));
    ui->contrasenaLogLineEdit->setText(QString::fromStdString(usuario_logeado->contrasena));
    ui->fechaDeNacimientoLogLineEdit->setText(QString::fromStdString(usuario_logeado->fechaNacimiento));
    ui->guardarModificacionButton->setVisible(false);

}

UserWindow::~UserWindow()
{
    delete ui;
}

void UserWindow::on_actionCerrar_Sesion_triggered()
{
    usuario_logeado = nullptr;
    MainWindow *login = new MainWindow;
    login->show();
    this->close();
}


void UserWindow::on_btnBuscar_clicked()
{
    QString correo = ui->inputBuscar->text();
    Structs::Usuario *temp = lista_usuarios.buscar(correo.toStdString());
    if(temp){
        ui->nombresLineEdit->setText(QString::fromStdString(temp->nombres));
        ui->apellidosLineEdit->setText(QString::fromStdString(temp->apellidos));
        ui->correoLineEdit->setText(QString::fromStdString(temp->correo));
        ui->fechaDeNacimientoLineEdit->setText(QString::fromStdString(temp->fechaNacimiento));
    }else{
        QMessageBox::information(nullptr, "Busqueda", "Usuario no encontrado");
    }

}


void UserWindow::on_inputBuscar_returnPressed()
{
    on_btnBuscar_clicked();
}


void UserWindow::on_actionPanel_Administrador_triggered()
{
    AdminWindow *admin = new AdminWindow;
    admin->show();
    this->close();
}


void UserWindow::on_btnModificarDatos_clicked()
{
    if(ui->btnModificarDatos->text()=="Modificar Datos"){
        ui->nombresLogLineEdit->setEnabled(true);
        ui->apellidosLogLineEdit->setEnabled(true);
        ui->fechaDeNacimientoLogLineEdit->setEnabled(true);
        ui->contrasenaLogLineEdit->setEnabled(true);
        ui->guardarModificacionButton->setVisible(true);
        ui->btnModificarDatos->setText("Cancelar");
    }else if(ui->btnModificarDatos->text()=="Cancelar"){
        ui->nombresLogLineEdit->setEnabled(false);
        ui->apellidosLogLineEdit->setEnabled(false);
        ui->fechaDeNacimientoLogLineEdit->setEnabled(false);
        ui->contrasenaLogLineEdit->setEnabled(false);
        ui->guardarModificacionButton->setVisible(false);
        ui->nombresLogLineEdit->setText(QString::fromStdString(usuario_logeado->nombres));
        ui->apellidosLogLineEdit->setText(QString::fromStdString(usuario_logeado->apellidos));
        ui->fechaDeNacimientoLogLineEdit->setText(QString::fromStdString(usuario_logeado->fechaNacimiento));
        ui->contrasenaLogLineEdit->setText(QString::fromStdString(usuario_logeado->contrasena));
        ui->btnModificarDatos->setText("Modificar Datos");
    }
}


void UserWindow::on_guardarModificacionButton_clicked()
{
    QString nombres = ui->nombresLogLineEdit->text();
    QString apellidos = ui->apellidosLogLineEdit->text();
    QString fecha= ui->fechaDeNacimientoLogLineEdit->text();
    string contrasena = ui->contrasenaLogLineEdit->text().toStdString();
    if(nombres.isEmpty() || apellidos.isEmpty() || fecha.isEmpty()){
        QMessageBox::warning(nullptr,"Modificar Datos","No puedes dejar campos importantes vacios");
    }else{
        if(ui->contrasenaLogLineEdit->text().isEmpty() || contrasena == usuario_logeado->contrasena){
            contrasena = usuario_logeado->contrasena;
        }else{
            contrasena = ui->contrasenaLogLineEdit->text().toStdString();
        }
        lista_usuarios.modificar(usuario_logeado->correo,nombres.toStdString(),apellidos.toStdString(),fecha.toStdString(),contrasena);
        usuario_logeado=nullptr;
        QMessageBox::information(nullptr,"Modificar Datos","Usuario Modificado con exito.\nDebe iniciar sesion nuevamente.");
    }
    MainWindow *login = new MainWindow;
    login->show();
    this->close();
}


void UserWindow::on_nombresLogLineEdit_returnPressed()
{
    on_guardarModificacionButton_clicked();
}


void UserWindow::on_apellidosLogLineEdit_returnPressed()
{
    on_guardarModificacionButton_clicked();
}


void UserWindow::on_correoLogLineEdit_returnPressed()
{
    on_guardarModificacionButton_clicked();
}


void UserWindow::on_fechaDeNacimientoLogLineEdit_returnPressed()
{
    on_guardarModificacionButton_clicked();
}


void UserWindow::on_contrasenaLogLineEdit_returnPressed()
{
    on_guardarModificacionButton_clicked();
}

