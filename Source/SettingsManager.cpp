#include "SettingsManager.h"

#include <string>
#include <optional>

namespace hydra {

	SettingsManager::SettingsManager(const std::shared_ptr<DbManager>& dbManager) : dbManager(dbManager) {
		initSettings();
	}

	void SettingsManager::save() const {
		dbManager->execute(
				"INSERT INTO settings ("
				"target_goal_liter, "
				"glass_size_deciliter, "
				"notification_freq, "
				"show_tray_icon) "
				"VALUES (" +
				std::to_string(settings.targetGoalLiter) + ", " +
				std::to_string(settings.glassSizeDeciliter) + ", " +
				std::to_string(settings.notificationFrequencyIndex) + ", " +
				std::to_string(settings.showTrayIcon ? 1 : 0) +
				");");
		emit saved(this);
	}

	float SettingsManager::getTargetGoalInLiters() const {
		return settings.targetGoalLiter;
	}

	void SettingsManager::setTargetGoalInLiters(float liters) {
		settings.targetGoalLiter = liters;
	}

	float SettingsManager::getGlassSizeInDeciliters() const {
		return settings.glassSizeDeciliter;
	}

	void SettingsManager::setGlassSizeInDeciliters(float deciliters) {
		settings.glassSizeDeciliter = deciliters;
	}

	int SettingsManager::getNotificationFrequencyIndex() const {
		return settings.notificationFrequencyIndex;
	}

	void SettingsManager::setNotificationFrequencyIndex(int index) {
		settings.notificationFrequencyIndex = index;
	}

	bool SettingsManager::shouldShowTrayIcon() const {
		return settings.showTrayIcon;
	}

	void SettingsManager::setShowTrayIcon(bool show) {
		settings.showTrayIcon = show;
	}

	void SettingsManager::initSettings() {
		static std::optional<Settings> settingsInDatabase;
		settingsInDatabase = std::nullopt;
		// We can't capture anything in the lambda because we have to match the expected
		// function signature. So the workaround here is to use a static instance, but this
		// is not thread-safe, try not to shoot ourselves in the foot here later.
		DbManager::Callback callback = [](void*, int, char** values, char**) {
			Settings instance;
			instance.targetGoalLiter = std::stof(values[0]);
			instance.glassSizeDeciliter = std::stof(values[1]);
			instance.notificationFrequencyIndex = std::stoi(values[2]);
			instance.showTrayIcon = static_cast<bool>(std::stoi(values[3]));
			settingsInDatabase = instance;
			return 0;
		};
		dbManager->execute("SELECT * FROM settings;", callback);
		if (settingsInDatabase) {
			settings = *settingsInDatabase;
			return;
		}
		// Initialize settings to the default values here
		settings.targetGoalLiter = 2.5f;
		settings.glassSizeDeciliter = 5.0f;
		settings.notificationFrequencyIndex = 3;
		settings.showTrayIcon = true;
		save();
	}

}

