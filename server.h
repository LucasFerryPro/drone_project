#ifndef SERVER_H
#define SERVER_H

#include <QString>
#include <QColor>
#include <QVector>
#include "vector2d.h"

/**
 * @class Server
 * @brief Class representing a server in the simulation.
 *
 * The Server class represents a server that can be part of the simulation,
 * with properties such as name, position, color, and neighboring servers.
 */
class Server {
public:
    /**
     * @brief Constructs a Server object with specified parameters.
     *
     * @param name The name of the server.
     * @param position The position of the server.
     * @param color The color of the server.
     */
    Server(const QString &name, const Vector2D &position, const QColor &color);

    /**
     * @brief Gets the name of the server.
     *
     * @return The name of the server.
     */
    QString getName() const;

    /**
     * @brief Gets the position of the server.
     *
     * @return The position of the server.
     */
    Vector2D getPosition() const;

    /**
     * @brief Gets the color of the server.
     *
     * @return The color of the server.
     */
    QColor getColor() const;

    /**
     * @brief Adds a neighboring server to the server.
     *
     * @param neighbor The neighboring server to add.
     */
    void addNeighbor(Server *neighbor);

    /**
     * @brief Gets the list of neighboring servers.
     *
     * @return A constant reference to the list of neighboring servers.
     */
    const QVector<Server*> &getNeighbors() const;

    /**
     * @brief Clears the server, removing all neighbors.
     *
     * This function clears the list of neighboring servers.
     * The other attributes (name, position, and color) remain unchanged.
     */
    void clearServer();

private:
    QString name; ///< The name of the server.
    Vector2D position; ///< The position of the server.
    QColor color; ///< The color of the server.
    QVector<Server*> neighbors; ///< List of neighboring servers.
};

#endif // SERVER_H
