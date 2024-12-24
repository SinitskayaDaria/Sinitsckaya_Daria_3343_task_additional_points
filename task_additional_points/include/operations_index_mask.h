#ifndef MASK_H
#define MASK_H

#include <array>
#include <vector>
#include <stdexcept>
#include <type_traits>

// шаблонный класс маски
template <std::size_t N>
class Mask {
    static_assert(N > 0, "Mask size must be greater than 0"); // проверка размер N больше 0

public:
    using MaskType = std::array<int, N>;

    Mask(std::initializer_list<int> values) {
        // проверка соответствия размеров
        if (values.size() != N) {
            throw std::invalid_argument("Mask size does not match the template parameter");
        }
        // копирование значений в маску
        std::copy(values.begin(), values.end(), mask_.begin());
        
        // проверка все значения маски равны 0 или 1
        for (int value : mask_) {
            if (value != 0 && value != 1) {
                throw std::invalid_argument("Mask values must be 0 or 1");
            }
        }
    }

    // метод получения размера маски
    constexpr std::size_t size() const noexcept {
        return N;
    }

    // метод доступа к элементу маски по индексу
    int at(std::size_t index) const {
        // проверка выхода за пределы
        if (index >= N) {
            throw std::out_of_range("Index out of range");
        }
        return mask_.at(index);
    }

    // метод применения маски к контейнеру и извлечения элементов
    template <typename Container>
    void slice(Container& container) const {
        static_assert(
            std::is_same_v<std::decay_t<decltype(container.at(0))>, typename Container::value_type>,
            "Container must have an at() method"
        );

        Container result;
        result.reserve(container.size());
        // проход по элементам контейнера
        for (std::size_t i = 0; i < container.size(); ++i) {
            if (mask_.at(i % N) == 1) {
                result.push_back(container.at(i));
            }
        }
        
        container = std::move(result); // обновление контейнера результатом
    }

    // метод преобразования элементов контейнера в соответствии с маской
    template <typename Container, typename TransformFunc>
    Container transform(const Container& container, TransformFunc func) const {
        static_assert(
            std::is_same_v<std::decay_t<decltype(container.at(0))>, typename Container::value_type>,
            "Container must have an at() method"
        );

        // Создаём копию контейнера
        Container result = container;
        // Применяем функцию к элементам, соответствующим маске
        for (std::size_t i = 0; i < container.size(); ++i) {
            if (mask_.at(i % N) == 1) {
                result.at(i) = func(container.at(i));
            }
        }
        return result; 
    }

    // метод применения маски к контейнеру и преобразования контейнера
    template <typename Container, typename TransformFunc>
    Container slice_and_transform(const Container& container, TransformFunc func) const {
        static_assert(
            std::is_same_v<std::decay_t<decltype(container.at(0))>, typename Container::value_type>,
            "Container must have an at() method"
        );

        Container result;
        result.reserve(container.size());
        for (std::size_t i = 0; i < container.size(); ++i) {
            if (mask_.at(i % N) == 1) {
                result.push_back(func(container.at(i)));
            }
        }
        return result; 
    }

private:
    MaskType mask_; // значения маски
};

#endif 
