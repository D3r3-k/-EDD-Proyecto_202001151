#ifndef DIALOGNUEVOPOST_H
#define DIALOGNUEVOPOST_H

#include <QDialog>

namespace Ui {
class DialogNuevoPost;
}

class DialogNuevoPost : public QDialog
{
    Q_OBJECT

public:
    explicit DialogNuevoPost(QWidget *parent = nullptr);
    explicit DialogNuevoPost(const int id, QWidget *parent = nullptr);
    ~DialogNuevoPost();
    void setID(const int id);
    int getID() const;
    std::string getPathImg();
    void setPathImg(const std::string path);

private slots:
    void on_buttonBox_accepted();

    void on_imagenButton_clicked();

private:
    Ui::DialogNuevoPost *ui;
    int postID = -1;
    std::string pathImg = "default.jpg";
    void llenarDatos(const int id);
};

#endif // DIALOGNUEVOPOST_H
