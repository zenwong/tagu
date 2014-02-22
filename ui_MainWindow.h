/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.2.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionActress;
    QAction *actionCompact;
    QAction *actionCover;
    QAction *actionThumbnail;
    QAction *actionScreenshot;
    QAction *actionVideos;
    QAction *actionOptions;
    QAction *actionReset_Database;
    QAction *actionSync_Database;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_2;
    QComboBox *comboAct;
    QListView *listActs;
    QLineEdit *editSearch;
    QRadioButton *radioAll;
    QRadioButton *radioAny;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout;
    QComboBox *comboTag;
    QListView *listTags;
    QListView *listView;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QMenuBar *menuBar;
    QMenu *menuView;
    QMenu *menuImport;
    QMenu *menuGeneral;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1280, 720);
        QFont font;
        font.setPointSize(10);
        font.setStyleStrategy(QFont::PreferAntialias);
        MainWindow->setFont(font);
        MainWindow->setStyleSheet(QLatin1String("QListView::item\n"
"{\n"
"    font-size: 16pt;\n"
"    font-weight: bold;\n"
"    color: black;\n"
"}"));
        actionActress = new QAction(MainWindow);
        actionActress->setObjectName(QStringLiteral("actionActress"));
        actionCompact = new QAction(MainWindow);
        actionCompact->setObjectName(QStringLiteral("actionCompact"));
        actionCover = new QAction(MainWindow);
        actionCover->setObjectName(QStringLiteral("actionCover"));
        actionThumbnail = new QAction(MainWindow);
        actionThumbnail->setObjectName(QStringLiteral("actionThumbnail"));
        actionScreenshot = new QAction(MainWindow);
        actionScreenshot->setObjectName(QStringLiteral("actionScreenshot"));
        actionVideos = new QAction(MainWindow);
        actionVideos->setObjectName(QStringLiteral("actionVideos"));
        actionOptions = new QAction(MainWindow);
        actionOptions->setObjectName(QStringLiteral("actionOptions"));
        actionReset_Database = new QAction(MainWindow);
        actionReset_Database->setObjectName(QStringLiteral("actionReset_Database"));
        actionSync_Database = new QAction(MainWindow);
        actionSync_Database->setObjectName(QStringLiteral("actionSync_Database"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        groupBox_2 = new QGroupBox(centralWidget);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(1);
        sizePolicy.setHeightForWidth(groupBox_2->sizePolicy().hasHeightForWidth());
        groupBox_2->setSizePolicy(sizePolicy);
        verticalLayout_2 = new QVBoxLayout(groupBox_2);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        comboAct = new QComboBox(groupBox_2);
        comboAct->setObjectName(QStringLiteral("comboAct"));
        QFont font1;
        font1.setFamily(QStringLiteral("Sans Serif"));
        font1.setPointSize(12);
        comboAct->setFont(font1);
        comboAct->setEditable(true);

        verticalLayout_2->addWidget(comboAct);

        listActs = new QListView(groupBox_2);
        listActs->setObjectName(QStringLiteral("listActs"));

        verticalLayout_2->addWidget(listActs);


        gridLayout->addWidget(groupBox_2, 2, 0, 1, 1);

        editSearch = new QLineEdit(centralWidget);
        editSearch->setObjectName(QStringLiteral("editSearch"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(5);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(editSearch->sizePolicy().hasHeightForWidth());
        editSearch->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(editSearch, 0, 1, 1, 1);

        radioAll = new QRadioButton(centralWidget);
        radioAll->setObjectName(QStringLiteral("radioAll"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(radioAll->sizePolicy().hasHeightForWidth());
        radioAll->setSizePolicy(sizePolicy2);

        gridLayout->addWidget(radioAll, 0, 2, 1, 1);

        radioAny = new QRadioButton(centralWidget);
        radioAny->setObjectName(QStringLiteral("radioAny"));
        sizePolicy2.setHeightForWidth(radioAny->sizePolicy().hasHeightForWidth());
        radioAny->setSizePolicy(sizePolicy2);
        radioAny->setChecked(true);

        gridLayout->addWidget(radioAny, 0, 3, 1, 1);

        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        sizePolicy.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy);
        verticalLayout = new QVBoxLayout(groupBox);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        comboTag = new QComboBox(groupBox);
        comboTag->setObjectName(QStringLiteral("comboTag"));
        comboTag->setEditable(true);

        verticalLayout->addWidget(comboTag);

        listTags = new QListView(groupBox);
        listTags->setObjectName(QStringLiteral("listTags"));

        verticalLayout->addWidget(listTags);


        gridLayout->addWidget(groupBox, 0, 0, 2, 1);

        listView = new QListView(centralWidget);
        listView->setObjectName(QStringLiteral("listView"));
        listView->setSelectionMode(QAbstractItemView::ContiguousSelection);
        listView->setFlow(QListView::TopToBottom);
        listView->setProperty("isWrapping", QVariant(true));
        listView->setResizeMode(QListView::Adjust);

        gridLayout->addWidget(listView, 1, 1, 2, 3);

        MainWindow->setCentralWidget(centralWidget);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1280, 19));
        menuView = new QMenu(menuBar);
        menuView->setObjectName(QStringLiteral("menuView"));
        menuImport = new QMenu(menuBar);
        menuImport->setObjectName(QStringLiteral("menuImport"));
        menuGeneral = new QMenu(menuBar);
        menuGeneral->setObjectName(QStringLiteral("menuGeneral"));
        MainWindow->setMenuBar(menuBar);

        menuBar->addAction(menuView->menuAction());
        menuBar->addAction(menuImport->menuAction());
        menuBar->addAction(menuGeneral->menuAction());
        menuView->addAction(actionActress);
        menuView->addAction(actionCompact);
        menuView->addAction(actionCover);
        menuView->addAction(actionThumbnail);
        menuView->addAction(actionScreenshot);
        menuImport->addAction(actionVideos);
        menuImport->addAction(actionSync_Database);
        menuImport->addAction(actionReset_Database);
        menuGeneral->addAction(actionOptions);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Tagu", 0));
        actionActress->setText(QApplication::translate("MainWindow", "Actress", 0));
        actionCompact->setText(QApplication::translate("MainWindow", "Compact", 0));
        actionCover->setText(QApplication::translate("MainWindow", "Cover", 0));
        actionThumbnail->setText(QApplication::translate("MainWindow", "Thumbnail", 0));
        actionScreenshot->setText(QApplication::translate("MainWindow", "Screenshot", 0));
        actionVideos->setText(QApplication::translate("MainWindow", "Import Videos", 0));
        actionOptions->setText(QApplication::translate("MainWindow", "Options", 0));
        actionReset_Database->setText(QApplication::translate("MainWindow", "Reset Database", 0));
        actionSync_Database->setText(QApplication::translate("MainWindow", "Sync Database", 0));
        groupBox_2->setTitle(QApplication::translate("MainWindow", "Actress", 0));
#ifndef QT_NO_TOOLTIP
        comboAct->setToolTip(QApplication::translate("MainWindow", "add a actress to video", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        listActs->setToolTip(QApplication::translate("MainWindow", "Double Click to Remove", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        editSearch->setToolTip(QApplication::translate("MainWindow", "search for tags or actress or title", 0));
#endif // QT_NO_TOOLTIP
        editSearch->setPlaceholderText(QApplication::translate("MainWindow", "Search", 0));
        radioAll->setText(QApplication::translate("MainWindow", "All", 0));
        radioAny->setText(QApplication::translate("MainWindow", "Any", 0));
        groupBox->setTitle(QApplication::translate("MainWindow", "Tags", 0));
#ifndef QT_NO_TOOLTIP
        comboTag->setToolTip(QApplication::translate("MainWindow", "Add a tag to a video", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        listTags->setToolTip(QApplication::translate("MainWindow", "Double Click to Remove", 0));
#endif // QT_NO_TOOLTIP
        menuView->setTitle(QApplication::translate("MainWindow", "View", 0));
        menuImport->setTitle(QApplication::translate("MainWindow", "Database", 0));
        menuGeneral->setTitle(QApplication::translate("MainWindow", "General", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
