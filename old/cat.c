#include <stdio.h>
#include <string.h>

void print_stdout(FILE *file);
void print_help(char *exec_name);

int main(int argc, char *argv[]) {

	if (argc == 1 ) {
		print_stdout(stdin);
		return 0; 
	}

	print_help(argv[0]);


}


// prints the contents in the file to stdout
void print_stdout(FILE *file) {

	if(file == NULL) return; // check the input file pointer

	char c;
	while ((c = fgetc(file)) != EOF) {
		fputc(c, stdout);
	}

}

void print_help(char *exec_name) {
	
	printf("Usage: %s [OPTION]... [FILE]...\n",exec_name);
	printf("My shitty implementation of cat\n\n");

	printf("Reads Standard input when no input file or '-' is given\n");

	printf("\nOPTIONS:\n");
	printf("\t--version\t\tPrints the version\n\n");
	printf("\t--help\t\t\tPrints this help\n\n");


	printf("LICENSE: No Licensing/Self Licensed idk\n");
	
	printf("UwU\n");
}
