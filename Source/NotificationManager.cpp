#include "NotificationManager.h"

#include <chrono>
#include <thread>

namespace hydra {


	NotificationFrequencyEntry::NotificationFrequencyEntry(std::int64_t seconds, const QString& representation) :
		seconds(seconds), representation(representation) {}

	static const std::vector<NotificationFrequencyEntry> FREQUENCY_ENTRIES {
		{ 0, QObject::tr("Off") },
		{ 30 * 60, QObject::tr("30 minutes") },
		{ 60 * 60, QObject::tr("1 hour") },
		{ 2 * 60 * 60, QObject::tr("2 hours") },
		{ 3 * 60 * 60, QObject::tr("3 hours") },
		{ 6 * 60 * 60, QObject::tr("6 hours") }
	};

	const std::vector<NotificationFrequencyEntry>& NotificationManager::getNotificationFrequencyEntries() {
		return FREQUENCY_ENTRIES;
	}

	NotificationManager::NotificationManager(QSystemTrayIcon* trayIcon, const SettingsManager* settings) :
		trayIcon(trayIcon), timer(new QTimer(this)) {
		timer->setSingleShot(false);
		QObject::connect(timer, &QTimer::timeout, this, &NotificationManager::sendNotification);
		reloadSettings(settings);
	}

	void NotificationManager::sendNotification() {
		trayIcon->showMessage(QObject::tr("Hydra"), QObject::tr("Time to drink some water!"));
	}

	void NotificationManager::reloadSettings(const SettingsManager* settings) {
		const auto& frequencyEntry = FREQUENCY_ENTRIES[settings->getNotificationFrequencyIndex()];
		if (frequencyEntry.seconds == 0) {
			timer->stop();
			return;
		}
		timer->setInterval(frequencyEntry.seconds * 1000);
		timer->start();
	}

}

