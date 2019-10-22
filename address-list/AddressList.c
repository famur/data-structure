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

void SearchContact( contact*pcon)
{
    char name[MAX] = { 0 };
    printf("请输入姓名：>");
    scanf("%s", name);
    int t=Search(pcon, name);
    if (t == -1)
    {
        printf("输入有误查无此人\n");
    }
    else
    {
        printf("查找成功\n");
        printf("%-10s\t%-5s\t%-5s\t%-12s\t%-20s\n", "姓名", "电话号码", "分类", "电子邮件地址", "条目编号");
        printf("%-10s\t%-12s\t%-5d\t%-12s\t%-5d\n",
                pcon->data[t].name,
                pcon->data[t].tele,
                pcon->data[t].type,
                pcon->data[t].email,
                pcon->data[t].num);
    }
}

void ModifyContact(contact*pcon)
{
    char name[MAX] = { 0 };
    printf("请输入姓名：>");
    scanf("%s", name);
    int t = Search(pcon, name);
    if (t == -1)
    {
        printf("输入有误查无法修改\n");
    }
    else
    {
        printf("请重新输入姓名：>");
        scanf("%s", pcon->data[t].name);
        printf("请重新输入电话号码：>");
        scanf("%s", pcon->data[t].tele);
        printf("请重新输入分类：>");
        scanf("%d", &pcon->data[t].type);
        printf("请重新输入电子邮件地址：>");
        scanf("%s", pcon->data[t].email);
        printf("请重新输入条目编号：>");
        scanf("%d", &pcon->data[t].num);
    }
    printf("修改成功\n");
}

void EmptyContact(contact*pcon)
{
    pcon->size = 0;
    printf("清空成功\n");
}

void SortContact(contact*pcon)
{
    int i = 0;
    for (i = 0; i < pcon->size; i++)
    {
        int t = 0;
        for (t = 1; t < pcon->size; t++)
        {
            int ret=strcmp(pcon->data[i].name, pcon->data[t].name);
            if (ret < 0)
            {
                char tmp[NAME_MAX];
                strcpy(tmp, pcon->data[i].name);
                strcpy(pcon->data[i].name, pcon->data[t].name);
                strcpy(pcon->data[t].name, tmp);
            }
        }
    }
    printf("排序成功\n");
    ShowContact(pcon);
}

void Capticty(contact*pcon)
{
    int x = pcon->capticty;
    assert(pcon != NULL);
    if (pcon->size == pcon->capticty)
    {
        pcon->data = (peolnfo*)realloc(pcon->data, sizeof(peolnfo)*((x)* 2));
    }
    pcon->capticty = x * 2;
    printf("增容成功！\n");
}

void Destroy(contact*pcon)
{
    assert(pcon != NULL);
    pcon->size = 0;
    pcon->capticty = 0;
    free(pcon->data);
    pcon = NULL;
}

void Save(contact*pcon)
{
    contact *p;
    FILE*fp;
    p = pcon;
    fp = fopen("C:\\Desktop\\bookslist.txt", "w");
    fprintf(fp,"%-10s\t%-5s\t%-5s\t%-8s\t%-8s\n", "姓名", "电话号码", "分类", "电子邮件地址", "条目编号");
    for (int i = 0; i < pcon->size; i++)
    {
        fprintf(fp,"%-10s\t%-5s\t%-5d\t%-12s\t%-20d\t\n",
                pcon->data[i].name,
                pcon->data[i].tele,
                pcon->data[i].type,
                pcon->data[i].email,
                pcon->data[i].num);
    }
    fclose(fp);
    printf("保存成功！\n");
}

void menu()
{
    printf("***************************************\n");
    printf("**********1.手机用户添加        2.手机用户删除***********\n");
    printf("**********3.手机用户查找        4.手机用户修改********\n");
    printf("**********5.手机用户查看        6.手机用户清空*********\n");
    printf("**********7.手机用户排序        8.手机用户保存**********\n");
    printf("**********0.退出程序                          **********\n");
    printf("***************************************\n");
}

void text()
{
    int input=0;
    contact con;
    InitContact(&con);
    do
    {
        menu();
        printf("请输入选择:>");
        scanf("%d", &input);
        switch (input)
        {
            case ADD:
                AddContact(&con);
                break;
            case DEL:
                DelContact(&con);
                break;
            case SEARCH:
                SearchContact(&con);
                break;
            case MODIFY:
                ModifyContact(&con);
                break;
            case SHOW:
                ShowContact(&con);
                break;
            case EMPTY:
                EmptyContact(&con);
                break;
            case SORT:
                SortContact(&con);
                break;
            case EXIT:
                Destroy(&con);
                break;
            case 8:
                Save(&con);
                printf("手机用户保存");
                break;
            default:
                printf("选择错误\n");
                break;
        }
    }while (input);
}

int main()
{
    text();
    return 0;
}
