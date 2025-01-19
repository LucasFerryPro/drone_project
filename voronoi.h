#ifndef VORONOI_H
#define VORONOI_H

#include <QVector>
#include <QPainter>
#include "server.h"
#include "vector2d.h"

/**
 * @class Voronoi
 * @brief A class to generate and draw Voronoi diagrams.
 *
 * This class is responsible for creating and rendering Voronoi diagrams based on a list of servers.
 * The diagram visually represents the regions of influence of each server.
 */
class Voronoi {
public:
    /**
     * @brief Construct a Voronoi diagram with a list of servers.
     *
     * This constructor initializes the Voronoi diagram using the provided list of servers.
     * Each server will have a region defined by the closest points to it.
     *
     * @param servers The list of servers to generate the Voronoi diagram.
     */
    Voronoi(const QVector<Server> &servers) : servers(servers) {}

    /**
     * @brief Draw the Voronoi diagram within a specified rectangle.
     *
     * This method draws the Voronoi diagram on a QPainter using the provided rectangle's dimensions.
     * The color of each pixel is determined by the closest server.
     *
     * @param painter The painter object used to draw the diagram.
     * @param rect The rectangle within which the diagram will be drawn.
     */
    void draw(QPainter &painter, const QRect &rect);

private:
    /**
     * @brief Get the color for a point based on the closest server.
     *
     * This method computes the color for a point based on the server closest to it.
     * The closer the point is to a server, the more intense the server's color will be.
     *
     * @param point The point to evaluate, for which the color is calculated.
     * @return The color that corresponds to the closest server for the point.
     */
    QColor getColorForPoint(const Vector2D &point);

    QVector<Server> servers; ///< The list of servers used to generate the Voronoi diagram.
};

#endif // VORONOI_H
