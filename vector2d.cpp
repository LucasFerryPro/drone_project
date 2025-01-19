#include "vector2d.h"

/**
 * @brief Scalar multiplication of a vector by a scalar.
 *
 * Multiplies a vector by a scalar value and returns the resulting vector.
 *
 * @param a The scalar value.
 * @param v The vector to multiply.
 * @return The resulting vector after multiplication.
 */
const Vector2D operator*(double a, const Vector2D& v) {
    return Vector2D(static_cast<float>(a * v.x), static_cast<float>(a * v.y));
}

/**
 * @brief Dot product of two vectors.
 *
 * Calculates the dot product of two vectors.
 * The dot product is the sum of the products of the corresponding components of the vectors.
 *
 * @param u The first vector.
 * @param v The second vector.
 * @return The result of the dot product.
 */
double operator*(const Vector2D& u, const Vector2D& v) {
    return u.x * v.x + u.y * v.y;
}

/**
 * @brief Addition of two vectors.
 *
 * Adds the components of two vectors and returns the resulting vector.
 *
 * @param u The first vector.
 * @param v The second vector.
 * @return The resulting vector from adding u and v.
 */
const Vector2D operator+(const Vector2D& u, const Vector2D& v) {
    return Vector2D(u.x + v.x, u.y + v.y);
}

/**
 * @brief Subtraction of two vectors.
 *
 * Subtracts the components of one vector from another and returns the resulting vector.
 *
 * @param u The first vector.
 * @param v The second vector.
 * @return The resulting vector from subtracting v from u.
 */
const Vector2D operator-(const Vector2D& u, const Vector2D& v) {
    return Vector2D(u.x - v.x, u.y - v.y);
}

/**
 * @brief Negation of a vector.
 *
 * Returns the negated vector (all components reversed).
 *
 * @param v The vector to negate.
 * @return The negated vector.
 */
const Vector2D operator-(const Vector2D& v) {
    return Vector2D(-v.x, -v.y);
}

/**
 * @brief Cross product of two vectors.
 *
 * Calculates the 2D cross product (determinant) of two vectors.
 *
 * @param u The first vector.
 * @param v The second vector.
 * @return The scalar result of the cross product.
 */
double operator^(const Vector2D& u, const Vector2D& v) {
    return u.x * v.y - u.y * v.x;
}

/**
 * @brief Equality comparison of two vectors.
 *
 * Compares the components of two vectors to check if they are equal.
 *
 * @param u The first vector.
 * @param v The second vector.
 * @return True if the vectors are equal, false otherwise.
 */
bool operator==(const Vector2D& u, const Vector2D& v) {
    return (u.x == v.x && u.y == v.y);
}

/**
 * @brief Inequality comparison of two vectors.
 *
 * Compares the components of two vectors to check if they are not equal.
 *
 * @param u The first vector.
 * @param v The second vector.
 * @return True if the vectors are not equal, false otherwise.
 */
bool operator!=(const Vector2D& u, const Vector2D& v) {
    return !(u == v);
}
