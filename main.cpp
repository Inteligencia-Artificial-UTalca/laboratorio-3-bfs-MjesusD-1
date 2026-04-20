#include "Map.h"
#include "Search.h"
#include "ColorMap.h"
#include <cassert>
#include <iostream>
#include <fstream>
#include <chrono>


float pathCost(const Map& map, const std::vector<std::pair<int,int>>& path){
    float cost = 0.0f;

    for(size_t i = 1; i < path.size(); i++){
        cost += Search::Cost(map, path[i-1], path[i]);
    }

    return cost;
}

template<typename Func>
void benchmarkCSV(
    std::ofstream& file,
    const std::string& mapName,
    const std::string& algoName,
    Func f,
    const Map& map,
    std::pair<int,int> start,
    std::pair<int,int> goal,
    int iterations)
{
    for(int i = 0; i < iterations; i++){

        auto t1 = std::chrono::high_resolution_clock::now();
        auto path = f(map, start, goal);
        auto t2 = std::chrono::high_resolution_clock::now();

        double timeMs =
            std::chrono::duration<double, std::milli>(t2 - t1).count();

        float cost = path.empty() ? -1.0f : pathCost(map, path);

        file << mapName << ","
             << algoName << ","
             << i << ","
             << timeMs << ","
             << cost << "\n";
    }
}

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
    //Map map(argv[1], BINARY);
    // o
    Map map(argv[1], HEIGHT);

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

    //ColorMap colorMap(map);
    //colorMap.print();

      //Calculate path distance
      //Print path distance
   
    /* std::vector<std::pair<int,int>> path;

    // BFS
    path = Search::BFS(map, {x1, y1}, {x2, y2});
    if (!path.empty()) {
        std::cout << "BFS cost: " << pathCost(map,path) << std::endl;
        colorMap.print(path);
    } else {
        std::cout << "BFS: No se encontró camino\n";
    }

    // Greedy
    path = Search::Greedy(map, {x1, y1}, {x2, y2});
    if (!path.empty()) {
        std::cout << "Greedy cost: " << pathCost(map,path) << std::endl;
        colorMap.print(path);
    } else {
        std::cout << "Greedy: No se encontró camino\n";
    }

    // A*
    path = Search::AStar(map, {x1, y1}, {x2, y2});
    if (!path.empty()) {
        std::cout << "A* cost: " << pathCost(map,path) << std::endl;
        colorMap.print(path);
    } else {
        std::cout << "A*: No se encontró camino\n";
    }

    // Weighted A*
    path = Search::AStarWeighted(map, {x1, y1}, {x2, y2}, 2.0f);
    if (!path.empty()) {
        std::cout << "WA* cost (w=2): " << pathCost(map,path) << std::endl;
        colorMap.print(path);
    } else {
        std::cout << "WA*: No se encontró camino\n";
    }*/

    std::pair<int,int> start = {x1, y1};
    std::pair<int,int> goal  = {x2, y2};

    int iterations = 20;

    std::ofstream file("results.csv");
    file << "map,algorithm,iteration,time_ms,cost\n";

    benchmarkCSV(file, argv[1], "BFS",
        Search::BFS, map, start, goal, iterations);

    benchmarkCSV(file, argv[1], "Greedy",
        Search::Greedy, map, start, goal, iterations);

    benchmarkCSV(file, argv[1], "AStar",
        Search::AStar, map, start, goal, iterations);

    benchmarkCSV(file, argv[1], "WAStar",
        [&](const Map& m, auto s, auto g){
            return Search::AStarWeighted(m, s, g, 2.0f);
        },
        map, start, goal, iterations);

    std::cout << "Benchmark completado. Archivo: results.csv\n";
    

    return 0;
}