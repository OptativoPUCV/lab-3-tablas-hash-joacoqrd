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

    long newCapacity = map->capacity * 2;
    Pair ** newBuckets = (Pair **) calloc(newCapacity, sizeof(Pair *));

    for (long i = 0; i < map->capacity; i++) {
        Pair * current = map->buckets[i];
        if (current != NULL && current->key != NULL) {
            long index = hash(current->key, newCapacity);

            while (newBuckets[index] != NULL) {
                index = (index + 1) % newCapacity;
            }

            newBuckets[index] = createPair(current->key, current->value);
        }
    }

    free(map->buckets); 
    map->buckets = newBuckets;
    map->capacity = newCapacity;
}


HashMap * createMap(long capacity) {
    HashMap * map;
    
    map = (HashMap *) calloc(1, sizeof(HashMap));    
    
    map->capacity = capacity;
    map->size = 0;
    map->current = -1;

    map->buckets = (Pair **) calloc(capacity, sizeof(Pair *));

    return map;
}

void eraseMap(HashMap * map,  char * key) {    
    Pair * borrar = searchMap (map, key);
    
    if (borrar == NULL){
        return;
    }
    
    borrar->key = NULL;
    borrar->value = NULL;

    map->size--;
}

Pair * searchMap(HashMap * map,  char * key) {   
    long posicion = hash(key, map->capacity);
    
    while (map->buckets[posicion] != NULL && map->buckets[posicion]->key!= NULL){
        if (is_equal(map->buckets[posicion]->key, key) == 1){
            map->current = posicion;
            return map->buckets[posicion];
        }else{
            posicion = (posicion + 1) % map->capacity;
        }
    }
    return NULL;
}

Pair * firstMap(HashMap * map) {
    long posicion = 0;

    while (map->buckets[posicion] != NULL && map->buckets[posicion]->key!= NULL){
        Pair * primero = map->buckets[posicion];

        if (primero != NULL){
            return primero;        
        }else{
            posicion = (posicion + 1) % map->capacity;
        }
    }
    return NULL;
}

Pair * nextMap(HashMap * map) {

    return NULL;
}
