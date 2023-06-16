COMPILER=gcc

LIB=create.c run.c

FLAG=-luuid

MAIN=main.c $(LIB)

all: main

main: $(MAIN)
	$(COMPILER) -o main $(MAIN) $(FLAG)