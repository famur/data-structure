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

void SeqListPopFront(SeqList* psl)
{
    int t = 0;
    for (t = 0; t < psl->size; t++)
    {
        psl->array[t] = psl->array[t+1];
    }
    (psl->size)--;
}

void SeqListInsert(SeqList* psl, int pos, SLDataType x)
{
    int z = 0;
    int t = 0;
    if ((psl->size ) == psl->capicity)
    {
        CheckCapacity(psl);
    }
    for (t = psl->size; t >=pos-1; t--)
    {
        z = psl->array[t];
        psl->array[pos - 1] = x;
        psl->array[t+1] =z;
    }
    (psl->size)++;
}

void SeqListErase(SeqList* psl, int pos)
{
    int z = 0;
    int t = 0;
    if (psl->size == 0)
    {
        printf("无法删除！\n");
    }
    for (t = pos; t<psl->size; t++)
    {
        z = psl->array[t];
        psl->array[t] = psl->array[t + 1];
        psl->array[t + 1] = z;
    }
    (psl->size)--;
}

void SeqListRemove(SeqList* psl, SLDataType x)
{
    int t = SeqListFind(psl, x);
    if (t+1==psl->size)
    {
        printf("无法移动！");
    }
    else
    {
        int y = 0;
        int z = 0;
        int q = t + 1;//移动次数
        for (y = t; y <q; y++)
        {
            z = psl->array[y];
            psl->array[y] = psl->array[y + 1];
            psl->array[y + 1] = z;
        }
    }
}

void SeqListModify(SeqList* psl, int pos, SLDataType x)
{
    psl->array[pos - 1] = x;
}

int SeqListLength(SeqList* psl)
{
    return psl->size;
}

void SeqListPrint(SeqList* psl)
{
    int i = 0;
    for (i = 0; i <psl->size; i++)
    {
        printf("%d ", psl->array[i]);
    }
    printf("\n");
}

int main()
{
    SeqList arr;
    SeqListInit(&arr, 3);
    SeqListPushBack(&arr, 7);
    SeqListPushBack(&arr, 8);
    SeqListPushBack(&arr, 9);
    SeqListPushBack(&arr, 1);
    SeqListPopBack(&arr);
    SeqListPrint(&arr);
    SeqListRemove(&arr, 1);
    SeqListPopFront(&arr);
    SeqListPushFront(&arr, 2);
    int t = SeqListFind(&arr, 1);
    printf("位置为：%d\n", t);
    SeqListModify(&arr, 2, 6);
    SeqListPrint(&arr);
    SeqListInsert(&arr, 1, 5);
    SeqListPrint(&arr);
    int z = SeqListLength(&arr);
    printf("个数为：%d\n",z);
    SeqListDestory(&arr);
    return 0;
}
