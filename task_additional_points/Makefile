# Указываем компилятор
CXX = g++

# Указываем флаги компиляции
CXXFLAGS = -g -Wall -std=c++17

# Указываем директорию с исходными файлами
SRC_DIR = source

# Указываем исходные файлы
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)

# Получаем объектные файлы, заменяя .cpp на .o
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp,$(SRC_DIR)/%.o,$(SOURCES))

# Исполняемый файл
TARGET = main

# Основная цель
all: $(TARGET)

# Создаем исполняемый файл
$(TARGET): $(OBJECTS)
	@echo "Linking: $@"
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(TARGET)

# Правило для компиляции .cpp файлов в .o
$(SRC_DIR)/%.o : $(SRC_DIR)/%.cpp
	@echo "Compiling: $<"
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Цель для очистки проектных файлов
clean:
	rm -rf $(SRC_DIR)/*.o $(TARGET)

.PHONY: all clean  # Указывает, что all и clean являются "фальшивыми" целями
