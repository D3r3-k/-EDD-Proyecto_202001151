#include "widgetpost.h"
#include "funciones.h"
#include "globales.h"
#include "ui_widgetpost.h"
#include "Structs.h"
#include <qmessagebox.h>

WidgetPost::WidgetPost(const int id, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WidgetPost)
{
    ui->setupUi(this);
    llenarDatos(id);
}

WidgetPost::~WidgetPost()
{
    delete ui;
}

void WidgetPost::llenarDatos(const int id){
    Structs::Publicacion *post = Func::buscarPost(id);
    if (post) {
        ui->label_user->setText(QString::fromStdString(post->correo_autor));
        ui->label_date->setText(QString::fromStdString(post->fecha));
        ui->label_contenido->setText(QString::fromStdString(post->contenido));
        if (usuario_logeado->correo != post->correo_autor) {
            ui->eliminarButton->setEnabled(false);
            ui->eliminarButton->setVisible(false);
        }
    }
}

void WidgetPost::setID(const int id){
    llenarDatos(id);
    postID=id;
}
int WidgetPost::getID() const
{
    return postID;
}

void WidgetPost::on_pushButton_comentar_clicked()
{
    QMessageBox::information(nullptr,"Comentarios", "Comentando el post con ID: "+QString::number(getID()));
}


void WidgetPost::on_eliminarButton_clicked()
{
    if (QMessageBox::question(nullptr, "Confirmación","¿Quieres eliminar esta publicación?",QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes) {
        Func::eliminarPublicacion(getID());
        Func::ActualizarFeed();
    }
}

