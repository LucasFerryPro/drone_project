/**
 * @file mainwindow.h
 * @brief Main window for the Drone Demo application.
 *
 * This file declares the MainWindow class, which is responsible for handling the GUI
 * of the drone simulation, including loading data from JSON, simulating drone movements,
 * and updating the user interface.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <drone.h>
#include <QListWidget>
#include <QMap>
#include <QTimer>
#include <QElapsedTimer>
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

/**
 * @class MainWindow
 * @brief The main window for the drone simulation.
 *
 * This class manages the main window, including UI setup, JSON file loading,
 * and the drone simulation logic.
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    /**
     * @brief Constructor for the MainWindow class.
     * @param parent The parent widget (optional).
     */
    MainWindow(QWidget *parent = nullptr);

    /**
     * @brief Destructor for the MainWindow class.
     */
    ~MainWindow();

    /**
     * @brief Load a JSON file containing drone and server data.
     * @param filePath The path to the JSON file.
     */
    void loadJson(const QString &filePath);

private slots:
    /**
     * @brief Handle the quit action from the menu.
     */
    void on_actionQuit_triggered();

    /**
     * @brief Handle the load action to load a new JSON file.
     */
    void on_actionLoad_triggered();

    /**
     * @brief Update the simulation at regular intervals.
     */
    void update();

private:
    Ui::MainWindow *ui; ///< UI object for managing the user interface.
    QMap<QString, Drone*> mapDrones; ///< Map of drone names to Drone objects.
    QTimer *timer; ///< Timer for simulating updates at regular intervals.
    QElapsedTimer elapsedTimer; ///< Timer for measuring elapsed time in the simulation.
};

#endif // MAINWINDOW_H
