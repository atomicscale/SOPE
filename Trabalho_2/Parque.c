#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>
#include <math.h>
#include <time.h>


#define FILE_LOG "parque.log"
#define FIFO_NAME_LENGTH 10
#define LAST_VEHICLE_ID -1
#define STATUS_MAX_LENGTH 20
#define FILE_LINE_MAX_LENGTH 100



//Checking if park is open (0) or closed (1)
int park_open;

int park_size;

//number of unavailable car-parking spaces on park
int occupied_space;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t file_mutex = PTHREAD_MUTEX_INITIALIZER;

int fd_parque_log;

typedef enum {NORTH, SOUTH, EAST, WEST} Direction;

  
  typedef struct {
  Direction direction;
  int id;
  char fifoName[FIFO_NAME_LENGTH] ;
  float parkingTime; 
  float parkingTimeTicks;
  int initialTicks;

  }Vehicle;

void log_write(Vehicle *vehicle, int state){
  char buffer[FILE_LINE_MAX_LENGTH];
  char status[STATUS_MAX_LENGTH];

  switch(state){
    case 0:
    strcpy(status, "entrance");
    break;
    case 1:
    strcpy(status, "full");
    break;
    case 2:
    strcpy(status, "closed");
    break;
    case 3:
    strcpy(status, "exit");
    break;
    case 4:
    strcpy(status, "parking");
    break;

  }
  if(state==2 || state ==3)
    sprintf(buffer, "%-8d ; %4d ; %7d ; %s\n",(int)(vehicle->initialTicks+vehicle->parkingTimeTicks), (park_size - occupied_space),vehicle->id, status);
  else
    sprintf(buffer, "%-8d ; %4d ; %7d ; %s\n",vehicle->initialTicks, (park_size - occupied_space),vehicle->id, status);
  write(fd_parque_log,buffer,strlen(buffer));

  strcpy(buffer, "");

}

void* vehicle_guide(void* arg){
  Vehicle vehicle= *(Vehicle*) arg;
  void* ret=NULL;
  int state;
  int fd_write;

  fd_write=open(vehicle.fifoName,O_WRONLY);
  //verify park state
  sleep(1);
  printf("ID VEHICLE:%d\n",vehicle.id);

  //Mutex that controlls the number of available spaces to park a car
  pthread_mutex_lock(&mutex);
  if(occupied_space<park_size && !park_open){
    state=0;
    occupied_space++;
    write(fd_write,&state,sizeof(int));
    pthread_mutex_lock(&file_mutex);
    log_write(&vehicle, 4);
    pthread_mutex_unlock(&file_mutex);
    printf("Entering park! ID %d capacidade %d, lugares %d\n",vehicle.id, park_size, occupied_space);
    pthread_mutex_unlock(&mutex);
    usleep(vehicle.parkingTime*pow(10,3));
    occupied_space--;
    state=3;
  }
  else if(park_open){
    pthread_mutex_unlock(&mutex);
    printf("The park is closed!\n");
    state=2;
  }
  else {
    pthread_mutex_unlock(&mutex);
    state=1;
    printf("The park is full!\n");
  }
  printf("ID VEHICLE:%d  State: %d\n", vehicle.id, state);
  write(fd_write,&state,sizeof(int));

  pthread_mutex_lock(&file_mutex);
  log_write(&vehicle, state);
  pthread_mutex_unlock(&file_mutex);

  close(fd_write);

  return ret;
}

//Opening North FIFO
void* func_north(void* arg){
  int fd_read;
  Vehicle vehicle;
  int read_ret;
  pthread_t tid_n;

  mkfifo("fifoN", 0660);

  printf("Opening Northern fifo\n");

  fd_read = open("fifoN", O_RDONLY | O_NONBLOCK);
  
  open("fifoN", O_WRONLY);

  printf("Fifo Opened\n");

  while(1){
    read_ret = read(fd_read, &vehicle, sizeof(Vehicle));
    if(vehicle.id == LAST_VEHICLE_ID)
    break;
    else  if(read_ret > 0){
      printf("NORTH PARK ID : %d\n", vehicle.id);
      if(pthread_create(&tid_n,NULL,vehicle_guide,&vehicle) != 0)
      perror("Func_North::Error creating thread\n");
    }
  }

  printf("North: Ending\n");

  close(fd_read);

  unlink("fifoN");

  pthread_exit(0);

}

//Now for south
void* func_south(void* arg){
  int fd_read;
  Vehicle vehicle;
  int read_ret;
  pthread_t tid_n;


  mkfifo("fifoS", 0660);

  printf("Opening Southern fifo\n");

  fd_read = open("fifoS", O_RDONLY | O_NONBLOCK);
  
  open("fifoS", O_WRONLY);

  printf("Fifo Opened\n");

  while(1){
    read_ret = read(fd_read, &vehicle, sizeof(Vehicle));
    if(vehicle.id == LAST_VEHICLE_ID)
    break;
    else  if(read_ret > 0){
      printf("SOUTH PARK ID : %d\n", vehicle.id);
      if(pthread_create(&tid_n,NULL,vehicle_guide,&vehicle) != 0)
      perror("Func_South::Error on creating thread\n");
    }
  }

  printf("South: Ending\n");

  close(fd_read);

  unlink("fifoS");

  pthread_exit(0);
}

//East
void* func_east(void* arg){
  int fd_read;
  Vehicle vehicle;
  int read_ret;
  pthread_t tid_n;


  mkfifo("fifoE", 0660);

  printf("Opening Eastern fifo\n");

  fd_read = open("fifoE", O_RDONLY | O_NONBLOCK);
  
  open("fifoE", O_WRONLY);

  printf("Fifo Opened\n");

  while(1){
    read_ret = read(fd_read, &vehicle, sizeof(Vehicle));
    if(vehicle.id == -1)
    break;
    else  if(read_ret > 0){
      printf("EAST PARK ID : %d\n", vehicle.id);
      if(pthread_create(&tid_n,NULL,vehicle_guide,&vehicle) != 0)
      perror("Func_East::Error on creating thread\n");
    }
  }

  printf("East: Ending\n");

  close(fd_read);

  unlink("fifoE");

  pthread_exit(0);
}

//West
void* func_west(void* arg){
  int fd_read;
  Vehicle vehicle;
  int read_ret;
  pthread_t tid_n;

  mkfifo("fifoW", 0660);

  printf("Opening Western fifo\n");

  fd_read = open("fifoW", O_RDONLY | O_NONBLOCK);
  
  open("fifoW", O_WRONLY);

  printf("Fifo Opened\n");

  while(1){
    read_ret = read(fd_read, &vehicle, sizeof(Vehicle));
    if(vehicle.id == -1)
    break;
    else  if(read_ret > 0){
      printf("West PARK ID : %d\n", vehicle.id);
      if(pthread_create(&tid_n,NULL,vehicle_guide,&vehicle) != 0)
      perror("Func_West::Error on creating thread\n");
    }
  }

  printf("West: Ending\n");

  close(fd_read);

  unlink("fifoW");

  pthread_exit(0);
}


int main(int argc, char* argv[]){

  if(argc != 3){
    fprintf(stderr, "Invalid number of arguments.\nThe input must be of type parque <N_LUGARES> <T_ABERTURA>.\n");
    return 1;
  }


  int total_spots=atoi(argv[1]);
  int time_open=atoi(argv[2]);
  //Initializing the park with max size.
  park_size = total_spots;
  occupied_space = 0;
  pthread_t tid_n, tid_s, tid_e, tid_w;

  //Initialize the last vehicle to inform the park is closed (-1).
  Vehicle last_vehicle;
  last_vehicle.id = -1;
  last_vehicle.parkingTime = 0;
  strcpy(last_vehicle.fifoName, "over");

  //Reseting File Parque.log
  FILE *fp=fopen(FILE_LOG,"w");
  fclose(fp);


  char name[]="/sem";
  sem_t *semaphore = sem_open(name, O_CREAT ,0660,1);

  //The park is open
  park_open = 0;

  //opening the file "parque.log" to write vehicles information
  fd_parque_log = open(FILE_LOG, O_WRONLY | O_CREAT  , 0600);

  char buffer[] = "t(ticks) ; nlug ; id_viat ; observ\n";

  write(fd_parque_log,buffer,strlen(buffer));


  //Creating the thread controller on the north pole of the park
  if(pthread_create(&tid_n,NULL,func_north,NULL) != 0)
  perror("Parque::Error on creating thread\n");
  //Creating the thread controller on the south pole of the park
  if(pthread_create(&tid_s,NULL,func_south,NULL) != 0)
  perror("Parque::Error on creating thread\n");
  //Creating the thread controller on the east pole of the park
  if(pthread_create(&tid_e,NULL,func_east,NULL) != 0)
  perror("Parque::Error on creating thread\n");
  //Creating the thread controller on the west pole of the park
  if(pthread_create(&tid_w,NULL,func_west,NULL) != 0)
  perror("Parque::Error on creating thread\n");

  sleep(time_open);
  printf("Ending\n");

  //The park is closed
  park_open = 1;

  int fd_north = open("fifoN", O_WRONLY);
  int fd_south = open("fifoS", O_WRONLY);
  int fd_east = open("fifoE", O_WRONLY);
  int fd_west = open("fifoW", O_WRONLY);


  sem_wait(semaphore);

  //Send to north controller a vehicle that tells the park is closed (last_vehicle has id -1)
  write(fd_north, &last_vehicle, sizeof(Vehicle));

  //Send to south controller a vehicle that tells the park is closed (last_vehicle has id -1)
  write(fd_south, &last_vehicle, sizeof(Vehicle));

  //Send to east controller a vehicle that tells the park is closed (last_vehicle has id -1)
  write(fd_east, &last_vehicle, sizeof(Vehicle));

  //Send to west controller a vehicle that tells the park is closed (last_vehicle has id -1)
  write(fd_west, &last_vehicle, sizeof(Vehicle));

  sem_post(semaphore);
  sem_close(semaphore);

  close(fd_north);
  close(fd_south);
  close(fd_east);
  close(fd_west);

  //pthread_join() function waits for the north thread to terminate
  if(pthread_join(tid_n, NULL) != 0)
  perror("Parque::Error on joinning thread\n");
  //pthread_join() function waits for the south thread to terminate
  if(pthread_join(tid_s, NULL) != 0)
  perror("Parque::Error on joinning thread\n");
  //pthread_join() function waits for the east thread to terminate
  if(pthread_join(tid_e, NULL) != 0)
  perror("Parque::Error on join thread\n");
  //pthread_join() function waits for the west thread to terminate
  if(pthread_join(tid_w, NULL) != 0)
  perror("Parque::Error on join thread\n");

  //Wait for all the vehicles till the park is empty, then end the program

  //  while(occupied_space!=0){}

  sem_unlink(name);

  pthread_exit(NULL);
}
