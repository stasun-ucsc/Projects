// cacher.h
// Helper functions for Assignment 5, cacher.c

#pragma once

typedef struct CacheObj *Cache;

Cache newCache(int size);

void freeCache(Cache *pC);

int insert_fifo(Cache c, char *elem);

int insert_lru(Cache c, char *elem);

int insert_clock(Cache c, char *elem);
