#ifndef BK_SAFE_H
#define BK_SAFE_H

#define NEW(T) (T*) SafeMalloc(sizeof(T))

void* SafeMalloc(size_t size);
void* SafeRealloc(void* ptr, size_t size);

#endif
