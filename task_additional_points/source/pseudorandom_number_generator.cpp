#include "pseudorandom_number_generator.h"
#include <stdexcept>

// Constructor
SimpleRNG::SimpleRNG(double a, double c, double m, double x0)
    : a(a), c(c), m(m), x0(x0), currentState(x0) {
    if (m <= 1 || a <= 0 || a >= 1 || c <= 0 || c >= m) {
        throw std::invalid_argument("Invalid parameters for SimpleRNG");
    }
}

// Reset method
void SimpleRNG::reset(double x0) {
    if (x0 >= 0) {
        this->x0 = x0;
    }
    currentState = this->x0;
}

// Iterator constructor
SimpleRNG::Iterator::Iterator(SimpleRNG* generator, bool isEnd, double eps)
    : generator(generator), currentValue(generator->currentState),
      initialValue(generator->currentState), isEnd(isEnd), eps(eps) {}

// Dereference operator
SimpleRNG::Iterator::reference SimpleRNG::Iterator::operator*() const {
    return currentValue;
}

// Pre-increment
SimpleRNG::Iterator& SimpleRNG::Iterator::operator++() {
    generateNext();
    return *this;
}

// Post-increment
SimpleRNG::Iterator SimpleRNG::Iterator::operator++(int) {
    Iterator tmp = *this;
    generateNext();
    return tmp;
}

// Equality operator
bool SimpleRNG::Iterator::operator==(const Iterator& other) const {
    if (isEnd || other.isEnd) {
        return isEnd == other.isEnd;
    }
    return std::abs(currentValue - initialValue) < eps;
}

// Inequality operator
bool SimpleRNG::Iterator::operator!=(const Iterator& other) const {
    return !(*this == other);
}

// Generate next value
void SimpleRNG::Iterator::generateNext() {
    if (!isEnd) {
        generator->currentState = std::fmod(generator->a * generator->currentState + generator->c, generator->m);
        currentValue = generator->currentState;
    }
}

// Begin iterator
SimpleRNG::Iterator SimpleRNG::begin() {
    return Iterator(this);
}

// End iterator
SimpleRNG::Iterator SimpleRNG::end(double eps) {
    return Iterator(this, true, eps);
}
