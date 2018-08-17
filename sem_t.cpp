//# semophare.h

 #include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <netdb.h>
#include <grp.h>
#include <arpa/inet.h>
#include <sys/file.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <setjmp.h>
#include <pthread.h>
#include <locale.h>

#include <string.h>
#pragma pack(1)
 #include	<semaphore.h>
typedef		sem_t					sema;
 
#define		init_sema(h)			sem_init(&(h),0,0)
#define		init_sema_n(h,n)		sem_init(&(h),0,n)
#define		dec_sema(h)				sem_wait(&(h))
#define		inc_sema(h)				sem_post(&(h))
#define		wait_sema(h)			sem_wait(&(h))
#define		wait_sema_t(h,t)		__sem_timedwait(&(h),t)
#define		clr_sema(h)				while(sem_trywait(&(h))>=0)

#define		close_sema(h)		sem_destroy(&(h))
#define     __sleep(t)			usleep(t*1000)


#define lockConnect(lock) wait_sema(lock)
#define unlockConnect(lock) inc_sema( lock)

//#pragma pack(1)
typedef struct Conn{
int cancel;
int data;
sema lock;
};
//#pragma pack()

pthread_t begin_thread (void(*thd_func)(void*), int s_size,void *para )
{
  pthread_t id;
  pthread_attr_t attr;
  struct sched_param param;
  pthread_attr_init(&attr);
  //................
  if(!pthread_create(&id,NULL,(void*(*)(void*))thd_func,para))
    return id;
    return 0;
}


static  void Multi_results_recvThd(void *para)
{
	Conn	*stmt;
	stmt = (Conn*)para;
 
	
	 
	        __sleep(30000);
	  
	   	printf("batch sleep 3\n");
	   	 
	   	if(stmt->cancel)
	   	{
	   	 	printf("batch  here cancel some ok 3\n");
	   	}
	   	
	   	// unlock
	   	
	   	unlockConnect(stmt->lock);
	 	printf("batch  here unlock  ok 3\n");
	        return  ;	
}



int main()
{
 

Conn * dbc=  new Conn();
memset(dbc,0x0,sizeof(Conn));

init_sema_n( dbc->lock,1);
dbc->cancel=1;

lockConnect(dbc->lock);
printf(" lock again \n");
sem_wait(&(dbc->lock));  

 begin_thread(Multi_results_recvThd,0,(void*)dbc);
  
 lockConnect(dbc->lock);
  
   	printf("here main thread enter lock again \n");
  
 	unlockConnect(dbc->lock);
 	
 	printf(" main thread  unlock ");
 	
 	close_sema(dbc->lock);
 	
 	return 0;
 	
}
