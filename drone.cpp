#include "drone.h"
#include <QPainter>
#include <QStyle>
#include <QDebug>

/**
 * @brief Constructor for the Drone class
 * @param n The name of the drone
 * @param parent The parent widget
 */
Drone::Drone(const QString &n, QWidget *parent)
    : QWidget{parent}, name(n) {
    status = landed;  // Initialize the drone's status to "landed"
    speed = 0;  // Initial speed is 0
    power = maxPower / 2.0;  // Initial power is half of the maximum power
    V.set(0, 0);  // Initialize the velocity vector to 0
    ForceCollision.set(0, 0);  // Initialize the collision force to 0
    position = Vector2D(50, 50);  // Initial position of the drone
    goalPosition = Vector2D(550, 600);  // Initial target position
    showCollision = false;  // No collision detected initially
    azimut = 0;  // Initial angle is 0

    // Initialize progress bars for speed and power
    speedPB = new QProgressBar(this);
    speedPB->setValue(speed);
    speedPB->setMaximum(maxSpeed);
    speedPB->setMinimum(0);
    speedPB->setFormat(name + " speed %p%");
    speedPB->setAlignment(Qt::AlignCenter);

    powerPB = new QProgressBar(this);
    powerPB->setValue(power);
    powerPB->setMaximum(maxPower);
    powerPB->setMinimum(0);
    powerPB->setFormat("power %p%");
    powerPB->setAlignment(Qt::AlignCenter);

    setBaseSize(barSpace + compasSize, 2 * compasSize);  // Set the base size of the widget
    setMinimumHeight(2 * compasSize);  // Set the minimum height
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);  // Set the size policy

    // Load images for the drone's UI
    compasImg.load("../../media/compas.png");
    stopImg.load("../../media/stop.png");
    takeoffImg.load("../../media/takeoff.png");
    landingImg.load("../../media/landing.png");
}

/**
 * @brief Destructor for the Drone class
 */
Drone::~Drone() {
    delete speedPB;  // Free the memory of the speed progress bar
    delete powerPB;  // Free the memory of the power progress bar
}

/**
 * @brief Handle the paint event (redraw the drone)
 * @param event The paint event
 */
void Drone::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    QBrush whiteBrush(Qt::SolidPattern);
    whiteBrush.setColor(Qt::white);
    QRect rect(0, 0, compasSize, compasSize);

    // Draw the image corresponding to the drone's status
    switch (status) {
    case landed: painter.drawImage(rect, stopImg); break;
    case takeoff: painter.drawImage(rect, takeoffImg); break;
    case landing: painter.drawImage(rect, landingImg); break;
    default : {
        painter.drawImage(rect, compasImg);
        QPointF *points = new QPointF[3];
        points[0] = QPointF(-compasSize / 5.0, 0);
        points[1] = QPointF(compasSize / 5.0, 0);
        points[2] = QPointF(0, compasSize / 2.2);
        painter.save();
        painter.translate(compasSize / 2.0, compasSize / 2.0);
        painter.rotate(azimut);
        painter.setBrush(Qt::white);
        painter.setPen(Qt::black);
        painter.drawPolygon(points, 3);
        painter.setBrush(Qt::red);
        painter.rotate(180);
        painter.drawPolygon(points, 3);
        painter.restore();
    }
    }
}

/**
 * @brief Handle the resize event
 * @param event The resize event
 */
void Drone::resizeEvent(QResizeEvent *) {
    QRect rect(compasSize + 5, 0, width() - compasSize - 5, compasSize / 2);
    speedPB->setGeometry(rect);  // Resize the speed progress bar
    rect.setRect(compasSize + 5, compasSize / 2, width() - compasSize - 5, compasSize / 2);
    powerPB->setGeometry(rect);  // Resize the power progress bar
}

/**
 * @brief Update the drone's state
 * @param dt The time elapsed since the last update
 */
void Drone::update(double dt) {
    if (status == landed) {
        power += dt * chargingSpeed;  // Charge the drone's battery
        if (power > maxPower) {
            power = maxPower;
        }
        powerPB->setValue(power);  // Update the power progress bar
        repaint();  // Redraw the drone
        return;
    }

    if (status == takeoff) {
        height += dt * takeoffSpeed;  // Increase the drone's height
        if (height >= hoveringHeight) {
            height = hoveringHeight;
            status = hovering;  // Switch to "hovering" mode
        }
        power -= dt * powerConsumption;  // Consume power
        if (power < 20 + powerConsumption / takeoffSpeed) {
            status = landing;  // Switch to "landing" mode if power is too low
            speed = 0;
        }
        powerPB->setValue(power);  // Update the power progress bar
        repaint();  // Redraw the drone
        return;
    }

    if (status == landing) {
        height -= dt * takeoffSpeed;  // Decrease the drone's height
        if (height <= 0) {
            height = 0;
            status = landed;  // Switch to "landed" mode
            showCollision = false;  // Reset collision detection
        }
        power -= dt * powerConsumption;  // Consume power
        powerPB->setValue(power);  // Update the power progress bar
        repaint();  // Redraw the drone
        return;
    }

    if (status >= hovering) {
        Vector2D toGoal = goalPosition - position;  // Vector to the target position
        double distance = toGoal.length();  // Distance to the target

        double damp = 1 - dt * (1 - damping);  // Calculate the damping factor
        V = damp * V + ((maxPower * dt / distance) * toGoal) + dt * ForceCollision;  // Update the velocity
        position += dt * V;  // Update the position
        speed = V.length();  // Update the speed
        Vector2D Vn = (1.0 / speed) * V;  // Normalized velocity vector

        // Calculate the azimuth based on the drone's direction
        if (Vn.y == 0) {
            if (Vn.x > 0) {
                azimut = -90;
            } else {
                azimut = 90.0;
            }
        } else if (Vn.y > 0) {
            azimut = 180.0 - 180.0 * atan(Vn.x / Vn.y) / M_PI;
        } else {
            azimut = -180.0 * atan(Vn.x / Vn.y) / M_PI;
        }

        // If the drone is close to the target and at low speed, switch to "landing" mode
        if (toGoal.length() < 1.0 && speed < 10) {
            V.set(0, 0);
            speed = 0;
            status = landing;
        }
        speedPB->setValue(speed);  // Update the speed progress bar
        power -= dt * powerConsumption;  // Consume power
        if (power < 20 + powerConsumption / takeoffSpeed) {
            speed = 0;
            V.set(0, 0);
            status = landing;  // Switch to "landing" mode if power is too low
        }
        powerPB->setValue(power);  // Update the power progress bar
    }
    repaint();  // Redraw the drone
}

/**
 * @brief Prepare data for collision detection
 */
void Drone::initCollision() {
    ForceCollision.set(0, 0);  // Reset the collision force
    showCollision = false;  // Reset collision detection
}

/**
 * @brief Add a collision force if another drone is too close
 * @param B The position of the other drone
 * @param threshold The collision detection distance
 */
void Drone::addCollision(const Vector2D &B, float threshold) {
    Vector2D AB = B - position;  // Vector between the two drones
    double l = AB.length();  // Distance between the two drones
    if (l < threshold) {
        ForceCollision += (-coefCollision / threshold) * AB;  // Add a collision force
        showCollision = true;  // Indicate that a collision has been detected
    }
}
