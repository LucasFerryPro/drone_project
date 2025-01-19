/**
 * @brief Drone_demo project
 * @author B.Piranda
 * @date Dec. 2024
 *
 * This file defines the Vector2D class that represents a 2D vector.
 * It includes various vector operations such as addition, scalar multiplication,
 * dot product, and orthogonal normalization.
 */

#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <cmath>

/**
 * @class Vector2D
 * @brief A class representing a 2D vector with basic vector operations.
 *
 * This class provides various methods to manipulate 2D vectors, including operations like
 * length calculation, normalization, and orthogonal vector generation.
 */
class Vector2D {
public:
    float x, y; ///< Coordinates of the vector

    /**
     * @brief Constructs a Vector2D with specified x and y components.
     *
     * @param p_x The x component of the vector.
     * @param p_y The y component of the vector.
     */
    Vector2D(float p_x, float p_y) : x(p_x), y(p_y) {}

    /**
     * @brief Default constructor for Vector2D.
     *
     * Initializes the vector to (0, 0).
     */
    Vector2D() : x(0), y(0) {}

    /**
     * @brief Copy constructor for Vector2D.
     *
     * @param p The vector to copy.
     */
    Vector2D(const Vector2D &p) : x(p.x), y(p.y) {}

    /**
     * @brief Set the components of the vector.
     *
     * @param p_x The new x component.
     * @param p_y The new y component.
     */
    void set(float p_x, float p_y) { x = p_x; y = p_y; }

    /**
     * @brief Get the length (or norm) of the vector.
     *
     * This method calculates and returns the magnitude of the vector.
     *
     * @return The length of the vector.
     */
    double length() const {
        return sqrt(x * x + y * y);
    }

    /**
     * @brief Normalize the vector.
     *
     * This method normalizes the vector, changing it to a vector with the same direction
     * but with a length (norm) of 1.
     */
    void normalize() {
        float l = float(length());
        x /= l;
        y /= l;
    }

    /**
     * @brief Get an orthogonal and normalized vector.
     *
     * This method returns a vector that is orthogonal (perpendicular) to the current vector
     * and normalized to have a length of 1.
     *
     * @return An orthogonal and normalized vector.
     */
    Vector2D orthoNormed() const {
        float l = float(length());
        return Vector2D(y / l, -x / l);
    }

    /**
     * @brief Access the components of the vector using index.
     *
     * @param i The index (0 for x, 1 for y).
     * @return The x component if i is 0, otherwise the y component.
     */
    float operator[](const int i) const {
        return (i == 0) ? x : y;
    }

    /**
     * @brief Add another vector to this vector.
     *
     * This method adds the components of another vector to the current vector.
     *
     * @param v The vector to add.
     */
    void operator+=(const Vector2D& v) {
        x += v.x;
        y += v.y;
    }

    // Friend operators
    friend double operator*(const Vector2D &, const Vector2D &);
    friend const Vector2D operator*(double, const Vector2D &);
    friend const Vector2D operator+(const Vector2D &, const Vector2D &);
    friend const Vector2D operator-(const Vector2D &, const Vector2D &);
    friend const Vector2D operator-(const Vector2D &);
    friend double operator^(const Vector2D &, const Vector2D &);
    friend bool operator==(const Vector2D &, const Vector2D &);
    friend bool operator!=(const Vector2D &, const Vector2D &);
};

// Friend operator declarations

/**
 * @brief Scalar multiplication of a vector.
 *
 * Multiplies a vector by a scalar (a real number).
 *
 * @param a The scalar value.
 * @param v The vector to multiply.
 * @return A new vector that is the result of the scalar multiplication.
 */
const Vector2D operator*(double a, const Vector2D &v);

/**
 * @brief Dot product of two vectors.
 *
 * Calculates the dot product of two vectors, which is the sum of the products
 * of their corresponding components.
 *
 * @param u The first vector.
 * @param v The second vector.
 * @return The dot product of the two vectors.
 */
double operator*(const Vector2D &u, const Vector2D &v);

/**
 * @brief Addition of two vectors.
 *
 * Adds the components of two vectors and returns a new vector as the result.
 *
 * @param u The first vector.
 * @param v The second vector.
 * @return The resulting vector from adding u and v.
 */
const Vector2D operator+(const Vector2D &u, const Vector2D &v);

/**
 * @brief Subtraction of two vectors.
 *
 * Subtracts the components of one vector from another and returns a new vector.
 *
 * @param u The first vector.
 * @param v The second vector.
 * @return The resulting vector from subtracting v from u.
 */
const Vector2D operator-(const Vector2D &u, const Vector2D &v);

/**
 * @brief Negation of a vector.
 *
 * Returns a new vector that is the negation of the current vector (flips its direction).
 *
 * @param v The vector to negate.
 * @return The negated vector.
 */
const Vector2D operator-(const Vector2D &v);

/**
 * @brief Cross product of two vectors.
 *
 * Calculates the 2D cross product of two vectors, which is the determinant of the
 * matrix formed by the vectors.
 *
 * @param u The first vector.
 * @param v The second vector.
 * @return The result of the cross product (a scalar value).
 */
double operator^(const Vector2D &u, const Vector2D &v);

/**
 * @brief Equality comparison of two vectors.
 *
 * Checks if two vectors are equal by comparing their components.
 *
 * @param u The first vector.
 * @param v The second vector.
 * @return True if the vectors are equal, false otherwise.
 */
bool operator==(const Vector2D &u, const Vector2D &v);

/**
 * @brief Inequality comparison of two vectors.
 *
 * Checks if two vectors are not equal by comparing their components.
 *
 * @param u The first vector.
 * @param v The second vector.
 * @return True if the vectors are not equal, false otherwise.
 */
bool operator!=(const Vector2D &u, const Vector2D &v);

#endif // VECTOR2D_H
