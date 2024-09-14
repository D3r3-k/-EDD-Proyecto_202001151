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
    ui->fechaDateEdit->setDisplayFormat("dd/MM/yyyy");

    QTime tiempoActual = QTime::currentTime();
    ui->horaTimeEdit->setTime(tiempoActual);
    ui->horaTimeEdit->setDisplayFormat("hh:mm");
}

DialogNuevoPost::~DialogNuevoPost()
{
    delete ui;
}

void DialogNuevoPost::on_buttonBox_accepted()
{
    int id = Func::obtenerIdPublicaciones()+1;
    string contenido = ui->contenidoLineEdit->toPlainText().toStdString();
    string hora = ui->horaTimeEdit->text().toStdString();
    string fecha = ui->fechaDateEdit->text().toStdString();
    Structs::Publicacion nuevo(id,usuario_logeado->correo,contenido,fecha,hora);
    lista_publicaciones.insertar(nuevo);
}

