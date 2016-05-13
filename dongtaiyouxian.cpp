#include<stdio.h>
/* ������ */ 
#define N 5

enum STATE{Ready,Run,RunOut};//ö������ 

/* ���̿��ƿ�ṹ */ 
struct PROCESS
{
  int ID; 		    //���̱�ʶ��
  int PRIORITY;     //����������
  int CPUTIME;      //������ռ�õ�CPUʱ��
  int ALLTIME;      //���̻���ռ�õ�CPUʱ��
  enum STATE State; //״̬
}Process[N];

/* ������������ */ 
int READY[N];
//����������� 
int RUNOUT[N][2];//1��num��2��time ������˳���ģ���һ�������Ƚ����� 

/**
 * ��ʼ����������
 */
void init()
{
  int i;
  for(i=0;i<N;++i)
  {
    READY[i]=-1;
    RUNOUT[i][0]=-1;
    RUNOUT[i][1]=-1;
    Process[i].ID=i;
    Process[i].CPUTIME=0;
    Process[i].State=Ready;
  }
  Process[0].PRIORITY=9;
  Process[0].ALLTIME=3;
  Process[1].PRIORITY=38;
  Process[1].ALLTIME=2;
  Process[2].PRIORITY=2;
  Process[2].ALLTIME=6;
  Process[3].PRIORITY=17;
  Process[3].ALLTIME=3;
  Process[4].PRIORITY=7;
  Process[4].ALLTIME=1;
} 


/**
 * ��ӡ��ǰʱ��Ƭ�ڵĽ������
 */
void print()
{
  int i;
  
//����˳���ӡ�������� 
  printf("\tReady_Queue: ");
  for(i=0;i<N;++i)
  {
    if(READY[i]>=0) //������Ľ���ֵΪ-1 ��-1�������ڵĺ��� 
		printf(" -> %d",Process[READY[i]].ID);
    else
		break;
  }

  printf("\n****************************************************\n");

  /* ��ӡPCB���ƿ���Ϣ */ 
  printf("ID\t");
  for(i=0;i<N;++i)
  {
    printf("\t%d",Process[i].ID);
  }
  printf("\nPRIORITY");
  for(i=0;i<N;++i)
  {
    printf("\t%d",Process[i].PRIORITY);
  }
  printf("\nCPUTIME\t");
  for(i=0;i<N;i++)
  {
    printf("\t%d",Process[i].CPUTIME);
  }
  printf("\nALLTIME\t");
  for(i=0;i<N;++i)
  {
    printf("\t%d",Process[i].ALLTIME);
  }
  printf("\nSTATE\t");
  for(i=0;i<N;i++)
  {
    switch(Process[i].State)
    {
      case 0:printf("\tReady");
 			 break;
      case 1:printf("\tRun");
             if(Process[i].ALLTIME!=0)
			 	Process[i].State=Ready;//һ��ʱ��Ƭ��ɺ�ı�״̬run 
             break;
      case 2:printf("\tRunOut");
	  		 break;
    }
  }
  printf("\n\tRUNOUT LIST:");
  for(i=0;i<N;i++)
  {
    if(RUNOUT[i][0]>=0)
    {
      printf(" -> %d(%d)",Process[RUNOUT[i][0]].ID,RUNOUT[i][1]);//��i����ɵĽ��̣��ڵڼ���ʱ��Ƭ��ɵ� 
    }else{
      printf("\n");
      break;
    } 
  }
  printf("\n");
}


/**
 * �������ȼ����������������
 */
void sort()
{
  int i,j,k;
  /* ��ʼ�������������� */
  for(i=0;i<N;i++)
  {
  	READY[i]=-1;
  }
  /* �������н��� */ 
  for(i=0;i<N;i++)
  {
    if(Process[i].State==Ready||Process[i].State==Run)
    {
      Process[i].State=Ready;
      /* ��RunOut�ĳ����ھ��������� */

 	   
      for(j=0;j<N;j++)
 	  {
        if(READY[j]<0)
		{
		  /* ��һ��״̬ΪReady����Run�Ľ��� */ 
          READY[j]=i;
          break;
        }else if(Process[i].PRIORITY<=Process[READY[j]].PRIORITY){
          continue;
        }else{
       	  /* ��ǰ�������̣�i�����ȼ����ڽ��̣�j�� */ 
          for(k=N-1;k>j;--k)//j���棬�Ӻ���ǰ��ȫ������Ųһλ 
 	      {
            READY[k]=READY[k-1];
          }
          READY[j]=i;//��ŵ��ǽ��̺� 
          break;
        }
      }
    }
  }
}


/**
 * ��������һ��ʱ��Ƭ��
 * �����̵����ȼ���ʱ��仯
 */ 
int run(int time)
{
  int i,num;
  num=READY[0];//���е��̷߳���ready�ĵ�0�� 
  if(num<0){//������Ľ���READYΪ-1 
    printf("Every process is OVER!\n");
      return 1;
  }else{
    printf("Running Process: %d\n",Process[num].ID);
    Process[num].PRIORITY-=3;
    Process[num].ALLTIME-=1;
    Process[num].CPUTIME+=1;
    Process[num].State=Run;
    for(i=0;i<N;i++){
      if(i!=num&&Process[i].State==Ready){
          Process[i].PRIORITY+=1;
      }
    }
    if(Process[num].ALLTIME==0)
    {
      Process[num].State=RunOut;
      for(i=0;i<N;++i)
      {
        if(RUNOUT[i][0]<0)
		{
          RUNOUT[i][0]=num;
          RUNOUT[i][1]=time;
          break;
        }
      }
    }
  }
  print();
  sort();
  return 0;
}


/* ������ */
int main()
{
  int Time=0;
  init();
  printf("��ʼ״̬�� \n");
  sort();
  /* ��ӡ����ʼ��״̬ */ 
  print();
  while(1)
  {
    Time++;
    getchar();//����س��������� 
    printf("--------------------------------------\n");
    printf("��%d��ʱ��Ƭ��\n",Time);
    if(run(Time)) break;
  }
  return 0;
}

