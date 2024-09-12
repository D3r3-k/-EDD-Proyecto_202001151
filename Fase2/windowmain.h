#ifndef WINDOWMAIN_H
#define WINDOWMAIN_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_loginButton_clicked();

    void on_passwordLineEdit_returnPressed();

    void on_emailLineEdit_returnPressed();

    void on_RegisterButton_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // WINDOWMAIN_H
