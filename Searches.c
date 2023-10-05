#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct connection Connection;
typedef struct city City;

struct connection{
    uint16_t distance;
    City* connected_city;
};

struct city
{
    char* name;
    uint16_t straight_distance;
    bool visited;
    Connection* connections[5];
};

void addConnection(City* src_city, City* dest_city, uint32_t dist)
{
    Connection** connections = src_city->connections;
    while(*connections)
        connections++;
    *connections = (Connection*)calloc(1, sizeof(Connection));
    if(*connections == NULL)
    {
        printf("could not malloc connection for %s to %s", src_city->name, dest_city->name);
        exit(0);
    }
    (*connections)->distance = dist;
    (*connections)->connected_city = dest_city;
}

City* createCity(char* name, uint16_t dist)
{
    City* res = (City*)calloc(1, sizeof(City));
    if(res == NULL)
    {
        printf("Error allocating city\n");
        exit(0);
    }
    res->straight_distance = dist;
    res->name = name;
    return res;
}

City* getCity(char* name, City* city_list[], uint32_t list_len)
{
    for(int i = 0; i < list_len; i++)
    {
        if(strcmp(name, city_list[i]->name) == 0)
            return city_list[i];
    }
    printf("Could not find city %s\n", name);
    exit(1);
}

City* breadthFirst(City* start, City* end, uint32_t max_path_len)
{
    City** path = calloc(max_path_len, sizeof(City*));
    if(path == NULL)
    {
        perror("Could not alloc space for path in breadth first");
        exit(0);
    }
    path[0] = start;
}

City* depthFirst(City* start, City* end, uint32_t max_path_len)
{
    City** path = calloc(max_path_len, sizeof(City*));
    if(path == NULL)
    {
        perror("Could not alloc space for path in depth first");
        exit(0);
    }
    path[0] = start;
}

City* AStar(City* start, City* end, uint32_t max_path_len)
{
    City** path = calloc(max_path_len, sizeof(City*));
    if(path == NULL)
    {
        perror("Could not alloc space for path in astar");
        exit(0);
    }
    path[0] = start;
}

int main()
{
    const unsigned citiesLen = 20;
    City* cities[20] = {
        createCity("Arad", 366),
        createCity("Bucharest", 0),
        createCity("Craiova", 160),
        createCity("Drobeta", 242),
        createCity("Eforie", 161),
        createCity("Fagaras", 176),
        createCity("Giurgiu", 77),
        createCity("Hirsova", 151),
        createCity("Iasi", 226),
        createCity("Lugoj", 244),
        createCity("Mehadia", 241),
        createCity("Neamt", 234),
        createCity("Oradea", 380),
        createCity("Pitesti", 100),
        createCity("Rimnicu Vilcea", 193),
        createCity("Sibiu", 253),
        createCity("Timisoara", 329),
        createCity("Urziceni", 80),
        createCity("Vaslui", 199),
        createCity("Zerind", 374)
    };

    #pragma region /* Setting up connections */
    #define getCityFromList(a) getCity(a, cities, citiesLen)
    #define addConnection2Way(a, b, c) addConnection(a, b, c);addConnection(b, a, c);
    
    addConnection2Way(getCityFromList("Arad"),      getCityFromList("Zerind"), 75);
    addConnection2Way(getCityFromList("Arad"),      getCityFromList("Sibiu"), 140);
    addConnection2Way(getCityFromList("Arad"),      getCityFromList("Timisoara"), 118);
    addConnection2Way(getCityFromList("Zerind"),   getCityFromList("Oradea"), 71);
    addConnection2Way(getCityFromList("Oradea"),    getCityFromList("Sibiu"), 151);
    addConnection2Way(getCityFromList("Timisoara"), getCityFromList("Lugoj"), 111);
    addConnection2Way(getCityFromList("Lugoj"),     getCityFromList("Mehadia"), 70);
    addConnection2Way(getCityFromList("Mehadia"),   getCityFromList("Drobeta"), 75);
    addConnection2Way(getCityFromList("Drobeta"),   getCityFromList("Craiova"), 120);
    addConnection2Way(getCityFromList("Craiova"),   getCityFromList("Rimnicu Vilcea"), 146);
    addConnection2Way(getCityFromList("Craiova"),   getCityFromList("Pitesti"), 138);
    addConnection2Way(getCityFromList("Sibiu"),     getCityFromList("Fagaras"), 99);
    addConnection2Way(getCityFromList("Sibiu"),     getCityFromList("Rimnicu Vilcea"), 80);
    addConnection2Way(getCityFromList("Pitesti"),   getCityFromList("Rimnicu Vilcea"), 97);
    addConnection2Way(getCityFromList("Fagaras"),    getCityFromList("Bucharest"), 211);
    addConnection2Way(getCityFromList("Pitesti"),   getCityFromList("Bucharest"), 101);
    addConnection2Way(getCityFromList("Giurgiu"),   getCityFromList("Bucharest"), 90);
    addConnection2Way(getCityFromList("Bucharest"), getCityFromList("Urziceni"), 85);
    addConnection2Way(getCityFromList("Urziceni"),  getCityFromList("Hirsova"), 98);
    addConnection2Way(getCityFromList("Urziceni"),  getCityFromList("Vaslui"), 142);
    addConnection2Way(getCityFromList("Hirsova"),   getCityFromList("Eforie"), 86);
    addConnection2Way(getCityFromList("Vaslui"),    getCityFromList("Iasi"), 92);
    addConnection2Way(getCityFromList("Iasi"),      getCityFromList("Neamt"), 87);

    #undef getCityFromList
    #undef addConnection2Way
    #pragma endregion 

    for(int i = 0; i < citiesLen; i++)
    {
        printf("%s %d\n", cities[i]->name, cities[i]->straight_distance);
    }
}