#ifndef WINDOWUSER_H
#define WINDOWUSER_H

#include <QMainWindow>

namespace Ui {
class UserWindow;
}

class UserWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit UserWindow(QWidget *parent = nullptr);
    ~UserWindow();

private slots:
    void on_actionCerrar_Sesion_triggered();

    void on_btnBuscar_clicked();

    void on_inputBuscar_returnPressed();

    void on_actionPanel_Administrador_triggered();

    void on_btnModificarDatos_clicked();

    void on_guardarModificacionButton_clicked();

    void on_nombresLogLineEdit_returnPressed();

    void on_apellidosLogLineEdit_returnPressed();

    void on_correoLogLineEdit_returnPressed();

    void on_fechaDeNacimientoLogLineEdit_returnPressed();

    void on_contrasenaLogLineEdit_returnPressed();

private:
    Ui::UserWindow *ui;
};

#endif // WINDOWUSER_H
