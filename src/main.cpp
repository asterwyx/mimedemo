#include <QApplication>
#include <QFile>
#include <QDataStream>
#include <QString>
#include <QMainWindow>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QDebug>

const char *Magic = "ADWF";
struct Weather
{
    qint32 minTemper = 0;
    qint32 maxTemper = 0;
    qint32 currentTemper = 0;
    QString description = "";
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QMainWindow window;
    if (argc != 2) {
        QMessageBox::critical(nullptr, "错误", QString("参数个数错误，参数个数为：%1").arg(argc));
        qInfo() << "usage: mimedemo <filepath>";
        exit(EXIT_FAILURE);
    }
    QString fileName = argv[1];
    QFile weatherFile(fileName);
    if (!weatherFile.exists()) {
        QMessageBox::critical(nullptr, "错误", QString("文件%1不存在").arg(fileName));
        qCritical() << "File does not exist!";
        exit(EXIT_FAILURE);
    }
    Weather weather;
    while (!weatherFile.open(QIODevice::ReadOnly))
        ;
    {
        QDataStream stream(&weatherFile);
        QString magicStr;
        stream >> magicStr;
        if (magicStr != Magic) {
            qCritical() << "File type does not support!";
            exit(EXIT_FAILURE);
        }
        stream >> weather.minTemper;
        stream >> weather.maxTemper;
        stream >> weather.currentTemper;
        stream >> weather.description;
        weatherFile.close();
    }
    QObject::connect(&app, &QApplication::aboutToQuit, &app, [&weatherFile, &weather]() {
        qDebug() << "Application is about to quit!";
        qDebug() << "Weather:" << weather.minTemper << "," << weather.maxTemper << "," << weather.currentTemper << ","
                 << weather.description;
        weatherFile.open(QIODevice::WriteOnly);
        QDataStream stream(&weatherFile);
        stream << QString(Magic);
        stream << weather.minTemper;
        stream << weather.maxTemper;
        stream << weather.currentTemper;
        stream << weather.description;
        weatherFile.close();
    });
    QWidget *centralWidget = new QWidget(&window);
    QVBoxLayout *windowLayout = new QVBoxLayout(centralWidget);
    QGridLayout *layout = new QGridLayout();
    windowLayout->addLayout(layout);
    QPushButton *saveButton = new QPushButton("保存天气信息", &window);
    {
        QLabel *label = new QLabel("最低温度: ", &window);
        label->setAlignment(Qt::AlignLeft);
        QLineEdit *edit = new QLineEdit(&window);
        edit->setText(QString::number(weather.minTemper));
        layout->addWidget(label, 0, 0);
        layout->addWidget(edit, 0, 1);
        QObject::connect(
            saveButton, &QPushButton::clicked, &window, [=, &weather]() { weather.minTemper = edit->text().toInt(); });
    }
    {
        QLabel *label = new QLabel("最高温度: ", &window);
        label->setAlignment(Qt::AlignLeft);
        QLineEdit *edit = new QLineEdit(&window);
        edit->setText(QString::number(weather.maxTemper));
        layout->addWidget(label, 1, 0);
        layout->addWidget(edit, 1, 1);
        QObject::connect(
            saveButton, &QPushButton::clicked, &window, [=, &weather]() { weather.maxTemper = edit->text().toInt(); });
    }
    {
        QLabel *label = new QLabel("当前温度: ", &window);
        label->setAlignment(Qt::AlignLeft);
        QLineEdit *edit = new QLineEdit(&window);
        edit->setText(QString::number(weather.currentTemper));
        layout->addWidget(label, 2, 0);
        layout->addWidget(edit, 2, 1);
        QObject::connect(
            saveButton, &QPushButton::clicked, &window, [=, &weather]() { weather.currentTemper = edit->text().toInt(); });
    }
    {
        QLabel *label = new QLabel("天气状况: ", &window);
        label->setAlignment(Qt::AlignLeft);
        QLineEdit *edit = new QLineEdit(&window);
        edit->setText(weather.description);
        layout->addWidget(label, 3, 0);
        layout->addWidget(edit, 3, 1);
        QObject::connect(saveButton, &QPushButton::clicked, &window, [=, &weather]() { weather.description = edit->text(); });
    }
    QObject::connect(saveButton, &QPushButton::clicked, &window, [&window]() {
        QMessageBox::information(&window, "保存结果", "成功保存天气信息!");
    });
    windowLayout->addWidget(saveButton);
    window.setCentralWidget(centralWidget);
    window.show();
    return app.exec();
}