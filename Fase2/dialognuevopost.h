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
    ~DialogNuevoPost();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::DialogNuevoPost *ui;
};

#endif // DIALOGNUEVOPOST_H
