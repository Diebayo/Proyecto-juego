#pragma once
#include "Celda.hpp" // Celda
#include <vector>
#include <SFML/Graphics.hpp>
#include <random>

class Tablero {
public:
    Tablero(int filas, int columnas, int minas)
        : filas(filas), columnas(columnas), minas(minas), finDelJuego(false), victoria(false) {
        celdas.resize(filas, std::vector<Celda>(columnas));
        colocarMinas();
        calcularMinasAlrededor();
    }

    void descubrir(int fila, int columna) {
        if (finDelJuego || fila < 0 || columna < 0 || fila >= filas || columna >= columnas)
            return;
        Celda& celda = celdas[fila][columna];
        if (celda.getEstado() == Celda::Descubierta || celda.getEstado() == Celda::Marcada)
            return;
        celda.setEstado(Celda::Descubierta);
        if (celda.esMina()) {
            finDelJuego = true;
            victoria = false;
        } else if (celda.getMinasAlrededor() == 0) {
            descubrirRecursivo(fila, columna);
        }
        // Verificar victoria
        int descubiertas = 0;
        for (int i = 0; i < filas; ++i)
            for (int j = 0; j < columnas; ++j)
                if (!celdas[i][j].esMina() && celdas[i][j].getEstado() == Celda::Descubierta)
                    ++descubiertas;
        if (descubiertas == filas * columnas - minas) {
            finDelJuego = true;
            victoria = true;
        }
    }

    void marcar(int fila, int columna) {
        if (finDelJuego || fila < 0 || columna < 0 || fila >= filas || columna >= columnas)
            return;
        Celda& celda = celdas[fila][columna];
        if (celda.getEstado() == Celda::Descubierta)
            return;
        if (celda.getEstado() == Celda::Marcada)
            celda.setEstado(Celda::Oculta);
        else
            celda.setEstado(Celda::Marcada);
    }

    bool esFinDelJuego() const { return finDelJuego; }
    bool esVictoria() const { return victoria; }

    void dibujar(sf::RenderWindow& window, float size = 32.0f) const {
        sf::Font font;
        font.loadFromFile("assets/fonts/arial.ttf"); // Cambia la ruta si tienes otra fuente
        for (int i = 0; i < filas; ++i) {
            for (int j = 0; j < columnas; ++j) {
                sf::Vector2f pos(j * size, i * size);
                window.draw(celdas[i][j].getShape(size, pos));
                if (celdas[i][j].getEstado() == Celda::Descubierta) {
                    if (celdas[i][j].esMina()) {
                        sf::CircleShape minaShape(size / 4);
                        minaShape.setFillColor(sf::Color::Black);
                        minaShape.setPosition(pos.x + size / 4, pos.y + size / 4);
                        window.draw(minaShape);
                    } else if (celdas[i][j].getMinasAlrededor() > 0) {
                        sf::Text text;
                        text.setFont(font);
                        text.setString(std::to_string(celdas[i][j].getMinasAlrededor()));
                        text.setCharacterSize(static_cast<unsigned int>(size / 2));
                        text.setFillColor(sf::Color::Blue);
                        text.setPosition(pos.x + size / 4, pos.y + size / 8);
                        window.draw(text);
                    }
                }
            }
        }
    }

    int getFilas() const { return filas; }
    int getColumnas() const { return columnas; }
    int getMinas() const { return minas; }
    const Celda& getCelda(int fila, int columna) const { return celdas[fila][columna]; }

private:
    int filas, columnas, minas;
    std::vector<std::vector<Celda>> celdas;
    bool finDelJuego;
    bool victoria;

    void colocarMinas() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> disFila(0, filas - 1);
        std::uniform_int_distribution<> disCol(0, columnas - 1);
        int colocadas = 0;
        while (colocadas < minas) {
            int f = disFila(gen);
            int c = disCol(gen);
            if (!celdas[f][c].esMina()) {
                celdas[f][c].setMina(true);
                ++colocadas;
            }
        }
    }

    void calcularMinasAlrededor() {
        for (int i = 0; i < filas; ++i) {
            for (int j = 0; j < columnas; ++j) {
                if (celdas[i][j].esMina()) {
                    celdas[i][j].setMinasAlrededor(-1);
                    continue;
                }
                int count = 0;
                for (int di = -1; di <= 1; ++di) {
                    for (int dj = -1; dj <= 1; ++dj) {
                        int ni = i + di, nj = j + dj;
                        if (ni >= 0 && ni < filas && nj >= 0 && nj < columnas && celdas[ni][nj].esMina())
                            ++count;
                    }
                }
                celdas[i][j].setMinasAlrededor(count);
            }
        }
    }

    void descubrirRecursivo(int fila, int columna) {
        for (int di = -1; di <= 1; ++di) {
            for (int dj = -1; dj <= 1; ++dj) {
                int ni = fila + di, nj = columna + dj;
                if (ni >= 0 && ni < filas && nj >= 0 && nj < columnas) {
                    Celda& celda = celdas[ni][nj];
                    if (celda.getEstado() == Celda::Oculta && !celda.esMina()) {
                        celda.setEstado(Celda::Descubierta);
                        if (celda.getMinasAlrededor() == 0)
                            descubrirRecursivo(ni, nj);
                    }
                }
            }
        }
    }
}
};