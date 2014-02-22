/********************************************************************************
** Form generated from reading UI file 'SettingsDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.2.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETTINGSDIALOG_H
#define UI_SETTINGSDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_SettingsDialog
{
public:
    QGridLayout *gridLayout;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout;
    QListView *listJavImport;
    QPushButton *btnJavAdd;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_2;
    QListView *listPornImport;
    QPushButton *btnPornAdd;
    QGroupBox *groupBox_4;
    QVBoxLayout *verticalLayout_4;
    QListView *listHentaiImport;
    QPushButton *btnHentaiAdd;
    QDialogButtonBox *buttonBox;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout_2;
    QToolButton *btnImageSave;
    QLineEdit *imageDir;
    QLineEdit *thumbWidth;
    QLineEdit *thumbPercentage;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;

    void setupUi(QDialog *SettingsDialog)
    {
        if (SettingsDialog->objectName().isEmpty())
            SettingsDialog->setObjectName(QStringLiteral("SettingsDialog"));
        SettingsDialog->resize(976, 500);
        gridLayout = new QGridLayout(SettingsDialog);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        groupBox = new QGroupBox(SettingsDialog);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        verticalLayout = new QVBoxLayout(groupBox);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        listJavImport = new QListView(groupBox);
        listJavImport->setObjectName(QStringLiteral("listJavImport"));

        verticalLayout->addWidget(listJavImport);

        btnJavAdd = new QPushButton(groupBox);
        btnJavAdd->setObjectName(QStringLiteral("btnJavAdd"));

        verticalLayout->addWidget(btnJavAdd);


        gridLayout->addWidget(groupBox, 0, 0, 1, 1);

        groupBox_2 = new QGroupBox(SettingsDialog);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        verticalLayout_2 = new QVBoxLayout(groupBox_2);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        listPornImport = new QListView(groupBox_2);
        listPornImport->setObjectName(QStringLiteral("listPornImport"));

        verticalLayout_2->addWidget(listPornImport);

        btnPornAdd = new QPushButton(groupBox_2);
        btnPornAdd->setObjectName(QStringLiteral("btnPornAdd"));

        verticalLayout_2->addWidget(btnPornAdd);


        gridLayout->addWidget(groupBox_2, 0, 1, 1, 1);

        groupBox_4 = new QGroupBox(SettingsDialog);
        groupBox_4->setObjectName(QStringLiteral("groupBox_4"));
        verticalLayout_4 = new QVBoxLayout(groupBox_4);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        listHentaiImport = new QListView(groupBox_4);
        listHentaiImport->setObjectName(QStringLiteral("listHentaiImport"));

        verticalLayout_4->addWidget(listHentaiImport);

        btnHentaiAdd = new QPushButton(groupBox_4);
        btnHentaiAdd->setObjectName(QStringLiteral("btnHentaiAdd"));

        verticalLayout_4->addWidget(btnHentaiAdd);


        gridLayout->addWidget(groupBox_4, 0, 2, 1, 1);

        buttonBox = new QDialogButtonBox(SettingsDialog);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        gridLayout->addWidget(buttonBox, 2, 2, 1, 1);

        groupBox_3 = new QGroupBox(SettingsDialog);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        gridLayout_2 = new QGridLayout(groupBox_3);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        btnImageSave = new QToolButton(groupBox_3);
        btnImageSave->setObjectName(QStringLiteral("btnImageSave"));

        gridLayout_2->addWidget(btnImageSave, 0, 2, 1, 1);

        imageDir = new QLineEdit(groupBox_3);
        imageDir->setObjectName(QStringLiteral("imageDir"));

        gridLayout_2->addWidget(imageDir, 0, 1, 1, 1);

        thumbWidth = new QLineEdit(groupBox_3);
        thumbWidth->setObjectName(QStringLiteral("thumbWidth"));

        gridLayout_2->addWidget(thumbWidth, 2, 1, 1, 1);

        thumbPercentage = new QLineEdit(groupBox_3);
        thumbPercentage->setObjectName(QStringLiteral("thumbPercentage"));

        gridLayout_2->addWidget(thumbPercentage, 3, 1, 1, 1);

        label = new QLabel(groupBox_3);
        label->setObjectName(QStringLiteral("label"));

        gridLayout_2->addWidget(label, 0, 0, 1, 1);

        label_2 = new QLabel(groupBox_3);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout_2->addWidget(label_2, 2, 0, 1, 1);

        label_3 = new QLabel(groupBox_3);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout_2->addWidget(label_3, 3, 0, 1, 1);


        gridLayout->addWidget(groupBox_3, 1, 0, 1, 2);

        buttonBox->raise();
        groupBox->raise();
        groupBox_2->raise();
        groupBox_3->raise();
        groupBox_4->raise();

        retranslateUi(SettingsDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), SettingsDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), SettingsDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(SettingsDialog);
    } // setupUi

    void retranslateUi(QDialog *SettingsDialog)
    {
        SettingsDialog->setWindowTitle(QApplication::translate("SettingsDialog", "Dialog", 0));
        groupBox->setTitle(QApplication::translate("SettingsDialog", "Jav Import Directrories", 0));
#ifndef QT_NO_TOOLTIP
        listJavImport->setToolTip(QApplication::translate("SettingsDialog", "Double Click to Remove", 0));
#endif // QT_NO_TOOLTIP
        btnJavAdd->setText(QApplication::translate("SettingsDialog", "Add", 0));
        groupBox_2->setTitle(QApplication::translate("SettingsDialog", "Porn Import Directories", 0));
#ifndef QT_NO_TOOLTIP
        listPornImport->setToolTip(QApplication::translate("SettingsDialog", "Double Click to Remove", 0));
#endif // QT_NO_TOOLTIP
        btnPornAdd->setText(QApplication::translate("SettingsDialog", "Add", 0));
        groupBox_4->setTitle(QApplication::translate("SettingsDialog", "Hentai Import Directories", 0));
#ifndef QT_NO_TOOLTIP
        listHentaiImport->setToolTip(QApplication::translate("SettingsDialog", "Double Click to Remove", 0));
#endif // QT_NO_TOOLTIP
        btnHentaiAdd->setText(QApplication::translate("SettingsDialog", "Add", 0));
        groupBox_3->setTitle(QApplication::translate("SettingsDialog", "Images", 0));
        btnImageSave->setText(QApplication::translate("SettingsDialog", "...", 0));
        imageDir->setPlaceholderText(QApplication::translate("SettingsDialog", "Images Save Directory", 0));
        thumbWidth->setPlaceholderText(QApplication::translate("SettingsDialog", "Thumbnail Width", 0));
#ifndef QT_NO_TOOLTIP
        thumbPercentage->setToolTip(QApplication::translate("SettingsDialog", "Thumbnail will be generated at this video duration percentage", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        thumbPercentage->setWhatsThis(QString());
#endif // QT_NO_WHATSTHIS
        thumbPercentage->setPlaceholderText(QApplication::translate("SettingsDialog", "Thumbnail Percentage", 0));
        label->setText(QApplication::translate("SettingsDialog", "Images Folder", 0));
        label_2->setText(QApplication::translate("SettingsDialog", "Thumbnail Width", 0));
        label_3->setText(QApplication::translate("SettingsDialog", "Thumbnail Percent", 0));
    } // retranslateUi

};

namespace Ui {
    class SettingsDialog: public Ui_SettingsDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTINGSDIALOG_H
