#include<stdio.h>
#include<malloc.h>

typedef int SLDataType;

typedef struct SeqList
{
    SLDataType* array; // 指向动态开辟的数组 
    int size; // 有效数据个数 
    int capicity; // 容量空间的大小 
}SeqList;

void SeqListInit(SeqList* psl, int capacity)
{
    psl->array = (SLDataType*)malloc(sizeof(SeqList)*capacity);
    psl->capicity = capacity;
    psl->size = 0;
    printf("初始化成功!\n");
}

void SeqListDestory(SeqList* psl)
{
    psl->size = 0;
    psl->capicity = 0;
    free(psl->array);
    psl = NULL;
    printf("销毁成功！");
}

void CheckCapacity(SeqList* psl)
{
    int x = psl->capicity;
    if (psl->size == psl->capicity)
    {
        psl->array = (SLDataType*)realloc(psl->array,sizeof(SeqList)*(x * 2));
        psl->capicity = (x * 2);
        printf("增容成功！\n");
    }
}

void SeqListPushBack(SeqList* psl, SLDataType x)
{
    SeqListInsert(psl, psl->size+1 , x);
}

void SeqListPopBack(SeqList* psl)
{
    SeqListErase(psl, psl->size);
}

int SeqListFind(SeqList* psl, SLDataType x)
{
    int z = 0;
    while (z < psl->size)
    {
        if (psl->array[z] == x)
        {
            return z;
        }
        z++;
    }
    return -1;
}

void SeqListPushFront(SeqList* psl, SLDataType x)
{
    int z = 0;
    int t = 0;
    int c = psl->array[0];
    if ((psl->size )== psl->capicity)
    {
        CheckCapacity(psl);
    }
    for (t = 0; t < psl->size; t++)
    {
        z = psl->array[t+1];
        psl->array[0] = x;
        psl->array[t + 1] = c;
        c = z;
    }
    (psl->size)++;
}
