/********************************************************************************
** Form generated from reading UI file 'test.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TEST_H
#define UI_TEST_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_test
{
public:
    QHBoxLayout *horizontalLayout;
    QTabWidget *tabWidget;
    QWidget *tab;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QLineEdit *lineEdit;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QPushButton *m_runBtn;
    QLabel *label;
    QLabel *label_3;
    QTableView *tableView;
    QWidget *tab_2;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_id;
    QHBoxLayout *horizontalLayout_15;
    QLabel *label_12;
    QLabel *m_lID018;
    QHBoxLayout *horizontalLayout_18;
    QLabel *label_14;
    QLabel *m_lID110;
    QHBoxLayout *horizontalLayout_16;
    QLabel *label_13;
    QLabel *m_lID111;
    QLabel *m_lCom;
    QPushButton *m_getDistanceBtn;
    QWidget *tab_3;
    QVBoxLayout *verticalLayout_4;
    QFrame *frame;
    QPushButton *btnRight;
    QPushButton *btnLeft;
    QPushButton *btnDown;
    QPushButton *btnReset;
    QPushButton *btnUp;
    QPushButton *btnPTZSwitch;
    QLabel *label_2;
    QFrame *frame_2;
    QLineEdit *leVecStep;
    QLabel *lbCurHorAngle;
    QLabel *lbVecMoveAngle;
    QLabel *lbHorMoveAngle;
    QLabel *label_34;
    QLineEdit *leHorStep;
    QLabel *label_32;
    QLabel *label_33;
    QLabel *label_29;
    QLabel *lbCurVecAngle;
    QPushButton *btn_surfaceCollection;
    QPushButton *btn_surfaceCorrect;
    QPushButton *btn_clearSurface;
    QLabel *label_surfaceCollectStastus;
    QCheckBox *checkBox_showCube;
    QPushButton *btn_LoadLocalData;
    QPushButton *btn_updataFitCoeff;
    QLabel *label_15;
    QLineEdit *lineEdit_bordCut;
    QWidget *horizontalLayoutWidget_2;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_16;
    QLineEdit *lineEdit_scaleX;
    QWidget *horizontalLayoutWidget_3;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_17;
    QLineEdit *lineEdit_scaleY;
    QPushButton *btn_updataXY;
    QWidget *tab_4;
    QFrame *frame_4;
    QLineEdit *lePower;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_7;
    QLineEdit *leIp;
    QPushButton *pbConnLidar;
    QComboBox *cbLidarRate;
    QComboBox *cbLidarFps;
    QPushButton *pbOriginCorrection;
    QPushButton *pbDepthCorrection;
    QPushButton *pbClearDepthData;
    QPushButton *pbDepthCollection;
    QLineEdit *lineEditLaserDepth;
    QLabel *label_8;
    QLineEdit *lineEditPtzYaw;
    QPushButton *pbLidarYaw;
    QLabel *label_9;
    QLabel *label_10;
    QPushButton *pbLidarPitch;
    QLineEdit *lineEditPtzPitch;
    QLineEdit *lineEdit_originCol;
    QLineEdit *lineEdit_originRow;
    QPushButton *btnUpdataColRow;
    QPushButton *btn_closeCor;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout_31;
    QLabel *label_11;
    QLineEdit *lineEdit_realDis;
    QPushButton *btn_DataCollect;
    QWidget *layoutWidget_2;
    QHBoxLayout *horizontalLayout_32;
    QLabel *label_24;
    QLineEdit *lineEditStartRow;
    QLabel *label_25;
    QLineEdit *lineEditEndRow;
    QWidget *layoutWidget_3;
    QHBoxLayout *horizontalLayout_33;
    QLabel *label_26;
    QLineEdit *lineEditStartCol;
    QLabel *label_27;
    QLineEdit *lineEditEndCol;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *btn_selectPixel;
    QPushButton *btn_showAllPixel;
    QLineEdit *lineEditLid;
    QLabel *label_28;
    QLineEdit *lePower_2;
    QFrame *frame_3;
    QVBoxLayout *vLayout_VTK;

    void setupUi(QWidget *test)
    {
        if (test->objectName().isEmpty())
            test->setObjectName(QString::fromUtf8("test"));
        test->resize(1269, 641);
        horizontalLayout = new QHBoxLayout(test);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        tabWidget = new QTabWidget(test);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        verticalLayout = new QVBoxLayout(tab);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        lineEdit = new QLineEdit(tab);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        horizontalLayout_2->addWidget(lineEdit);

        pushButton = new QPushButton(tab);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        horizontalLayout_2->addWidget(pushButton);

        pushButton_2 = new QPushButton(tab);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));

        horizontalLayout_2->addWidget(pushButton_2);


        verticalLayout->addLayout(horizontalLayout_2);

        m_runBtn = new QPushButton(tab);
        m_runBtn->setObjectName(QString::fromUtf8("m_runBtn"));

        verticalLayout->addWidget(m_runBtn);

        label = new QLabel(tab);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout->addWidget(label);

        label_3 = new QLabel(tab);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        verticalLayout->addWidget(label_3);

        tableView = new QTableView(tab);
        tableView->setObjectName(QString::fromUtf8("tableView"));

        verticalLayout->addWidget(tableView);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        verticalLayout_2 = new QVBoxLayout(tab_2);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        label_id = new QLabel(tab_2);
        label_id->setObjectName(QString::fromUtf8("label_id"));

        verticalLayout_2->addWidget(label_id);

        horizontalLayout_15 = new QHBoxLayout();
        horizontalLayout_15->setObjectName(QString::fromUtf8("horizontalLayout_15"));
        label_12 = new QLabel(tab_2);
        label_12->setObjectName(QString::fromUtf8("label_12"));

        horizontalLayout_15->addWidget(label_12);

        m_lID018 = new QLabel(tab_2);
        m_lID018->setObjectName(QString::fromUtf8("m_lID018"));

        horizontalLayout_15->addWidget(m_lID018);

        horizontalLayout_15->setStretch(0, 1);
        horizontalLayout_15->setStretch(1, 2);

        verticalLayout_2->addLayout(horizontalLayout_15);

        horizontalLayout_18 = new QHBoxLayout();
        horizontalLayout_18->setObjectName(QString::fromUtf8("horizontalLayout_18"));
        label_14 = new QLabel(tab_2);
        label_14->setObjectName(QString::fromUtf8("label_14"));

        horizontalLayout_18->addWidget(label_14);

        m_lID110 = new QLabel(tab_2);
        m_lID110->setObjectName(QString::fromUtf8("m_lID110"));
        m_lID110->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        horizontalLayout_18->addWidget(m_lID110);

        horizontalLayout_18->setStretch(0, 1);
        horizontalLayout_18->setStretch(1, 2);

        verticalLayout_2->addLayout(horizontalLayout_18);

        horizontalLayout_16 = new QHBoxLayout();
        horizontalLayout_16->setObjectName(QString::fromUtf8("horizontalLayout_16"));
        label_13 = new QLabel(tab_2);
        label_13->setObjectName(QString::fromUtf8("label_13"));

        horizontalLayout_16->addWidget(label_13);

        m_lID111 = new QLabel(tab_2);
        m_lID111->setObjectName(QString::fromUtf8("m_lID111"));
        m_lID111->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        horizontalLayout_16->addWidget(m_lID111);

        horizontalLayout_16->setStretch(0, 1);
        horizontalLayout_16->setStretch(1, 2);

        verticalLayout_2->addLayout(horizontalLayout_16);

        m_lCom = new QLabel(tab_2);
        m_lCom->setObjectName(QString::fromUtf8("m_lCom"));

        verticalLayout_2->addWidget(m_lCom);

        m_getDistanceBtn = new QPushButton(tab_2);
        m_getDistanceBtn->setObjectName(QString::fromUtf8("m_getDistanceBtn"));

        verticalLayout_2->addWidget(m_getDistanceBtn);

        tabWidget->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        verticalLayout_4 = new QVBoxLayout(tab_3);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        frame = new QFrame(tab_3);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        btnRight = new QPushButton(frame);
        btnRight->setObjectName(QString::fromUtf8("btnRight"));
        btnRight->setGeometry(QRect(310, 100, 100, 50));
        btnLeft = new QPushButton(frame);
        btnLeft->setObjectName(QString::fromUtf8("btnLeft"));
        btnLeft->setGeometry(QRect(90, 100, 100, 50));
        btnDown = new QPushButton(frame);
        btnDown->setObjectName(QString::fromUtf8("btnDown"));
        btnDown->setGeometry(QRect(200, 140, 100, 50));
        btnReset = new QPushButton(frame);
        btnReset->setObjectName(QString::fromUtf8("btnReset"));
        btnReset->setGeometry(QRect(310, 10, 100, 50));
        btnUp = new QPushButton(frame);
        btnUp->setObjectName(QString::fromUtf8("btnUp"));
        btnUp->setGeometry(QRect(200, 60, 100, 50));
        btnPTZSwitch = new QPushButton(frame);
        btnPTZSwitch->setObjectName(QString::fromUtf8("btnPTZSwitch"));
        btnPTZSwitch->setGeometry(QRect(90, 10, 100, 50));
        label_2 = new QLabel(frame);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(20, 150, 221, 51));
        frame_2 = new QFrame(frame);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setGeometry(QRect(20, 200, 581, 321));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        leVecStep = new QLineEdit(frame_2);
        leVecStep->setObjectName(QString::fromUtf8("leVecStep"));
        leVecStep->setGeometry(QRect(10, 150, 231, 21));
        lbCurHorAngle = new QLabel(frame_2);
        lbCurHorAngle->setObjectName(QString::fromUtf8("lbCurHorAngle"));
        lbCurHorAngle->setGeometry(QRect(140, 50, 101, 21));
        lbCurHorAngle->setMinimumSize(QSize(91, 21));
        lbCurHorAngle->setMaximumSize(QSize(16777215, 21));
        lbVecMoveAngle = new QLabel(frame_2);
        lbVecMoveAngle->setObjectName(QString::fromUtf8("lbVecMoveAngle"));
        lbVecMoveAngle->setGeometry(QRect(220, 60, 0, 0));
        lbVecMoveAngle->setMinimumSize(QSize(0, 0));
        lbVecMoveAngle->setMaximumSize(QSize(0, 16777215));
        lbHorMoveAngle = new QLabel(frame_2);
        lbHorMoveAngle->setObjectName(QString::fromUtf8("lbHorMoveAngle"));
        lbHorMoveAngle->setGeometry(QRect(210, 30, 0, 0));
        lbHorMoveAngle->setMaximumSize(QSize(0, 16777215));
        label_34 = new QLabel(frame_2);
        label_34->setObjectName(QString::fromUtf8("label_34"));
        label_34->setGeometry(QRect(200, 10, 21, 20));
        leHorStep = new QLineEdit(frame_2);
        leHorStep->setObjectName(QString::fromUtf8("leHorStep"));
        leHorStep->setGeometry(QRect(10, 110, 231, 21));
        label_32 = new QLabel(frame_2);
        label_32->setObjectName(QString::fromUtf8("label_32"));
        label_32->setGeometry(QRect(30, 50, 81, 21));
        label_33 = new QLabel(frame_2);
        label_33->setObjectName(QString::fromUtf8("label_33"));
        label_33->setGeometry(QRect(30, 80, 71, 21));
        label_29 = new QLabel(frame_2);
        label_29->setObjectName(QString::fromUtf8("label_29"));
        label_29->setGeometry(QRect(90, 9, 131, 31));
        lbCurVecAngle = new QLabel(frame_2);
        lbCurVecAngle->setObjectName(QString::fromUtf8("lbCurVecAngle"));
        lbCurVecAngle->setGeometry(QRect(140, 80, 101, 21));
        lbCurVecAngle->setMinimumSize(QSize(91, 21));
        lbCurVecAngle->setMaximumSize(QSize(16777215, 21));
        btn_surfaceCollection = new QPushButton(frame_2);
        btn_surfaceCollection->setObjectName(QString::fromUtf8("btn_surfaceCollection"));
        btn_surfaceCollection->setGeometry(QRect(440, 160, 81, 31));
        btn_surfaceCorrect = new QPushButton(frame_2);
        btn_surfaceCorrect->setObjectName(QString::fromUtf8("btn_surfaceCorrect"));
        btn_surfaceCorrect->setGeometry(QRect(350, 110, 81, 31));
        btn_clearSurface = new QPushButton(frame_2);
        btn_clearSurface->setObjectName(QString::fromUtf8("btn_clearSurface"));
        btn_clearSurface->setGeometry(QRect(440, 110, 81, 31));
        label_surfaceCollectStastus = new QLabel(frame_2);
        label_surfaceCollectStastus->setObjectName(QString::fromUtf8("label_surfaceCollectStastus"));
        label_surfaceCollectStastus->setGeometry(QRect(10, 180, 231, 31));
        checkBox_showCube = new QCheckBox(frame_2);
        checkBox_showCube->setObjectName(QString::fromUtf8("checkBox_showCube"));
        checkBox_showCube->setGeometry(QRect(310, 30, 131, 21));
        btn_LoadLocalData = new QPushButton(frame_2);
        btn_LoadLocalData->setObjectName(QString::fromUtf8("btn_LoadLocalData"));
        btn_LoadLocalData->setGeometry(QRect(260, 110, 81, 31));
        btn_updataFitCoeff = new QPushButton(frame_2);
        btn_updataFitCoeff->setObjectName(QString::fromUtf8("btn_updataFitCoeff"));
        btn_updataFitCoeff->setGeometry(QRect(260, 150, 81, 31));
        label_15 = new QLabel(frame_2);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setGeometry(QRect(270, 70, 61, 21));
        lineEdit_bordCut = new QLineEdit(frame_2);
        lineEdit_bordCut->setObjectName(QString::fromUtf8("lineEdit_bordCut"));
        lineEdit_bordCut->setGeometry(QRect(340, 70, 113, 21));
        horizontalLayoutWidget_2 = new QWidget(frame_2);
        horizontalLayoutWidget_2->setObjectName(QString::fromUtf8("horizontalLayoutWidget_2"));
        horizontalLayoutWidget_2->setGeometry(QRect(260, 200, 171, 31));
        horizontalLayout_4 = new QHBoxLayout(horizontalLayoutWidget_2);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(0, 0, 0, 0);
        label_16 = new QLabel(horizontalLayoutWidget_2);
        label_16->setObjectName(QString::fromUtf8("label_16"));

        horizontalLayout_4->addWidget(label_16);

        lineEdit_scaleX = new QLineEdit(horizontalLayoutWidget_2);
        lineEdit_scaleX->setObjectName(QString::fromUtf8("lineEdit_scaleX"));

        horizontalLayout_4->addWidget(lineEdit_scaleX);

        horizontalLayout_4->setStretch(0, 1);
        horizontalLayout_4->setStretch(1, 3);
        horizontalLayoutWidget_3 = new QWidget(frame_2);
        horizontalLayoutWidget_3->setObjectName(QString::fromUtf8("horizontalLayoutWidget_3"));
        horizontalLayoutWidget_3->setGeometry(QRect(260, 230, 171, 31));
        horizontalLayout_5 = new QHBoxLayout(horizontalLayoutWidget_3);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        horizontalLayout_5->setContentsMargins(0, 0, 0, 0);
        label_17 = new QLabel(horizontalLayoutWidget_3);
        label_17->setObjectName(QString::fromUtf8("label_17"));

        horizontalLayout_5->addWidget(label_17);

        lineEdit_scaleY = new QLineEdit(horizontalLayoutWidget_3);
        lineEdit_scaleY->setObjectName(QString::fromUtf8("lineEdit_scaleY"));

        horizontalLayout_5->addWidget(lineEdit_scaleY);

        horizontalLayout_5->setStretch(0, 1);
        horizontalLayout_5->setStretch(1, 3);
        btn_updataXY = new QPushButton(frame_2);
        btn_updataXY->setObjectName(QString::fromUtf8("btn_updataXY"));
        btn_updataXY->setGeometry(QRect(440, 230, 75, 31));

        verticalLayout_4->addWidget(frame);

        tabWidget->addTab(tab_3, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName(QString::fromUtf8("tab_4"));
        frame_4 = new QFrame(tab_4);
        frame_4->setObjectName(QString::fromUtf8("frame_4"));
        frame_4->setGeometry(QRect(20, 10, 531, 571));
        frame_4->setFrameShape(QFrame::StyledPanel);
        frame_4->setFrameShadow(QFrame::Raised);
        lePower = new QLineEdit(frame_4);
        lePower->setObjectName(QString::fromUtf8("lePower"));
        lePower->setGeometry(QRect(110, 60, 113, 20));
        label_4 = new QLabel(frame_4);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(20, 60, 54, 12));
        label_5 = new QLabel(frame_4);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(20, 90, 54, 12));
        label_6 = new QLabel(frame_4);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(20, 120, 54, 12));
        label_7 = new QLabel(frame_4);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(20, 20, 54, 12));
        leIp = new QLineEdit(frame_4);
        leIp->setObjectName(QString::fromUtf8("leIp"));
        leIp->setGeometry(QRect(110, 20, 113, 20));
        pbConnLidar = new QPushButton(frame_4);
        pbConnLidar->setObjectName(QString::fromUtf8("pbConnLidar"));
        pbConnLidar->setGeometry(QRect(250, 20, 141, 21));
        cbLidarRate = new QComboBox(frame_4);
        cbLidarRate->setObjectName(QString::fromUtf8("cbLidarRate"));
        cbLidarRate->setGeometry(QRect(110, 90, 111, 21));
        cbLidarFps = new QComboBox(frame_4);
        cbLidarFps->setObjectName(QString::fromUtf8("cbLidarFps"));
        cbLidarFps->setGeometry(QRect(110, 120, 111, 21));
        pbOriginCorrection = new QPushButton(frame_4);
        pbOriginCorrection->setObjectName(QString::fromUtf8("pbOriginCorrection"));
        pbOriginCorrection->setGeometry(QRect(90, 170, 75, 23));
        pbDepthCorrection = new QPushButton(frame_4);
        pbDepthCorrection->setObjectName(QString::fromUtf8("pbDepthCorrection"));
        pbDepthCorrection->setGeometry(QRect(270, 170, 75, 23));
        pbClearDepthData = new QPushButton(frame_4);
        pbClearDepthData->setObjectName(QString::fromUtf8("pbClearDepthData"));
        pbClearDepthData->setGeometry(QRect(360, 170, 75, 23));
        pbDepthCollection = new QPushButton(frame_4);
        pbDepthCollection->setObjectName(QString::fromUtf8("pbDepthCollection"));
        pbDepthCollection->setGeometry(QRect(180, 170, 75, 23));
        lineEditLaserDepth = new QLineEdit(frame_4);
        lineEditLaserDepth->setObjectName(QString::fromUtf8("lineEditLaserDepth"));
        lineEditLaserDepth->setGeometry(QRect(180, 200, 61, 21));
        label_8 = new QLabel(frame_4);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(110, 200, 71, 20));
        lineEditPtzYaw = new QLineEdit(frame_4);
        lineEditPtzYaw->setObjectName(QString::fromUtf8("lineEditPtzYaw"));
        lineEditPtzYaw->setGeometry(QRect(70, 280, 61, 21));
        pbLidarYaw = new QPushButton(frame_4);
        pbLidarYaw->setObjectName(QString::fromUtf8("pbLidarYaw"));
        pbLidarYaw->setGeometry(QRect(20, 250, 75, 23));
        label_9 = new QLabel(frame_4);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(20, 280, 51, 20));
        label_10 = new QLabel(frame_4);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(160, 280, 51, 20));
        pbLidarPitch = new QPushButton(frame_4);
        pbLidarPitch->setObjectName(QString::fromUtf8("pbLidarPitch"));
        pbLidarPitch->setGeometry(QRect(160, 250, 75, 23));
        lineEditPtzPitch = new QLineEdit(frame_4);
        lineEditPtzPitch->setObjectName(QString::fromUtf8("lineEditPtzPitch"));
        lineEditPtzPitch->setGeometry(QRect(210, 280, 61, 21));
        lineEdit_originCol = new QLineEdit(frame_4);
        lineEdit_originCol->setObjectName(QString::fromUtf8("lineEdit_originCol"));
        lineEdit_originCol->setGeometry(QRect(10, 330, 81, 21));
        lineEdit_originRow = new QLineEdit(frame_4);
        lineEdit_originRow->setObjectName(QString::fromUtf8("lineEdit_originRow"));
        lineEdit_originRow->setGeometry(QRect(110, 330, 71, 21));
        btnUpdataColRow = new QPushButton(frame_4);
        btnUpdataColRow->setObjectName(QString::fromUtf8("btnUpdataColRow"));
        btnUpdataColRow->setGeometry(QRect(190, 330, 111, 21));
        btn_closeCor = new QPushButton(frame_4);
        btn_closeCor->setObjectName(QString::fromUtf8("btn_closeCor"));
        btn_closeCor->setGeometry(QRect(0, 170, 75, 23));
        layoutWidget = new QWidget(frame_4);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 510, 371, 34));
        horizontalLayout_31 = new QHBoxLayout(layoutWidget);
        horizontalLayout_31->setSpacing(10);
        horizontalLayout_31->setObjectName(QString::fromUtf8("horizontalLayout_31"));
        horizontalLayout_31->setContentsMargins(3, 3, 3, 3);
        label_11 = new QLabel(layoutWidget);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        horizontalLayout_31->addWidget(label_11);

        lineEdit_realDis = new QLineEdit(layoutWidget);
        lineEdit_realDis->setObjectName(QString::fromUtf8("lineEdit_realDis"));

        horizontalLayout_31->addWidget(lineEdit_realDis);

        btn_DataCollect = new QPushButton(layoutWidget);
        btn_DataCollect->setObjectName(QString::fromUtf8("btn_DataCollect"));

        horizontalLayout_31->addWidget(btn_DataCollect);

        layoutWidget_2 = new QWidget(frame_4);
        layoutWidget_2->setObjectName(QString::fromUtf8("layoutWidget_2"));
        layoutWidget_2->setGeometry(QRect(10, 400, 371, 30));
        horizontalLayout_32 = new QHBoxLayout(layoutWidget_2);
        horizontalLayout_32->setObjectName(QString::fromUtf8("horizontalLayout_32"));
        horizontalLayout_32->setContentsMargins(10, 3, 10, 3);
        label_24 = new QLabel(layoutWidget_2);
        label_24->setObjectName(QString::fromUtf8("label_24"));
        label_24->setMinimumSize(QSize(30, 0));
        QFont font;
        font.setFamily(QString::fromUtf8("Arial"));
        label_24->setFont(font);

        horizontalLayout_32->addWidget(label_24);

        lineEditStartRow = new QLineEdit(layoutWidget_2);
        lineEditStartRow->setObjectName(QString::fromUtf8("lineEditStartRow"));
        lineEditStartRow->setMinimumSize(QSize(80, 0));
        lineEditStartRow->setFont(font);

        horizontalLayout_32->addWidget(lineEditStartRow);

        label_25 = new QLabel(layoutWidget_2);
        label_25->setObjectName(QString::fromUtf8("label_25"));
        label_25->setFont(font);

        horizontalLayout_32->addWidget(label_25);

        lineEditEndRow = new QLineEdit(layoutWidget_2);
        lineEditEndRow->setObjectName(QString::fromUtf8("lineEditEndRow"));
        lineEditEndRow->setMinimumSize(QSize(80, 0));
        lineEditEndRow->setFont(font);

        horizontalLayout_32->addWidget(lineEditEndRow);

        layoutWidget_3 = new QWidget(frame_4);
        layoutWidget_3->setObjectName(QString::fromUtf8("layoutWidget_3"));
        layoutWidget_3->setGeometry(QRect(10, 430, 371, 31));
        horizontalLayout_33 = new QHBoxLayout(layoutWidget_3);
        horizontalLayout_33->setObjectName(QString::fromUtf8("horizontalLayout_33"));
        horizontalLayout_33->setContentsMargins(10, 3, 10, 3);
        label_26 = new QLabel(layoutWidget_3);
        label_26->setObjectName(QString::fromUtf8("label_26"));
        label_26->setMinimumSize(QSize(30, 0));
        label_26->setFont(font);

        horizontalLayout_33->addWidget(label_26);

        lineEditStartCol = new QLineEdit(layoutWidget_3);
        lineEditStartCol->setObjectName(QString::fromUtf8("lineEditStartCol"));
        lineEditStartCol->setMinimumSize(QSize(80, 0));
        lineEditStartCol->setFont(font);

        horizontalLayout_33->addWidget(lineEditStartCol);

        label_27 = new QLabel(layoutWidget_3);
        label_27->setObjectName(QString::fromUtf8("label_27"));
        label_27->setFont(font);

        horizontalLayout_33->addWidget(label_27);

        lineEditEndCol = new QLineEdit(layoutWidget_3);
        lineEditEndCol->setObjectName(QString::fromUtf8("lineEditEndCol"));
        lineEditEndCol->setMinimumSize(QSize(80, 0));
        lineEditEndCol->setFont(font);

        horizontalLayout_33->addWidget(lineEditEndCol);

        horizontalLayoutWidget = new QWidget(frame_4);
        horizontalLayoutWidget->setObjectName(QString::fromUtf8("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(10, 470, 371, 31));
        horizontalLayout_3 = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        btn_selectPixel = new QPushButton(horizontalLayoutWidget);
        btn_selectPixel->setObjectName(QString::fromUtf8("btn_selectPixel"));
        btn_selectPixel->setFont(font);

        horizontalLayout_3->addWidget(btn_selectPixel);

        btn_showAllPixel = new QPushButton(horizontalLayoutWidget);
        btn_showAllPixel->setObjectName(QString::fromUtf8("btn_showAllPixel"));

        horizontalLayout_3->addWidget(btn_showAllPixel);

        lineEditLid = new QLineEdit(frame_4);
        lineEditLid->setObjectName(QString::fromUtf8("lineEditLid"));
        lineEditLid->setGeometry(QRect(56, 371, 114, 21));
        lineEditLid->setMinimumSize(QSize(80, 0));
        lineEditLid->setFont(font);
        label_28 = new QLabel(frame_4);
        label_28->setObjectName(QString::fromUtf8("label_28"));
        label_28->setGeometry(QRect(20, 370, 30, 24));
        label_28->setMinimumSize(QSize(30, 0));
        label_28->setFont(font);
        lePower_2 = new QLineEdit(frame_4);
        lePower_2->setObjectName(QString::fromUtf8("lePower_2"));
        lePower_2->setGeometry(QRect(240, 60, 113, 20));
        tabWidget->addTab(tab_4, QString());

        horizontalLayout->addWidget(tabWidget);

        frame_3 = new QFrame(test);
        frame_3->setObjectName(QString::fromUtf8("frame_3"));
        frame_3->setMinimumSize(QSize(650, 0));
        frame_3->setFrameShape(QFrame::StyledPanel);
        frame_3->setFrameShadow(QFrame::Raised);
        vLayout_VTK = new QVBoxLayout(frame_3);
        vLayout_VTK->setObjectName(QString::fromUtf8("vLayout_VTK"));

        horizontalLayout->addWidget(frame_3);


        retranslateUi(test);

        tabWidget->setCurrentIndex(3);


        QMetaObject::connectSlotsByName(test);
    } // setupUi

    void retranslateUi(QWidget *test)
    {
        test->setWindowTitle(QApplication::translate("test", "test", nullptr));
        pushButton->setText(QApplication::translate("test", "choose", nullptr));
        pushButton_2->setText(QApplication::translate("test", "determine", nullptr));
        m_runBtn->setText(QApplication::translate("test", "run", nullptr));
        label->setText(QString());
        label_3->setText(QString());
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("test", "\346\273\221\350\275\250\346\265\213\350\257\225", nullptr));
        label_id->setText(QString());
        label_12->setText(QApplication::translate("test", "\346\265\213\350\267\235\344\273\2521(id: 018)\357\274\232", nullptr));
        m_lID018->setText(QString());
        label_14->setText(QApplication::translate("test", "\346\265\213\350\267\235\344\273\2521(id: 110)\357\274\232", nullptr));
        m_lID110->setText(QString());
        label_13->setText(QApplication::translate("test", "\346\265\213\350\267\235\344\273\2521(id: 111)\357\274\232", nullptr));
        m_lID111->setText(QString());
        m_lCom->setText(QString());
        m_getDistanceBtn->setText(QApplication::translate("test", "\350\216\267\345\217\226\350\267\235\347\246\273", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("test", "\346\265\213\350\267\235\344\273\252\346\265\213\350\257\225", nullptr));
        btnRight->setText(QApplication::translate("test", "Right", nullptr));
        btnLeft->setText(QApplication::translate("test", "Left", nullptr));
        btnDown->setText(QApplication::translate("test", "Down", nullptr));
        btnReset->setText(QApplication::translate("test", "Reset", nullptr));
        btnUp->setText(QApplication::translate("test", "Up", nullptr));
        btnPTZSwitch->setText(QApplication::translate("test", "Open", nullptr));
        label_2->setText(QString());
        leVecStep->setPlaceholderText(QApplication::translate("test", "Single Vertical Rotation Angle", nullptr));
        lbCurHorAngle->setText(QApplication::translate("test", "0", nullptr));
        lbVecMoveAngle->setText(QApplication::translate("test", "0", nullptr));
        lbHorMoveAngle->setText(QApplication::translate("test", "0", nullptr));
        label_34->setText(QString());
        leHorStep->setPlaceholderText(QApplication::translate("test", "Single Horizontal Rotation Angle", nullptr));
        label_32->setText(QApplication::translate("test", "Horizontal:", nullptr));
        label_33->setText(QApplication::translate("test", "Vertical:", nullptr));
        label_29->setText(QApplication::translate("test", "Current Angle", nullptr));
        lbCurVecAngle->setText(QApplication::translate("test", "0", nullptr));
        btn_surfaceCollection->setText(QApplication::translate("test", "SurfaceCol", nullptr));
        btn_surfaceCorrect->setText(QApplication::translate("test", "\350\256\241\347\256\227", nullptr));
        btn_clearSurface->setText(QApplication::translate("test", "\346\270\205\351\231\244", nullptr));
        label_surfaceCollectStastus->setText(QString());
        checkBox_showCube->setText(QApplication::translate("test", "ShowValidCube", nullptr));
        btn_LoadLocalData->setText(QApplication::translate("test", "\345\212\240\350\275\275", nullptr));
        btn_updataFitCoeff->setText(QApplication::translate("test", "\344\270\212\344\274\240", nullptr));
        label_15->setText(QApplication::translate("test", "\350\276\271\347\274\230\346\211\243\351\231\244", nullptr));
        lineEdit_bordCut->setText(QApplication::translate("test", "0", nullptr));
        label_16->setText(QApplication::translate("test", "  Scale_X", nullptr));
        lineEdit_scaleX->setText(QApplication::translate("test", "1.0", nullptr));
        label_17->setText(QApplication::translate("test", "  Scale_Y", nullptr));
        lineEdit_scaleY->setText(QApplication::translate("test", "1.0", nullptr));
        btn_updataXY->setText(QApplication::translate("test", "Updata", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("test", "\344\272\221\345\217\260\346\265\213\350\257\225", nullptr));
        label_4->setText(QApplication::translate("test", "\345\212\237\347\216\207\357\274\232", nullptr));
        label_5->setText(QApplication::translate("test", "\351\207\215\345\244\215\347\216\207\357\274\232", nullptr));
        label_6->setText(QApplication::translate("test", "\345\270\247\347\216\207\357\274\232", nullptr));
        label_7->setText(QApplication::translate("test", "ip:", nullptr));
        leIp->setText(QApplication::translate("test", "192.168.1.101", nullptr));
        pbConnLidar->setText(QApplication::translate("test", "\346\234\252\350\277\236\346\216\245", nullptr));
        pbOriginCorrection->setText(QApplication::translate("test", "OriginCor", nullptr));
        pbDepthCorrection->setText(QApplication::translate("test", "DepthCor", nullptr));
        pbClearDepthData->setText(QApplication::translate("test", "ClearDepth", nullptr));
        pbDepthCollection->setText(QApplication::translate("test", "DepthCol", nullptr));
        label_8->setText(QApplication::translate("test", "laserDepth:", nullptr));
        pbLidarYaw->setText(QApplication::translate("test", "LidarYaw", nullptr));
        label_9->setText(QApplication::translate("test", "ptzYaw:", nullptr));
        label_10->setText(QApplication::translate("test", "ptzPitch:", nullptr));
        pbLidarPitch->setText(QApplication::translate("test", "LidarPitch", nullptr));
        btnUpdataColRow->setText(QApplication::translate("test", "UpdataColRow", nullptr));
        btn_closeCor->setText(QApplication::translate("test", "SwitchCor", nullptr));
        label_11->setText(QApplication::translate("test", "real dis", nullptr));
        btn_DataCollect->setText(QApplication::translate("test", "dataCollect", nullptr));
        label_24->setText(QApplication::translate("test", "row", nullptr));
        lineEditStartRow->setText(QApplication::translate("test", "0", nullptr));
        label_25->setText(QApplication::translate("test", "-", nullptr));
        lineEditEndRow->setText(QApplication::translate("test", "719", nullptr));
        label_26->setText(QApplication::translate("test", "col", nullptr));
        lineEditStartCol->setText(QApplication::translate("test", "0", nullptr));
        label_27->setText(QApplication::translate("test", "-", nullptr));
        lineEditEndCol->setText(QApplication::translate("test", "4095", nullptr));
        btn_selectPixel->setText(QApplication::translate("test", "Select pixel", nullptr));
        btn_showAllPixel->setText(QApplication::translate("test", "showAllPixel", nullptr));
        lineEditLid->setText(QApplication::translate("test", "0", nullptr));
        label_28->setText(QApplication::translate("test", "lid", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_4), QApplication::translate("test", "\351\233\267\350\276\276\350\256\276\347\275\256", nullptr));
    } // retranslateUi

};

namespace Ui {
    class test: public Ui_test {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TEST_H
