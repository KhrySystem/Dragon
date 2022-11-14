#include <dragon/dragon.h>

void dgInitVector(DgVector* v)
{
    v->capacity = DRAGON_VECTOR_INIT_CAPACITY;
    v->total = 0;
    v->items = malloc(sizeof(void *) * v->capacity);
}

int dgSizeOfVector(DgVector *v)
{
    return v->total;
}

static void dgVectorResize(DgVector *v, int capacity)
{
    void **items = realloc(v->items, sizeof(void *) * capacity);
    if (items) {
        v->items = items;
        v->capacity = capacity;
    }
}

void dgVectorAppend(DgVector *v, void *item)
{
    if (v->capacity == v->total)
        dgVectorResize(v, v->capacity * 2);
    v->items[v->total++] = item;
}

void dgVectorSet(DgVector *v, int index, void *item)
{
    if (index >= 0 && index < v->total)
        v->items[index] = item;
}

void *dgVectorGet(DgVector *v, int index)
{
    if (index >= 0 && index < v->total)
        return v->items[index];
    return NULL;
}

void dgVectorRemove(DgVector *v, int index)
{
    if (index < 0 || index >= v->total)
        return;

    v->items[index] = NULL;

    for (int i = index; i < v->total - 1; i++) {
        v->items[i] = v->items[i + 1];
        v->items[i + 1] = NULL;
    }

    v->total--;

    if (v->total > 0 && v->total == v->capacity / 4)
        dgVectorResize(v, v->capacity / 2);
}

void dgVectorFree(DgVector *v)
{
    free(v->items);
}