# Makefile

# Compilador y flags
CXX = g++
CXXFLAGS = -Wall -g -Iincludes -Ilib

# Directorios
SRC_DIR = .
LIB_DIR = lib

# Archivos fuente
SRC = $(SRC_DIR)/main.cpp \
      $(SRC_DIR)/Funciones.cpp \
      $(SRC_DIR)/Menus.cpp \
      $(LIB_DIR)/ListaEnlazadaSimple.cpp \
      $(LIB_DIR)/ListaEnlazadaSolicitudes.cpp \
      $(LIB_DIR)/ListaEnlazadaDoble.cpp \
	  

# Nombre del ejecutable
TARGET = $(SRC_DIR)/output/ejecutable

# Compilación final
all: $(TARGET)

$(TARGET):
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

# Limpiar archivos generados
clean:
	rm -f $(TARGET)
