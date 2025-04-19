#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value) {
/*
    if (map->size + 1 > map->capacity * 0.70){
     enlarge(map);
     //al parecer no es necesario en este caso como en la actividad en clases
}
*/
    long posicion = hash(key, map->capacity); //posicion sera el resultado de la funcion hash, lo que nos dara donde se ubica en el arreglo
    while (map->buckets[posicion] != NULL && map->buckets[posicion]->key!= NULL){ //mientras que la casilla sea distinta de NULL o que no este vacia
        if (is_equal(map->buckets[posicion]->key, key)) { //si ya existe la clave, no hagas nada
            map->buckets[posicion]->value = value;
            return;
        }
        posicion = (posicion + 1) % map->capacity; //aumenta la posicion para seguir avanzando
    }
    Pair * par =createPair(key, value); //se crean 
    map->size++;
    map->buckets[posicion] = par;
}

void enlarge(HashMap * map) {
    enlarge_called = 1; //no borrar (testing purposes)

    long newCapacity = map->capacity * 2; //duplicamos la capacidad
    Pair ** newBuckets = (Pair **) calloc(newCapacity, sizeof(Pair *)); //calloc para que sea NULL

    for (long i = 0; i < map->capacity; i++) { //recorremos 
        Pair * current = map->buckets[i]; //pasamos del viejo a un aux nueva o similar
        if (current != NULL && current->key != NULL) { //si esta libre la casilla
            long index = hash(current->key, newCapacity); //da la ubicacioon

            while (newBuckets[index] != NULL) { 
                index = (index + 1) % newCapacity;
            }

            newBuckets[index] = createPair(current->key, current->value);
        }
    }

    free(map->buckets); 
    map->buckets = newBuckets; //actualizamos
    map->capacity = newCapacity; //actualizamos
}


HashMap * createMap(long capacity) {
    HashMap * map; //se crea el mapa
    
    map = (HashMap *) calloc(1, sizeof(HashMap));     //se consigue espacio para el mapa inicializando en NULL
    
    map->capacity = capacity;  //asisgnamos valores
    map->size = 0; //asisgnamos valores
    map->current = -1; //asisgnamos valores

    map->buckets = (Pair **) calloc(capacity, sizeof(Pair *)); //creamos para bucket

    return map;
}

void eraseMap(HashMap * map,  char * key) {    
    Pair * borrar = searchMap (map, key); //buscamos el que hay que borrar y lo guardamos
    
    if (borrar == NULL){ //si no hay nada para borrar, termina
        return;
    }
    
    borrar->key = NULL; //convierte la key en NULL para que no exista
    borrar->value = NULL; //convierte el value en NULL para que no exista

    map->size--; //achica en 1 
}

Pair * searchMap(HashMap * map,  char * key) {   
    long posicion = hash(key, map->capacity); //obtenemos la posicion en la que deberia estar
    
    while (map->buckets[posicion] != NULL && map->buckets[posicion]->key!= NULL){ //mientras que la cassilal sea valida
        if (is_equal(map->buckets[posicion]->key, key) == 1){ //si los datos concuerdan
            map->current = posicion; //actualizamos el current al dato
            return map->buckets[posicion]; //devolvemos la posicion
        }else{
            posicion = (posicion + 1) % map->capacity; //si no es igual, ve al siguiente
        }
    }
    return NULL;
}

Pair * firstMap(HashMap * map) {
    long posicion = 0; //parte en 0 por logica

    while (posicion < map->capacity){ //recorremos
        Pair * primero = map->buckets[posicion]; // ponemos lo que hay en la casilla 0

        if (map->buckets[posicion] != NULL && map->buckets[posicion]->key != NULL){ //si hay algo valido en la casilla actual
            map->current = posicion; //actualizamos posicion
            return primero; //devolvemos primero

        }else{
            posicion++; //avanzamos a la siguiente
        }
    }
    return NULL; //no encontro nada
}

Pair * nextMap(HashMap * map) {
    long posicion = map->current + 1; //ponemos el siguiente del actual

    while (posicion < map->capacity){ //recorremos
        Pair * par = map->buckets[posicion]; //ponemos lo que hay en la casilla actual

        if (map->buckets[posicion] != NULL && map->buckets[posicion]->key != NULL){ //si hay algo valido en la casilal actual
            map->current = posicion; //actualizamos posicion
            return par; //devolvemos el par que es el next
        }else{
            posicion++; //avanzamos a la siguiente
        }
    }
    return NULL; //no hay next
}
