#pragma once

#include <sqlite3.h>

#include <string>
#include <functional>
#include <optional>

namespace hydra {

	class DbManager {

	public:

		// Using definition for SQLite callback function signature
		using Callback = int(*)(void*,int,char**,char**);

		DbManager(const std::string& filePath);
		~DbManager();

		void execute(const std::string& sql, const std::optional<Callback>& callback = std::nullopt) const;
		void close() const;

	private:

		sqlite3* handle;

		void createTables() const;

	};

}

