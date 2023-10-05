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
    Connection connections[5];
};

void addConnection(City* src_city, uint32_t dist, City* dest_city)
{
    Connection* connections = src_city->connections;
    while(connections)
        connections++;
    connections->distance = dist;
    connections->connected_city = dest_city;
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

    #define getCity(a) getCity(a, cities, citiesLen)
    #define addConnection(a, b, c) addConnection(a, b, c);addConnection(b, a, c);

    addConnection(getCity("Arad"),      getCity("Zerind"), 75);
    addConnection(getCity("Arad"),      getCity("Sibiu"), 140);
    addConnection(getCity("Arad"),      getCity("Timisoara"), 118);
    addConnection(getCity("Zerinda"),   getCity("Oradea"), 71);
    addConnection(getCity("Oradea"),    getCity("Sibiu"), 151);
    addConnection(getCity("Timisoara"), getCity("Lugoj"), 111);
    addConnection(getCity("Lugoj"),     getCity("Mehadia"), 70);
    addConnection(getCity("Mehadia"),   getCity("Drobeta"), 75);
    addConnection(getCity("Drobeta"),   getCity("Craiova"), 120);
    addConnection(getCity("Craiova"),   getCity("Rimnicu Vilcea"), 146);
    addConnection(getCity("Craiova"),   getCity("Pitesti"), 138);
    addConnection(getCity("Sibiu"),     getCity("Fagaras"), 99);
    addConnection(getCity("Sibiu"),     getCity("Rimnicu Vilcea"), 80);
    addConnection(getCity("Pitesti"),   getCity("Rimnicu Vilcea"), 97);
    addConnection(getCity("Fagras"),    getCity("Bucharest"), 211);
    addConnection(getCity("Pitesti"),   getCity("Bucharest"), 101);
    addConnection(getCity("Giurgiu"),   getCity("Bucharest"), 90);
    addConnection(getCity("Bucharest"), getCity("Urziceni"), 85);
    addConnection(getCity("Urziceni"),  getCity("Hirsova"), 98);
    addConnection(getCity("Urziceni"),  getCity("Vaslui"), 142);
    addConnection(getCity("Hirsova"),   getCity("Eforie"), 86);
    addConnection(getCity("Vaslui"),    getCity("Iasi"), 92);
    addConnection(getCity("Iasi"),      getCity("Neamt"), 87);

    for(int i = 0; i < citiesLen; i++)
    {
        printf("%s %d\n", cities[i]->name, cities[i]->straight_distance);
    }
}