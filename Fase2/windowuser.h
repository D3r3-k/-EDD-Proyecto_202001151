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

    void on_btn_generar_reporte_clicked();

    void on_btnEliminarCuenta_clicked();

    void on_btn_nuevo_post_clicked();

    void on_btn_post_fecha_clicked();

    void on_btn_post_recorrido_clicked();

    void on_btn_generar_bst_clicked();

private:
    Ui::UserWindow *ui;
};

#endif // WINDOWUSER_H
