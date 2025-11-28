#pragma once
#include "Celda.hpp" // Celda
#include <vector>
#include <SFML/Graphics.hpp>
#include <random>

class Tablero {
public:
    Tablero(int filas, int columnas, int minas);
    void descubrir(int fila, int columna);
    void marcar(int fila, int columna);
    bool esFinDelJuego() const;
    bool esVictoria() const;
    void dibujar(sf::RenderWindow& window, float size = 32.0f) const;
    int getFilas() const { return filas; }
    int getColumnas() const { return columnas; }
    int getMinas() const { return minas; }
    const Celda& getCelda(int fila, int columna) const { return celdas[fila][columna]; }

private:
    int filas, columnas, minas;
    std::vector<std::vector<Celda>> celdas;
    bool finDelJuego;
    bool victoria;
    void colocarMinas();
    void calcularMinasAlrededor();
    void descubrirRecursivo(int fila, int columna);
};