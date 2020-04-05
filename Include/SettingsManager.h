#pragma once

#include "DbManager.h"

#include <QObject>

#include <memory>

namespace hydra {

	class SettingsManager : public QObject {

		Q_OBJECT

	public:

		SettingsManager(const std::shared_ptr<DbManager>& dbManager);

		float getTargetGoalInLiters() const;
		float getGlassSizeInDeciliters() const;
		int getNotificationFrequencyIndex() const;
		bool shouldShowTrayIcon() const;

	public slots:

		void save() const;
		void setTargetGoalInLiters(float liters);
		void setGlassSizeInDeciliters(float deciliters);
		void setNotificationFrequencyIndex(int index);
		void setShowTrayIcon(bool show);

	signals:

		void saved(const SettingsManager* instance) const;

	private:

		struct Settings {
			float targetGoalLiter;
			float glassSizeDeciliter;
			int notificationFrequencyIndex;
			bool showTrayIcon;
		};

		std::shared_ptr<DbManager> dbManager;
		Settings settings;
		
		void initSettings();

	};

}

