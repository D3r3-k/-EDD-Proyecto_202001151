#include "widgetcomment.h"
#include "funciones.h"
#include "ui_widgetcomment.h"

WidgetComment::WidgetComment(const StructsComment::Comentario &c, QWidget *parent)
    : QWidget(parent), c(c), ui(new Ui::WidgetComment)
{
    ui->setupUi(this);
    setComment(c);
}

WidgetComment::~WidgetComment()
{
    delete ui;
}

void WidgetComment::llenarDatos(const StructsComment::Comentario &c)
{
    if (!c.usuario.empty()) {  // Validación de correo no vacío
        ui->label_user->setText(QString::fromStdString(c.usuario));
        QString fecha = QString::fromStdString(Func::convertirFechayHora(c.fecha_hora));
        ui->label_date->setText(fecha);
        ui->label_comentario->setText(QString::fromStdString(c.texto));
    }
}

void WidgetComment::setComment(const StructsComment::Comentario &c)
{
    this->c = c;
    llenarDatos(c);
}

StructsComment::Comentario WidgetComment::getComment() const
{
    return c;
}
