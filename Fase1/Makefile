# Makefile

# Compilador y flags
CXX = g++
CXXFLAGS = -Wall -g -Iincludes -Ilib

# Directorios
SRC_DIR = .

# Archivos fuente
SRC = $(SRC_DIR)/main.cpp \
	$(SRC_DIR)/menus.cpp

# Nombre del ejecutable
TARGET = $(SRC_DIR)/output/ejecutable

# Compilación final
all: $(TARGET)

$(TARGET):
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

# Limpiar archivos generados
clean:
	rm -f $(TARGET)
