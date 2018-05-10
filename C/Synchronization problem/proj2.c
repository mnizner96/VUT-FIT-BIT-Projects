#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>

#define E_OK 0
#define E_PARAM 1
#define E_SEMAPHORE 2
#define E_SHM 3

typedef struct params {		//definicia argumentov
	long P;
	long C;
	int PT;
	int RT;
} Tparam;

FILE *out = NULL;

sem_t *s_finish,
    *s_write, *counter, *queue, *checkin, *boarding, *unloading, *empty;

int shm_process_id = 0;
int shm_passenger_id = 0;
int shm_board_id = 0;
int shm_unboard_id = 0;

int *process_id = NULL;
int *passenger_id = NULL;
int *board_id = NULL;
int *unboard_id = NULL;

pid_t pid_car, pid_passenger;

int get_params(int argc, char *argv[], Tparam * x);
int load_resources();
int free_resources();
int process_count(int *x);
void car(Tparam params);
void passenger(Tparam params);
void end();

int main(int argc, char *argv[])
{
	Tparam params;
	int error_params = get_params(argc, argv, &params);	//definicia premennej signalizujucej chybu pri citani parametrov
	if (error_params == E_PARAM) {
		fprintf(stderr,
			"Error, invalid combination of parameters\n");
		return EXIT_FAILURE;
	}
	int error_resources = load_resources();	//definicia premennej signalizujucej chybu pri alokacii semaforov alebo segmentov zdielanej pamate
	if (error_resources == E_SEMAPHORE) {
		fprintf(stderr,
			"Error, invalid allocation of semaphores\n");
		int error_free = free_resources();
		if (error_free == E_SEMAPHORE) {
			fprintf(stderr,
				"Error, semaphore didnt destroyed\n");
		}
		if (error_free == E_SHM) {
			fprintf(stderr, "Error with memory free\n");
		}
		return EXIT_FAILURE;
	}
	if (error_resources == E_SHM) {
		fprintf(stderr, "Error, invalid allocation of memory\n");
		int error_free = free_resources();
		if (error_free == E_SEMAPHORE) {
			fprintf(stderr,
				"Error, semaphore didnt destroyed\n");
		}
		if (error_free == E_SHM) {
			fprintf(stderr, "Error with memory free\n");
		}
		return EXIT_FAILURE;
	}
	if ((out = fopen("proj2.out", "w")) == NULL) {
		fprintf(stderr, "Error opening proj2.out\n");
		return EXIT_FAILURE;
	}
	int childs[params.P];

	setbuf(out, NULL);
	srand(time(0));

	signal(SIGTERM, end);	//zachytenie signalu
	signal(SIGINT, end);

	*process_id = 1;	//inicializacia ID procesov
	*passenger_id = 1;
	*board_id = 1;
	*unboard_id = 1;

	int pgt = 0;		//passenger generation time
	pid_car = fork();
	if (pid_car == 0)	//proces voziku
	{
		car(params);
	} else if (pid_car > 0)	//pomocny proces pre generovanie procesov pasazierov
	{
		int i;
		for (i = 0; i < params.P; i++) {
			if (params.PT != 0) {
				pgt = (random() % ((params.PT) + 1));
			}
			usleep(pgt * 1000);
			pid_passenger = fork();
			if (pid_passenger == 0)	//proces pasaziera
			{
				passenger(params);
			} else if (pid_passenger > 0) {
				childs[i] = pid_passenger;
			} else {
				fprintf(stderr,
					"Error, invalid passenger creation\n");
			}
		}
	} else {
		fprintf(stderr, "Error, invalid car creation\n");
	}
	int i;
	for (i = 0; i < params.P; i++) {
		waitpid(childs[i], NULL, 0);
	}
	free_resources();
	return 0;
}

int get_params(int argc, char *argv[], Tparam * x)
{
	int error = E_OK;
	char *p = NULL;
	if (argc == 5) {
		if (isdigit(*argv[1])) {
			x->P = strtoul(argv[1], &p, 10);
		} else {
			error = E_PARAM;
		}
		if (isdigit(*argv[2])) {
			x->C = strtoul(argv[2], &p, 10);
		} else {
			error = E_PARAM;
		}
		if (isdigit(*argv[3])) {
			x->PT = strtoul(argv[3], &p, 10);
		} else {
			error = E_PARAM;
		}
		if (isdigit(*argv[4])) {
			x->RT = strtoul(argv[4], &p, 10);
		} else {
			error = E_PARAM;
		}
	} else {
		error = E_PARAM;
	}
	if (!((x->P > 0) && (x->P > x->C))
	    || !((x->C > 0) && ((x->P % x->C) == 0)) || !((x->PT >= 0)
							  && (x->PT <
							      5001))
	    || !((x->RT >= 0) && (x->RT < 5001))) {
		error = E_PARAM;
	}
	if (*p != 0) {
		error = E_PARAM;
	}
	return error;
}


int load_resources()
{
	int error = E_OK;
	if ((s_finish =
	     mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE,
		  MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED) {
		error = E_SEMAPHORE;
	}
	if ((queue =
	     mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE,
		  MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED) {
		error = E_SEMAPHORE;
	}
	if ((checkin =
	     mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE,
		  MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED) {
		error = E_SEMAPHORE;
	}
	if ((boarding =
	     mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE,
		  MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED) {
		error = E_SEMAPHORE;
	}
	if ((unloading =
	     mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE,
		  MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED) {
		error = E_SEMAPHORE;
	}
	if ((counter =
	     mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE,
		  MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED) {
		error = E_SEMAPHORE;
	}
	if ((s_write =
	     mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE,
		  MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED) {
		error = E_SEMAPHORE;
	}
	if ((empty =
	     mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE,
		  MAP_ANONYMOUS | MAP_SHARED, 0, 0)) == MAP_FAILED) {
		error = E_SEMAPHORE;
	}

	if (sem_init(s_finish, 1, 0) == -1) {
		error = E_SEMAPHORE;
	}
	if (sem_init(queue, 1, 0) == -1) {
		error = E_SEMAPHORE;
	}
	if (sem_init(checkin, 1, 1) == -1) {
		error = E_SEMAPHORE;
	}
	if (sem_init(boarding, 1, 0) == -1) {
		error = E_SEMAPHORE;
	}
	if (sem_init(unloading, 1, 0) == -1) {
		error = E_SEMAPHORE;
	}
	if (sem_init(counter, 1, 1) == -1) {
		error = E_SEMAPHORE;
	}
	if (sem_init(s_write, 1, 1) == -1) {
		error = E_SEMAPHORE;
	}
	if (sem_init(empty, 1, 0) == -1) {
		error = E_SEMAPHORE;
	}

	if ((shm_process_id =
	     shmget(IPC_PRIVATE, sizeof(int),
		    IPC_CREAT | IPC_EXCL | 0666)) == -1) {
		error = E_SHM;
	}
	if ((process_id = (int *) shmat(shm_process_id, NULL, 0)) == NULL) {
		error = E_SHM;
	}
	if ((shm_passenger_id =
	     shmget(IPC_PRIVATE, sizeof(int),
		    IPC_CREAT | IPC_EXCL | 0666)) == -1) {
		error = E_SHM;
	}
	if ((passenger_id =
	     (int *) shmat(shm_passenger_id, NULL, 0)) == NULL) {
		error = E_SHM;
	}
	if ((shm_board_id =
	     shmget(IPC_PRIVATE, sizeof(int),
		    IPC_CREAT | IPC_EXCL | 0666)) == -1) {
		error = E_SHM;
	}
	if ((board_id = (int *) shmat(shm_board_id, NULL, 0)) == NULL) {
		error = E_SHM;
	}
	if ((shm_unboard_id =
	     shmget(IPC_PRIVATE, sizeof(int),
		    IPC_CREAT | IPC_EXCL | 0666)) == -1) {
		error = E_SHM;
	}
	if ((unboard_id = (int *) shmat(shm_unboard_id, NULL, 0)) == NULL) {
		error = E_SHM;
	}
	return error;
}

int free_resources()
{
	int error = E_OK;
	if (sem_destroy(s_finish) == -1) {
		error = E_SEMAPHORE;
	}
	if (sem_destroy(queue) == -1) {
		error = E_SEMAPHORE;
	}
	if (sem_destroy(checkin) == -1) {
		error = E_SEMAPHORE;
	}
	if (sem_destroy(boarding) == -1) {
		error = E_SEMAPHORE;
	}
	if (sem_destroy(unloading) == -1) {
		error = E_SEMAPHORE;
	}
	if (sem_destroy(counter) == -1) {
		error = E_SEMAPHORE;
	}
	if (sem_destroy(s_write) == -1) {
		error = E_SEMAPHORE;
	}
	if (sem_destroy(empty) == -1) {
		error = E_SEMAPHORE;
	}

	if (shmctl(shm_process_id, IPC_RMID, NULL) == -1) {
		error = E_SHM;
	}
	if (shmctl(shm_passenger_id, IPC_RMID, NULL) == -1) {
		error = E_SHM;
	}
	if (shmctl(shm_board_id, IPC_RMID, NULL) == -1) {
		error = E_SHM;
	}
	if (shmctl(shm_unboard_id, IPC_RMID, NULL) == -1) {
		error = E_SHM;
	}
	fclose(out);
	return error;
}


int process_count(int *x)
{
	int previous = 0;
	sem_wait(counter);
	previous = *x;
	(*x)++;
	sem_post(counter);
	return previous;
}

void end()
{
	free_resources();
	kill(getppid(), SIGTERM);
	exit(EXIT_FAILURE);
}

void car(Tparam params)
{

	int RT = params.RT;
	sem_wait(s_write);
	fprintf(out, "%d\t: C 1\t: started\n", process_count(process_id));
	sem_post(s_write);
	fflush(out);
	int rides = params.P / params.C;
	int i;
	for (i = 0; i < rides; i++) {
		sem_wait(s_write);
		fprintf(out, "%d\t: C 1\t: load\n",
			process_count(process_id));
		sem_post(s_write);
		fflush(out);
		sem_post(queue);	//signal pasazierom pre nastupovanie
		sem_wait(boarding);	//cakanie na vyvolanie operacie run
		if (RT != 0) {
			RT = (random() % (params.RT + 1));
		}
		sem_wait(s_write);
		fprintf(out, "%d\t: C 1\t: run\n",
			process_count(process_id));
		sem_post(s_write);
		fflush(out);
		usleep(RT * 1000);
		sem_wait(s_write);
		fprintf(out, "%d\t: C 1\t: unload\n",
			process_count(process_id));
		sem_post(s_write);
		sem_post(unloading);	//signal pasazierom pre vystupovanie
		sem_wait(empty);	//cakanie na vyvolanie noveho cyklu
		fflush(out);
	}
	sem_post(s_finish);	//signal pasazierom pre ukoncenie
	sem_wait(s_write);
	fprintf(out, "%d\t: C 1\t: finished\n", process_count(process_id));
	sem_post(s_write);
	fflush(out);
	exit(0);
}

void passenger(Tparam params)
{
	int current_passenger = 0;
	int board = 0;
	int unboard = 0;
	sem_wait(checkin);
	current_passenger = process_count(passenger_id);
	fprintf(out, "%d\t: P %d\t: started\t\n",
		process_count(process_id), current_passenger);
	sem_post(checkin);
	fflush(out);
	sem_wait(queue);	//cakanie na nastupovanie
	sem_wait(checkin);
	board = process_count(board_id);
	fprintf(out, "%d\t: P %d\t: board\n", process_count(process_id),
		current_passenger);
	sem_post(checkin);
	fflush(out);
	if (board == params.C) {
		*board_id = 1;
		sem_wait(s_write);
		fprintf(out, "%d\t: P %d\t: board last\n",
			process_count(process_id), current_passenger);
		sem_post(s_write);
		fflush(out);
		sem_post(boarding);	//signal voziku pre zahajenie operacie run
	} else {
		sem_wait(s_write);
		fprintf(out, "%d\t: P %d\t: board order %d\n",
			process_count(process_id), current_passenger,
			board);
		sem_post(s_write);
		sem_post(queue);	//signal dalsiemu pasazierovi pre nastupovanie
		fflush(out);
	}
	sem_wait(unloading);	//cakanie na vystupovanie
	sem_wait(checkin);
	unboard = process_count(unboard_id);
	fprintf(out, "%d\t: P %d\t: unboard\n", process_count(process_id),
		current_passenger);
	sem_post(checkin);
	fflush(out);
	if (unboard == params.C) {
		*unboard_id = 1;
		sem_wait(s_write);
		fprintf(out, "%d\t: P %d\t: unboard last\n",
			process_count(process_id), current_passenger);
		sem_post(s_write);
		fflush(out);
		sem_post(empty);	//signal voziku pre zahajenie noveho cyklu
	} else {
		sem_wait(s_write);
		fprintf(out, "%d\t: P %d\t: unboard order %d\n",
			process_count(process_id), current_passenger,
			unboard);
		sem_post(s_write);
		sem_post(unloading);	//signal dalsiemu pasazierovi pre vystupovanie
		fflush(out);
	}
	sem_wait(s_finish);	//cakanie na ukoncenie
	sem_wait(s_write);
	fprintf(out, "%d\t: P %d\t: finished\n", process_count(process_id),
		current_passenger);
	sem_post(s_write);
	sem_post(s_finish);	//signal dalsiemu pasazierovi na ukoncenie
	fflush(out);
	exit(0);
}
