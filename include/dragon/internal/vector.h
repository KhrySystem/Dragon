#pragma once

#ifndef DRAGON_VECTOR_INIT_CAPACITY
#define DRAGON_VECTOR_INIT_CAPACITY 0
#endif

#define VECTOR_INIT(vec) DgVector vec; dgInitVector(&vec)
#define VECTOR_ADD(vec, item) dgVectorAppend(&vec, (void *) item)
#define VECTOR_SET(vec, id, item) dgVectorSet(&vec, id, (void *) item)
#define VECTOR_GET(vec, type, id) (type) dgVectorGet(&vec, id)
#define VECTOR_DELETE(vec, id) dgVectorRemove(&vec, id)
#define VECTOR_TOTAL(vec) dgSizeOfVector(&vec)
#define VECTOR_EMPTY(vec) VECTOR_TOTAL(vec) == 0
#define VECTOR_FREE(vec) dgVectorFree(&vec)

typedef struct DgVector {
    void **items;
    int capacity;
    int total;
} DgVector;

void dgInitVector(DgVector*);
int dgSizeOfVector(DgVector*);
static void dgVectorResize(DgVector*, int);
void dgVectorAppend(DgVector*, void *);
void dgVectorSet(DgVector*, int, void *);
void *dgVectorGet(DgVector*, int);
void dgVectorRemove(DgVector*, int);
void dgVectorFree(DgVector*);
