#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h> 
#include <semaphore.h>
#include <pthread.h>
#include <sys/types.h>

#define GERADOR_FILE_NAME "gerador.log"
#define FIFO_NAME_LENGTH 10
#define STATUS_MAX_LENGTH 20
#define FILE_LINE_MAX_LENGTH 100
#define DEST_MAX_LENGTH 10

int id=0;
int fd_gerador_log;
float numberOfTicks = 0;




typedef enum {NORTH, SOUTH, EAST, WEST} Direction;

typedef struct {
  Direction direction;
  int id;
  char fifoName[FIFO_NAME_LENGTH] ;
  float parkingTime;
  float parkingTimeTicks;
  int initialTicks;
  }Vehicle;
  
  
  
  
  
void log_write(Vehicle *vehicle){
  char buffer[FILE_LINE_MAX_LENGTH];
  char dest[DEST_MAX_LENGTH];
  
  
  switch (vehicle->direction){
    case NORTH:
    strcpy(dest, "N");
    break;
    case SOUTH:
    strcpy(dest, "S");
    break;
    case EAST:
    strcpy(dest, "E");
    break;
    case WEST:
    strcpy(dest, "O");
    break;
  }
  
	

	
	sprintf(buffer, "%-8d ; %7d ;    %s   ; %10d ; %6d \n",(int)(vehicle->initialTicks+vehicle->parkingTimeTicks), vehicle->id, dest, (int)vehicle->parkingTimeTicks,(int)(numberOfTicks-vehicle->initialTicks+vehicle->parkingTimeTicks) );
	
	write(fd_gerador_log,buffer,strlen(buffer));
	
	strcpy(buffer, "");
}

void create_generator_log()
{
  FILE* file = fopen(GERADOR_FILE_NAME, "w");
  
  fclose(file);
  fd_gerador_log = open(GERADOR_FILE_NAME, O_WRONLY | O_CREAT, 0600);
  char str[] = "t(ticks); id_viat ; destin ; t_estacion ; t_vida \n";
	
  write(fd_gerador_log, str, strlen(str));
  
  
}
  
  void* funcVehicle(void *arg){
	Vehicle vehicle = *(Vehicle*) arg;
	void *ret = NULL;
	
	int fdWrite;
	
	mkfifo(vehicle.fifoName,0660);
	
	

	switch(vehicle.direction){
	case NORTH:
		fdWrite = open("fifoN",O_WRONLY | O_NONBLOCK);
		break;
	case SOUTH:
		fdWrite = open("fifoS",O_WRONLY | O_NONBLOCK);
		break;
	case WEST:
		fdWrite = open("fifoW",O_WRONLY | O_NONBLOCK);
		break;
	case EAST:
		fdWrite = open("fifoE",O_WRONLY | O_NONBLOCK);
		break;
	}


	if(fdWrite != -1){
		write(fdWrite,&vehicle,sizeof(Vehicle));
		close(fdWrite);
                printf("Vehicle %s added \n",vehicle.fifoName);
		log_write(&vehicle);
	}

	
	unlink(vehicle.fifoName);


	return ret;
  }
	
	Direction getRandDirection(){

	int x = rand() %  4;
	switch(x){
	case 0:	
		return NORTH;
		break;
	case 1:
		return SOUTH;
		break;
	case 2:
		return EAST;
		break;
	case 3:	
		return WEST;
		break;
		}
		return WEST;
	}

int generateVehicle(float unitClock, float numberOfTicks){
	Vehicle *vehicle = (Vehicle*)malloc(sizeof(Vehicle));
	vehicle->id = id;
	id++;
	pthread_t tid;
	vehicle->initialTicks = numberOfTicks;
	vehicle->direction= getRandDirection();

	float parkTime = ((rand() %10)+1) * unitClock;
	vehicle->parkingTime = parkTime;
	vehicle->parkingTimeTicks=vehicle->parkingTime/unitClock;
	
	sprintf(vehicle->fifoName,"%s%d","fifo",vehicle->id);
	 
	
	

	if(pthread_create(&tid, NULL, funcVehicle,vehicle) != 0) perror("ERROR ON GERADOR :: THREAD \n");

	int nextCarTime;
	int r = rand() % 10;

	if(r < 5)
		nextCarTime= 0;
	else if(r < 8)
		nextCarTime = 1;
	else nextCarTime = 2;


	return nextCarTime;
}

int main(int argc, char* argv[]){

	if(argc != 3){
				perror("Invalid number of arguments! \n");
				exit(1);
			}

	srand(time(NULL));
	float generateTime = atoi(argv[1]);
	float unitClock = atoi(argv[2]);
	
	int ticksForNextCar = 0;
	create_generator_log();
	float MaxNumberOfTicks = (generateTime / unitClock) *1000; // Number of events that are going to happen
	printf("generateTime: %f \n",generateTime);
	printf("Number of ticks: %f \n",numberOfTicks);
	
	do{
		if(ticksForNextCar == 0) ticksForNextCar = generateVehicle(unitClock, numberOfTicks);
		else ticksForNextCar--;
		usleep(unitClock * 1000);
		numberOfTicks++;
	} while( numberOfTicks < MaxNumberOfTicks);



	pthread_exit(NULL);
}

