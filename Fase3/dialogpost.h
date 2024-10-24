#ifndef DIALOGPOST_H
#define DIALOGPOST_H

#include <QDialog>

namespace Ui {
class DialogPost;
}

class DialogPost : public QDialog
{
    Q_OBJECT

public:
    explicit DialogPost(const int id, QWidget *parent = nullptr);
    ~DialogPost();
    void setID(const int id);
    int getID() const;
    void actualizarComentarios();

private slots:
    void on_pushButton_arbol_comentarios_clicked();

    void on_pushButton_comentar_clicked();

    void on_pushButton_editar_clicked();

    void on_pushButton_eliminar_clicked();

private:
    Ui::DialogPost *ui;
    int postID;
    void llenarDatos(const int id);
};

#endif // DIALOGPOST_H
