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

int SeqListBinaryFind(SeqList* s)
{
    int x = 0;
    printf("请输入查找的数>");
    scanf("%d", &x);
    int begin = 0, end = s->_size - 1;
    while (begin <= end)
    {
        int mid = (begin + end) / 2;
        if (s->_array[mid] < x)
        {
            begin = mid + 1;
        }
        else if (s->_array[mid] > x)
        {
            end = mid - 1;
        }
        else
        {
            printf("找到了：%d\n", s->_array[mid]);
            return mid;
        }
    }
    printf("该数字不存在！\n");
    return -1;
}

void SeqListDestory(SeqList* s)
{
    if (s->_array)
    {
        free(s->_array);
        s->_array = NULL;
        s->_capacity = s->_size = 0;
    }
}

void SeqListSort(SeqList* s)
{
    if (s->_size == 0)
    {
        printf("无法排序!\n");
    }
    else
    {
        int i = 0;
        for (i = 0; i < s->_size-1; i++)
        {
            int x = 0;
            if (s->_array[i]>s->_array[i + 1])
            {
                x = s->_array[i];
                s->_array[i] = s->_array[i + 1];
                s->_array[i + 1] = x;
            }
        }
    }
    printf("排序成功！\n");
}

void menu()
{
    printf("**********************************\n");
    printf("***     1.头插      2.头删     ***\n");
    printf("***     3.尾插      4.尾删     ***\n");
    printf("***     5.展示      6.排序     ***\n");
    printf("***     7.查找      0.退出     ***\n");
    printf("**********************************\n");
}

int main()
{
    SeqList s;
    int n = 0;
    SeqListInit(&s, 10);
    do
    {
        menu();
        printf("请输入选择>");
        scanf("%d", &n);
        switch (n)
        {
            case 1:
                SeqListPushFront(&s);
                break;
            case 2:
                SeqListPopFront(&s);
                break;
            case 3:
                SeqListPushBack(&s);
                break;
            case 4:
                SeqListPopBack(&s);
                break;
            case 5:
                SeqListPrint(&s);
                break;
            case 6:
                SeqListSort(&s);
                break;
            case 7:
                SeqListBinaryFind(&s);
                break;
            case 0:
                SeqListDestory(&s);
                break;
            default:
                printf("输入有误重新输入\n");
                break;
        }
    } while (n);
    return 0;
}
