#ifndef WIDGETBLOCK_H
#define WIDGETBLOCK_H

#include "../Structs.h"
#include <QWidget>

namespace Ui {
class WidgetBlock;
}

class WidgetBlock : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetBlock(Structs::Block &block, QWidget *parent = nullptr);
    ~WidgetBlock();
    void setBlock(Structs::Block &b);
    Structs::Block getBlock() const;

private:
    Ui::WidgetBlock *ui;
    Structs::Block block;
    void llenarDatos(Structs::Block &b);
};

#endif // WIDGETBLOCK_H
