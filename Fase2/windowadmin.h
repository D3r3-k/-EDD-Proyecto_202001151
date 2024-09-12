#ifndef WINDOWADMIN_H
#define WINDOWADMIN_H

#include <QMainWindow>

namespace Ui {
class AdminWindow;
}

class AdminWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit AdminWindow(QWidget *parent = nullptr);
    ~AdminWindow();

private slots:
    void on_actionCerrar_Sesion_triggered();

    void on_btnGenerarReportes_clicked();

    void on_btnSubirUsuarios_clicked();

    void on_btnOrdenar_clicked();

    void on_btn_abrir_imgUsuarios_clicked();

    void on_inputBuscar_returnPressed();

    void on_btnBuscar_clicked();


    void on_actionVista_Usuario_triggered();

    void on_btnSubirSolicitudes_clicked();

private:
    Ui::AdminWindow *ui;
};

#endif // WINDOWADMIN_H
