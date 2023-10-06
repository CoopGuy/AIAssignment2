#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#pragma region /* A generic singly linked node implementation */
// 
typedef struct node Node;
typedef struct node{
    void *data;
    Node *next;
} Node;

Node* createNode(void* data)
{
    Node* node = (Node*)calloc(1, sizeof(Node));
    node->data = data;
    return node;
}
#pragma endregion

#pragma region /* Simple queue implementation */
typedef struct queue{
    Node* head;
    Node* tail;
} Queue;

void enqueue(Queue* queue, Node* node)
{
    if(!queue->tail)
    {
        queue->head = node;
        queue->tail = node;
    }
    else
    {
        queue->tail->next = node;
        queue->tail = node;
    } 
}

Node* dequeue(Queue* queue)
{
    if(queue->head == NULL)return NULL;

    Node *temp = queue->head;

    queue->head = queue->head->next;
    if(queue->head == NULL)queue->tail = NULL;
    
    return temp;
}

bool inqueue(Queue* queue, void* data)
{
    Node* activeNode = queue->head;
    while(activeNode)
    {
        if(activeNode->data == data)return true;
        activeNode = activeNode->next;
    }
    return false;
}
#pragma endregion

#pragma region /* Simple stack implementation */
typedef struct stack{
    Node* head;
    uint32_t size;
} Stack;

void push(Stack* stack, Node* node)
{
    if(stack->head == NULL)
    {
        stack->head = node;
    }
    else
    {
        node->next = stack->head;
        stack->head = node;
    }
    stack->size++;
}

Node* pop(Stack* stack)
{
    if(stack->head == NULL)
        return NULL;
    else
    {
        Node* temp = stack->head;
        stack->head = temp->next;
        stack->size--;
        return temp;
    }
}
#pragma endregion

#pragma region /* City abstraction structs/implementation */
typedef struct connection Connection;
typedef struct city City;

// a struct representing a connection
struct connection{
    uint16_t distance; // the length of the connection
    City* connected_city; // the city the connection is connecting to
};

/// @brief a struct to store data about a city
struct city
{
    char* name; // the name of the city
    uint16_t straight_distance; // the straight distance to bucharest
    union {
        bool visited;
        City* addedBy;
    } visited; 
    // storage that indicates whether the city has been visited
    // and can also store what city added it to the path

    Connection* connections[5]; // a list of the connections to the city
};


/// @brief adds a connection to a city
/// @param src_city one of two cities that should be connected
/// @param dest_city one of two cities that should be connected
/// @param dist the distance of the connection
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

/// @brief an initializer function for a city struct
/// @param name the name of the city
/// @param dist the straight distance to Bucharest
/// @return a city struct with the given data
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

/// @brief a function to search an array for a city of a specific name
/// @param name name of the target city 
/// @param city_list a list of cities
/// @param list_len the length of the list of cities
/// @return a pointer to the city
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

#pragma endregion

#pragma region /* Generic utility functions/abstractions */
/// @brief iterates through an array of pointers and finds the index of the first null pointer
/// @param arr the array you want to find the length of
/// @return the length of the array
uint32_t nullTermArrLen(void *arr[])
{
    uint32_t count = 0;
    for(int i = 0; true; i++)
    {
        if((void*)arr[i] == NULL)return i;
    }
}

/// @brief Since the search functions modify the original cities
///        this resets those changes
/// @param cities an array of cities
/// @param arr_size the size of the cities arr
void resetCities(City* cities[], uint32_t arr_size)
{
    for(int i = 0; i < arr_size; i++)
    {
        cities[i]->visited.visited = false;
    }
}
#pragma endregion

#pragma region /* Search algorithm implementations*/
/// @brief walks cities by their 'addedBy' property to create
///        a list of cities representing a path from start to finish
/// @param end a pointer the the last city in a path
/// @return an array of City* in order of pathing from start to finish
City** walkBack(City* start, City* end)
{
    
    Stack stack = {0};
    push(&stack, createNode(end));

    // loop until you traced your steps back to the start of the path
    while(end != start)
    {
        // push each visited city onto a stack in reverse order they were visited
        push(&stack, createNode(end->visited.addedBy));
        
        // walk backwards along the path to trace it
        end = end->visited.addedBy;
    }
    
    // allocate memory for a list of cities
    City** path = (City**)calloc(stack.size + 1, sizeof(City*));
    if(!path)
    {
        perror("unable to calloc in walkback");
        exit(0);
    }

    // pop cities off the stack to get them in the correct order
    // and add them to the list as they get popped
    for(int i = 0; stack.size > 0; i++)
    {
        path[i] = (City*)pop(&stack)->data;
    }
    
    return path;
}

/// @brief Breadth first search of a graph
/// @param start a pointer to the city you wish to start at
/// @param end a pointer to the city you wish to end at
/// @return a list of cities in the order of the path found
City** breadthFirst(City* start, City* end)
{
    // Set up a queue structure for the search process
    Queue queue = {NULL, NULL};
    Node* currentNode = NULL;

    // enque the start city
    enqueue(&queue, createNode((void*)start));

    // loop until there are no more cities in the queue
    // (which will only happen if the target cant be found)
    while(currentNode = dequeue(&queue))
    {
        City* currentCity = (City*)currentNode->data;

        // if we have dequeued the target city from the queue
        // we have reached our destination and should walk back
        // through the queueing process to trace our path to the
        // destination
        if(currentCity == end) 
            return walkBack(start, currentCity);

        // we get the number of connections in the current 
        // cities connections array
        uint32_t connections_arr_len = \
            nullTermArrLen((void**)(currentCity)->connections);

        // loop over each connection and queue the connected city
        for(int i = 0; i < connections_arr_len; i++)
        {
            City* connected_city = \
                currentCity->connections[i]->connected_city;
            
            // if the city to be added is the starting city 
            // or has already been visited skip it
            if(connected_city->visited.addedBy != NULL || connected_city == start)continue;

            // set the city as "visited" by storing a reference to the city that added it
            connected_city->visited.addedBy = currentCity;

            // queue the connected city
            enqueue(&queue, createNode(connected_city));
        }
        // free the node we no longer need it once its been dequeued
        free(currentNode);
    }

    return NULL;
}

/// @brief Depth first search of a graph
/// @param start a pointer to the city you wish to start at
/// @param end a pointer to the city you wish to end at
/// @return a list of cities in the order of the path found
City** depthFirst(City* start, City* end)
{
    Stack stack = {NULL, 0};
    Node* currentNode = NULL;

    push(&stack, createNode((void*)start));

    // loop until there are no more cities in the stack
    // (which will only happen if the target cant be found)
    while(currentNode = pop(&stack))
    {
        City* currentCity = (City*)currentNode->data;

        // if we have dequeued the target city from the queue
        // we have reached our destination and should walk back
        // through the queueing process to trace our path to the
        // destination
        if(currentCity == end) 
            return walkBack(start, currentCity);

        // we get the number of connections in the current 
        // cities connections array
        uint32_t connections_arr_len = \
            nullTermArrLen((void**)(currentCity)->connections);

        // loop over each connection and queue the connected city
        for(int i = 0; i < connections_arr_len; i++)
        {
            City* connected_city = \
                currentCity->connections[i]->connected_city;
            
            // if the city to be added is the starting city 
            // or has already been visited skip it
            if(connected_city->visited.addedBy != NULL || connected_city == start)continue;

            // set the city as "visited" by storing a reference to the city that added it
            connected_city->visited.addedBy = currentCity;

            // queue the connected city
            push(&stack, createNode(connected_city));
        }
        // free the node we no longer need it once its been dequeued
        free(currentNode);
    }

    return NULL;
}

//TODO: Implement
City* AStar(City* start, City* end)
{
    return NULL;
}
#pragma endregion

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
    
    addConnection2Way(getCityFromList("Arad"),      getCityFromList("Sibiu"), 140);
    addConnection2Way(getCityFromList("Arad"),      getCityFromList("Zerind"), 75);
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

    //#undef getCityFromList
    #undef addConnection2Way
    #pragma endregion 

    City** buf = depthFirst(getCityFromList("Arad"), getCityFromList("Bucharest"));

    for(int i = 0; i < nullTermArrLen((void**)buf); i++)
    {
        printf("%s %d\n", buf[i]->name, buf[i]->straight_distance);
    }
}