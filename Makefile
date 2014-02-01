CC = gcc
CFLAGS = -O2 -Wall

colorspace: colorspace.c
	$(CC) $(CFLAGS) colorspace.c -o colorspace

clean:
	rm -f colorspace

