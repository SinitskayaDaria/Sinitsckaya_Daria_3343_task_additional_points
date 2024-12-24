#ifndef MEMRESERVER_H
#define MEMRESERVER_H

#include <stdexcept>
#include <type_traits>
#include <utility>

// исключение при нехватке слотов для создания новых объектов
class NotEnoughSlotsError : public std::runtime_error {
public:
    explicit NotEnoughSlotsError(const std::string& message)
        : std::runtime_error(message) {}
};

// исключение при попытке доступа к пустому слоту
class EmptySlotError : public std::runtime_error {
public:
    explicit EmptySlotError(const std::string& message)
        : std::runtime_error(message) {}
};

// шаблонный класс управления статической памятью
template <typename T, std::size_t N>
class MemReserver {
    static_assert(N > 0, "Size N must be greater than 0"); // проверка размер N больше 0

public:
    // конструктор инициализирует все слоты как неиспользуемые
    MemReserver() : count_(0) {
        for (std::size_t i = 0; i < N; ++i) {
            slots_used_[i] = false; 
        }
    }

    // деструктор очищает все используемые слоты
    ~MemReserver() {
        for (std::size_t i = 0; i < N; ++i) {
            if (slots_used_[i]) {
                reinterpret_cast<T*>(&memory_[i])->~T();
            }
        }
    }

    // метод создания нового объекта в слоте, с передачей аргументов в конструктор
    template <typename... Args>
    T& create(Args&&... args) {
        // нахождение первого неиспользуемого слота
        for (std::size_t i = 0; i < N; ++i) {
            if (!slots_used_[i]) {
                slots_used_[i] = true;                                      // пометка слота как используемого
                ++count_;                                                   // увеличение счётчика объектов
                T* obj = new (&memory_[i]) T(std::forward<Args>(args)...);  // конструирование объекта 
                return *obj;                                                // ссылка на созданный объект
            }
        }
        // исключение все слоты заняты
        throw NotEnoughSlotsError("No slots available. Objects created: " + std::to_string(count_));
    }

    // метод удаления объекта из указанного слота 
    //(в задании назван delete, но это зарезервированное слово, не могу использовать)
    void remove(std::size_t index) {
        // проверка допустимость индекса и наличие объекта в слоте
        if (index >= N || !slots_used_[index]) {
            throw EmptySlotError("Slot " + std::to_string(index) + " is empty or invalid");
        }
        reinterpret_cast<T*>(&memory_[index])->~T(); // вызов деструктора для объекта
        slots_used_[index] = false;                  // пометка слота как неиспользуемого
        --count_;                                    // уменьшение счётчика объектов
    }

    // метод получения текущего количества созданных объектов
    std::size_t count() const {
        return count_; 
    }

    // метод получения ссылки на объект в указанном слоте
    T& get(std::size_t index) {
        // проверка допустимость индекса и наличие объекта в слоте
        if (index >= N || !slots_used_[index]) {
            throw EmptySlotError("Slot " + std::to_string(index) + " is empty or invalid");
        }
        return *reinterpret_cast<T*>(&memory_[index]); 
    }

    // метод получения позиции объекта в памяти
    std::size_t position(const T& obj) const {
        const T* obj_ptr = &obj; // указатель на переданный объект
        // проход по слотам
        for (std::size_t i = 0; i < N; ++i) {
            if (slots_used_[i] && reinterpret_cast<const T*>(&memory_[i]) == obj_ptr) {
                return i; 
            }
        }
        // исключение объект не найден
        throw std::runtime_error("Object not found in storage");
    }

private:
    alignas(T) char memory_[N * sizeof(T)]; // память для хранения объектов типа T
    bool slots_used_[N];                    // массив для отслеживания занятости слотов
    std::size_t count_;                     // счетчик текущего количества созданных объектов
};

#endif 
