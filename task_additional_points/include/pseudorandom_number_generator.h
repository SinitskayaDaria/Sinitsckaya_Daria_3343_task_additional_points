#ifndef SIMPLERNG_H
#define SIMPLERNG_H

#include <vector>
#include <iterator>
#include <cmath>

class SimpleRNG {
public:
    // Constructor
    SimpleRNG(double a, double c, double m, double x0 = 0);

    // Reset method
    void reset(double x0 = -1);

    // Iterator class
    class Iterator {
    public:
        using value_type = double;
        using difference_type = std::ptrdiff_t;
        using pointer = const double*;
        using reference = const double&;
        using iterator_category = std::input_iterator_tag;

        // Constructor
        Iterator(SimpleRNG* generator, bool isEnd = false, double eps = 0.05);

        // Operators
        reference operator*() const;
        Iterator& operator++();
        Iterator operator++(int);
        bool operator==(const Iterator& other) const;
        bool operator!=(const Iterator& other) const;

    private:
        SimpleRNG* generator;
        double currentValue;
        double initialValue;
        bool isEnd;
        double eps;

        void generateNext();
    };

    // Methods to get iterators
    Iterator begin();
    Iterator end(double eps = 0.05);

private:
    double a, c, m;
    double x0;
    double currentState;
};

#endif // SIMPLERNG_H
