#include "windowuser.h"
#include "arbolbst.h"
#include "dialognuevopost.h"
#include "funciones.h"
#include "globales.h"
#include "windowadmin.h"
#include "windowmain.h"
#include "ui_windowuser.h"
#include <QDesktopServices>
#include <QMessageBox>

string path_bsl = "";

UserWindow::UserWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::UserWindow)
{
    ui->setupUi(this);
    try {
        Func::userTablaUsuarios = ui->tableUsuarios;
        Func::userTablaEnviadas = ui->tableSolicitudesEnv;
        Func::userTablaRecibidas = ui->tableSolicitudesRec;
        Func::userPostFeed = ui->scrollAreaPost;
        Func::userFriends = ui->scrollArea_amigos;
        Func::selectedDate = ui->comboBox_post_fecha;
        Func::selectedOrder = ui->ordenComboBox;
        Func::countPost = ui->cantidadSpinBox;

        // MenuBar
        ui->menubar->setStyleSheet("QMenu { min-width: 120px; }");
        if (usuario_logeado->rol == "user")
        {
            ui->actionPanel_Administrador->setVisible(false);
        }
        // TabMenu
        ui->tabWidget->setCurrentIndex(0);
        // MenuBar
        QString user = QString::fromStdString(usuario_logeado->nombres);
        ui->menuBienvenido->setTitle("Bienvenido: " + user);

    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}

UserWindow::~UserWindow()
{
    delete ui;
}

void UserWindow::on_actionCerrar_Sesion_triggered()
{
    Func::CerrarSesion();
    MainWindow *login = new MainWindow;
    login->show();
    this->close();
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

// METODOS PARA BUSQUEDAS

void UserWindow::on_btnBuscar_clicked()
{
    QString correo = ui->inputBuscar->text();
    Structs::Usuario *temp = lista_usuarios.buscar(correo.toStdString());
    if (temp)
    {
        ui->nombresLineEdit->setText(QString::fromStdString(temp->nombres));
        ui->apellidosLineEdit->setText(QString::fromStdString(temp->apellidos));
        ui->correoLineEdit->setText(QString::fromStdString(temp->correo));
        ui->fechaDeNacimientoLineEdit->setText(QString::fromStdString(temp->fechaNacimiento));
    }
    else
    {
        QMessageBox::information(nullptr, "Busqueda", "Usuario no encontrado");
        ui->nombresLineEdit->setText("");
        ui->apellidosLineEdit->setText("");
        ui->correoLineEdit->setText("");
        ui->fechaDeNacimientoLineEdit->setText("");
    }
}

// METODOS PARA LOS REPORTES
void UserWindow::on_pushButton_generar_reportes_clicked()
{
    try
    {
        // fechas con mas publicaciones
        ListaEnlazada::ListaEnlazada<Structs::ReportePosts> post_date = Func::obtenerReporteFechasPost();
        Func::bubbleSort(post_date);
        ListaEnlazada::ListaEnlazada<Structs::Publicacion> misPosts = Func::obtenerMisPosts();
        Func::bubbleSort(misPosts);
        ui->tableRepoPublicaciones->clearContents();
        ui->tableRepoPublicaciones->setRowCount(3);
        int row = 0;
        for (int i = 0; i < post_date.size(); ++i)
        {
            Structs::ReportePosts *temp = post_date.obtener(i);
            if (temp)
            {
                QTableWidgetItem *fecha = new QTableWidgetItem(QString::fromStdString(temp->fecha));
                QTableWidgetItem *cantidad = new QTableWidgetItem(QString::number(temp->publicaciones.size()));
                ui->tableRepoPublicaciones->setItem(row, 0, fecha);
                ui->tableRepoPublicaciones->setItem(row, 1, cantidad);
                row++;
            }
            if (i == 3) {
                break;
            }
        }
        // publicaciones con mas comentarios
        ui->tableRepoComentarios->clearContents();
        ui->tableRepoComentarios->setRowCount(3);
        int row2 = 0;
        for (int i = 0; i < misPosts.size(); ++i)
        {
            Structs::Publicacion *temp = misPosts.obtener(i);
            if (temp)
            {
                QTableWidgetItem *fecha = new QTableWidgetItem(QString::fromStdString(temp->fecha));
                QTableWidgetItem *usuario = new QTableWidgetItem(QString::fromStdString(temp->correo_autor));
                QTableWidgetItem *cantidad = new QTableWidgetItem(QString::number(temp->comentarios->size()));
                ui->tableRepoComentarios->setItem(row2, 0, fecha);
                ui->tableRepoComentarios->setItem(row2, 1, usuario);
                ui->tableRepoComentarios->setItem(row2, 2, cantidad);
                row2++;
            }
            if (i == 3) {
                break;
            }
        }
        // actualizar fechas
        ui->comboBox_BST->clear();
        for (int i = 0; i < post_date.size(); ++i)
        {
            Structs::ReportePosts *f = post_date.obtener(i);
            if (f)
            {
                QString fecha = QString::fromStdString(f->fecha);
                ui->comboBox_BST->addItem(fecha);
            }
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}

void UserWindow::on_pushButton_reporte_generar_bst_clicked()
{
    try {
        ArbolBST* arbol = new ArbolBST();
        qInfo() << ui->comboBox_BST->currentText();
        string fecha = Func::convertirFecha(ui->comboBox_BST->currentText().toStdString());
        ListaEnlazada::ListaEnlazada<Structs::ReportePosts> post_date = Func::obtenerReporteFechasPost();
        for (int i = 0; i < post_date.size(); ++i) {
            Structs::ReportePosts *rp = post_date.obtener(i);
            if (rp) {
                arbol->add(*rp);
            }
        }
        path_bsl = arbol->graficar(fecha);
        QString ruta_imagen = QString::fromStdString(path_bsl);
        QPixmap imagen(ruta_imagen);
        QPixmap imagenEscalada = imagen.scaled(ui->label_BST->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        ui->label_BST->setPixmap(imagenEscalada);
    } catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}

void UserWindow::on_pushButton_reporte_abrir_bst_clicked()
{
    // Verificar que la ruta de la imagen no esté vacía
    if (path_bsl.empty()) {
        QMessageBox::warning(this, "Error", "No se ha generado ninguna imagen.");
        return;
    }
    QString rutaImagen = QString::fromStdString(path_bsl);
    if (!QDesktopServices::openUrl(QUrl::fromLocalFile(rutaImagen))) {
        QMessageBox::warning(this, "Error", "No se pudo abrir la imagen.");
    }
}

// METODOS PARA PUBLICACIONES

void UserWindow::on_btn_nuevo_post_clicked()
{
    // Abre el diálogo para crear un nuevo post
    DialogNuevoPost dialog;
    if (dialog.exec() == QDialog::Accepted)
    {
        Func::ActualizarFeed();
    }
}

void UserWindow::on_btn_post_filtrar_clicked()
{
    Func::ActualizarFeed();
}

// METODOS PARA EL PERFIL

void UserWindow::on_btnModificarDatos_clicked()
{
    if (ui->btnModificarDatos->text() == "Modificar Datos")
    {
        ui->nombresLogLineEdit->setEnabled(true);
        ui->apellidosLogLineEdit->setEnabled(true);
        ui->fechaDeNacimientoLogLineEdit->setEnabled(true);
        ui->contrasenaLogLineEdit->setEnabled(true);
        ui->guardarModificacionButton->setVisible(true);
        ui->btnModificarDatos->setText("Cancelar");
    }
    else if (ui->btnModificarDatos->text() == "Cancelar")
    {
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
    QString fecha = ui->fechaDeNacimientoLogLineEdit->text();
    string contrasena = ui->contrasenaLogLineEdit->text().toStdString();
    if (nombres.isEmpty() || apellidos.isEmpty() || fecha.isEmpty())
    {
        QMessageBox::warning(nullptr, "Modificar Datos", "No puedes dejar campos importantes vacios");
    }
    else
    {
        if (ui->contrasenaLogLineEdit->text().isEmpty() || contrasena == usuario_logeado->contrasena)
        {
            contrasena = usuario_logeado->contrasena;
        }
        else
        {
            contrasena = ui->contrasenaLogLineEdit->text().toStdString();
        }
        lista_usuarios.modificar(usuario_logeado->correo, nombres.toStdString(), apellidos.toStdString(), fecha.toStdString(), contrasena);
        usuario_logeado = nullptr;
        QMessageBox::information(nullptr, "Modificar Datos", "Usuario Modificado con exito.\nDebe iniciar sesion nuevamente.");
    }
    MainWindow *login = new MainWindow;
    login->show();
    this->close();
}

void UserWindow::on_btnEliminarCuenta_clicked()
{
    if (QMessageBox::question(nullptr, "Confirmación", "¿Quieres eliminar tu cuenta?", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
    {
        Func::EliminarMiCuenta();
        MainWindow *login = new MainWindow;
        login->show();
        this->close();
    }
}

void UserWindow::on_tabWidget_currentChanged(int index)
{
    if (index == 0) {

    }else if (index == 1) {
        // Tab Publicaciones
        Func::ActualizarFeed();
    }else if (index == 2) {
        // Tab Solicitudes
        ui->tableUsuarios->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        ui->tableSolicitudesRec->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        ui->tableSolicitudesEnv->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        Func::ActualizarTablas();
    }else if (index == 3) {
        // Tab Reportes
        ui->tableRepoComentarios->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        ui->tableRepoPublicaciones->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    }else if (index == 4) {
        // Tab Perfil
        Func::ActualizarListaAmigos();
        ui->nombresLogLineEdit->setText(QString::fromStdString(usuario_logeado->nombres));
        ui->apellidosLogLineEdit->setText(QString::fromStdString(usuario_logeado->apellidos));
        ui->correoLogLineEdit->setText(QString::fromStdString(usuario_logeado->correo));
        ui->contrasenaLogLineEdit->setText(QString::fromStdString(usuario_logeado->contrasena));
        ui->fechaDeNacimientoLogLineEdit->setText(QString::fromStdString(usuario_logeado->fechaNacimiento));
        ui->guardarModificacionButton->setVisible(false);
    }
}

