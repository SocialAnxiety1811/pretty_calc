#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QPushButton>
#include <QGridLayout>
#include <QDebug>
#include <QLabel>
#include <QStateMachine>
#include <QFile>
#include <QSignalTransition>
#include <QMap>
#include <QFontMetrics>
#include <QVector>

#include "SwitchWidget.h"

enum States {
    numState,
    zeroState,
    operState,
    multOrDivState,
    EqualState,
    ACState,
    percentState,
    reverseState,
    pointState
};

enum operatorStates
{
    division,
    multiply,
    subtraction,
    addition
};

struct aboutButton {
    QPushButton* button;
    QString objectName;
    States state;
};

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QWidget *buttons =      new QWidget(this);
    QWidget *screenWidget = new QWidget(this);

    QLabel *processLabel = new QLabel(screenWidget);
    QLabel *equalLabel =   new QLabel(screenWidget);

    QWidget *horizontalWidget = new QWidget(buttons);
    QWidget *verticalWidget =   new QWidget(buttons);
    QWidget *numWidget =        new QWidget(buttons);

    QGridLayout *mainLayout = new QGridLayout(buttons);
    QGridLayout *numLayout = new QGridLayout(numWidget);

    QHBoxLayout *horizontalLayout = new QHBoxLayout(horizontalWidget);
    QVBoxLayout *verticalLayout =   new QVBoxLayout(verticalWidget);
    QGridLayout *screenLayout =     new QGridLayout(screenWidget);

    QPushButton *buttonAC =      new QPushButton("AC");
    QPushButton *buttonReverse = new QPushButton("+/-");
    QPushButton *buttonPercent = new QPushButton("%");

    QPushButton *buttonDivision = new QPushButton("/");
    QPushButton *buttonMult =     new QPushButton("x");
    QPushButton *buttonMinus =    new QPushButton("-");
    QPushButton *buttonPlus =     new QPushButton("+");
    QPushButton *buttonEqual =    new QPushButton("=");

    QPushButton *buttonZero =       new QPushButton("0");
    QPushButton *buttonPoint =      new QPushButton(".");
    QPushButton *buttonDoubleZero = new QPushButton("00");

    QWidget *switchBody        = new QWidget(screenWidget);
    SwitchWidget *switchWidget = new SwitchWidget(switchBody);

    double engine(double first, double second, operatorStates currentState);
    void computeLocalValue(bool& beforeThereWasMultOrDiv, int currentInd, operatorStates beforeOperatorMultOrDivState);
    QVariant backEnd(QPushButton* currentSender, States state);
    void render(QVariant result, States currentState, QPushButton* clickedButton);

    double m_buffer;
    QVector <QVariant> expression;
    QString processLabelStr{};

    operatorStates setOperState();

    aboutButton aboutButton1 {buttonAC,"buttonACObj" ,ACState};
    aboutButton aboutButton2 {buttonReverse, "buttonReverseObj", reverseState};
    aboutButton aboutButton3 {buttonPercent, "buttonPercentObj", percentState};
    aboutButton aboutButton4 {buttonDivision, "buttonDivisionObj", multOrDivState};
    aboutButton aboutButton5 {buttonMult, "buttonMultObj", multOrDivState};
    aboutButton aboutButton6 {buttonMinus, "buttonMinusObj", operState};
    aboutButton aboutButton7 {buttonPlus, "buttonPlusObj", operState};
    aboutButton aboutButton8 {buttonEqual, "buttonEqualObj", EqualState};
    aboutButton aboutButton9 {buttonDoubleZero, "buttonDoubleZeroObj", zeroState};
    aboutButton aboutButton10 {buttonPoint, "buttonPointObj", pointState};
    aboutButton abouts[10] {
        aboutButton1, aboutButton2, aboutButton3, aboutButton4, aboutButton5, aboutButton6,
        aboutButton7, aboutButton8, aboutButton9, aboutButton10};

    void buttonHandler(aboutButton boutButton);

private slots:
    //theme
    void onSwitchToDark();
    void onSwitchToWhite();

    void calcSlot();
};
#endif // MAINWINDOW_H
