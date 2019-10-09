#include<stdio.h>
#include<malloc.h>

typedef int SLDataType;

typedef struct SeqList
{
    SLDataType* _array;
    int _size;
    int _capacity;
}SeqList;

void SeqListInit(SeqList* s, int capacity)
{
    s->_array = (SLDataType*)malloc(sizeof(SLDataType)*capacity);
    s->_size = 0;
    s->_capacity = capacity;
}

void SeqListPushFront(SeqList* s)
{
    int x = 0;
    printf("请输入插入数据>");
    scanf("%d", &x);
    int end = s->_size - 1;
    while (end >= 0)
    {
        s->_array[end + 1] = s->_array[end];
        --end;
    }
    s->_array[0] = x;
    s->_size++;
}

void SeqListPrint(SeqList* s)
{
    int i = 0;
    if (s->_size == 0)
    {
        printf("链表为空！\n");
    }
    else
    {
        for (i = 0; i < s->_size; i++)
        {
            printf(" %d ", s->_array[i]);
        }
        printf("\n");
    }
}

void SeqListPopFront(SeqList* s)
{
    int begin = 0;
    while (begin < s->_size - 1)
    {
        s->_array[begin] = s->_array[begin + 1];
        ++begin;
    }
    s->_size--;
}

void SeqListPushBack(SeqList* s)
{
    int p = s->_size;
    printf("请输入插入数字>");
    int y= 0;
    scanf("%d", &y);
    s->_array[s->_size++] = y;
}

void SeqListPopBack(SeqList*s)
{
    s->_size--;
}
