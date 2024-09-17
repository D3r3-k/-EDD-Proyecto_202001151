#include "dialognuevopost.h"
#include "ui_dialognuevopost.h"
#include "globales.h"
#include "funciones.h"

#include <QTime>
#include <QDate>

DialogNuevoPost::DialogNuevoPost(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogNuevoPost)
{
    ui->setupUi(this);

    // TODO: Datos por defecto
    ui->correoLineEdit->setText(QString::fromStdString(usuario_logeado->correo));

    QDate fechaActual = QDate::currentDate();
    ui->fechaDateEdit->setDate(fechaActual);
    ui->fechaDateEdit->setDisplayFormat("dd-MM-yyyy");

    QTime tiempoActual = QTime::currentTime();
    ui->horaTimeEdit->setTime(tiempoActual);
    ui->horaTimeEdit->setDisplayFormat("hh:mm");
}

DialogNuevoPost::DialogNuevoPost(const int id, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogNuevoPost)
{
    ui->setupUi(this);
    setID(id);
}

DialogNuevoPost::~DialogNuevoPost()
{
    delete ui;
}

void DialogNuevoPost::llenarDatos(const int id){
    qInfo() << &"Llenando datos del id "[id];
    Structs::Publicacion *post = Func::buscarPost(id);
    if (post) {
        ui->correoLineEdit->setText(QString::fromStdString(post->correo_autor));
        QString fechaStr = QString::fromStdString(post->fecha);
        QDate qFecha = QDate::fromString(fechaStr, "dd-MM-yyyy");
        ui->fechaDateEdit->setDate(qFecha);
        QString horaStr = QString::fromStdString(post->hora);
        QTime qHora = QTime::fromString(horaStr);
        ui->horaTimeEdit->setTime(qHora);
        ui->contenidoLineEdit->setPlainText(QString::fromStdString(post->contenido));
        setPathImg(post->imagen);
    }
}


void DialogNuevoPost::on_buttonBox_accepted()
{
    if (getID()==-1) {
        int id = Func::obtenerPostID()+1;
        string contenido = ui->contenidoLineEdit->toPlainText().toStdString();
        string hora = ui->horaTimeEdit->text().toStdString();
        string fecha = ui->fechaDateEdit->text().toStdString();
        Structs::Publicacion nuevo(id,usuario_logeado->correo,contenido,fecha,hora);
        lista_publicaciones.insertar(nuevo);
        qInfo() << "Se agrega id:" << QString::number(id);
    }else{
        qInfo() << "Se modifica";
    }
}

// =============== [METODOS PARA MODIFICAR] ===============
void DialogNuevoPost::setID(const int id){
    postID=id;
    llenarDatos(id);
}
int DialogNuevoPost::getID() const
{
    return postID;
}

void DialogNuevoPost::setPathImg(const std::string path){
    pathImg = path;
}
std::string DialogNuevoPost::getPathImg()
{
    return pathImg;
}
