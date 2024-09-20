#include "dialogpost.h"
#include "dialognuevopost.h"
#include "funciones.h"
#include "globales.h"
#include "ui_dialogpost.h"
#include "widgetcomment.h"
#include <QDesktopServices>
#include <qdatetime.h>
#include <qmessagebox.h>

DialogPost::DialogPost(const int id, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogPost)
{
    ui->setupUi(this);
    qInfo() << id;
    setID(id);
}

DialogPost::~DialogPost()
{
    delete ui;
}

void DialogPost::setID(const int id){
    postID=id;
    llenarDatos(id);
}

int DialogPost::getID() const {
    return postID;
}

void DialogPost::llenarDatos(const int id){
    Structs::Publicacion *post = Func::buscarPost(id);
    if (post) {
        ui->label_user->setText(QString::fromStdString(post->correo_autor));
        ui->label_date->setText(QString::fromStdString(post->fecha));
        if (usuario_logeado->correo != post->correo_autor) {
            ui->pushButton_editar->setEnabled(false);
            ui->pushButton_editar->setVisible(false);
            ui->pushButton_eliminar->setEnabled(false);
            ui->pushButton_eliminar->setVisible(false);
        }
        if (post->imagen != "default.jpg") {
            QPixmap pix(QString::fromStdString(post->imagen));
            QPixmap scaledPix = pix.scaled(300,300, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            ui->label_imagen->setPixmap(scaledPix);
            ui->label_imagen->setAlignment(Qt::AlignCenter);
        }else{
            QPixmap pix(":/images/default.jpg");
            QPixmap scaledPix = pix.scaled(300,300, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            ui->label_imagen->setPixmap(scaledPix);
            ui->label_imagen->setAlignment(Qt::AlignCenter);
        }
        actualizarComentarios();
    }
}

void DialogPost::actualizarComentarios(){
    int id = getID();

    Structs::Publicacion *post = Func::buscarPost(id);
    ListaEnlazada::ListaEnlazada<StructsComment::Comentario> comentarios = post->comentarios->obtenerComentarios();
    QWidget *contentWidget = ui->scrollArea_comentarios->widget();
    QVBoxLayout *layout = qobject_cast<QVBoxLayout *>(contentWidget->layout());
    if (!layout)
    {
        layout = new QVBoxLayout(contentWidget);
        contentWidget->setLayout(layout);
    }
    while (QLayoutItem *item = layout->takeAt(0))
    {
        if (item->widget())
        {
            item->widget()->deleteLater();
        }
        delete item;
    }
    for (int i = 0; i < comentarios.size(); ++i)
    {
        StructsComment::Comentario *p = comentarios.obtener(i);
        if (p)
        {
            WidgetComment *newPost = new WidgetComment(*p);
            layout->addWidget(newPost);
        }
    }
}

// TODO: Funcionalidades de los botones
void DialogPost::on_pushButton_editar_clicked()
{
    int id = getID();
    DialogNuevoPost mdf(id);
    if (mdf.exec() == QDialog::Accepted) {
        Func::ActualizarFeed();
        this->close();
    }
}


void DialogPost::on_pushButton_eliminar_clicked()
{
    int id = getID();
    if (QMessageBox::question(nullptr, "Confirmación","¿Quieres eliminar esta publicación (ID: "+QString::number(id)+")?",QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes) {
        Func::eliminarPublicacion(id);
        Func::ActualizarFeed();
        this->close();
    }
}


void DialogPost::on_pushButton_arbol_comentarios_clicked()
{
    int id = getID();
    Structs::Publicacion *post = Func::buscarPost(id);
    std::string path =post->comentarios->graficar();
    QString rutaImagen = QString::fromStdString(path);
    if (!QDesktopServices::openUrl(QUrl::fromLocalFile(rutaImagen))) {
        QMessageBox::warning(this, "Error", "No se pudo abrir la imagen.");
    }
}


void DialogPost::on_pushButton_comentar_clicked()
{
    int id = getID();
    QString comentario = ui->plainTextEdit_comentario->toPlainText();
    QDate date = QDate::currentDate();
    QTime time = QTime::currentTime();
    QString fecha_hora = date.toString("dd-MM-yyyy") + " " + time.toString("hh:mm:ss");
    StructsComment::Comentario nuevo(fecha_hora.toStdString(), usuario_logeado->correo, comentario.toStdString());
    Func::ComentarPublicacion(id, nuevo);
    ui->plainTextEdit_comentario->clear();
    actualizarComentarios();
}


