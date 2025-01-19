/**
 * @brief Drone_demo project
 * @author B.Piranda
 * @date dec. 2024
 **/
#ifndef DRONE_H
#define DRONE_H

#include <QWidget>
#include <QProgressBar>
#include <vector2d.h>
#include <QImage>

/**
 * @brief Drone class representing a drone in the simulation
 */
class Drone : public QWidget {
    Q_OBJECT
public:
    const double maxSpeed = 50; ///< Max speed in pixels per second
    const double maxPower = 200; ///< Max power of drone motors
    const double takeoffSpeed = 2.5; ///< Takeoff speed in units per second
    const double hoveringHeight = 5; ///< Hovering height in units
    const double coefCollision = 1000; ///< Coefficient for collision avoidance
    const double damping = 0.2; ///< Damping for motion simulation
    const double chargingSpeed = 10; ///< Charging speed in power per second
    const double powerConsumption = 5; ///< Power consumption in power per second

    /**
     * @brief Enum representing the status of the drone
     */
    enum droneStatus { landed, takeoff, landing, hovering, turning, flying };

    /**
     * @brief Drone constructor
     * @param p_name The name of the drone
     * @param parent The parent widget
     */
    explicit Drone(const QString &p_name, QWidget *parent = nullptr);

    /**
     * @brief Drone destructor
     */
    ~Drone();

    /**
     * @brief Make the drone takeoff to move to a target position
     */
    inline void start() { status = takeoff; height = 0; repaint(); }

    /**
     * @brief Ask for landing
     */
    inline void stop() { status = landing; }

    /**
     * @brief Set the speed of the drone
     * @param s The speed to set
     */
    inline void setSpeed(double s) { speedSetpoint = (s > maxSpeed ? maxSpeed : s); }

    /**
     * @brief Set the initial position of the drone (takeoff place)
     * @param pos The position to set
     */
    inline void setInitialPosition(const Vector2D& pos) { if (status == landed) position = pos; }

    /**
     * @brief Set the goal position of the drone (landing place)
     * @param pos The position to set
     */
    inline void setGoalPosition(const Vector2D& pos) { goalPosition = pos; }

    /**
     * @brief Get the current position of the drone
     * @return The current position
     */
    inline Vector2D getPosition() { return position; }

    /**
     * @brief Get the current status of the drone
     * @return The current status
     */
    inline droneStatus getStatus() { return status; }

    /**
     * @brief Get the name of the drone
     * @return The name of the drone
     */
    inline QString getName() { return name; }

    /**
     * @brief Get the direction of motion of the drone (angle in degrees relative to the y direction)
     * @return The angle in degrees
     */
    inline double getAzimut() { return azimut; }

    /**
     * @brief Get the power level of the drone (between 0 and 100)
     * @return The power level
     */
    inline double getPower() { return 100.0 * power / maxPower; }

    /**
     * @brief Handle the paint event
     * @param event The paint event
     */
    void paintEvent(QPaintEvent*) override;

    /**
     * @brief Handle the resize event
     * @param event The resize event
     */
    void resizeEvent(QResizeEvent *event) override;

    /**
     * @brief Update the drone's state
     * @param dt The time elapsed since the last update
     */
    void update(double dt);

    /**
     * @brief Prepare data for collision detection
     */
    void initCollision();

    /**
     * @brief Add a collision force
     * @param A The position of the other drone to test
     * @param threshold The distance for collision detection
     */
    void addCollision(const Vector2D& A, float threshold);

    /**
     * @brief Check if a collision has occurred
     * @return True if a collision has occurred
     */
    bool hasCollision() { return showCollision; }

    /**
     * @brief Set the target server for the drone
     * @param serverName The name of the target server
     */
    void setTargetServer(const QString &serverName) { targetServer = serverName; }

    /**
     * @brief Get the target server of the drone
     * @return The name of the target server
     */
    QString getTargetServer() const { return targetServer; }

private:
    const int compasSize = 48; ///< Size of the compass image
    const int barSpace = 150; ///< Minimum size of the progress bar
    droneStatus status; ///< Current status of the drone
    double height; ///< Current height of the drone
    QString name; ///< Name of the drone
    QProgressBar *speedPB; ///< Progress bar for speed
    QProgressBar *powerPB; ///< Progress bar for power
    Vector2D position; ///< Current position of the drone
    Vector2D goalPosition; ///< Goal position of the drone
    Vector2D direction; ///< Current direction of the drone
    Vector2D V; ///< Current speed vector
    Vector2D ForceCollision; ///< Force generated by collision detection
    double speed; ///< Current speed
    double speedSetpoint; ///< Speed to reach if possible
    double power; ///< Current power
    double azimut; ///< Rotation angle of the drone
    QImage compasImg, stopImg, takeoffImg, landingImg; ///< Images for the drone's UI
    bool showCollision; ///< True if a collision is detected
    QString targetServer; ///< Target server name
};

#endif // DRONE_H
