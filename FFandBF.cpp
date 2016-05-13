#include<stdio.h>
#include<stdlib.h>
#define SIZE 500            /* �ڴ��ʼ��С */
#define MINSIZE 10            /* ��Ƭ����С��Χ */
enum STATE { Free, Busy };
//˫������ 
struct subAreaNode {
    int addr;               // ��ʼ��ַ
    int size;               // ������С
    int id;             // ��ҵ��
    STATE state;            // ����״̬
    subAreaNode *pre;       // ����ǰ��ָ��
    subAreaNode *nxt;       // ��������ָ��
}subHead;

/* ��ʼ�����з����� */
void intSubArea()
{
    /* �����ʼ�����ڴ� */
    subAreaNode *fir = (subAreaNode *)malloc(sizeof(subAreaNode));
    /* ���׸�������ֵ */
    fir->addr   = 0;
    fir->size   = SIZE;   //500kb
    fir->state  = Free;
    fir->id = -1;
    fir->pre    = &subHead;
    fir->nxt    = NULL;
    /* ��ʼ������ͷ����Ϣ */
    subHead.pre = NULL;
    subHead.nxt = fir;
}

// �״���Ӧ�㷨
int firstFit(int id, int size)
{
    subAreaNode *p = subHead.nxt;
    while(p != NULL)
    {
        if(p->state == Free && p->size >= size) {
            // �ҵ�Ҫ����Ŀ��з���
            if(p->size - size <= MINSIZE) {
                // ������䣨С����Ƭ��С���ȣ� 
                p->state = Busy;
                p->id = id;
            } else {
                // �����СΪsize������ p ��㣬ʣ��ռ�Ϊnode��� 
                subAreaNode *node = (subAreaNode *)malloc(sizeof(subAreaNode));
                node->addr = p->addr + size;
                node->size = p->size - size;
                node->state = Free;
                node->id = -1;
                // �޸ķ������ڵ�ָ��
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
            printf("�ڴ����ɹ���\n");
            return 1;
        }
        p = p->nxt;
    }
    printf("�Ҳ������ʵ��ڴ����������ʧ��...\n");
    return 0;
}

// �����Ӧ�㷨
int bestFit(int id, int size)
{
    subAreaNode *tar = NULL;
    int tarSize = SIZE + 1;
    subAreaNode *p = subHead.nxt;
    while(p != NULL)
    {
        // Ѱ����ѿ������� 
        if(p->state == Free && p->size >= size && p->size < tarSize) {
            tar = p;
            tarSize = p->size;
        }
        p = p->nxt;
    }
    if(tar != NULL) {
        // �ҵ�Ҫ����Ŀ��з���
        if(tar->size - size <= MINSIZE) {
                // �������
                tar->state = Busy;
                tar->id = id;
            } else {
                // �����СΪsize������
                subAreaNode *node = (subAreaNode *)malloc(sizeof(subAreaNode));
                node->addr = tar->addr + size;
                node->size = tar->size - size;
                node->state = Free;
                node->id = -1;
                // �޸ķ������ڵ�ָ��
                node->pre = tar;
                node->nxt = tar->nxt;
                if(tar->nxt != NULL) {
                    tar->nxt->pre  = node;
                }
                tar->nxt = node;
                // �����������
                tar->size = size;
                tar->state = Busy;
                tar->id = id;
            }
            printf("�ڴ����ɹ���\n");
            return 1;
    } else {
        // �Ҳ������ʵĿ��з���
        printf("�Ҳ������ʵ��ڴ����������ʧ��...\n");
        return 0;
    }
}

// �����ڴ�
int freeSubArea(int id)
{
    int flag = 0;
    subAreaNode *p = subHead.nxt, *pp;
    while(p != NULL)
    {
    	/* ����Busy״̬�Ľ��̲��ܱ����� */
        if(p->state == Busy && p->id == id) {
            flag = 1;
            /** 
			 * ���ǵ�һ��������ǰһ����������Free״̬ 
			 * �Լ���һ���������ڴ���Free״̬ 
			 */ 
            if((p->pre != &subHead && p->pre->state == Free) 
                && (p->nxt != NULL && p->nxt->state == Free)) {
                // ���1���ϲ�������������
                // �Ⱥϲ�������
                pp = p;
                p = p->pre;
                p->size += pp->size;
                p->nxt = pp->nxt;
                pp->nxt->pre = p;
                free(pp);
                // ��ϲ�������
                pp = p->nxt;
                p->size += pp->size;
                p->nxt = pp->nxt;
                if(pp->nxt != NULL) {
                    pp->nxt->pre = p;
                }
                free(pp);
            } else if((p->pre == &subHead || p->pre->state == Busy)
                && (p->nxt != NULL && p->nxt->state == Free)) {
                /* ���2��ֻ�ϲ�����ķ��� */ 
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
                /* ���3��ֻ�ϲ�����ķ��� */
                pp = p;
                p = p->pre;
                p->size += pp->size;
                p->nxt = pp->nxt;
                if(pp->nxt != NULL) {
                    pp->nxt->pre = p;
                }
                free(pp);
            } else {
                // ���4�����·��������úϲ�
                p->state = Free;
                p->id = -1;
            }
        } 
        p = p->nxt;
    }
    if(flag == 1) {
        // ���ճɹ�
        printf("�ڴ�������ճɹ�...\n");
        return 1;
    } else {
        // �Ҳ���Ŀ����ҵ������ʧ��
        printf("�Ҳ���Ŀ����ҵ���ڴ��������ʧ��...\n");
        return 0;
    }
}

// ��ʾ���з��������
void showSubArea()
{
    printf("*********************************************\n");
    printf("**         ��ǰ���ڴ����������£�        **\n");
    printf("*********************************************\n");
    printf("** ��ʼ��ַ | �ռ��С | ����״̬ | ��ҵ�� **\n");
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
    	printf("***********************ʵ��5***********************\n\n"); 
        printf("��ѡ������㷨��0 �״���Ӧ�㷨��1 �����Ӧ�㷨  :  ");
        scanf("%d", &option);
        if(option == 0) {
            printf("\n��������״���Ӧ�㷨��ģ��\n");
            break;
        } else if(option == 1) {
            printf("\n������������Ӧ�㷨��ģ��\n");
            break;
        } else {
            printf("���������� 0/1\n\n");
        }
    }
    while(1)
    {
        printf("\n");
        printf("*********************************************\n");
        printf("  1: �����ڴ�    2: �����ڴ�    0: �˳�  \n");
        printf("*********************************************\n");
        scanf("%d", &ope);
        if(ope == 0) break;
        if(ope == 1) {
            // ģ������ڴ�
            printf("��������ҵ�ţ� ");
            scanf("%d", &id);
            printf("��������Ҫ������ڴ��С(KB)�� ");
            scanf("%d", &size);
            if(size <= 0) {
                printf("���󣺷����ڴ��С����Ϊ��ֵ\n");
                continue;
            }
            // ���÷����㷨
            if(option == 0) {
                firstFit(id, size);
            } else {
                bestFit(id, size);
            }
            // ��ʾ���з��������
            showSubArea();
        } else if(ope == 2) {
            // ģ������ڴ�
            printf("������Ҫ���յ���ҵ�ţ� ");
            scanf("%d", &id);
            freeSubArea(id);
            // ��ʾ���з��������
            showSubArea();
        } else {
            printf("���������� 0/1/2\n");
        }
    }
    printf("ʵ�����\n");
    return 0;
}
