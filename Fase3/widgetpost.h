#ifndef WIDGETPOST_H
#define WIDGETPOST_H

#include <QWidget>

namespace Ui {
class WidgetPost;
}

class WidgetPost : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetPost(const int id, QWidget *parent = nullptr);
    ~WidgetPost();
    void setID(const int id);
    int getID() const;

private slots:

    void on_pushButton_ver_clicked();

    void on_eliminarButton_clicked();

    void on_editarButton_clicked();

private:
    Ui::WidgetPost *ui;
    int postID;
    void llenarDatos(const int id);
};

#endif // WIDGETPOST_H
