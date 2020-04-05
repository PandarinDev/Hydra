#pragma once

#include "SettingsManager.h"

#include <QObject>
#include <QString>
#include <QTimer>
#include <QSystemTrayIcon>

#include <cstdint>
#include <vector>

namespace hydra {

	struct NotificationFrequencyEntry {

		std::int64_t seconds;
		QString representation;

		NotificationFrequencyEntry(std::int64_t seconds, const QString& representation);

	};

	class NotificationManager : public QObject {

		Q_OBJECT

	public:

		static const std::vector<NotificationFrequencyEntry>& getNotificationFrequencyEntries();

		NotificationManager(QSystemTrayIcon* trayIcon, const SettingsManager* settings);

	public slots:

		void sendNotification();
		void reloadSettings(const SettingsManager* settings);

	private:

		using Time = std::chrono::time_point<std::chrono::steady_clock>; 
	
		QSystemTrayIcon* trayIcon;
		QTimer* timer;

	};

}

