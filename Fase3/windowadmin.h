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
    // Menu
    void on_actionCerrar_Sesion_triggered();
    void on_actionVista_Usuario_triggered();

    // Buscar
    void on_pushButton_buscar_buscar_clicked();
    void on_pushButton_buscar_aplicar_clicked();
    void on_lineEdit_buscar_buscar_returnPressed();

    // Carga masiva
    void on_pushButton_cargar_usuarios_clicked();
    void on_pushButton_cargar_solicitudes_clicked();
    void on_pushButton_cargar_publicaciones_clicked();

    // Reportes
    void on_pushButton_reportes_generar_clicked();
    void on_pushButton_reporte_users_clicked();
    void on_pushButton_reporte_posts_clicked();


    void on_pushButton_reporte_amistades_clicked();

    void on_pushButton_reporte_amistades_ady_clicked();

private:
    Ui::AdminWindow *ui;
};

#endif // WINDOWADMIN_H
