CC = gcc

# define ALGO_NEON to test Neon instructions
# ALGORITHM = -D ALGO_NEON
# CFLAGS = -O3 -Wall -mfloat-abi=softfp -mcpu=cortex-a9 -mfpu=neon-fp16 -ffast-math -funroll-loops

# the default is C
ALGORITHM = -D ALGO_C
CFLAGS = -O3 -Wall

colorspace: colorspace.c
	$(CC) $(CFLAGS) $(ALGORITHM) colorspace.c -o colorspace

clean:
	rm -f colorspace

