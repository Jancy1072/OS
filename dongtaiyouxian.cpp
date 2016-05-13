#include<stdio.h>
/* 进程数 */ 
#define N 5

enum STATE{Ready,Run,RunOut};//枚举类型 

/* 进程控制块结构 */ 
struct PROCESS
{
  int ID; 		    //进程标识数
  int PRIORITY;     //进程优先数
  int CPUTIME;      //进程已占用的CPU时间
  int ALLTIME;      //进程还需占用的CPU时间
  enum STATE State; //状态
}Process[N];

/* 就绪队列数组 */ 
int READY[N];
//运行完的数组 
int RUNOUT[N][2];//1存num，2存time ，按照顺序存的，第一个是最先结束的 

/**
 * 初始化进程数据
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
 * 打印当前时间片内的进程情况
 */
void print()
{
  int i;
  
//按照顺序打印就绪队列 
  printf("\tReady_Queue: ");
  for(i=0;i<N;++i)
  {
    if(READY[i]>=0) //运行完的进程值为-1 ，-1在数组内的后面 
		printf(" -> %d",Process[READY[i]].ID);
    else
		break;
  }

  printf("\n****************************************************\n");

  /* 打印PCB控制块信息 */ 
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
			 	Process[i].State=Ready;//一个时间片完成后改变状态run 
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
      printf(" -> %d(%d)",Process[RUNOUT[i][0]].ID,RUNOUT[i][1]);//第i个完成的进程，在第几个时间片完成的 
    }else{
      printf("\n");
      break;
    } 
  }
  printf("\n");
}


/**
 * 根据优先级重新排序就绪队列
 */
void sort()
{
  int i,j,k;
  /* 初始化就绪队列数组 */
  for(i=0;i<N;i++)
  {
  	READY[i]=-1;
  }
  /* 遍历所有进程 */ 
  for(i=0;i<N;i++)
  {
    if(Process[i].State==Ready||Process[i].State==Run)
    {
      Process[i].State=Ready;
      /* 已RunOut的程序不在就绪队列中 */

 	   
      for(j=0;j<N;j++)
 	  {
        if(READY[j]<0)
		{
		  /* 第一个状态为Ready或者Run的进程 */ 
          READY[j]=i;
          break;
        }else if(Process[i].PRIORITY<=Process[READY[j]].PRIORITY){
          continue;
        }else{
       	  /* 当前遍历进程（i）优先级高于进程（j） */ 
          for(k=N-1;k>j;--k)//j后面，从后往前，全部往后挪一位 
 	      {
            READY[k]=READY[k-1];
          }
          READY[j]=i;//存放的是进程号 
          break;
        }
      }
    }
  }
}


/**
 * 进程运行一个时间片中
 * 各进程的优先级和时间变化
 */ 
int run(int time)
{
  int i,num;
  num=READY[0];//运行的线程放在ready的第0个 
  if(num<0){//运行完的进程READY为-1 
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


/* 主函数 */
int main()
{
  int Time=0;
  init();
  printf("初始状态： \n");
  sort();
  /* 打印出初始化状态 */ 
  print();
  while(1)
  {
    Time++;
    getchar();//输入回车继续运行 
    printf("--------------------------------------\n");
    printf("第%d个时间片：\n",Time);
    if(run(Time)) break;
  }
  return 0;
}

