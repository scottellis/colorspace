CC = gcc
CFLAGS = -O3 -Wall -mfloat-abi=softfp -mcpu=cortex-a9 -mfpu=neon-fp16 -ffast-math -funroll-loops

colorspace: colorspace.c
	$(CC) $(CFLAGS) colorspace.c -o colorspace

clean:
	rm -f colorspace

