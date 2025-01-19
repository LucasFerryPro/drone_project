#include "voronoi.h"

/**
 * @brief Draw the Voronoi diagram within a specified rectangle.
 *
 * This method iterates over all the pixels in the specified rectangle and determines
 * the color for each pixel based on the closest server. It then draws the pixel with the calculated color.
 *
 * @param painter The painter object used for drawing the diagram.
 * @param rect The rectangle within which the diagram will be drawn.
 */
void Voronoi::draw(QPainter &painter, const QRect &rect) {
    for (int x = rect.left(); x < rect.right(); ++x) {
        for (int y = rect.top(); y < rect.bottom(); ++y) {
            Vector2D point(x, y);  ///< Current pixel position
            QColor color = getColorForPoint(point);  ///< Color based on the closest server
            painter.setPen(color);  ///< Set the pixel color
            painter.drawPoint(x, y);  ///< Draw the pixel at the specified position
        }
    }
}

/**
 * @brief Get the color for a point based on the closest server.
 *
 * This method finds the closest server to the given point and returns the color of that server.
 * Additionally, it adjusts the lightness of the color depending on the distance to the server.
 *
 * @param point The point whose color is to be determined based on the nearest server.
 * @return The color associated with the closest server to the point.
 */
QColor Voronoi::getColorForPoint(const Vector2D &point) {
    double minDistance = std::numeric_limits<double>::max();  ///< Initialize minimum distance to the maximum possible value.
    QColor color;  ///< Variable to hold the color of the closest server.

    // Iterate over all servers to find the closest one
    for (const Server &server : servers) {
        double distance = (server.getPosition() - point).length();  ///< Calculate the distance from the server to the point
        if (distance < minDistance) {
            minDistance = distance;  ///< Update the minimum distance
            color = server.getColor();  ///< Update the color with the closest server's color

            // Adjust the lightness of the color based on the distance
            int lightness = color.lightness();  ///< Get the current lightness of the color
            if (distance < 50) {
                lightness += 20;  ///< Lighten the color if the distance is small (closer to the server)
            } else {
                lightness -= 10;  ///< Darken the color if the distance is larger
            }
            // Apply the adjusted lightness to the color
            color.setHsl(color.hue(), color.saturation(), qBound(0, lightness, 255));  ///< Ensure the lightness remains within valid bounds.
        }
    }

    return color;  ///< Return the color for the point based on the closest server.
}
