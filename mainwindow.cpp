#include "mainwindow.h"
#include "./ui_mainwindow.h"

QFile whiteStyleFile(":/style.css");
QFile darkStyleFile(":/dark-style.css");

QMap <QString, States> buttonStates;
QMap <QString, operatorStates> equalStatesMap;

//обработка кнопок (не включая кнопки цифр)
void MainWindow::buttonHandler(aboutButton boutButton) {
    boutButton.button->setObjectName(boutButton.objectName);
    connect(boutButton.button, &QPushButton::clicked, this, &MainWindow::calcSlot);
    buttonStates.insert(boutButton.objectName, boutButton.state);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    //FrontEnd
    setGeometry(100, 100, 250, 500);
    setFixedSize(270, 500);
    statusBar()->hide();
    setWindowTitle("Calculator");

    buttons->setObjectName("Buttons");
    buttons->setGeometry(0, 200, 270, 300);

    horizontalWidget->setObjectName("otherWidget");

    horizontalLayout->addWidget(buttonAC);
    horizontalLayout->addWidget(buttonReverse);
    horizontalLayout->addWidget(buttonPercent);

    screenLayout->addWidget(switchBody, 0, 3, 1, 2);
    switchBody->setObjectName("switchWid");
    screenLayout->addWidget(equalLabel, 6, 0, 2, 8);
    screenLayout->addWidget(processLabel, 1, 0, 5, 8);

    equalLabel->setText("0");
    equalLabel->setAlignment(Qt::AlignRight);
    equalLabel->setObjectName("equalLabelCss");

    processLabel->setText("0");
    processLabel->setAlignment(Qt::AlignRight);
    processLabel->setObjectName("processLabelCss");

    screenWidget->setGeometry(0, 0, 270, 200);
    screenWidget->setObjectName("process");

    mainLayout->addWidget(horizontalWidget, 0, 0, 1, 3);

    verticalWidget->setObjectName("operationsWidget");

    verticalLayout->addWidget(buttonDivision);
    verticalLayout->addWidget(buttonMult);
    verticalLayout->addWidget(buttonMinus);
    verticalLayout->addWidget(buttonPlus);
    verticalLayout->addWidget(buttonEqual);

    mainLayout->addWidget(verticalWidget, 0, 4, 5, 1);

    numWidget->setObjectName("numsWidget");
    //layout STRATCH

    //обработка цифр
    int buttonValue = 1;
    for (int row {0}; row < 3; ++row) {
        for (int col{0}; col < 3; ++col) {
            QPushButton *button = new QPushButton(QString::number(buttonValue++));
            numLayout->addWidget(button, row, col);
            button->setObjectName(QStringLiteral("pushedNum%1").arg(buttonValue-1));
            connect(button, &QPushButton::clicked, this, &MainWindow::calcSlot);
            buttonStates.insert(QStringLiteral("pushedNum%1").arg(buttonValue-1), numState);
        }
    }

    //обработка остальных кнопок
    for (int i{0}; i < 10; ++i) {
        buttonHandler(abouts[i]);
    }

    numLayout->addWidget(buttonZero, 3, 0);
    numLayout->addWidget(buttonPoint, 3, 1);
    numLayout->addWidget(buttonDoubleZero, 3, 2);
    buttonZero->setObjectName("buttonZeroObj");
    mainLayout->addWidget(numWidget, 1, 0, 4, 3);
    mainLayout->setHorizontalSpacing(0);
    mainLayout->setVerticalSpacing(0);

    connect(buttonZero, &QPushButton::clicked, this, &MainWindow::calcSlot);

    //checks
    buttonDivision->setCheckable(true);
    buttonMult->    setCheckable(true);
    // buttonAC->      setCheckable(true);
    buttonPercent-> setCheckable(true);

    buttonStates.insert("buttonZeroObj", zeroState);

    equalStatesMap.insert("buttonDivisionObj", division);
    equalStatesMap.insert("buttonMultObj",     multiply);
    equalStatesMap.insert("buttonMinusObj", subtraction);
    equalStatesMap.insert("buttonPlusObj",     addition);

    expression.append("0");
    //style
    QStateMachine *styleMachine = new QStateMachine();

    QState *whiteState = new QState();
    QState *darkState = new QState();

    styleMachine->addState(whiteState);
    styleMachine->addState(darkState);

    styleMachine->setInitialState(whiteState);

    QSignalTransition *toDarkTransition = whiteState->addTransition(switchWidget->switchCircle, &QPushButton::clicked, darkState);
    QSignalTransition *toWhiteTransition = darkState->addTransition(switchWidget->switchCircle, &QPushButton::clicked, whiteState);

    connect(toDarkTransition, &QSignalTransition::triggered, this, &MainWindow::onSwitchToDark);
    connect(toWhiteTransition, &QSignalTransition::triggered, this, &MainWindow::onSwitchToWhite);

    whiteStyleFile.open(QFile::ReadOnly);
    this->setStyleSheet(whiteStyleFile.readAll());

    styleMachine->start();
}

MainWindow::~MainWindow()
{
}

void MainWindow::onSwitchToDark()
{
    whiteStyleFile.close();
    darkStyleFile.open(QFile::ReadOnly);
    this->setStyleSheet(darkStyleFile.readAll());
}

void MainWindow::onSwitchToWhite()
{
    darkStyleFile.close();
    whiteStyleFile.open(QFile::ReadOnly);
    this->setStyleSheet(whiteStyleFile.readAll());
}

//BACKEND
double MainWindow::engine(double first, double second, operatorStates currentState)
{
    static std::map<operatorStates, std::function<double(double, double)>> operations =
    {
        {division,    [&](double a, double b) { return (b != 0) ? a / b : 0; }},
        {multiply,    [&](double a, double b) { return a * b; }},
        {subtraction, [&](double a, double b) { return a - b; }},
        {addition,    [&](double a, double b) { return a + b; }}
    };

    double insideResult = 0.0;
    if (operations.find(currentState) != operations.end()) {
        insideResult = operations[currentState](first, second);
    } else {
        qDebug() << "sad";
    }
    return insideResult;
}

void MainWindow::computeLocalValue(bool& beforeThereWasMultOrDiv, int currentInd, operatorStates beforeOperatorMultOrDivState)
{
    if (beforeThereWasMultOrDiv)
    {
        int indOfMultOrDivOper = currentInd - 1;

        double x = expression[indOfMultOrDivOper - 1].toDouble();
        double y = expression[indOfMultOrDivOper + 1].toDouble();
        double result = engine(x, y, beforeOperatorMultOrDivState);

        QString resultStr {QString::number(result, 'g', 15)};

        expression.remove(indOfMultOrDivOper);
        expression.remove(indOfMultOrDivOper);
        expression[indOfMultOrDivOper - 1] = resultStr;

        beforeThereWasMultOrDiv = false;
    }
}

QVariant MainWindow::backEnd(QPushButton* currentSender, States state)
{
    static int currentInd{0};

    static operatorStates beforeOperatorMultOrDivState{};
    static bool beforeThereWasMultOrDiv{false};

    QString currentValueStr = expression[currentInd].toString();

    switch (state)
    {
    case numState:
    {
        QString digit{currentSender->text()};

        if (expression[currentInd] == 0)
        {
            expression[currentInd] = digit;
        }
        else
        {
            currentValueStr += digit;
            expression[currentInd] = currentValueStr;
        }
        for (auto& el: expression)
        {
            qDebug() << el;
        }
        return expression[currentInd].toDouble();
    }
    case zeroState:
    {
        QString zero{currentSender->text()};

        if (!(expression[currentInd] == zero))
        {
            currentValueStr += zero;
            expression[currentInd] = currentValueStr;
        }
        return expression[currentInd];
    }
    case pointState:
    {
        if (!(currentValueStr.contains(".")))
        {
            currentValueStr += currentSender->text();
            expression[currentInd] = currentValueStr;
        }
        return expression[currentInd].toString();
    }
    case ACState:
    {
        expression.clear();
        expression.append("0");
        currentInd = 0;
        currentValueStr.clear();
        beforeThereWasMultOrDiv = false;
        return expression[currentInd];
    }
    case reverseState:
    {
        double valueToReverse {expression[currentInd].toDouble()};
        valueToReverse *= -1;
        expression[currentInd] = QString::number(valueToReverse, 'g', 15);
        return expression[currentInd];
    }
    case multOrDivState:
    {
        beforeOperatorMultOrDivState = equalStatesMap.value(currentSender->objectName());
        expression.append(currentSender->text());
        beforeThereWasMultOrDiv = true;
        currentValueStr.clear();

        currentInd += 2;
        expression.append("");
        for (auto& el: expression)
        {
            qDebug() << el;
        }
        return currentValueStr;
    }
    case operState:
    {
        QString currentOperator = currentSender->text();
        expression.append(currentOperator);

        if (beforeThereWasMultOrDiv)
        {
            computeLocalValue(beforeThereWasMultOrDiv, currentInd, beforeOperatorMultOrDivState);
        }
        else currentInd += 2;

        expression.append("");
        currentValueStr.clear();
        for (auto& el: expression)
        {
            qDebug() << el;
        }
        return currentValueStr;
    }
    case EqualState:
    {
        computeLocalValue(beforeThereWasMultOrDiv, currentInd, beforeOperatorMultOrDivState);

        for (int operandIndex {0}; operandIndex < expression.size() - 2; operandIndex += 2)
        {
            int operatorIndex {operandIndex + 1};

            operatorStates currentOperState;
            if (expression[operatorIndex] == "+")  currentOperState = addition;
            else currentOperState = subtraction;

            m_buffer = engine(expression[operandIndex].toDouble(), expression[operandIndex + 2].toDouble(), currentOperState);
            expression[operandIndex + 2] = m_buffer;
        }

        double result;
        result = expression[expression.size() - 1].toDouble();
        QString resultStr = {QString::number(result, 'g', 15)};

        expression.clear();
        expression.append(resultStr);
        currentInd = 0;
        currentValueStr.clear();

        return result;
    }
    case percentState:
    {
        double beforeNum {expression[currentInd - 2].toDouble()};
        double currentNum {expression[currentInd].toDouble()};
        double currentPercentNum {(beforeNum * currentNum) / 100};

        expression[currentInd] = QString::number(currentPercentNum);
        currentValueStr += "%";

        return currentValueStr;
    }
    }
}

void MainWindow::calcSlot()
{
    QPushButton* clickedButton = qobject_cast<QPushButton*>(sender());
    States currentState = buttonStates.value(clickedButton->objectName());

    QVariant result = backEnd(clickedButton, currentState);
    render(result, currentState, clickedButton);
}

void MainWindow::render(QVariant result, States currentState, QPushButton* clickedButton)
{
    static bool beforeWasPoint{};
    static QString processText;
    switch (currentState)
    {
    case numState:
    {
        double resultValue = result.toDouble();
        // if (currentState == pointState) equalLabel->setText(equalLabel->text() + ".0");
        equalLabel->setText(QString::number(resultValue, 'g', 15));
        if (processText == "") processLabel->setText(QString::number(resultValue, 'g', 15));
        else processLabel->setText(processText + QString::number(resultValue, 'g', 15));
        break;
    }
    case zeroState:
    {
        if (beforeWasPoint)
        {
            beforeWasPoint = false;
            return;
        }
        // double resultValue = result.toDouble();
        // if (equalLabel->text() == "0") return;
        // equalLabel->setText(QString::number(resultValue, 'g', 15) + "0");
        // if (processText == "") processLabel->setText(QString::number(resultValue, 'g', 15));
        // else processLabel->setText(processText + "0");
        // break;
    }
    case pointState:
    {
        if (beforeWasPoint) return;
        beforeWasPoint = true;
        equalLabel->setText(equalLabel->text() + ".0");
        processLabel->setText(processLabel->text() + ".0");
        break;
    }
    case operState:
    {
        beforeWasPoint = false;
        equalLabel->clear();
        processLabel->setText(processLabel->text() + " " + clickedButton->text() + " ");
        processText = processLabel->text();
        break;
    }
    case multOrDivState:
    {
        beforeWasPoint = false;
        equalLabel->clear();
        processLabel->setText(processLabel->text() + " " + clickedButton->text() + " ");
        processText = processLabel->text();
        break;
    }
    case ACState:
    {
        equalLabel->setText("0");
        processLabel->setText("0");
        processText.clear();
        beforeWasPoint = false;
        break;
    }
    }

    // equalLabel->clear();

    // switch (currentState)
    // {
    // case ACState:
    // {
    //     processLabel->setText("0");
    //     equalLabel->setText("0");
    //     break;
    // }
    // case operState:
    // {
    //     processLabel->setText(processLabel->text() + " " + clickedButton->text());
    //     break;
    // }
    // case multOrDivState:
    // {
    //     processLabel->setText(processLabel->text() + " " + clickedButton->text());
    //     break;
    // }
    // case pointState:
    // {
    //     processLabel->setText(processLabel->text() + ".0");
    //     break;
    // }
    // }

    // if (result.type() == QVariant::Double)
    // {
    //     (currentState == pointState) ? processLabel->setText(processLabel->text() + ".") : processLabel->clear();
    //     double resultValue = result.toDouble();
    //     if (currentState == pointState) equalLabel->setText(equalLabel->text() + ".0");
    //     equalLabel->setText(equalLabel->text() + QString::number(resultValue, 'g', 15));
    //     processLabel->setText(processLabel->text() + QString::number(resultValue, 'g', 15));
    // }
    // else if (result.type() == QVariant::String) {
    //     QString resultValue = result.toString();
    //     qDebug() << resultValue;
    //     equalLabel->setText(equalLabel->text() + resultValue);
    //     processLabel->setText(processLabel->text() + " " + resultValue);
    // }
}
