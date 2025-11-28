
#include <SFML/Graphics.hpp>
#include "../include/Tablero.hpp"


int FILAS = 10;
int COLUMNAS = 10;
int MINAS = 15;
float CELDA_SIZE = 32.0f;


int main()
{
    sf::RenderWindow window(sf::VideoMode(COLUMNAS * CELDA_SIZE, FILAS * CELDA_SIZE), "Buscaminas");
    Tablero tablero(FILAS, COLUMNAS, MINAS);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Verificar si se ha cerrado la ventana
            if (event.type == sf::Event::Closed)
                window.close();
            // Manejo de mouse para descubrir o marcar celdas
            if (event.type == sf::Event::MouseButtonPressed) {
                int x = event.mouseButton.x / CELDA_SIZE;
                int y = event.mouseButton.y / CELDA_SIZE;
                if (x >= 0 && x < COLUMNAS && y >= 0 && y < FILAS) {
                    if (event.mouseButton.button == sf::Mouse::Left)
                        tablero.descubrir(y, x);
                    else if (event.mouseButton.button == sf::Mouse::Right)
                        tablero.marcar(y, x);
                }
            }
        }

        // Limpiar la ventana
        window.clear();
        // Dibujar el tablero
        tablero.dibujar(window, CELDA_SIZE);

        // Mostrar la ventana
        window.display();
    }

    return 0;
}