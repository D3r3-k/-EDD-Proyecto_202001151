#include "windowadmin.h"
#include "funciones.h"
#include "globales.h"
#include "windowmain.h"
#include "ui_windowadmin.h"
#include "windowuser.h"
#include <QMessageBox>
#include <QPixmap>
#include <QFileDialog>
#include <QDesktopServices>

string img_usuarios = "";

AdminWindow::AdminWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::AdminWindow)
{
    ui->setupUi(this);
    Func::adminTablaUsuarios = ui->tableUsuarios;

    // MenuBar
    QString user = QString::fromStdString(usuario_logeado->nombres);
    ui->menuBienvenido->setTitle("Bienvenido: "+user);
    // TabMenu
    ui->tabWidget->setCurrentIndex(0);
    // Tab Buscar
    // Ajustar el tamaño de las columnas para que se distribuyan proporcionalmente
    ui->tableUsuarios->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Asegurar que las filas no se expandan para ocupar todo el espacio disponible
    ui->tableUsuarios->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    // Desactivar la expansión vertical de las filas para que todas mantengan el tamaño definido
    ui->tableUsuarios->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->tableUsuarios->verticalHeader()->setStretchLastSection(false);
    ui->menubar->setStyleSheet("QMenu { min-width: 120px; }");
    Func::ActualizarTablaAdmin(0);
}

AdminWindow::~AdminWindow()
{
    delete ui;
}

void AdminWindow::on_actionCerrar_Sesion_triggered()
{
    usuario_logeado = nullptr;
    MainWindow *login = new MainWindow;
    login->show();
    this->close();
}


void AdminWindow::on_btnGenerarReportes_clicked()
{
    // Obtener la ruta de la imagen desde graficar()
    std::string path_usuarios = lista_usuarios.graficar();
    // Convertir la cadena std::string a QString
    QString ruta_imagen = QString::fromStdString(path_usuarios);
    // Cargar la imagen en un QPixmap
    QPixmap imagen(ruta_imagen);
    // Escalar la imagen al tamaño del QLabel si es necesario
    QPixmap imagenEscalada = imagen.scaled(ui->img_usuarios->size(), Qt::IgnoreAspectRatio);
    // Establecer el QPixmap en el QLabel
    ui->img_usuarios->setPixmap(imagenEscalada);
    img_usuarios = path_usuarios;
}


void AdminWindow::on_btnSubirUsuarios_clicked()
{
    // Abrir un cuadro de diálogo para seleccionar un archivo .json
    QString archivoPath = QFileDialog::getOpenFileName(
        this,
        tr("Seleccionar archivo JSON"),      // Título de la ventana
        "",                                  // Directorio inicial (puedes especificar uno si lo deseas)
        tr("Archivos JSON (*.json)")         // Filtro para mostrar solo archivos .json
        );

    // Verifica si se ha seleccionado un archivo
    if (!archivoPath.isEmpty()) {
        // Utiliza el path del archivo seleccionado
        Func::CargarUsuarios(archivoPath.toStdString());
        // Aquí puedes agregar el código para procesar el archivo .json
        Func::ActualizarTablaAdmin(0);
    }
}


void AdminWindow::on_btnSubirSolicitudes_clicked()
{
    // Abrir un cuadro de diálogo para seleccionar un archivo .json
    QString archivoPath = QFileDialog::getOpenFileName(
        this,
        tr("Seleccionar archivo JSON"),      // Título de la ventana
        "",                                  // Directorio inicial (puedes especificar uno si lo deseas)
        tr("Archivos JSON (*.json)")         // Filtro para mostrar solo archivos .json
        );

    // Verifica si se ha seleccionado un archivo
    if (!archivoPath.isEmpty()) {
        // Utiliza el path del archivo seleccionado
        Func::CargarSolicitudes(archivoPath.toStdString());
    }
}

void AdminWindow::on_btnOrdenar_clicked()
{
    ui->inputBuscar->setText("");
    int ordenarPor= ui->comboBoxOrdenar->currentIndex();
    if (ordenarPor == 0) {
        Func::ActualizarTablaAdmin(0);
    }else if (ordenarPor==1) {
        Func::ActualizarTablaAdmin(1);
    }else if (ordenarPor==2) {
        Func::ActualizarTablaAdmin(2);
    }
}


void AdminWindow::on_btn_abrir_imgUsuarios_clicked()
{
    // Verificar que la ruta de la imagen no esté vacía
    if (img_usuarios.empty()) {
        QMessageBox::warning(this, "Error", "No se ha generado ninguna imagen.");
        return;
    }
    // Convertir std::string a QString
    QString rutaImagen = QString::fromStdString(img_usuarios);
    // Abrir el archivo de imagen utilizando el programa predeterminado del sistema
    if (!QDesktopServices::openUrl(QUrl::fromLocalFile(rutaImagen))) {
        QMessageBox::warning(this, "Error", "No se pudo abrir la imagen.");
    }
}


void AdminWindow::on_inputBuscar_returnPressed()
{
    on_btnBuscar_clicked();
}


void AdminWindow::on_btnBuscar_clicked()
{
    QString correo = ui->inputBuscar->text();
    if (correo.isEmpty()) {
        on_btnOrdenar_clicked();
    }else{
        Structs::Usuario *tempUser = lista_usuarios.buscar(correo.toStdString());
        if (tempUser) {
            ListaEnlazada::ListaEnlazada<Structs::Usuario> temp;
            temp.insertar(*tempUser);
            Func::ActualizarTablaUsuariosAdmin(ui->tableUsuarios,temp);
        }else{
            ui->tableUsuarios->setRowCount(0);
            ui->tableUsuarios->clear();
        }
    }
}


void AdminWindow::on_actionVista_Usuario_triggered()
{
    UserWindow *user= new UserWindow;
    user->show();
    this->close();
}



