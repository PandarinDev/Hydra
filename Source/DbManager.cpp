#include "DbManager.h"

#include <stdexcept>

namespace hydra {

	DbManager::DbManager(const std::string& filePath) {
		if (sqlite3_open(filePath.c_str(), &handle) != SQLITE_OK) {
			throw std::runtime_error("Failed to open SQLite database.");
		}
		createTables();
	}

	DbManager::~DbManager() {
		close();
	}

	void DbManager::execute(const std::string& sql, const std::optional<DbManager::Callback>& callback) const {
		char* errorMsg = nullptr;
		sqlite3_exec(handle, sql.c_str(), callback.value_or(nullptr), nullptr, &errorMsg);
		if (errorMsg != nullptr) {
			std::runtime_error error("SQLite error: " + std::string(errorMsg));
			sqlite3_free(errorMsg);
			throw error;
		}
	}

	void DbManager::close() const {
		if(sqlite3_close(handle) != SQLITE_OK) {
			throw std::runtime_error("Failed to close SQLite database.");
		}
	}

	void DbManager::createTables() const {
		// Create the "settings" table
		execute(
				"CREATE TABLE IF NOT EXISTS settings("
				"target_goal_liter REAL, "
				"glass_size_deciliter REAL, "
				"notification_freq INTEGER, "
				"show_tray_icon BOOLEAN"
				");");
		// Create the "intake" table
		execute(
				"CREATE TABLE IF NOT EXISTS intake("
				"id INTEGER PRIMARY KEY, "
				"time INTEGER, "
				"glasses REAL"
				");");
	}

}

