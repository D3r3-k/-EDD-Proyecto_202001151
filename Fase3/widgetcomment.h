#ifndef WIDGETCOMMENT_H
#define WIDGETCOMMENT_H

#include "StructsComment.h"
#include <QWidget>

namespace Ui {
class WidgetComment;
}

class WidgetComment : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetComment(const StructsComment::Comentario &c, QWidget *parent = nullptr);
    ~WidgetComment();

    void setComment(const StructsComment::Comentario &c);
    StructsComment::Comentario getComment() const;

private:
    Ui::WidgetComment *ui;
    StructsComment::Comentario c;
    void llenarDatos(const StructsComment::Comentario &c);
};

#endif // WIDGETCOMMENT_H
