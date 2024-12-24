#include <stdexcept>

#include "../include/number_generator.h"

SimpleRNG::SimpleRNG(double m, double a, double c, double x0)
    : m(m), a(a), c(c), x0(x0), currentState(x0) {
    // проверка корректности параметров для генератора
    if (m <= 1 || a <= 0 || a >= 1 || c <= 0 || c >= m) {
        throw std::invalid_argument("Invalid parameters for SimpleRNG");
    }
}

// метод сброса состояния генератора
void SimpleRNG::reset(double x0) {
    if (x0 >= 0) {
        this->x0 = x0;
    }
    currentState = this->x0; 
}

SimpleRNG::Iterator::Iterator(SimpleRNG* generator, bool isEnd, double eps)
    : generator(generator), currentValue(generator->currentState),
      initialValue(generator->currentState), isEnd(isEnd), eps(eps), step(0) {}

// оператор разыменования
SimpleRNG::Iterator::reference SimpleRNG::Iterator::operator*() const {
    return currentValue;
}

// оператор прединкремента 
SimpleRNG::Iterator& SimpleRNG::Iterator::operator++() {
    generateNext(); // генерирование следующего значения
    return *this;   // возврат текущего итератора
}

// оператор постинкремента
SimpleRNG::Iterator SimpleRNG::Iterator::operator++(int) {
    Iterator tmp = *this;   // сохранение текущего состояния
    generateNext();         // генерирование следующего значения
    return tmp;             // возврат сохраненного состояния
}

// оператор сравнения на равенство
bool SimpleRNG::Iterator::operator==(const Iterator& other) const {
    if (isEnd || other.isEnd) {
        return isEnd == other.isEnd;
    }
    // сравнение текущего и начального значения с заданной точностью
    return std::abs(currentValue - initialValue) < eps || step >= maxSteps;
}

// оператор неравенства
bool SimpleRNG::Iterator::operator!=(const Iterator& other) const {
    return !(*this == other); 
}

// метод генерации следующего значения псевдослучайной последовательности
void SimpleRNG::Iterator::generateNext() {
    if (!isEnd && step < maxSteps) {
        // линейный конгруэнтный генератор для получения нового состояния
        generator->currentState = std::fmod(generator->a * generator->currentState + generator->c, generator->m);
        currentValue = generator->currentState; // обновление текущего значения
        ++step;

        // проверка на достижение максимального числа шагов
        if (step >= maxSteps) {
            isEnd = true;
        }
    } else {
        isEnd = true;
    }
}

// метод получения итератора начала последовательности
SimpleRNG::Iterator SimpleRNG::begin() {
    return Iterator(this); 
}

// метод получения итератора конеца последовательности
SimpleRNG::Iterator SimpleRNG::end(double eps) {
    return Iterator(this, true, eps); 
}
