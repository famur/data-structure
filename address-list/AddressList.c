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

