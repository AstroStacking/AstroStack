#pragma once

#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_Multi2MultiInterface
{
public:
    QVBoxLayout* verticalLayout;

    void setupUi(QGroupBox* Multi, QWidget* child)
    {
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Multi->sizePolicy().hasHeightForWidth());
        Multi->setSizePolicy(sizePolicy);
        Multi->setCheckable(true);
        verticalLayout = new QVBoxLayout(Multi);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->addWidget(child);
        retranslateUi(Multi);

        QMetaObject::connectSlotsByName(Multi);
    } // setupUi

    void retranslateUi(QGroupBox* Multi)
    {
        Multi->setWindowTitle(QCoreApplication::translate("Multi2MultiInterface", "Frame", nullptr));
        Multi->setTitle(QString());
    } // retranslateUi
};

namespace Ui
{
class Multi2MultiInterface : public Ui_Multi2MultiInterface
{
};
} // namespace Ui

QT_END_NAMESPACE
