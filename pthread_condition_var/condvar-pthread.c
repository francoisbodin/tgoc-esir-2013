#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int count = 0;

int consume( void )
{
  int i = 10;
  while( i-- ){
      pthread_mutex_lock( &mutex );
      //wait for a value to be produced
      pthread_cond_wait( &cond, &mutex );
      printf( "Consumed %d\n", count );
      pthread_cond_signal( &cond );      
      pthread_mutex_unlock( &mutex );
    }
  return( 0 );
}

void*  produce( void * arg )
{
  int i = 10;
  while( i--){
      pthread_mutex_lock( &mutex );
      count++;
      printf( "Produced %d\n", count);
      // has produced a value (count++) now free the consumer
      pthread_cond_signal( &cond );      
      // wait for the consumer to be done
      pthread_cond_wait( &cond, &mutex );
      pthread_mutex_unlock( &mutex );
    }
  return( 0 );
}

int main( void )
{
  pthread_t mytask_thread;
  pthread_create(&mytask_thread, NULL, &produce, NULL );
  consume();
  pthread_join(mytask_thread, NULL);
  return 0;
}

