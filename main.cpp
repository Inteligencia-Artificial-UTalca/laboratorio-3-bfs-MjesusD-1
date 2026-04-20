#include "Map.h"
#include "Search.h"
#include "ColorMap.h"
#include <cassert>
#include <iostream>

int main(int argc, char *argv[]){

    //Verify that the amount of arguments is correct
    //6 arguments: program name, map file, x1, y1, x2, y2
    if (argc != 6) {
        std::cerr << "Uso: " << argv[0] << " <mapa> x1 y1 x2 y2\n";
        return 1;
    }

    int x1, y1, x2, y2;

    try {
        x1 = std::stoi(argv[2]);
        y1 = std::stoi(argv[3]);
        x2 = std::stoi(argv[4]);
        y2 = std::stoi(argv[5]);
    } catch (...) {
        std::cerr << "Error: coordenadas deben ser números\n";
        return 1;
    }

    //Load map with class Map
    Map map(argv[1]);

    //Verificación de las coordenadas
    int width = map.width();
    int height = map.height();

    // Verificar que estén dentro de los límites
    if (x1 < 0 || x1 >= height || y1 < 0 || y1 >= width ||
    x2 < 0 || x2 >= height || y2 < 0 || y2 >= width) {
    std::cerr << "Error: coordenadas fuera de los límites del mapa\n";
    std::cerr << "Mapa: " << width << "x" << height << std::endl;
    std::cerr << "Límites válidos: x=[0," << width-1 << "], y=[0," << height-1 << "]\n";
    return 1;

}

    // Verificar posiciones iguales
    if (x1 == x2 && y1 == y2) {
        std::cout << "Distancia: 0\n";
        return 0;
    }

    ColorMap colorMap(map);
    colorMap.print();

      //Calculate path distance
      //Print path distance
    
    //auto path = Search::BFS(map, {x1, y1}, {x2, y2});

    //auto path = Search::Greedy(map, {x1, y1}, {x2, y2});

    //auto path = Search::AStar(map, {x1, y1}, {x2, y2});

    auto path = Search::AStarWeighted(map, {x1, y1}, {x2, y2}, 2.0f);

    if (path.empty()) {
        std::cout << "No se encontró camino\n";
    } else {
        colorMap.print(path);
        std::cout << "Distancia: " << path.size() - 1 << std::endl;
    }

    return 0;
}