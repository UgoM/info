#include "src/core/gui/ChatWidget.h"
#include <QtGui>

ChatWidget::ChatWidget()
{
    QLabel * label1 = new QLabel(tr("Chat :"));
    QLabel * label2 = new QLabel(tr("Not fully implemented yet"));
    textDisplay = new QTextEdit();
    QLabel * label3 = new QLabel(tr("Send Message :"));
    textInput = new QLineEdit();

    textDisplay->setFocusPolicy(Qt::NoFocus);
    textDisplay->setReadOnly(true);

	QVBoxLayout * vLayout = new QVBoxLayout();
    vLayout->addWidget(label1);
    vLayout->addWidget(label2);
    vLayout->addWidget(textDisplay, 1);
    vLayout->addWidget(label3);
    vLayout->addWidget(textInput);
	setLayout(vLayout);

    connect(textInput, SIGNAL(returnPressed()), this, SLOT(returnPressed()));
}

/** \brief What's happening when new chat data arrive
  * \param s : data
  */
void ChatWidget::newChatData(QString s)
{
    textDisplay->append(s);
    /// \todo faire un peu de mise en forme des messages
}
/** \brief process and send input data
  */
void ChatWidget::returnPressed()
{
    QString text = textInput->text();
    if (text.isEmpty())
        return;

    QSettings settings;
    QString nick = settings.value("global/username").toString();
    emit sendDataToServer(nick + " : " + text);
    textInput->clear();
    qDebug() << "ChatWidget::returnPressed()" << text;
}
