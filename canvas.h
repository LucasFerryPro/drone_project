/*!
 * @file canvas.h
 * @brief Declaration of the Canvas class, which manages an interactive map displaying drones and servers.
 *
 * The Canvas class handles graphical rendering, Voronoi diagram generation, and user interaction with drones.
 *
 * @details
 * This class is part of the Drone Demo project and provides functionality to display server positions,
 * generate Voronoi diagrams, and manage drones' movements and collision detection.
 *
 * @author B.Piranda
 * @date December 2024
 */

#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QVector>
#include <QMap>
#include "server.h"
#include "voronoi.h"
#include "drone.h"

/*!
 * @class Canvas
 * @brief A class to manage the graphical representation of an interactive map with drones and servers.
 */
class Canvas : public QWidget {
    Q_OBJECT

public:
    /*!
     * @brief Size of the drone image displayed on the canvas.
     */
    const int droneIconSize = 64;

    /*!
     * @brief Distance used to detect collisions between drones.
     */
    const double droneCollisionDistance = droneIconSize * 1.5;

    /*!
     * @brief Constructor for the Canvas class.
     * @param parent Pointer to the parent widget (default is nullptr).
     */
    explicit Canvas(QWidget *parent = nullptr);

    /*!
     * @brief Sets the map of drones identified by their names.
     * @param map A map of "drone name" to "drone pointer".
     */
    inline void setMap(QMap<QString, Drone*> *map) { mapDrones = map; }

    /*!
     * @brief Handles the paint event to redraw the canvas.
     * @param event The paint event.
     */
    void paintEvent(QPaintEvent* event) override;

    /*!
     * @brief Handles mouse press events for drone interaction.
     * @param event The mouse press event.
     */
    void mousePressEvent(QMouseEvent *event) override;

    /*!
     * @brief Sets the list of servers displayed on the canvas.
     * @param servers A vector of server objects.
     */
    void setServers(const QVector<Server> &servers);

    /*!
     * @brief Handles resize events and regenerates the Voronoi diagram.
     * @param event The resize event.
     */
    void resizeEvent(QResizeEvent *event) override;

    /*!
     * @brief Finds a server by its name.
     * @param name The name of the server to find.
     * @return A pointer to the server if found, or nullptr otherwise.
     */
    Server* findServerByName(const QString &name);

    /*!
     * @brief Clears all servers on the canvas by resetting their neighbors.
     */
    void clearServers();

signals:

private:
    QMap<QString, Drone*> *mapDrones = nullptr; ///< Map of drones identified by their names.
    QImage droneImg; ///< Image representing the drone on the canvas.
    QVector<Server> servers; ///< List of servers on the canvas.
    QImage voronoiImage; ///< Precomputed Voronoi diagram image.

    /*!
     * @brief Generates the Voronoi diagram image for the current set of servers.
     */
    void generateVoronoiImage();
};

#endif // CANVAS_H
