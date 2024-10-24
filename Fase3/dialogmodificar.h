#ifndef DIALOGMODIFICAR_H
#define DIALOGMODIFICAR_H

#include <QDialog>

namespace Ui {
class DialogModificar;
}

class DialogModificar : public QDialog
{
    Q_OBJECT

public:
    explicit DialogModificar(const std::string correo, QWidget *parent = nullptr);
    ~DialogModificar();
    void setCorreo(const std::string correo);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::DialogModificar *ui;
    void llenarDatos(const std::string correo);
};

#endif // DIALOGMODIFICAR_H
