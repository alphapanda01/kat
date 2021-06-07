#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void file_copy(FILE *from, FILE *to);

int main(int argc, char *argv[]) {

	if (argc < 2) {
		printf("USAGE:\n \t%s file1 file2 file3\n\t\tOR\n\t%s -\t For reading from stdin\n",argv[0],argv[0]);
		exit(1);
	}

	for(int i = 1;argc-- > 1; i++) {
		if (strcmp(argv[i], "-") == 0) {
			file_copy(stdin, stdout);
		}
		else {
			FILE *f = fopen(argv[i],"r");
			if (f == NULL) {
				printf("Unable to read file: %s",argv[i]);
				continue;
			} 
			file_copy(f, stdout);
			fclose(f);
		}
	}



	return 0;

}

void file_copy(FILE *from, FILE *to) {

	if (from == NULL) return;
	char c;
	while((c = fgetc(from)) != EOF)
		fputc(c, to);	
}
