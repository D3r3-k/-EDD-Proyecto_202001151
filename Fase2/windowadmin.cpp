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
string img_publicaciones = "";
string img_amistades = "";

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

/*
======================================================
                    MENU
======================================================
*/
void AdminWindow::on_actionVista_Usuario_triggered()
{
    UserWindow *user= new UserWindow;
    user->show();
    this->close();
}

void AdminWindow::on_actionCerrar_Sesion_triggered()
{
    usuario_logeado = nullptr;
    MainWindow *login = new MainWindow;
    login->show();
    this->close();
}
/*
======================================================
                    TAB BUSCAR
======================================================
*/

void AdminWindow::on_pushButton_buscar_buscar_clicked()
{
    QString correo = ui->lineEdit_buscar_buscar->text();
    if (correo.isEmpty()) {
        on_pushButton_buscar_aplicar_clicked();
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


void AdminWindow::on_pushButton_buscar_aplicar_clicked()
{
    ui->lineEdit_buscar_buscar->setText("");
    int ordenarPor= ui->comboBox_buscar_ordenar->currentIndex();
    if (ordenarPor == 0) {
        Func::ActualizarTablaAdmin(0);
    }else if (ordenarPor==1) {
        Func::ActualizarTablaAdmin(1);
    }else if (ordenarPor==2) {
        Func::ActualizarTablaAdmin(2);
    }
}


void AdminWindow::on_lineEdit_buscar_buscar_returnPressed()
{
    on_pushButton_buscar_buscar_clicked();
}

/*
======================================================
                TAB CARGA MASIVA
======================================================
*/

void AdminWindow::on_pushButton_cargar_usuarios_clicked()
{
    QString archivoPath = QFileDialog::getOpenFileName(this,tr("Seleccionar archivo JSON"),"",tr("Archivos JSON (*.json)"));
    if (!archivoPath.isEmpty()) {
        Func::CargarUsuarios(archivoPath.toStdString());
        Func::ActualizarTablaAdmin(0);
    }
}


void AdminWindow::on_pushButton_cargar_solicitudes_clicked()
{
    QString archivoPath = QFileDialog::getOpenFileName(this,tr("Seleccionar archivo JSON"),"",tr("Archivos JSON (*.json)"));
    if (!archivoPath.isEmpty()) {
        Func::CargarSolicitudes(archivoPath.toStdString());
    }
}


void AdminWindow::on_pushButton_cargar_publicaciones_clicked()
{
    QString archivoPath = QFileDialog::getOpenFileName(this,tr("Seleccionar archivo JSON"),"",tr("Archivos JSON (*.json)"));
    if (!archivoPath.isEmpty()) {
        Func::CargarPublicaciones(archivoPath.toStdString());
    }
}

/*
======================================================
                    TAB REPORTES
======================================================
*/
void AdminWindow::on_pushButton_reporte_users_clicked()
{
    // Verificar que la ruta de la imagen no esté vacía
    if (img_usuarios.empty()) {
        QMessageBox::warning(this, "Error", "No se ha generado ninguna imagen.");
        return;
    }
    QString rutaImagen = QString::fromStdString(img_usuarios);
    if (!QDesktopServices::openUrl(QUrl::fromLocalFile(rutaImagen))) {
        QMessageBox::warning(this, "Error", "No se pudo abrir la imagen.");
    }
}


void AdminWindow::on_pushButton_reporte_posts_clicked()
{
    // Verificar que la ruta de la imagen no esté vacía
    if (img_publicaciones.empty()) {
        QMessageBox::warning(this, "Error", "No se ha generado ninguna imagen.");
        return;
    }
    QString rutaImagen = QString::fromStdString(img_publicaciones);
    if (!QDesktopServices::openUrl(QUrl::fromLocalFile(rutaImagen))) {
        QMessageBox::warning(this, "Error", "No se pudo abrir la imagen.");
    }
}


void AdminWindow::on_pushButton_reporte_amistades_clicked()
{
    // Verificar que la ruta de la imagen no esté vacía
    if (img_amistades.empty()) {
        QMessageBox::warning(this, "Error", "No se ha generado ninguna imagen.");
        return;
    }
    QString rutaImagen = QString::fromStdString(img_amistades);
    if (!QDesktopServices::openUrl(QUrl::fromLocalFile(rutaImagen))) {
        QMessageBox::warning(this, "Error", "No se pudo abrir la imagen.");
    }
}

void AdminWindow::on_pushButton_reportes_generar_clicked()
{
    // Graficar usuarios
    std::string path_usuarios = lista_usuarios.graficar();
    QString ruta_imagen = QString::fromStdString(path_usuarios);
    QPixmap imagen(ruta_imagen);
    QPixmap imagenEscalada = imagen.scaled(ui->img_usuarios->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->img_usuarios->setPixmap(imagenEscalada);
    img_usuarios = path_usuarios;
    // Graficar publicaciones
    std::string path_posts = Func::graficarPublicaciones();
    QString ruta_imagen_posts = QString::fromStdString(path_posts);
    QPixmap imagen2(ruta_imagen_posts);
    QPixmap imagenEscalada2 = imagen2.scaled(ui->img_publicaciones->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->img_publicaciones->setPixmap(imagenEscalada2);
    img_publicaciones = path_posts;
    // Graficar amistades
    std::string path_friends = relaciones_amistad.graficar();
    QString ruta_imagen_friends = QString::fromStdString(path_friends);
    QPixmap imagen3(ruta_imagen_friends);
    QPixmap imagenEscalada3 = imagen3.scaled(ui->img_amistades->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->img_amistades->setPixmap(imagenEscalada3);
    img_amistades = path_friends;
}



