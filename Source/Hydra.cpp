#include "DbManager.h"
#include "SettingsDialog.h"
#include "SettingsManager.h"
#include "NotificationManager.h"

#include <QApplication>
#include <QMenu>
#include <QAction>
#include <QDialog>
#include <QIcon>
#include <QSystemTrayIcon>

#include <sqlite3.h>

#include <cstdlib>
#include <memory>
#include <future>
#include <iostream>

using namespace hydra;

QMenu* createContextMenu(QDialog* settingsDialog) {
	QMenu* contextMenu = new QMenu();
	QMenu* drankMenu = new QMenu(QObject::tr("Just drank some water"));
	QAction* settingsAction = new QAction(QObject::tr("Settings"));
	QObject::connect(settingsAction, &QAction::triggered, settingsDialog, &QDialog::show);
	QAction* quitAction = new QAction(QObject::tr("Quit"));
	QObject::connect(quitAction, &QAction::triggered, &QApplication::quit);
	contextMenu->addMenu(drankMenu);
	contextMenu->addAction(settingsAction);
	contextMenu->addAction(quitAction);

	return contextMenu;
}

int main(int argc, char** argv) {
	// Check if there is an open Xorg display
	if (getenv("DISPLAY") == nullptr) {
		std::cerr << "No graphical environment is available, shutting down." << std::endl;
		return 1;
	}

	std::shared_ptr<DbManager> dbManager;
	try {
		dbManager = std::make_shared<DbManager>(".data");
	} catch (const std::runtime_error& e) {
		std::cerr << "SQL error: " << e.what() << std::endl;
		return 1;
	}

	SettingsManager* settingsManager;
	try {
		settingsManager = new SettingsManager(dbManager);
	} catch (const std::runtime_error& e) {
		std::cerr << "Failed to read settings: " << e.what() << std::endl;
		return 1;
	}

	QApplication application(argc, argv);
	QObject::connect(&application, &QApplication::aboutToQuit, [dbManager]() {
		try {
			dbManager->close();
		} catch (const std::runtime_error& e) {
			std::cerr << "Failed to close database connection." << std::endl;
		}
	});
	application.setQuitOnLastWindowClosed(false);

	QIcon icon("water_bottle.svg");
	QSystemTrayIcon* trayIcon = new QSystemTrayIcon(icon);
	SettingsDialog* settingsDialog = new SettingsDialog(trayIcon, settingsManager);
	QObject::connect(settingsDialog, &SettingsDialog::accepted, settingsManager, &SettingsManager::save);
	trayIcon->setVisible(settingsManager->shouldShowTrayIcon());
	trayIcon->setContextMenu(createContextMenu(settingsDialog));

	NotificationManager* notificationManager = new NotificationManager(trayIcon, settingsManager);
	QObject::connect(settingsManager, &SettingsManager::saved, notificationManager, &NotificationManager::reloadSettings);
	QObject::connect(settingsManager, &SettingsManager::saved, [trayIcon](const SettingsManager* settings) {
		trayIcon->setVisible(settings->shouldShowTrayIcon());
	});
	return application.exec();
}

