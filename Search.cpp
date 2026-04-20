#include "Search.h"
#include <queue>
#include <chrono>
#include <unordered_map>
#include <iostream>
#include <limits>
#include <cmath>


namespace std
{
    //this is needed to store a pair<int,int> in an associative container
    //such as unorered_set and unordered_map
	template<> struct hash<std::pair<int,int>>
	{
		std::size_t operator()( const std::pair<int,int> & p) const noexcept
		{
            hash<int> hasher;
			return hasher(p.first) ^ (hasher(p.second)<<1);

		}
	};
}


std::vector<std::pair<int,int>> Search::reconstruct(const std::unordered_map<std::pair<int,int>,std::pair<int,int>> &pathCache, const std::pair<int,int> &start){
	std::deque<std::pair<int,int>> nodes;
	auto node = start;//make copy

    //traverse path from goal to start

    while (true) {
    nodes.push_front(node);

    auto it = pathCache.find(node);
    if (it == pathCache.end()) break;

    node = it->second;
}

    //revert path and return it
    std::vector<std::pair<int,int>> vec;
    for(auto p:nodes){
        vec.push_back(p);
    }
    return vec;
}

std::vector<std::pair<int,int>> Search::BFS(const Map& map, std::pair<int,int> start, std::pair<int,int> goal){
    std::cout<<"===========================\nRunning BFS...\n";
	auto startTime = std::chrono::high_resolution_clock::now();

    //stores possible directions
    std::pair<int,int> dirs[]{{-1,0},{0,1},{1,0},{0,-1}};

    std::vector<std::vector<bool>> visited(map.h, std::vector<bool>(map.w, false));      //we'll just use a matrix og booleans to indicated if visited
    std::queue<std::pair<int,int>> OPEN;
    std::unordered_map<std::pair<int,int>,std::pair<int,int>> pathCache;    ////hashmap to reconstruct path: child -> parent

    //add firts node to open list
    OPEN.push(start);
    visited[start.first][start.second] = true;

    while(!OPEN.empty()){
        //get node
        auto pos = OPEN.front();
        OPEN.pop();

        //check if node is goal
		if(pos==goal){
			auto endTime = std::chrono::high_resolution_clock::now();
			int count=0;
            for(int i=0;i<map.h;i++){
                for(int j=0;j<map.w;j++){
                    if(visited[i][j])count++;
                }
            }
            std::cout<<"VISITED: "<<count<<std::endl;
			std::cout<<"OPEN: "<<OPEN.size()<<std::endl;
			std::cout<<"FOUND in "<<(endTime-startTime).count()/1000000.0<<"ms\n";
			return reconstruct(pathCache,pos);
		}

		for(auto dir:dirs){
			//copy the position

            //then move it
            
            //if illegal or visited, skip it
            
            //add child to open list

            //register path

             int nx = pos.first + dir.first;
             int ny = pos.second + dir.second;

            // verificar límites
            if(nx < 0 || nx >= map.h || ny < 0 || ny >= map.w)
                continue;

            // verificar obstáculo
            if(map._map[nx][ny] == 1)
                continue;

            // verificar visitado
            if(visited[nx][ny])
                continue;

            // marcar visitado
            visited[nx][ny] = true;

            // agregar a cola
            OPEN.push({nx, ny});

            // guardar padre
            pathCache[{nx, ny}] = pos;
        }
	}
	std::cout<<"NOT FOUND!!!!\n";
    
    //let's just return start and goal to draw them
    std::vector<std::pair<int,int>> path;
    path.push_back(start);
    path.push_back(goal);
    return {};
}


float Search::Heuristic(std::pair<int,int> a, std::pair<int,int> b) {
    int dx = a.first - b.first;   // diferencia columnas
    int dy = a.second - b.second; // diferencia filas
    //return std::sqrt(dx*dx + dy*dy);  // Distancia euclidiana
    return abs(a.first - b.first) + abs(a.second - b.second); //Manhattan
}

std::vector<std::pair<int,int>> Search::Greedy(const Map& map,
    std::pair<int,int> start, std::pair<int,int> goal) {
    
    std::cout<<"===========================\nRunning Greedy Best-First...\n";
    auto startTime = std::chrono::high_resolution_clock::now();

    //stores possible directions
    std::pair<int,int> dirs[]{{-1,0},{0,1},{1,0},{0,-1}};

    std::vector<std::vector<bool>> visited(map.h, std::vector<bool>(map.w, false));
    std::unordered_map<std::pair<int,int>,std::pair<int,int>> pathCache;

    // Priority Queue 
    using pPair = std::pair<float, std::pair<int,int>>; // {heurística, posición}
    std::priority_queue<pPair, std::vector<pPair>, std::greater<pPair>> OPEN;

    //add first node to open list
    OPEN.push({Heuristic(start, goal), start});
    visited[start.first][start.second] = true;

    while(!OPEN.empty()){
        //get node (Menor heurística)
        auto curr = OPEN.top();
        auto pos = curr.second;
        OPEN.pop();

        //check if node is goal
        if(pos==goal){
            auto endTime = std::chrono::high_resolution_clock::now();
            int count=0;
            for(int i=0;i<map.h;i++){
                for(int j=0;j<map.w;j++){
                    if(visited[i][j])count++;
                }
            }
            std::cout<<"VISITED: "<<count<<std::endl;
            std::cout<<"OPEN: "<<OPEN.size()<<std::endl;
            std::cout<<"FOUND in "<<(endTime-startTime).count()/1000000.0<<"ms\n";
            return reconstruct(pathCache,pos);
        }

        for(auto dir:dirs){
            int nx = pos.first + dir.first;
            int ny = pos.second + dir.second;

            // verificar límites
            if(nx < 0 || nx >= map.h || ny < 0 || ny >= map.w)
                continue;

            // verificar obstáculo
            if(map._map[nx][ny] == 1)
                continue;

            // verificar visitado
            if(visited[nx][ny])
                continue;

            // marcar visitado
            visited[nx][ny] = true;

            // agregar a priority queue con heurística
            OPEN.push({Heuristic({nx, ny}, goal), {nx, ny}});

            // guardar padre
            pathCache[{nx, ny}] = pos;
        }
    }
    std::cout<<"NOT FOUND!!!!\n";
    
    //let's just return start and goal to draw them
    std::vector<std::pair<int,int>> path;
    path.push_back(start);
    path.push_back(goal);
    return {};
}

//A*
std::vector<std::pair<int,int>> Search::AStar(const Map& map,std::pair<int,int> start,std::pair<int,int> goal){
    
    std::cout<<"===========================\nRunning A*...\n";
    std::vector<std::vector<float>> gScore(
        map.h,
        std::vector<float>(map.w, std::numeric_limits<float>::infinity())
    );

    gScore[start.first][start.second] = 0;

    std::vector<std::vector<bool>> closed(
        map.h,
        std::vector<bool>(map.w, false)
    );

    using pPair = std::pair<float, std::pair<int,int>>;

    struct Compare {
        bool operator()(const pPair& a, const pPair& b) {
        return a.first > b.first;
        }
    };
    
    std::priority_queue<pPair, std::vector<pPair>, Compare> OPEN;

    // insertar nodo inicial
    OPEN.push({Heuristic(start, goal), start});

    std::unordered_map<std::pair<int,int>, std::pair<int,int>> pathCache;

    while(!OPEN.empty()){
        auto current = OPEN.top();
        auto pos = current.second;
        OPEN.pop();

        if(closed[pos.first][pos.second])
            continue;

        closed[pos.first][pos.second] = true;

        if(pos == goal){
        return reconstruct(pathCache, pos);
        }

        std::pair<int,int> dirs[]{{-1,0},{0,1},{1,0},{0,-1}};

        for(auto dir:dirs){
            int nx = pos.first + dir.first;
            int ny = pos.second + dir.second;

            if(nx < 0 || nx >= map.h || ny < 0 || ny >= map.w)
            continue;

            if(map._map[nx][ny] == 1)
            continue;

            if(closed[nx][ny])
            continue;

            float tentative_g = gScore[pos.first][pos.second] + 1.0f;

            if(tentative_g < gScore[nx][ny]){

                gScore[nx][ny] = tentative_g;

                pathCache[{nx, ny}] = pos;

                float f = tentative_g + Heuristic({nx, ny}, goal);
                OPEN.push({f, {nx, ny}});
            }
        }
    }   

    return {};
}


//Weighted A*

std::vector<std::pair<int,int>> Search::AStarWeighted(const Map& map,std::pair<int,int> start,std::pair<int,int> goal,float weight){
    std::cout<<"===========================\nRunning Weighted A* (w="<<weight<<")...\n";

    std::vector<std::vector<float>> gScore(
        map.h,
        std::vector<float>(map.w, std::numeric_limits<float>::infinity())
    );
    gScore[start.first][start.second] = 0;

    std::vector<std::vector<bool>> closed(
        map.h,
        std::vector<bool>(map.w, false)
    );

    using pPair = std::pair<float, std::pair<int,int>>;

    struct Compare {
        bool operator()(const pPair& a, const pPair& b) {
            return a.first > b.first; // min-heap por f
        }
    };

    std::priority_queue<pPair, std::vector<pPair>, Compare> OPEN;

    // f inicial con peso

    OPEN.push({weight * Heuristic(start, goal), start});

    std::unordered_map<std::pair<int,int>, std::pair<int,int>> pathCache;

    std::pair<int,int> dirs[]{{-1,0},{0,1},{1,0},{0,-1}};

    while(!OPEN.empty()){
        auto current = OPEN.top();
        auto pos = current.second;
        OPEN.pop();

        if(closed[pos.first][pos.second])
            continue;

        closed[pos.first][pos.second] = true;

        if(pos == goal){
            return reconstruct(pathCache, pos);
        }

        for(auto dir:dirs){
            int nx = pos.first + dir.first;
            int ny = pos.second + dir.second;

            if(nx < 0 || nx >= map.h || ny < 0 || ny >= map.w)
                continue;

            if(map._map[nx][ny] == 1)
                continue;

            if(closed[nx][ny])
                continue;

            float tentative_g = gScore[pos.first][pos.second] + 1.0f;

            if(tentative_g < gScore[nx][ny]){
                gScore[nx][ny] = tentative_g;

                pathCache[{nx, ny}] = pos;

                //peso en la heurística
                float f = tentative_g + weight * Heuristic({nx, ny}, goal);

                OPEN.push({f, {nx, ny}});
            }
        }
    }

    return {};
}

