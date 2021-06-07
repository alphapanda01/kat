kitty: src/kat.c 
	$(CC) -Wall -Wextra -Werror -O3 src/kat.c -o bin/kat

kitty2: src/kat_adv.c 
	$(CC) -Wall -Wextra -O3 src/kat_adv.c -o bin/kat

clean:
	@rm -rf bin/*

install: 
	install bin/kat /usr/bin/

.PHONY: clean
