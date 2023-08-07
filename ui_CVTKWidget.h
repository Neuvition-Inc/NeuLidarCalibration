/********************************************************************************
** Form generated from reading UI file 'CVTKWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CVTKWIDGET_H
#define UI_CVTKWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "QVTKWidget.h"

QT_BEGIN_NAMESPACE

class Ui_CVTKWidget
{
public:
    QVBoxLayout *verticalLayout;
    QVTKWidget *qvtkWidget;

    void setupUi(QWidget *CVTKWidget)
    {
        if (CVTKWidget->objectName().isEmpty())
            CVTKWidget->setObjectName(QString::fromUtf8("CVTKWidget"));
        CVTKWidget->resize(683, 463);
        verticalLayout = new QVBoxLayout(CVTKWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        qvtkWidget = new QVTKWidget(CVTKWidget);
        qvtkWidget->setObjectName(QString::fromUtf8("qvtkWidget"));

        verticalLayout->addWidget(qvtkWidget);


        retranslateUi(CVTKWidget);

        QMetaObject::connectSlotsByName(CVTKWidget);
    } // setupUi

    void retranslateUi(QWidget *CVTKWidget)
    {
        CVTKWidget->setWindowTitle(QApplication::translate("CVTKWidget", "CVTKWidget", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CVTKWidget: public Ui_CVTKWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CVTKWIDGET_H
