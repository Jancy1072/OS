#include<stdio.h>
#include<stdlib.h>
#define SIZE 500            /* 内存初始大小 */
#define MINSIZE 10            /* 碎片的最小范围 */
enum STATE { Free, Busy };
//双向链表 
struct subAreaNode {
    int addr;               // 起始地址
    int size;               // 分区大小
    int id;             // 作业号
    STATE state;            // 分区状态
    subAreaNode *pre;       // 分区前向指针
    subAreaNode *nxt;       // 分区后向指针
}subHead;

/* 初始化空闲分区链 */
void intSubArea()
{
    /* 分配初始分区内存 */
    subAreaNode *fir = (subAreaNode *)malloc(sizeof(subAreaNode));
    /* 给首个分区赋值 */
    fir->addr   = 0;
    fir->size   = SIZE;   //500kb
    fir->state  = Free;
    fir->id = -1;
    fir->pre    = &subHead;
    fir->nxt    = NULL;
    /* 初始化分区头部信息 */
    subHead.pre = NULL;
    subHead.nxt = fir;
}

// 首次适应算法
int firstFit(int id, int size)
{
    subAreaNode *p = subHead.nxt;
    while(p != NULL)
    {
        if(p->state == Free && p->size >= size) {
            // 找到要分配的空闲分区
            if(p->size - size <= MINSIZE) {
                // 整块分配（小于碎片最小长度） 
                p->state = Busy;
                p->id = id;
            } else {
                // 分配大小为size的区间 p 结点，剩余空间为node结点 
                subAreaNode *node = (subAreaNode *)malloc(sizeof(subAreaNode));
                node->addr = p->addr + size;
                node->size = p->size - size;
                node->state = Free;
                node->id = -1;
                // 修改分区链节点指针
                node->pre = p;
                node->nxt = p->nxt;
                if(p->nxt != NULL) {
                    p->nxt->pre  = node;
                }
                p->nxt = node;
                p->size = size;
                p->state = Busy;
                p->id = id;
            }
            printf("内存分配成功！\n");
            return 1;
        }
        p = p->nxt;
    }
    printf("找不到合适的内存分区，分配失败...\n");
    return 0;
}

// 最佳适应算法
int bestFit(int id, int size)
{
    subAreaNode *tar = NULL;
    int tarSize = SIZE + 1;
    subAreaNode *p = subHead.nxt;
    while(p != NULL)
    {
        // 寻找最佳空闲区间 
        if(p->state == Free && p->size >= size && p->size < tarSize) {
            tar = p;
            tarSize = p->size;
        }
        p = p->nxt;
    }
    if(tar != NULL) {
        // 找到要分配的空闲分区
        if(tar->size - size <= MINSIZE) {
                // 整块分配
                tar->state = Busy;
                tar->id = id;
            } else {
                // 分配大小为size的区间
                subAreaNode *node = (subAreaNode *)malloc(sizeof(subAreaNode));
                node->addr = tar->addr + size;
                node->size = tar->size - size;
                node->state = Free;
                node->id = -1;
                // 修改分区链节点指针
                node->pre = tar;
                node->nxt = tar->nxt;
                if(tar->nxt != NULL) {
                    tar->nxt->pre  = node;
                }
                tar->nxt = node;
                // 分配空闲区间
                tar->size = size;
                tar->state = Busy;
                tar->id = id;
            }
            printf("内存分配成功！\n");
            return 1;
    } else {
        // 找不到合适的空闲分区
        printf("找不到合适的内存分区，分配失败...\n");
        return 0;
    }
}

// 回收内存
int freeSubArea(int id)
{
    int flag = 0;
    subAreaNode *p = subHead.nxt, *pp;
    while(p != NULL)
    {
    	/* 处于Busy状态的进程才能被回收 */
        if(p->state == Busy && p->id == id) {
            flag = 1;
            /** 
			 * 不是第一个分区且前一个分区处于Free状态 
			 * 以及后一个分区处于处于Free状态 
			 */ 
            if((p->pre != &subHead && p->pre->state == Free) 
                && (p->nxt != NULL && p->nxt->state == Free)) {
                // 情况1：合并上下两个分区
                // 先合并上区间
                pp = p;
                p = p->pre;
                p->size += pp->size;
                p->nxt = pp->nxt;
                pp->nxt->pre = p;
                free(pp);
                // 后合并下区间
                pp = p->nxt;
                p->size += pp->size;
                p->nxt = pp->nxt;
                if(pp->nxt != NULL) {
                    pp->nxt->pre = p;
                }
                free(pp);
            } else if((p->pre == &subHead || p->pre->state == Busy)
                && (p->nxt != NULL && p->nxt->state == Free)) {
                /* 情况2：只合并下面的分区 */ 
                pp = p->nxt;
                p->size += pp->size;
                p->state = Free;
                p->id = -1;
                p->nxt = pp->nxt;
                if(pp->nxt != NULL) {
                    pp->nxt->pre = p;
                }
                free(pp);
            } else if((p->pre != &subHead && p->pre->state == Free)
                && (p->nxt == NULL || p->nxt->state == Busy)) {
                /* 情况3：只合并上面的分区 */
                pp = p;
                p = p->pre;
                p->size += pp->size;
                p->nxt = pp->nxt;
                if(pp->nxt != NULL) {
                    pp->nxt->pre = p;
                }
                free(pp);
            } else {
                // 情况4：上下分区均不用合并
                p->state = Free;
                p->id = -1;
            }
        } 
        p = p->nxt;
    }
    if(flag == 1) {
        // 回收成功
        printf("内存分区回收成功...\n");
        return 1;
    } else {
        // 找不到目标作业，回收失败
        printf("找不到目标作业，内存分区回收失败...\n");
        return 0;
    }
}

// 显示空闲分区链情况
void showSubArea()
{
    printf("*********************************************\n");
    printf("**         当前的内存分配情况如下：        **\n");
    printf("*********************************************\n");
    printf("** 起始地址 | 空间大小 | 工作状态 | 作业号 **\n");
    subAreaNode *p = subHead.nxt;
    while(p != NULL)
    {
        printf("**-----------------------------------------**\n");
        printf("**");
        printf("%5d  k  |", p->addr);
        printf("%5d  k  |", p->size);
        printf("   %4s   |", p->state == Free ? "Free" : "Busy");
        if(p->id > 0) {
            printf("%4d    ", p->id);
        } else {
            printf("        ");
        }
        printf("**\n"); 
        p = p->nxt;
    }
    printf("*********************************************\n");
}

int main()
{
    int option, ope, id, size;
    intSubArea();
    while(1)
    {
    	printf("***********************实验5***********************\n\n"); 
        printf("请选择分配算法：0 首次适应算法，1 最佳适应算法  :  ");
        scanf("%d", &option);
        if(option == 0) {
            printf("\n下面进行首次适应算法的模拟\n");
            break;
        } else if(option == 1) {
            printf("\n下面进行最佳适应算法的模拟\n");
            break;
        } else {
            printf("错误：请输入 0/1\n\n");
        }
    }
    while(1)
    {
        printf("\n");
        printf("*********************************************\n");
        printf("  1: 分配内存    2: 回收内存    0: 退出  \n");
        printf("*********************************************\n");
        scanf("%d", &ope);
        if(ope == 0) break;
        if(ope == 1) {
            // 模拟分配内存
            printf("请输入作业号： ");
            scanf("%d", &id);
            printf("请输入需要分配的内存大小(KB)： ");
            scanf("%d", &size);
            if(size <= 0) {
                printf("错误：分配内存大小必须为正值\n");
                continue;
            }
            // 调用分配算法
            if(option == 0) {
                firstFit(id, size);
            } else {
                bestFit(id, size);
            }
            // 显示空闲分区链情况
            showSubArea();
        } else if(ope == 2) {
            // 模拟回收内存
            printf("请输入要回收的作业号： ");
            scanf("%d", &id);
            freeSubArea(id);
            // 显示空闲分区链情况
            showSubArea();
        } else {
            printf("错误：请输入 0/1/2\n");
        }
    }
    printf("实验结束\n");
    return 0;
}
