#include "widgetfriend.h"
#include "funciones.h"
#include "globales.h"
#include "ui_widgetfriend.h"

#include <QMessageBox>

WidgetFriend::WidgetFriend(const Structs::Usuario &user, QWidget *parent)
    : QWidget(parent), user(user), ui(new Ui::WidgetFriend)
{
    ui->setupUi(this);
    setUser(user);
}

WidgetFriend::~WidgetFriend()
{
    delete ui;
}

void WidgetFriend::llenarDatos(const Structs::Usuario &user)
{
    if (!user.correo.empty()) {
        ui->label_nombres->setText(QString::fromStdString(user.nombres+" "+user.apellidos));
        ui->label_correo->setText(QString::fromStdString(user.correo));
    }
}

void WidgetFriend::setUser(const Structs::Usuario &user)
{
    this->user = user;
    llenarDatos(user);
}

Structs::Usuario WidgetFriend::getUser() const
{
    return user;
}

void WidgetFriend::on_pushButton_eliminar_clicked()
{
    Structs::Usuario u = getUser();
    if (QMessageBox::question(nullptr, "Confirmación","¿Quieres eliminar a: "+QString::fromStdString(u.correo)+"?",QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes) {
        relaciones_amistad.eliminarRelacionEntreUsuarios(usuario_logeado->correo,u.correo);
        Func::ActualizarListaAmigos();
    }
}

