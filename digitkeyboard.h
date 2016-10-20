#ifndef DIGITKEYBOARD_H
#define DIGITKEYBOARD_H

#include <QDialog>

namespace Ui {
class DigitKeyboard;
}

class DigitKeyboard : public QDialog
{
    Q_OBJECT

public:
    explicit DigitKeyboard(QWidget *parent = 0);
    ~DigitKeyboard();

    void setRange(float min, float max);

    void setMessage(QString message);

    float getValue();

private slots:
    void on_btnOk_clicked();

    void on_btnCancel_clicked();

    void on_btnSymbol_0_clicked();

    void on_btnSymbol_1_clicked();

    void on_btnSymbol_2_clicked();

    void on_btnSymbol_3_clicked();

    void on_btnSymbol_4_clicked();

    void on_btnSymbol_5_clicked();

    void on_btnSymbol_6_clicked();

    void on_btnSymbol_7_clicked();

    void on_btnSymbol_8_clicked();

    void on_btnSymbol_9_clicked();

    void on_btnSymbol_minus_clicked();

    void on_btnSymbol_dot_clicked();

    void on_btnClear_clicked();

private:
    Ui::DigitKeyboard *ui;

    float minVal=0, maxVal=0;

    QString message;

    float value;
};

#endif // DIGITKEYBOARD_H
