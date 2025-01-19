#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QListWidgetItem>

/**
 * @brief Constructor for the MainWindow class.
 *
 * This constructor sets up the UI, initializes a timer for regular updates,
 * and starts the elapsed timer to track simulation time.
 * @param parent The parent widget (optional).
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);

    // Create a timer for simulation updates
    timer = new QTimer(this);
    timer->setInterval(100);  // Set the update interval to 100 ms
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));  // Connect the update slot
    timer->start();  // Start the timer

    elapsedTimer.start();  // Start measuring elapsed time for the simulation
}

/**
 * @brief Destructor for the MainWindow class.
 *
 * This destructor cleans up allocated resources, including the UI and timer.
 */
MainWindow::~MainWindow() {
    delete ui;  ///< Free memory allocated for the UI.
    delete timer;  ///< Free memory allocated for the timer.
}

/**
 * @brief Handle the quit action triggered from the menu.
 *
 * This function will close the application when the "Quit" action is triggered.
 */
void MainWindow::on_actionQuit_triggered() {
    QApplication::quit();  // Exit the application
}

/**
 * @brief Handle the load action triggered from the menu.
 *
 * This function opens a file dialog to select a JSON file and loads the data
 * from the selected file into the simulation.
 */
void MainWindow::on_actionLoad_triggered() {
    QString filePath = QFileDialog::getOpenFileName(this, "Open JSON File", "", "JSON Files (*.json)");
    if (!filePath.isEmpty()) {
        loadJson(filePath);  // Load the JSON file if the path is not empty
    }
}

/**
 * @brief Load a JSON file containing drone and server data.
 *
 * This function loads drone and server data from a given JSON file, clears the existing
 * data from the UI, and sets up new servers and drones in the simulation.
 *
 * @param filePath The path to the JSON file containing the data.
 */
void MainWindow::loadJson(const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Could not open file:" << filePath;
        return;
    }

    QByteArray data = file.readAll();
    QJsonDocument doc(QJsonDocument::fromJson(data));
    QJsonObject json = doc.object();

    // Clear the existing servers and drones in the UI
    ui->widget->clearServers();  // Clear the server list from the canvas
    for (auto &drone : mapDrones) {
        delete drone;  // Free the memory for each drone
    }
    mapDrones.clear();  // Clear the map of drones
    ui->listDronesInfo->clear();  // Clear the drone list widget

    QVector<Server> servers;

    // Load servers from the JSON file
    QJsonArray serverArray = json["servers"].toArray();
    for (const QJsonValue &serverValue : serverArray) {
        QJsonObject server = serverValue.toObject();
        QString name = server["name"].toString();
        QString positionStr = server["position"].toString();
        QString colorStr = server["color"].toString();

        QStringList posList = positionStr.split(",");
        Vector2D position(posList[0].toFloat(), posList[1].toFloat());
        QColor color(colorStr);

        servers.append(Server(name, position, color));
        qDebug() << "Loaded server:" << name << "at position:" << positionStr << "with color:" << colorStr;
    }

    ui->widget->setServers(servers);  // Set the list of servers in the canvas

    // Load drones from the JSON file
    QJsonArray droneArray = json["drones"].toArray();
    for (const QJsonValue &droneValue : droneArray) {
        QJsonObject drone = droneValue.toObject();
        QString name = drone["name"].toString();
        QString positionStr = drone["position"].toString();
        QString colorStr = drone["color"].toString();
        QString server = drone["server"].toString();

        QStringList posList = positionStr.split(",");
        Vector2D position(posList[0].toFloat(), posList[1].toFloat());

        Drone *newDrone = new Drone(name);
        newDrone->setInitialPosition(position);
        newDrone->setTargetServer(server);

        mapDrones[name] = newDrone;

        QListWidgetItem *LWitems = new QListWidgetItem(ui->listDronesInfo);
        ui->listDronesInfo->addItem(LWitems);
        ui->listDronesInfo->setItemWidget(LWitems, newDrone);

        qDebug() << "Loaded drone:" << name << "at position:" << positionStr << "with color:" << colorStr << "and server:" << server;
    }

    ui->widget->setMap(&mapDrones);  // Set the map of drones in the canvas

    file.close();
}

/**
 * @brief Update the simulation at regular intervals.
 *
 * This method updates each drone's position and checks for collisions with other drones.
 * It adjusts the number of simulation steps based on elapsed time to ensure smooth performance.
 */
void MainWindow::update() {
    static int last = elapsedTimer.elapsed();  // Last update time
    static int steps = 5;  // Number of simulation steps per update
    int current = elapsedTimer.elapsed();  // Current time
    double dt = (current - last) / (1000.0 * steps);  // Time difference between updates

    // Update each drone in the simulation
    for (int step = 0; step < steps; step++) {
        for (auto &drone : mapDrones) {
            QString targetServerName = drone->getTargetServer();
            Server* targetServer = ui->widget->findServerByName(targetServerName);

            if (targetServer) {
                drone->setGoalPosition(targetServer->getPosition());  // Set the drone's goal position
            }

            // Handle collisions between drones
            if (drone->getStatus() != Drone::landed) {
                drone->initCollision();  // Reset collision state
                for (auto &obs : mapDrones) {
                    if (obs->getStatus() != Drone::landed && obs->getName() != drone->getName()) {
                        Vector2D B = obs->getPosition();  // Position of another drone
                        drone->addCollision(B, ui->widget->droneCollisionDistance);  // Add collision force
                    }
                }
            }

            drone->update(dt);  // Update the drone's state
        }
    }

    int d = elapsedTimer.elapsed() - current;  // Time elapsed in this update
    ui->statusbar->showMessage("duration:" + QString::number(d) + " steps=" + QString::number(steps));  // Show the duration in the status bar

    // Adjust the number of steps based on elapsed time
    if (d > 90) {
        steps /= 2;
    } else {
        if (steps < 10) steps++;
    }
    last = current;  // Update the last update time
    ui->widget->repaint();  // Redraw the widget to reflect changes
}
