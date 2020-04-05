#include "SettingsDialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QPushButton>

namespace hydra {

	SettingsDialog::SettingsDialog(QSystemTrayIcon* icon, SettingsManager* settings) {
		configureDialog(icon, settings);
	}

	void SettingsDialog::configureDialog(QSystemTrayIcon* icon, SettingsManager* settings) {
		QVBoxLayout* layout = new QVBoxLayout();
		setLayout(layout);
		
		QHBoxLayout* goalLayout = new QHBoxLayout();
		QLabel* targetGoalLabel = new QLabel(QObject::tr("Target goal:"));
		QLineEdit* targetGoalLineEdit = new QLineEdit();
		targetGoalLineEdit->setText(QString::number(settings->getTargetGoalInLiters()));
		QObject::connect(targetGoalLineEdit, &QLineEdit::textChanged, [settings](const QString& newText) {
			settings->setTargetGoalInLiters(newText.toFloat());
		});
		QLabel* literPerDayLabel = new QLabel(QObject::tr("L / day"));
		goalLayout->addWidget(targetGoalLabel);
		goalLayout->addWidget(targetGoalLineEdit);
		goalLayout->addWidget(literPerDayLabel);

		QHBoxLayout* glassLayout = new QHBoxLayout();
		QLabel* glassSizeLabel = new QLabel(QObject::tr("Glass size:"));
		QLineEdit* glassSizeLineEdit = new QLineEdit();
		glassSizeLineEdit->setText(QString::number(settings->getGlassSizeInDeciliters()));
		QObject::connect(glassSizeLineEdit, &QLineEdit::textChanged, [settings](const QString& newText) {
			settings->setGlassSizeInDeciliters(newText.toFloat());
		});
		QLabel* decilitersLabel = new QLabel(QObject::tr("DL"));
		glassLayout->addWidget(glassSizeLabel);
		glassLayout->addWidget(glassSizeLineEdit);
		glassLayout->addWidget(decilitersLabel);

		QHBoxLayout* notificationLayout = new QHBoxLayout();
		QLabel* notificationFreqLabel = new QLabel(QObject::tr("Notification frequency:"));
		QComboBox* notificationFreqComboBox = new QComboBox();
		notificationFreqComboBox->addItem(QObject::tr("Off"));
		notificationFreqComboBox->addItem(QObject::tr("30 minutes"));
		notificationFreqComboBox->addItem(QObject::tr("1 hour"));
		notificationFreqComboBox->addItem(QObject::tr("2 hours"));
		notificationFreqComboBox->addItem(QObject::tr("3 hours"));
		notificationFreqComboBox->addItem(QObject::tr("6 hours"));
		notificationFreqComboBox->setCurrentIndex(settings->getNotificationFrequencyIndex());
		QObject::connect(notificationFreqComboBox, QOverload<int>::of(&QComboBox::activated), settings, &SettingsManager::setNotificationFrequencyIndex);
		notificationLayout->addWidget(notificationFreqLabel);
		notificationLayout->addWidget(notificationFreqComboBox);

		QHBoxLayout* trayIconLayout = new QHBoxLayout();
		QLabel* showTrayIconLabel = new QLabel(QObject::tr("Show tray icon:"));
		QCheckBox* showTrayIconCheckBox = new QCheckBox();
		showTrayIconCheckBox->setChecked(settings->shouldShowTrayIcon());
		QObject::connect(showTrayIconCheckBox, &QCheckBox::toggled, settings, &SettingsManager::setShowTrayIcon);
		trayIconLayout->addWidget(showTrayIconLabel);
		trayIconLayout->addWidget(showTrayIconCheckBox);

		QHBoxLayout* buttonsLayout = new QHBoxLayout();
		QPushButton* okButton = new QPushButton(QObject::tr("OK"));
		QObject::connect(okButton, &QPushButton::clicked, this, &QDialog::accept);
		QPushButton* cancelButton = new QPushButton(QObject::tr("Cancel"));
		QObject::connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
		buttonsLayout->addWidget(okButton);
		buttonsLayout->addWidget(cancelButton);
		buttonsLayout->setContentsMargins(0, 10, 0, 0);

		layout->addLayout(goalLayout);
		layout->addLayout(glassLayout);
		layout->addLayout(notificationLayout);
		layout->addLayout(trayIconLayout);
		layout->addLayout(buttonsLayout);
	}

}

