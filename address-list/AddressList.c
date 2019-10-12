#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<malloc.h>
#include<assert.h>

enum Option
{
    EXIT,
    ADD,
    DEL,
    SEARCH,
    MODIFY,
    SHOW,
    EMPTY,
    SORT
};

#define MAX 1000
#define NAME_MAX 20
#define SEX_MAX 5
#define TELE_MAX 12
#define ADDR_MAX 20

typedef struct peolnfo
{
    char name[NAME_MAX];
    char tele[TELE_MAX];
    int type;
    char email[ADDR_MAX];
    int num;
}peolnfo;

typedef struct contact
{
    peolnfo*data;
    int size ;
    int capticty;
}contact;

void InitContact(contact*pcon)
{
    pcon->data = (peolnfo*)malloc(sizeof(peolnfo)* 1);
    pcon->size = 0;
    pcon->capticty = 1;
    printf("初始化成功！\n");
}

void AddContact(contact*pcon)
{
    assert(pcon != NULL);
    if (pcon->size == pcon->capticty)
    {
        Capticty(pcon);
    }
    printf("请输入姓名：>");
    scanf("%s", pcon->data[pcon->size].name);
    printf("请输入电话号码：>");
    scanf("%s", pcon->data[pcon->size].tele);
    printf("请输入分类：>");
    scanf("%d", &pcon->data[pcon->size].type);
    printf("请输入电子邮件地址：>");
    scanf("%s", pcon->data[pcon->size].email);
    printf("请输入条目编号：>");
    scanf("%d", &pcon->data[pcon->size].num);
    pcon->size++;
    printf("添加成功\n");
}

void ShowContact( contact*pcon)
{
    int i = 0;
    printf("%-10s\t%-5s\t%-5s\t%-12s\t%-20s\n", "姓名", "电话号码", "分类", "电子邮件地址", "条目编号");
    for (i = 0; i < pcon->size; i++)
    {
        printf("%-10s\t%-12s\t%-5d\t%-12s\t%-5d\n",
                pcon->data[i].name,
                pcon->data[i].tele,
                pcon->data[i].type,
                pcon->data[i].email,
                pcon->data[i].num);
    }
}

int Search(contact*pcon, char name[])
{
    int i = 0;
    for (i = 0; i < pcon->size; i++)
    {
        if (strcmp(pcon->data[i].name,name)==0)
        {
            return i;
        }
    }
    return -1;
}

void DelContact(contact*pcon)
{
    if (pcon->size == 0)
    {
        printf("通讯录已空无法删除\n");
    }
    else
    {
        char name[MAX] = { 0 };
        printf("请输入姓名：>");
        scanf("%s", name);
        int t = Search(pcon, name);
        if (t == -1)
        {
            printf("输入姓名有误无法删除\n");
        }
        else
        {
            int i = 0;
            {
                for (i = t; i < pcon->size - 1; i++)
                {
                    pcon->data[i] = pcon->data[i + 1];
                }
                pcon->size--;
                printf("删除成功\n");
            }
        }
    }
    
}
