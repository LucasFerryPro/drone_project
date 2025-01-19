/*!
 * @file canvas.cpp
 * @brief Implementation of the Canvas class for managing graphical rendering and interaction with drones and servers.
 *
 * This file contains the implementation of methods for rendering the map, generating Voronoi diagrams,
 * handling user input, and managing the display of drones and servers.
 */

#include "canvas.h"
#include <QPainter>
#include <QResizeEvent>

/*!
 * @brief Constructor for the Canvas class.
 * @param parent Parent widget.
 */
Canvas::Canvas(QWidget *parent) : QWidget{parent} {
    droneImg.load("../../media/drone.png"); // Load the drone image from the specified path.
    setMouseTracking(true); // Enable mouse tracking for interaction.
}

/*!
 * @brief Sets the list of servers and regenerates the Voronoi image.
 * @param servers The list of servers to display.
 */
void Canvas::setServers(const QVector<Server> &servers) {
    this->servers = servers;
    generateVoronoiImage(); // Regenerate the Voronoi diagram.
    repaint(); // Trigger a repaint of the canvas.
}

/*!
 * @brief Generates the Voronoi diagram image for the current canvas size.
 */
void Canvas::generateVoronoiImage() {
    voronoiImage = QImage(size(), QImage::Format_ARGB32);
    QPainter painter(&voronoiImage);

    for (int x = 0; x < width(); ++x) {
        for (int y = 0; y < height(); ++y) {
            Vector2D point(x, y);
            QColor color = Qt::white;
            double minDistance = std::numeric_limits<double>::max();

            for (const Server &server : servers) {
                double distance = (server.getPosition() - point).length();
                if (distance < minDistance) {
                    minDistance = distance;
                    color = server.getColor();
                }
            }

            painter.setPen(color);
            painter.drawPoint(x, y);
        }
    }
}

/*!
 * @brief Finds a server by its name.
 * @param name The name of the server.
 * @return Pointer to the server if found, otherwise nullptr.
 */
Server* Canvas::findServerByName(const QString &name) {
    for (Server &server : servers) {
        if (server.getName() == name) {
            return &server;
        }
    }
    return nullptr;
}

/**
 * @brief Handle the paint event (redraw the canvas)
 * @param event The paint event
 */
void Canvas::paintEvent(QPaintEvent *) {
    QPainter painter(this);  // Create a QPainter to draw on the canvas
    QBrush whiteBrush(Qt::SolidPattern);  // White brush for the background
    QPen penCol(Qt::DashDotDotLine);  // Pen for collision visualization
    penCol.setColor(Qt::lightGray);
    penCol.setWidth(3);
    whiteBrush.setColor(Qt::white);
    painter.fillRect(0, 0, width(), height(), whiteBrush);  // Fill the background with white

    painter.drawImage(0, 0, voronoiImage);  // Draw the Voronoi image

    painter.setRenderHint(QPainter::Antialiasing, true);  // Enable antialiasing for smooth rendering

    // Draw each server
    for (const Server &server : servers) {
        Vector2D pos = server.getPosition();  // Server position
        QString name = server.getName();  // Server name

        painter.setBrush(server.getColor());  // Server color
        painter.setPen(Qt::black);  // Black outline
        painter.drawEllipse(pos.x, pos.y, 12, 12);  // Draw a circle representing the server

        painter.setPen(Qt::black);  // Black text
        painter.setFont(QFont("Arial", 10, QFont::Bold));  // Bold font
        painter.drawText(pos.x + 15, pos.y + 10, name);  // Display the server name
    }

    // Draw each drone
    if (mapDrones) {
        Vector2D p;
        QRect rect(-droneIconSize / 2, -droneIconSize / 2, droneIconSize, droneIconSize);  // Rectangle for the drone icon
        QRect rectCol(-droneCollisionDistance / 2, -droneCollisionDistance / 2, droneCollisionDistance, droneCollisionDistance);  // Rectangle for the collision zone

        for (auto &drone : *mapDrones) {
            painter.save();  // Save the painter state
            painter.translate(drone->getPosition().x, drone->getPosition().y);  // Translate to the drone's position
            painter.rotate(drone->getAzimut());  // Apply rotation based on the drone's azimuth
            painter.drawImage(rect, droneImg);  // Draw the drone image

            // Draw status indicators (LEDs) for the drone
            if (drone->getStatus() != Drone::landed) {
                painter.setPen(Qt::NoPen);
                painter.setBrush(Qt::red);
                painter.drawEllipse((-185.0 / 511.0) * droneIconSize, (-185.0 / 511.0) * droneIconSize, (65.0 / 511.0) * droneIconSize, (65.0 / 511.0) * droneIconSize);
                painter.drawEllipse((115.0 / 511.0) * droneIconSize, (-185.0 / 511.0) * droneIconSize, (65.0 / 511.0) * droneIconSize, (65.0 / 511.0) * droneIconSize);
                painter.setBrush(Qt::green);
                painter.drawEllipse((-185.0 / 511.0) * droneIconSize, (115.0 / 511.0) * droneIconSize, (70.0 / 511.0) * droneIconSize, (70.0 / 511.0) * droneIconSize);
                painter.drawEllipse((115.0 / 511.0) * droneIconSize, (115.0 / 511.0) * droneIconSize, (70.0 / 511.0) * droneIconSize, (70.0 / 511.0) * droneIconSize);
            }

            // Draw the collision zone if a collision is detected
            if (drone->hasCollision()) {
                painter.setPen(penCol);
                painter.setBrush(Qt::NoBrush);
                painter.drawEllipse(rectCol);
            }
            painter.restore();  // Restore the painter state
        }
    }
}

/*!
 * @brief Mouse press event handler for setting drone goals.
 * @param event The mouse press event.
 */
void Canvas::mousePressEvent(QMouseEvent *event) {
    auto it = mapDrones->begin();
    while (it != mapDrones->end() && (*it)->getStatus() != Drone::landed) {
        ++it;
    }
    if (it != mapDrones->end()) {
        (*it)->setGoalPosition(Vector2D(event->pos().x(), event->pos().y()));
        (*it)->start();
    }
    repaint();
}

/*!
 * @brief Resize event handler to regenerate the Voronoi diagram.
 * @param event The resize event.
 */
void Canvas::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    generateVoronoiImage();
    repaint();
}

/*!
 * @brief Clears all servers by resetting their neighbors.
 */
void Canvas::clearServers() {
    for (Server &server : servers) {
        server.clearServer();
    }
}
