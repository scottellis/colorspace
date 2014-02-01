#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

#define NUM_ITERATIONS 1000

int get_file_size(char *name);
long elapsed_msec(struct timespec *start, struct timespec *end); 
void run_tests(int count, int size, unsigned char *in, unsigned char *out);
void c_yuy2_to_uyvy(int size, unsigned char *in, unsigned char *out);
void neon_yuy2_to_uyvy(int size, unsigned char *in, unsigned char *out);

int main(int argc, char **argv)
{
	int size, fd;
	unsigned char *in = NULL;
	unsigned char *out = NULL;

	if (argc < 2) {
		printf("Usage: %s <yuy2 input file>\n", argv[0]);
		exit(1);
	}

	size = get_file_size(argv[1]);	

	if (size <= 0)
		exit(1);

	if ((size % 4) != 0) {
		printf("Impossible size %d\n", size);
		exit(1);
	}
	
	fd = open(argv[1], O_RDONLY);
	if (fd < 0) {
		perror("open");
		exit(1);
	}

	in = (unsigned char *) malloc(size);
	if (!in) {
		perror("malloc");
		goto done;
	}

	out = (unsigned char *) malloc(size);
	if (!out) {
		perror("malloc");
		goto done;
	}
	
	if (read(fd, in, size) != size) {
		perror("read");
		goto done;
	}

	memset(out, 0, size);

	run_tests(NUM_ITERATIONS, size, in, out);

done:
	if (in)
		free(in);

	if (out)
		free(out);

	close(fd);

	return 0;	
}

void run_tests(int count, int size, unsigned char *in, unsigned char *out)
{
	struct timespec start, end;
	int i;
	long elapsed; 

	if (clock_gettime(CLOCK_MONOTONIC, &start)) {
		perror("clock_gettime");
		return;
	}

	for (i = 0; i < count; i++)
		c_yuy2_to_uyvy(size, in, out);

	if (clock_gettime(CLOCK_MONOTONIC, &end)) {
		perror("clock_gettime");
		return;
	}

	elapsed = elapsed_msec(&start, &end);	

	printf("Iterations: %d\n", count);
	printf("Elapsed time: %ld ms\n", elapsed);

	if (elapsed > 0) {
		printf("Rate: %0.2f conversions/sec\n", (1000.0 * (float)count) / (float)elapsed);
		printf("Avg time: %0.2f ms\n", (float)elapsed / (float)count);
	}
}

void c_yuy2_to_uyvy(int size, unsigned char *in, unsigned char *out)
{
	int i;

	for (i = 0; i < size; i += 4) {
		out[i] = in[i+1];
		out[i+1] = in[i];
		out[i+2] = in[i+3];
		out[i+3] = in[i+2];		
	}
}

long elapsed_msec(struct timespec *start, struct timespec *end)
{
	long msec;

	if (end->tv_nsec < start->tv_nsec) {
		end->tv_nsec += 1000000000;
		end->tv_sec--;
	}

	msec = (end->tv_nsec - start->tv_nsec) / 1000000;

	return ((end->tv_sec - start->tv_sec) * 1000) + msec;
}

int get_file_size(char *name)
{
	struct stat st;

	if (stat(name, &st)) {
		perror("stat");
		return -1;
	}

	return st.st_size;
}
