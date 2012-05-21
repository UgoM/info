#include "src/core/gui/ChatWidget.h"
#include <QtGui>

ChatWidget::ChatWidget()
{
    QLabel * label1 = new QLabel(tr("Chat :"));
    QLabel * label2 = new QLabel(tr("Not implemented yet"));
    QTextEdit * textDisplay = new QTextEdit();
    QLabel * label3 = new QLabel(tr("Send Message :"));
    QLineEdit * textInput = new QLineEdit();

    textDisplay->setFocusPolicy(Qt::NoFocus);
    textDisplay->setReadOnly(true);

	QVBoxLayout * vLayout = new QVBoxLayout();
    vLayout->addWidget(label1);
    vLayout->addWidget(label2);
    vLayout->addWidget(textDisplay, 1);
    vLayout->addWidget(label3);
    vLayout->addWidget(textInput);
	setLayout(vLayout);
}

