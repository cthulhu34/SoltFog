#include "digitkeyboard.h"
#include "ui_digitkeyboard.h"
#include <QFile>
#include <QKeyEvent>
#include <QInputMethodEvent>

DigitKeyboard::DigitKeyboard(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DigitKeyboard)
{
    ui->setupUi(this);

    //QFile styleTab;
    //styleTab.setFileName(":/styles/keyboard-style");
   // styleTab.open(QFile::ReadOnly);
   // QString qssStr = styleTab.readAll();
   // this->setStyleSheet(qssStr);

    ui->lineEdit->setFocus();
}

DigitKeyboard::~DigitKeyboard()
{
    delete ui;
}

void DigitKeyboard::on_btnOk_clicked()
{

    if(ui->lineEdit->hasAcceptableInput())
    {
        value = ui->lineEdit->text().toFloat();
        this->accept();
    }
    else
    {
        //ui->lineEdit->clear();
        setMessage("Неверное значение");
    }
}

void DigitKeyboard::on_btnCancel_clicked()
{
     this->reject();
}

void DigitKeyboard::setRange(float min, float max)
{
    this->minVal = min;
    this->maxVal = max;

    this->ui->lblMinValue->setText(QString::number(min, 'f', 2));
    this->ui->lblMaxValue->setText(QString::number(max, 'f', 2));

    QDoubleValidator *validator = new QDoubleValidator(minVal, maxVal, 2, this);
    validator->setNotation(QDoubleValidator::StandardNotation);
    ui->lineEdit->setValidator(validator);
}

void DigitKeyboard::setMessage(QString message)
{
    this->message = message;
    this->ui->lblStatusBar->setText(message);
}

void DigitKeyboard::on_btnSymbol_0_clicked()
{
    QInputMethodEvent ev;
    ev.setCommitString("0");
    QCoreApplication::sendEvent(ui->lineEdit, &ev);

}

void DigitKeyboard::on_btnSymbol_1_clicked()
{
    QInputMethodEvent ev;
    ev.setCommitString("1");
    QCoreApplication::sendEvent(ui->lineEdit, &ev);
}

void DigitKeyboard::on_btnSymbol_2_clicked()
{
    QInputMethodEvent ev;
    ev.setCommitString("2");
    QCoreApplication::sendEvent(ui->lineEdit, &ev);
}

void DigitKeyboard::on_btnSymbol_3_clicked()
{
    QInputMethodEvent ev;
    ev.setCommitString("3");
    QCoreApplication::sendEvent(ui->lineEdit, &ev);
}

void DigitKeyboard::on_btnSymbol_4_clicked()
{
    QInputMethodEvent ev;
    ev.setCommitString("4");
    QCoreApplication::sendEvent(ui->lineEdit, &ev);
}

void DigitKeyboard::on_btnSymbol_5_clicked()
{
    QInputMethodEvent ev;
    ev.setCommitString("5");
    QCoreApplication::sendEvent(ui->lineEdit, &ev);
}

void DigitKeyboard::on_btnSymbol_6_clicked()
{
    QInputMethodEvent ev;
    ev.setCommitString("6");
    QCoreApplication::sendEvent(ui->lineEdit, &ev);
}

void DigitKeyboard::on_btnSymbol_7_clicked()
{
    QInputMethodEvent ev;
    ev.setCommitString("7");
    QCoreApplication::sendEvent(ui->lineEdit, &ev);
}

void DigitKeyboard::on_btnSymbol_8_clicked()
{
    QInputMethodEvent ev;
    ev.setCommitString("8");
    QCoreApplication::sendEvent(ui->lineEdit, &ev);
}


void DigitKeyboard::on_btnSymbol_9_clicked()
{
    QInputMethodEvent ev;
    ev.setCommitString("9");
    QCoreApplication::sendEvent(ui->lineEdit, &ev);
}

void DigitKeyboard::on_btnSymbol_minus_clicked()
{
    QInputMethodEvent ev;
    ev.setCommitString("-");
    QCoreApplication::sendEvent(ui->lineEdit, &ev);
}

void DigitKeyboard::on_btnSymbol_dot_clicked()
{
    QInputMethodEvent ev;
    ev.setCommitString(".");
    QCoreApplication::sendEvent(ui->lineEdit, &ev);
}

void DigitKeyboard::on_btnClear_clicked()
{
    ui->lineEdit->clear();
}

float DigitKeyboard::getValue()
{
    //return ui->lineEdit->text().toFloat();
    return this->value;
}
