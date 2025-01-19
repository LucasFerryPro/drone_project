#include "server.h"

/**
 * @brief Constructor for the Server class.
 *
 * This constructor initializes the server with a name, position, and color.
 *
 * @param name The name of the server.
 * @param position The position of the server.
 * @param color The color of the server.
 */
Server::Server(const QString &name, const Vector2D &position, const QColor &color)
    : name(name), position(position), color(color) {}

/**
 * @brief Gets the name of the server.
 *
 * This method returns the name of the server.
 *
 * @return The name of the server.
 */
QString Server::getName() const {
    return name;
}

/**
 * @brief Gets the position of the server.
 *
 * This method returns the position of the server.
 *
 * @return The position of the server.
 */
Vector2D Server::getPosition() const {
    return position;
}

/**
 * @brief Gets the color of the server.
 *
 * This method returns the color of the server.
 *
 * @return The color of the server.
 */
QColor Server::getColor() const {
    return color;
}

/**
 * @brief Adds a neighboring server to the list of neighbors.
 *
 * This method adds a neighboring server to the list of servers that are
 * adjacent to the current server.
 *
 * @param neighbor The neighboring server to add.
 */
void Server::addNeighbor(Server *neighbor) {
    neighbors.append(neighbor);  ///< Add the neighbor server to the list.
}

/**
 * @brief Gets the list of neighboring servers.
 *
 * This method returns a constant reference to the list of neighboring servers.
 *
 * @return A constant reference to the list of neighboring servers.
 */
const QVector<Server*> &Server::getNeighbors() const {
    return neighbors;
}

/**
 * @brief Clears the server.
 *
 * This method removes all neighboring servers from the list of neighbors.
 * It does not modify other attributes like name, position, or color.
 */
void Server::clearServer() {
    neighbors.clear();  ///< Clear the list of neighboring servers.
}
