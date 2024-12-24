#ifndef SIMPLERNG_H
#define SIMPLERNG_H

#include <vector>
#include <iterator>
#include <cmath>

// класс генерации псевдослучайных чисел
class SimpleRNG {
public:
    SimpleRNG(double m, double a, double c, double x0 = 0);

    void reset(double x0 = -1); // метод сброса состояния генератора

    // класс итератора для обхода чисел
    class Iterator {
    public:
        using value_type = double;                          // тип значения
        using difference_type = std::ptrdiff_t;             // разность между итераторами
        using pointer = const double*;                      // указатель на значение
        using reference = const double&;                    // ссылка на значение
        using iterator_category = std::input_iterator_tag;  // категория итератора

        Iterator(SimpleRNG* generator, bool isEnd = false, double eps = 0.05);  // конструктор итератора
        reference operator*() const;                                            // оператор разыменования 
        Iterator& operator++();                                                 // оператор прединкремента 
        Iterator operator++(int);                                               // оператор постинкремента
        bool operator==(const Iterator& other) const;                           // оператор сравнения на равенство
        bool operator!=(const Iterator& other) const;                           // оператор неравенства

    private:
        SimpleRNG* generator;    // указатель на экземпляр генератора
        double currentValue;     // текущее значение
        double initialValue;     // начальное значение
        bool isEnd;              // флаг конеца
        double eps;              // точность для сравнения значений
        int step;                // счётчик шагов
        const int maxSteps = 25; // максимальное количество шагов для предотвращения зацикливания

        void generateNext();     // метод генерации следующего значения
    };

    Iterator begin();                   // метод получения итератора, указывающего на начало последовательности
    Iterator end(double eps = 0.05);    // метод получения итератора, указывающего на конец последовательности 

private:
    double m, a, c;         // параметры для генерации псевдослучайных чисел
    double x0;              // начальное значение
    double currentState;    // текущее состояние генератора
};

#endif 
