COMPILER=gcc

LIB=create.c run.c

FLAG=-luuid

MAIN=tinyvps.c $(LIB)

all: main

main: $(MAIN)
	$(COMPILER) -o tinyvps $(MAIN) $(FLAG)