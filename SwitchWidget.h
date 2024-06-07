#ifndef SWITCHWIDGET_H
#define SWITCHWIDGET_H

#include <QMainWindow>
#include <QWidget>
#include <QPushButton>

#include <QPropertyAnimation>


QT_BEGIN_NAMESPACE
namespace Ui { class SwitchWidget; }
QT_END_NAMESPACE

class SwitchWidget : public QWidget
{
    Q_OBJECT

public:
    SwitchWidget(QWidget *parent = nullptr);
    ~SwitchWidget();
    QPushButton *switchCircle;

private:
    QPropertyAnimation *moveCircle;
    bool switchOn {false};

public slots:
    void onSwitchClicked();

};
#endif // SWITCHWIDGET_H
