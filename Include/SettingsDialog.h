#pragma once

#include "SettingsManager.h"

#include <QDialog>
#include <QSystemTrayIcon>

namespace hydra {

	class SettingsDialog : public QDialog {

	Q_OBJECT

	public:

		SettingsDialog(QSystemTrayIcon* icon, SettingsManager* settings);

	private:

		void configureDialog(QSystemTrayIcon* icon, SettingsManager* settings);

	};

}

