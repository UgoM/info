#pragma once

#include <QWidget>

class QTextEdit;
class QLineEdit;

class ChatWidget : public QWidget {

    Q_OBJECT

    public:
        ChatWidget();

    private:
        QTextEdit * textDisplay;
        QLineEdit * textInput;

    private slots:
        void returnPressed();

    public slots:
        void newChatData(QString s);

    signals:
        void sendDataToServer(QString s);
};

