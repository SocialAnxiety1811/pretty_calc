#include "switchwidget.h"

SwitchWidget::SwitchWidget(QWidget *parent)
    : QWidget(parent)
{
    if (parent != nullptr)
        this->resize(parent->width(), parent->height());

    switchCircle = new QPushButton(this);
    switchCircle->resize(20, 20);
    switchCircle->setObjectName("switchCirc");

    connect(switchCircle, &QPushButton::clicked, this, &SwitchWidget::onSwitchClicked);

    moveCircle = new QPropertyAnimation(switchCircle, "geometry", this);
    moveCircle->setDuration(100); // длительность анимации
}

SwitchWidget::~SwitchWidget() {}

void SwitchWidget::onSwitchClicked()
{
    int currentX = switchCircle->x();

    if (switchOn == false) {
        moveCircle->setStartValue(switchCircle->geometry());
        moveCircle->setEndValue(
            QRect(currentX + 26, switchCircle->y(), switchCircle->width(), switchCircle->height()));
    } else {
        moveCircle->setStartValue(switchCircle->geometry());
        moveCircle->setEndValue(
            QRect(currentX - 26, switchCircle->y(), switchCircle->width(), switchCircle->height()));
    }

    moveCircle->start();
    switchOn = !switchOn;
}
