/********************************************************************************
** Form generated from reading UI file 'MainWin.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWIN_H
#define UI_MAINWIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWin
{
public:
    QVBoxLayout *verticalLayout;
    QWidget *widget;
    QVBoxLayout *verticalLayout_2;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLabel *label_2;
    QPushButton *MinButton;
    QPushButton *MaxButton;
    QPushButton *CloseButton;
    QWidget *widget_3;
    QHBoxLayout *horizontalLayout_2;
    QWidget *widget_4;
    QFrame *frame;
    QFrame *frame_2;
    QVBoxLayout *verticalLayout_3;
    QFrame *frame_3;
    QPushButton *m_SwitchBtn;
    QLabel *label_3;
    QLabel *label_4;
    QFrame *frame_4;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_7;
    QLabel *label_8;
    QFrame *frame_5;
    QLabel *label_9;
    QLabel *label_10;
    QProgressBar *progressBar;
    QLabel *label_11;
    QPushButton *m_StartBtn;
    QPushButton *m_ChooseFileBtn;
    QPushButton *m_SaveObjPCL;
    QFrame *frame_6;
    QLabel *label_12;
    QLabel *label_13;
    QLabel *label_14;
    QLabel *label_15;
    QLabel *label_16;
    QLabel *label_17;
    QLabel *label_18;
    QLabel *label_19;
    QLabel *label_20;
    QLabel *label_21;
    QLabel *label_22;
    QLabel *label_23;
    QWidget *widget_5;

    void setupUi(QWidget *MainWin)
    {
        if (MainWin->objectName().isEmpty())
            MainWin->setObjectName(QString::fromUtf8("MainWin"));
        MainWin->resize(1190, 842);
        MainWin->setStyleSheet(QString::fromUtf8("QWidge{\n"
"						background: rgb(0, 0, 0);\n"
"}\n"
"QMessageBox{\n"
"						background-color: rgb(110, 119, 138);\n"
"                     border:1px solid #CCFFF6;\n"
"                     border-radius:3px;\n"
"                    }\n"
""));
        verticalLayout = new QVBoxLayout(MainWin);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        widget = new QWidget(MainWin);
        widget->setObjectName(QString::fromUtf8("widget"));
        verticalLayout_2 = new QVBoxLayout(widget);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        widget_2 = new QWidget(widget);
        widget_2->setObjectName(QString::fromUtf8("widget_2"));
        widget_2->setStyleSheet(QString::fromUtf8("width: 1920px;\n"
"height: 60px;\n"
"background: #23272D;"));
        horizontalLayout = new QHBoxLayout(widget_2);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(3, 0, 3, 0);
        label = new QLabel(widget_2);
        label->setObjectName(QString::fromUtf8("label"));
        label->setStyleSheet(QString::fromUtf8("border-image: url();\n"
"width: 144px;\n"
"height: 17px;\n"
"font-size: 24px;\n"
"font:  \"URW Gothic L\";\n"
"font: italic 24pt \"URW Bookman L\";\n"
"font-weight: bold;\n"
"color:rgb(119, 211, 233);\n"
"line-height: 280px;\n"
""));
        label->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(label);

        label_2 = new QLabel(widget_2);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setStyleSheet(QString::fromUtf8("width: 100px;\n"
"height: 16px;\n"
"font-size: 18px;\n"
"font-family: Arial;\n"
"font-weight: 400;\n"
"color: #FFFFFF;\n"
""));
        label_2->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(label_2);

        MinButton = new QPushButton(widget_2);
        MinButton->setObjectName(QString::fromUtf8("MinButton"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);
        sizePolicy.setHorizontalStretch(30);
        sizePolicy.setVerticalStretch(30);
        sizePolicy.setHeightForWidth(MinButton->sizePolicy().hasHeightForWidth());
        MinButton->setSizePolicy(sizePolicy);
        MinButton->setMinimumSize(QSize(30, 30));
        MinButton->setMaximumSize(QSize(30, 30));
        MinButton->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"width: 30px;\n"
"height: 30px;\n"
"border-image:url(:/image/res/icon_minus.png);\n"
"background-color: rgba(0, 113, 142, 0%);\n"
"border-radius:0px\n"
"}\n"
"QPushButton:hover{\n"
"width: 50px;\n"
"height: 50px;\n"
"background-color: rgba(0, 113, 142, 50%);\n"
"}\n"
"QPushButton:pressed{\n"
"background-color: rgba(0, 113, 142, 100%);\n"
"}\n"
""));

        horizontalLayout->addWidget(MinButton);

        MaxButton = new QPushButton(widget_2);
        MaxButton->setObjectName(QString::fromUtf8("MaxButton"));
        sizePolicy.setHeightForWidth(MaxButton->sizePolicy().hasHeightForWidth());
        MaxButton->setSizePolicy(sizePolicy);
        MaxButton->setMinimumSize(QSize(30, 30));
        MaxButton->setMaximumSize(QSize(30, 30));
        MaxButton->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"width: 30px;\n"
"height: 30px;\n"
"border-image:url(:/image/res/icon_max.png);\n"
"background-color: rgba(0, 113, 142, 0%);\n"
"border-radius:0px\n"
"}\n"
"QPushButton:hover{\n"
"width: 50px;\n"
"height: 50px;\n"
"background-color: rgba(0, 113, 142, 50%);\n"
"}\n"
"QPushButton:pressed{\n"
"background-color: rgba(0, 113, 142, 100%);\n"
"}\n"
""));

        horizontalLayout->addWidget(MaxButton);

        CloseButton = new QPushButton(widget_2);
        CloseButton->setObjectName(QString::fromUtf8("CloseButton"));
        CloseButton->setMinimumSize(QSize(30, 30));
        CloseButton->setMaximumSize(QSize(30, 30));
        CloseButton->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"width: 30px;\n"
"height: 30px;\n"
"border-image:url(:/image/res/icon_close.png);\n"
"background-color: rgba(0, 113, 142, 0%);\n"
"border-radius:0px\n"
"}\n"
"QPushButton:hover{\n"
"width: 50px;\n"
"height: 50px;\n"
"background-color: rgba(0, 113, 142, 50%);\n"
"}\n"
"QPushButton:pressed{\n"
"background-color: rgba(0, 113, 142, 100%);\n"
"}"));

        horizontalLayout->addWidget(CloseButton);

        horizontalLayout->setStretch(0, 3);
        horizontalLayout->setStretch(1, 20);
        horizontalLayout->setStretch(2, 2);
        horizontalLayout->setStretch(3, 2);
        horizontalLayout->setStretch(4, 2);

        verticalLayout_2->addWidget(widget_2);

        widget_3 = new QWidget(widget);
        widget_3->setObjectName(QString::fromUtf8("widget_3"));
        widget_3->setStyleSheet(QString::fromUtf8(""));
        horizontalLayout_2 = new QHBoxLayout(widget_3);
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        widget_4 = new QWidget(widget_3);
        widget_4->setObjectName(QString::fromUtf8("widget_4"));
        widget_4->setStyleSheet(QString::fromUtf8("width: 1920px;\n"
"height: 60px;\n"
"background: #23272D;"));

        horizontalLayout_2->addWidget(widget_4);

        frame = new QFrame(widget_3);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setStyleSheet(QString::fromUtf8("background: rgb(0, 0, 0);"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);

        horizontalLayout_2->addWidget(frame);

        frame_2 = new QFrame(widget_3);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setMinimumSize(QSize(340, 0));
        frame_2->setMaximumSize(QSize(340, 16777215));
        frame_2->setStyleSheet(QString::fromUtf8("width: 1920px;\n"
"height: 60px;\n"
"background: #23272D;"));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        verticalLayout_3 = new QVBoxLayout(frame_2);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        frame_3 = new QFrame(frame_2);
        frame_3->setObjectName(QString::fromUtf8("frame_3"));
        frame_3->setFrameShape(QFrame::StyledPanel);
        frame_3->setFrameShadow(QFrame::Raised);
        m_SwitchBtn = new QPushButton(frame_3);
        m_SwitchBtn->setObjectName(QString::fromUtf8("m_SwitchBtn"));
        m_SwitchBtn->setEnabled(true);
        m_SwitchBtn->setGeometry(QRect(110, 50, 100, 91));
        m_SwitchBtn->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	border-image:url(:/image/res/icon_connect_nor.png);	\n"
"	background-color:rgba(255, 255, 255, 0);\n"
"}\n"
"QPushButton:hover{\n"
"    border-image:url(:/image/res/icon_connect_hover.png);\n"
"	 background-color:rgba(255, 255, 255, 0);\n"
"}\n"
"QPushButton:pressed{\n"
"    border-image:url(:/image/res/icon_connect_pressed.png);\n"
"	 background-color:rgba(255, 255, 255, 0);\n"
"}"));
        label_3 = new QLabel(frame_3);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(10, 15, 5, 17));
        label_3->setStyleSheet(QString::fromUtf8("width: 3px;\n"
"height: 12px;\n"
"border-image: url();\n"
"background: #0066FF ;\n"
"\n"
""));
        label_4 = new QLabel(frame_3);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(30, 10, 119, 22));
        label_4->setStyleSheet(QString::fromUtf8("width: 112px;\n"
"height: 11px;\n"
"font-size: 14px;\n"
"font-family: Arial;\n"
"font-weight: 400;\n"
"color: #0066FF ;\n"
"line-height: 36px;\n"
"\n"
""));
        label_4->setAlignment(Qt::AlignCenter);

        verticalLayout_3->addWidget(frame_3);

        frame_4 = new QFrame(frame_2);
        frame_4->setObjectName(QString::fromUtf8("frame_4"));
        frame_4->setFrameShape(QFrame::StyledPanel);
        frame_4->setFrameShadow(QFrame::Raised);
        label_5 = new QLabel(frame_4);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(10, 15, 5, 17));
        label_5->setStyleSheet(QString::fromUtf8("width: 3px;\n"
"height: 12px;\n"
"border-image: url();\n"
"background: #0066FF ;\n"
"\n"
""));
        label_6 = new QLabel(frame_4);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(30, 10, 119, 22));
        label_6->setStyleSheet(QString::fromUtf8("width: 112px;\n"
"height: 11px;\n"
"font-size: 14px;\n"
"font-family: Arial;\n"
"font-weight: 400;\n"
"color: #0066FF ;\n"
"line-height: 36px;\n"
"\n"
""));
        label_6->setAlignment(Qt::AlignCenter);
        label_7 = new QLabel(frame_4);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(20, 46, 81, 31));
        label_7->setStyleSheet(QString::fromUtf8("width: 112px;\n"
"height: 11px;\n"
"font-size: 18px;\n"
"font-family: Arial;\n"
"font-weight: 400;\n"
"color:#FFFFFF;\n"
"line-height: 36px;"));
        label_8 = new QLabel(frame_4);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(120, 46, 121, 31));
        label_8->setStyleSheet(QString::fromUtf8("width: 112px;\n"
"height: 11px;\n"
"font-size: 18px;\n"
"font-family: Arial;\n"
"font-weight: 400;\n"
"color:rgba(240, 244, 17,255);\n"
"line-height: 36px;"));
        label_8->setAlignment(Qt::AlignCenter);

        verticalLayout_3->addWidget(frame_4);

        frame_5 = new QFrame(frame_2);
        frame_5->setObjectName(QString::fromUtf8("frame_5"));
        frame_5->setFrameShape(QFrame::StyledPanel);
        frame_5->setFrameShadow(QFrame::Raised);
        label_9 = new QLabel(frame_5);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(10, 15, 5, 17));
        label_9->setStyleSheet(QString::fromUtf8("width: 3px;\n"
"height: 12px;\n"
"border-image: url();\n"
"background: #0066FF ;\n"
"\n"
""));
        label_10 = new QLabel(frame_5);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(30, 10, 119, 22));
        label_10->setStyleSheet(QString::fromUtf8("width: 112px;\n"
"height: 11px;\n"
"font-size: 14px;\n"
"font-family: Arial;\n"
"font-weight: 400;\n"
"color: #0066FF ;\n"
"line-height: 36px;\n"
"\n"
""));
        label_10->setAlignment(Qt::AlignCenter);
        progressBar = new QProgressBar(frame_5);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setGeometry(QRect(10, 70, 301, 23));
        progressBar->setStyleSheet(QString::fromUtf8("QProgressBar {\n"
"\n"
"border: 2px solid grey;\n"
"\n"
"border-radius: 5px;\n"
"\n"
"}\n"
"\n"
"QProgressBar::chunk {\n"
"\n"
"background-color: rgb(85, 170, 127);\n"
"\n"
"width: 20px;\n"
"\n"
"}"));
        progressBar->setValue(0);
        progressBar->setAlignment(Qt::AlignCenter);
        label_11 = new QLabel(frame_5);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setGeometry(QRect(10, 100, 301, 41));
        label_11->setStyleSheet(QString::fromUtf8("width: 112px;\n"
"height: 11px;\n"
"font-size: 18px;\n"
"font-family: Arial;\n"
"font-weight: 400;\n"
"color:#FFFFFF;\n"
"line-height: 36px;"));
        label_11->setAlignment(Qt::AlignCenter);
        m_StartBtn = new QPushButton(frame_5);
        m_StartBtn->setObjectName(QString::fromUtf8("m_StartBtn"));
        m_StartBtn->setEnabled(true);
        m_StartBtn->setGeometry(QRect(10, 190, 301, 31));
        m_StartBtn->setStyleSheet(QString::fromUtf8("QPushButton\n"
"{\n"
"	background-color:rgba(45, 36, 193, 216);\n"
"}\n"
"QPushButton:hover\n"
"{\n"
"	background-color:rgb(44, 17, 244);\n"
"}\n"
"QPushButton:pressed\n"
"{\n"
"	background-color:rgb(15, 61, 143);\n"
"}"));
        m_ChooseFileBtn = new QPushButton(frame_5);
        m_ChooseFileBtn->setObjectName(QString::fromUtf8("m_ChooseFileBtn"));
        m_ChooseFileBtn->setGeometry(QRect(10, 150, 301, 31));
        m_ChooseFileBtn->setStyleSheet(QString::fromUtf8("QPushButton\n"
"{\n"
"	background-color:rgba(45, 36, 193, 216);\n"
"}\n"
"QPushButton:hover\n"
"{\n"
"	background-color:rgb(44, 17, 244);\n"
"}\n"
"QPushButton:pressed\n"
"{\n"
"	background-color:rgb(15, 61, 143);\n"
"}"));
        m_SaveObjPCL = new QPushButton(frame_5);
        m_SaveObjPCL->setObjectName(QString::fromUtf8("m_SaveObjPCL"));
        m_SaveObjPCL->setGeometry(QRect(10, 230, 301, 31));
        m_SaveObjPCL->setStyleSheet(QString::fromUtf8("QPushButton\n"
"{\n"
"	background-color:rgba(45, 36, 193, 216);\n"
"}\n"
"QPushButton:hover\n"
"{\n"
"	background-color:rgb(44, 17, 244);\n"
"}\n"
"QPushButton:pressed\n"
"{\n"
"	background-color:rgb(15, 61, 143);\n"
"}"));

        verticalLayout_3->addWidget(frame_5);

        frame_6 = new QFrame(frame_2);
        frame_6->setObjectName(QString::fromUtf8("frame_6"));
        frame_6->setFrameShape(QFrame::StyledPanel);
        frame_6->setFrameShadow(QFrame::Raised);
        label_12 = new QLabel(frame_6);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setGeometry(QRect(10, 15, 5, 17));
        label_12->setStyleSheet(QString::fromUtf8("width: 3px;\n"
"height: 12px;\n"
"border-image: url();\n"
"background: #0066FF ;\n"
"\n"
""));
        label_13 = new QLabel(frame_6);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setGeometry(QRect(30, 10, 119, 22));
        label_13->setStyleSheet(QString::fromUtf8("width: 112px;\n"
"height: 11px;\n"
"font-size: 14px;\n"
"font-family: Arial;\n"
"font-weight: 400;\n"
"color: #0066FF ;\n"
"line-height: 36px;\n"
"\n"
""));
        label_13->setAlignment(Qt::AlignCenter);
        label_14 = new QLabel(frame_6);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setGeometry(QRect(10, 50, 140, 17));
        label_14->setStyleSheet(QString::fromUtf8("width: 112px;\n"
"height: 11px;\n"
"font-size: 16px;\n"
"font-family: Arial;\n"
"font-weight: 400;\n"
"color:rgba(255, 255, 200,200);\n"
"line-height: 36px;"));
        label_14->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_15 = new QLabel(frame_6);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setGeometry(QRect(10, 90, 140, 20));
        label_15->setStyleSheet(QString::fromUtf8("width: 112px;\n"
"height: 11px;\n"
"font-size: 16px;\n"
"font-family: Arial;\n"
"font-weight: 400;\n"
"color:rgba(255, 255, 200,200);\n"
"line-height: 36px;"));
        label_15->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_16 = new QLabel(frame_6);
        label_16->setObjectName(QString::fromUtf8("label_16"));
        label_16->setGeometry(QRect(10, 130, 140, 20));
        label_16->setStyleSheet(QString::fromUtf8("width: 112px;\n"
"height: 11px;\n"
"font-size: 16px;\n"
"font-family: Arial;\n"
"font-weight: 400;\n"
"color:rgba(255, 255, 200,200);\n"
"line-height: 36px;"));
        label_16->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_17 = new QLabel(frame_6);
        label_17->setObjectName(QString::fromUtf8("label_17"));
        label_17->setGeometry(QRect(10, 170, 140, 17));
        label_17->setStyleSheet(QString::fromUtf8("width: 112px;\n"
"height: 11px;\n"
"font-size: 16px;\n"
"font-family: Arial;\n"
"font-weight: 400;\n"
"color:rgba(255, 255, 200,200);\n"
"line-height: 36px;"));
        label_17->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_18 = new QLabel(frame_6);
        label_18->setObjectName(QString::fromUtf8("label_18"));
        label_18->setGeometry(QRect(10, 210, 140, 17));
        label_18->setStyleSheet(QString::fromUtf8("width: 112px;\n"
"height: 11px;\n"
"font-size: 16px;\n"
"font-family: Arial;\n"
"font-weight: 400;\n"
"color:rgba(255, 255, 200,200);\n"
"line-height: 36px;"));
        label_18->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_19 = new QLabel(frame_6);
        label_19->setObjectName(QString::fromUtf8("label_19"));
        label_19->setGeometry(QRect(150, 50, 140, 17));
        label_19->setStyleSheet(QString::fromUtf8("width: 112px;\n"
"height: 11px;\n"
"font-size: 16px;\n"
"font-family: Arial;\n"
"font-weight: 400;\n"
"color:rgba(240, 244, 17,255);\n"
"line-height: 36px;"));
        label_20 = new QLabel(frame_6);
        label_20->setObjectName(QString::fromUtf8("label_20"));
        label_20->setGeometry(QRect(150, 90, 140, 17));
        label_20->setStyleSheet(QString::fromUtf8("width: 112px;\n"
"height: 11px;\n"
"font-size: 16px;\n"
"font-family: Arial;\n"
"font-weight: 400;\n"
"color:rgba(240, 244, 17,255);\n"
"line-height: 36px;"));
        label_21 = new QLabel(frame_6);
        label_21->setObjectName(QString::fromUtf8("label_21"));
        label_21->setGeometry(QRect(150, 130, 140, 17));
        label_21->setStyleSheet(QString::fromUtf8("width: 112px;\n"
"height: 11px;\n"
"font-size: 16px;\n"
"font-family: Arial;\n"
"font-weight: 400;\n"
"color:rgba(240, 244, 17,255);\n"
"line-height: 36px;"));
        label_22 = new QLabel(frame_6);
        label_22->setObjectName(QString::fromUtf8("label_22"));
        label_22->setGeometry(QRect(150, 170, 140, 17));
        label_22->setStyleSheet(QString::fromUtf8("width: 112px;\n"
"height: 11px;\n"
"font-size: 16px;\n"
"font-family: Arial;\n"
"font-weight: 400;\n"
"color:rgba(240, 244, 17,255);\n"
"line-height: 36px;"));
        label_23 = new QLabel(frame_6);
        label_23->setObjectName(QString::fromUtf8("label_23"));
        label_23->setGeometry(QRect(150, 210, 140, 17));
        label_23->setStyleSheet(QString::fromUtf8("width: 112px;\n"
"height: 11px;\n"
"font-size: 16px;\n"
"font-family: Arial;\n"
"font-weight: 400;\n"
"color:rgba(240, 244, 17,255);\n"
"line-height: 36px;"));

        verticalLayout_3->addWidget(frame_6);

        verticalLayout_3->setStretch(0, 3);
        verticalLayout_3->setStretch(1, 2);
        verticalLayout_3->setStretch(2, 6);
        verticalLayout_3->setStretch(3, 5);

        horizontalLayout_2->addWidget(frame_2);

        horizontalLayout_2->setStretch(0, 1);
        horizontalLayout_2->setStretch(1, 20);
        horizontalLayout_2->setStretch(2, 8);

        verticalLayout_2->addWidget(widget_3);

        widget_5 = new QWidget(widget);
        widget_5->setObjectName(QString::fromUtf8("widget_5"));
        widget_5->setStyleSheet(QString::fromUtf8("width: 1920px;\n"
"height: 60px;\n"
"background: #23272D;"));

        verticalLayout_2->addWidget(widget_5);

        verticalLayout_2->setStretch(0, 10);
        verticalLayout_2->setStretch(1, 100);
        verticalLayout_2->setStretch(2, 1);

        verticalLayout->addWidget(widget);


        retranslateUi(MainWin);

        QMetaObject::connectSlotsByName(MainWin);
    } // setupUi

    void retranslateUi(QWidget *MainWin)
    {
        MainWin->setWindowTitle(QApplication::translate("MainWin", "Form", nullptr));
        label->setText(QApplication::translate("MainWin", "Neuvition", nullptr));
        label_2->setText(QApplication::translate("MainWin", "\351\233\267\350\276\276\346\240\207\345\256\232\347\263\273\347\273\237", nullptr));
        MinButton->setText(QString());
        MaxButton->setText(QString());
        CloseButton->setText(QString());
        m_SwitchBtn->setText(QString());
        label_3->setText(QString());
        label_4->setText(QApplication::translate("MainWin", "\350\256\276\345\244\207\350\277\236\346\216\245", nullptr));
        label_5->setText(QString());
        label_6->setText(QApplication::translate("MainWin", "\350\277\236\346\216\245\347\212\266\346\200\201", nullptr));
        label_7->setText(QApplication::translate("MainWin", "\350\277\236\346\216\245\347\212\266\346\200\201\357\274\232", nullptr));
        label_8->setText(QApplication::translate("MainWin", "\346\234\252\350\277\236\346\216\245", nullptr));
        label_9->setText(QString());
        label_10->setText(QApplication::translate("MainWin", "\346\240\207\345\256\232\350\277\233\345\272\246", nullptr));
        label_11->setText(QApplication::translate("MainWin", "20m\350\267\235\347\246\273\346\240\207\345\256\232", nullptr));
        m_StartBtn->setText(QApplication::translate("MainWin", "\345\274\200\345\247\213\346\240\207\345\256\232", nullptr));
        m_ChooseFileBtn->setText(QApplication::translate("MainWin", "\351\200\211\346\213\251\346\273\221\350\275\250\346\226\207\344\273\266", nullptr));
        m_SaveObjPCL->setText(QApplication::translate("MainWin", "\344\277\235\345\255\230\347\233\256\346\240\207\347\202\271\344\272\221", nullptr));
        label_12->setText(QString());
        label_13->setText(QApplication::translate("MainWin", "\346\240\207\345\256\232\344\277\241\346\201\257", nullptr));
        label_14->setText(QApplication::translate("MainWin", "\345\257\274\350\210\252\350\267\235\347\246\273\357\274\232", nullptr));
        label_15->setText(QApplication::translate("MainWin", "\345\215\225\347\202\271\346\265\213\351\207\217\350\267\235\347\246\273\357\274\232", nullptr));
        label_16->setText(QApplication::translate("MainWin", "\351\233\267\350\276\276\346\265\213\351\207\217\350\267\235\347\246\273\357\274\232", nullptr));
        label_17->setText(QApplication::translate("MainWin", "\344\272\221\345\217\260\346\260\264\345\271\263\350\247\222\345\272\246\357\274\232", nullptr));
        label_18->setText(QApplication::translate("MainWin", "\344\272\221\345\217\260\345\236\202\347\233\264\350\247\222\345\272\246\357\274\232", nullptr));
        label_19->setText(QApplication::translate("MainWin", "0mm", nullptr));
        label_20->setText(QApplication::translate("MainWin", "0mm", nullptr));
        label_21->setText(QApplication::translate("MainWin", "0mm", nullptr));
        label_22->setText(QApplication::translate("MainWin", "0\345\272\246", nullptr));
        label_23->setText(QApplication::translate("MainWin", "0\345\272\246", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWin: public Ui_MainWin {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWIN_H
