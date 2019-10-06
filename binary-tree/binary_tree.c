#include<stdio.h>
#include<malloc.h>

#define MAXSIZE 100

typedef char BTdata;

//二叉树的二叉链表数据类型的定义
typedef struct BinaryTree
{

    BTdata data;
    struct BinaryTree*left;
    struct BinaryTree*right;
}BTnode;

BTnode* NewBTnode(BTdata a)
{
    BTnode* node = (BTnode*)malloc(sizeof(BTnode));
    node->data = a;
    node->left = NULL;
    node->right = NULL;
    return node;
}

//以先序序列创建二叉树
BTnode* CreatTree(BTdata* a, int* p)
{
    if(a[*p] != '#')
    {
        BTnode *root = NewBTnode(a[*p]);
        ++(*p);
        root->left = CreatTree(a, p);
        ++(*p);
        root->right = CreatTree(a, p);
        return root;
    }
    else
    {
        return NULL;
    }
}

//叶子结点数
int BitreeLeaf(BTnode* root)
{
    if(root == NULL)
    {
        return 0;
    }
    if(root->left == NULL && root->right == NULL)
    {
        return 1;
    }
    return (BitreeLeaf(root->left) + BitreeLeaf(root->right));
}

int main()
{
    BTdata a[] = "ABD##E#H##CF##G##";
    int i = 0;
    BTnode* tree = CreatTree(a, &i);
    printf("叶子结点数:%d\n", BitreeLeaf(tree));
    return 0;
}
