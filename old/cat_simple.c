#include <stdio.h>
#include <string.h>

void file_copy(FILE *from, FILE *to);
void print_help(char *exec_name);


int main(int argc, char *argv[]) {

	if (argc > 1) {
		for (int i = 0; i<argc; i++) {

			if (!strcmp(argv[i],"-h")) {
				print_help(argv[1]);
				return 0;
				
			}
		}
	}
	else {
		file_copy(stdin,stdout);
		return 0;
	}

	for (int i = 1; argc-- > 1; i++) {
		if(strcmp(argv[i], "-") == 0) {
			file_copy(stdin, stdout);
		}
		else {
			FILE *f = fopen(argv[i], "r");
			file_copy(f, stdout);
		}
	}

	return 0;

}

void file_copy(FILE *from, FILE *to) {

	if (from == NULL) return; // to check if the from file is not null

	char c;
	while((c = fgetc(from)) != EOF)
		fputc(c,to);
}


void print_help(char *exec_name) {

	printf("This is a test cat \n");
	printf("USAGE:\n\t%s -h\t\tTo print this help\n",exec_name);
	printf("\t%s file1 file2 file3\t To print file contents",exec_name);
	printf("\t%s -\tTo print contents from stdint",exec_name);

	return;
}
