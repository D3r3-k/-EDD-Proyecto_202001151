#ifndef WIDGETFRIEND_H
#define WIDGETFRIEND_H

#include "Structs.h"
#include <QWidget>

namespace Ui {
class WidgetFriend;
}

class WidgetFriend : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetFriend(const Structs::Usuario &user, QWidget *parent = nullptr);
    ~WidgetFriend();
    void setUser(const Structs::Usuario &u);
    Structs::Usuario getUser() const;

private slots:
    void on_pushButton_eliminar_clicked();

private:
    Ui::WidgetFriend *ui;
    Structs::Usuario user;
    void llenarDatos(const Structs::Usuario &user);
};

#endif // WIDGETFRIEND_H
