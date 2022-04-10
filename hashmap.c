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
  long index = hash(key, map->capacity);
  while (map->buckets[index] != NULL && map->buckets[index]->key != NULL)
  {
    if (is_equal(map->buckets[index]->key,key) == 1 ) return;
    index = (index + 1) % map->capacity;
  }
  if (map->buckets[index] != NULL)
  {
    map->buckets[index]->key = key;
    map->buckets[index]->value = value;
  }
  else
    map->buckets[index] = createPair(key,value);
  map->size++;
  if ((double) map->size/(double) map->capacity > 0.75) enlarge(map);

}

void enlarge(HashMap * map) {
    enlarge_called = 1; //no borrar (testing purposes)
    Pair ** aux = (Pair**) calloc(map->capacity, sizeof(Pair*));
    for (long i = map->current ; i < map->capacity ; i++)
    {
      if (map->buckets[i] != NULL && map->buckets[i]->key != NULL)
      {
        strcpy(aux[i]->key,map->buckets[i]->key);
        strcpy(aux[i]->value,map->buckets[i]->value);
      }
    }
    map->capacity = (map->capacity * 2);
    

}


HashMap * createMap(long capacity) 
{
  
    HashMap *index2 = (HashMap *)malloc(sizeof(HashMap)) ;

    index2->capacity = capacity;
    index2->size = 0;
    index2->current = -1;
  
    index2->buckets = (Pair **)calloc(capacity,sizeof(Pair*)) ; 
    
  
    return index2;
}

void eraseMap(HashMap * map,  char * key) 
{    
  long index3 = hash(key, map->capacity);
  while (map->buckets[index3] != NULL && map->buckets[index3]->key != NULL)
  {
    if (is_equal(map->buckets[index3]->key,key) == 1 )
    {
      map->current = index3;
      map->buckets[index3]->key = NULL;
      map->size = map->size -1;
    }
    index3 = (index3 + 1) % map->capacity;
  }
  //index3 = (index3 + 1) % map->capacity;


}

Pair * searchMap(HashMap * map,  char * key) {   
  long index3 = hash(key, map->capacity);
  while (map->buckets[index3] != NULL && map->buckets[index3]->key != NULL)
  {
    if (is_equal(map->buckets[index3]->key,key) == 1 )
    {
      map->current = index3;
      return map->buckets[index3];
    }
    index3 = (index3 + 1) % map->capacity;
  }

  return NULL;
}

Pair * firstMap(HashMap * map) 
{  
  for (int i = 0 ; i < map->size ; i++)
    {
      if (map->buckets[i] != NULL)
      {
        map->current = i;
        if (map->buckets[i]->key != NULL)
        {
          return map->buckets[i];
        }
        //return map->buckets[i];
      }
      
    }

    return NULL;
}

Pair * nextMap(HashMap * map) 
{
   for ( long i = map->current + 1 ; i < map->capacity ; i++)
   {
     if (map->buckets[i] != NULL && map->buckets[i]->key != NULL)
     {
       map->current = i;
       return map->buckets[i];   
       
     }
   }
    return NULL;
}
