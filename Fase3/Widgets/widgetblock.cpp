#include "widgetblock.h"
#include "Widgets/ui_widgetblock.h"

WidgetBlock::WidgetBlock(Structs::Block &block, QWidget *parent)
    : QWidget(parent),
    ui(new Ui::WidgetBlock),
    block(block)
{
    ui->setupUi(this);
    setBlock(block);
    QPalette palette = this->palette();
    palette.setColor(QPalette::WindowText, QColor("#000"));
    if (block.validate) {
        palette.setColor(QPalette::Window, QColor("#A2CA93"));
    }else{
        palette.setColor(QPalette::Window, QColor("#CF7B7B"));
    }
    this->setAutoFillBackground(true);
    this->setPalette(palette);
    QPalette spin = ui->bloqueSpinBox->palette();
    spin.setColor(QPalette::Base, QColor("#fff"));
    spin.setColor(QPalette::Text, QColor("#000"));
    ui->bloqueSpinBox->setPalette(spin);
    ui->nonceLineEdit->setPalette(spin);
    ui->timestampLineEdit->setPalette(spin);
    ui->plainTextEdit_data->setPalette(spin);
    ui->hashBlockLineEdit->setPalette(spin);
    ui->prevHashLineEdit->setPalette(spin);
    ui->rootHashLineEdit->setPalette(spin);
}

WidgetBlock::~WidgetBlock()
{
    delete ui;
}

void WidgetBlock::llenarDatos(Structs::Block &b){
    ui->bloqueSpinBox->setValue(b.index);
    ui->nonceLineEdit->setText(QString::number(b.nonce));
    ui->timestampLineEdit->setText(QString::fromStdString(b.timestamp));
    QString data = QString::fromStdString(b.getJsonData());
    ui->plainTextEdit_data->setPlainText(data);
    ui->rootHashLineEdit->setText(QString::fromStdString(b.rootHash));
    ui->hashBlockLineEdit->setText(QString::fromStdString(b.hashBlock));
    ui->prevHashLineEdit->setText(QString::fromStdString(b.prevHash));
}

void WidgetBlock::setBlock(Structs::Block &b){
    this->block = b;
    llenarDatos(b);
}

Structs::Block WidgetBlock::getBlock() const {
    return block;
}
