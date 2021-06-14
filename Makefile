kitty: build src/kat.c 
	$(CC) -Wall -Wextra -O3 src/kat.c -o bin/kat

clean:
	@rm -rf bin/*

build:
	@mkdir -p bin

install: 
	install bin/kat /usr/bin/

tests: beta
	./tests/run_tests.sh

.PHONY: clean install
