#ifndef WINDOWREGISTER_H
#define WINDOWREGISTER_H

#include <QMainWindow>

namespace Ui {
class WindowRegister;
}

class WindowRegister : public QMainWindow
{
    Q_OBJECT

public:
    explicit WindowRegister(QWidget *parent = nullptr);
    ~WindowRegister();

private slots:
    void on_registrarButton_clicked();

    void on_confirmarContrasenaLineEdit_returnPressed();

    void on_contrasenaLineEdit_returnPressed();

    void on_correoLineEdit_returnPressed();

    void on_apellidosLineEdit_returnPressed();

    void on_nombresLineEdit_returnPressed();

    void on_backButton_clicked();

private:
    Ui::WindowRegister *ui;
};

#endif // WINDOWREGISTER_H
