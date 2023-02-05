#pragma once

#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_MonoInterface
{
public:
    QVBoxLayout* verticalLayout;
    QHBoxLayout* horizontalLayout_2;
    QCheckBox* saveOutput;
    QLineEdit* filename;
    QPushButton* filenameOpen;

    void setupUi(QGroupBox* Mono, QWidget* child)
    {
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Mono->sizePolicy().hasHeightForWidth());
        Mono->setSizePolicy(sizePolicy);
        Mono->setCheckable(true);
        verticalLayout = new QVBoxLayout(Mono);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->addWidget(child);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        saveOutput = new QCheckBox(Mono);
        saveOutput->setObjectName(QString::fromUtf8("saveOutput"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(saveOutput->sizePolicy().hasHeightForWidth());
        saveOutput->setSizePolicy(sizePolicy1);

        horizontalLayout_2->addWidget(saveOutput);

        filename = new QLineEdit(Mono);
        filename->setObjectName(QString::fromUtf8("filename"));
        filename->setEnabled(false);

        horizontalLayout_2->addWidget(filename);

        filenameOpen = new QPushButton(Mono);
        filenameOpen->setObjectName(QString::fromUtf8("filenameOpen"));
        filenameOpen->setEnabled(false);
        sizePolicy1.setHeightForWidth(filenameOpen->sizePolicy().hasHeightForWidth());
        filenameOpen->setSizePolicy(sizePolicy1);

        horizontalLayout_2->addWidget(filenameOpen);


        verticalLayout->addLayout(horizontalLayout_2);


        retranslateUi(Mono);

        QMetaObject::connectSlotsByName(Mono);
    } // setupUi

    void retranslateUi(QGroupBox* Mono)
    {
        Mono->setWindowTitle(QCoreApplication::translate("MonoInterface", "Frame", nullptr));
        Mono->setTitle(QString());
        saveOutput->setText(QCoreApplication::translate("MonoInterface", "Save output:", nullptr));
        filenameOpen->setText(QCoreApplication::translate("MonoInterface", "...", nullptr));
    } // retranslateUi
};

namespace Ui
{
class MonoInterface : public Ui_MonoInterface
{
};
} // namespace Ui

QT_END_NAMESPACE
