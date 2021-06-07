#include <stdio.h>
#include <unistd.h> // for getopt
#include <stdlib.h> // for abort() exit() 
#include <string.h> // for strcmp()

#include <sys/stat.h> // for dir check

// An error macro
#define err(A, MSG, ...) if(!A) {\
	fprintf(stderr,"[ERROR] " MSG, ##__VA_ARGS__);\
	exit(1); }

#define KAT_VERSION "0.69_v2"

// to fix few std C99 problem
int getopt(int argc, char *const argv[], const char *optstring);

void print_help(char *exec_name); 
void print_version();

void print_stdout(FILE *f); // prints the file to stdout
void adv_print(FILE *f); // for more options

void check_file(char* file_path, FILE *f); // checks 
int isdirectory(char *path); 

#ifndef bool
typedef enum {FALSE, TRUE} bool;
#endif

// The hated global vairables
bool TAB = FALSE;
bool LINE_NUM = FALSE;
bool LINE_ENDS = FALSE;
bool ALL = FALSE;

int main(int argc, char *argv[]) {

	
	if (argc < 2) {
		print_stdout(stdin);
		return 1;
	}
	
	extern char *optarg;
	extern int optind, optopt;

	int arg;

	while((arg = getopt(argc, argv, ":vhtenA")) != -1) {

		switch(arg) {
			case 'v':
				print_version();
        exit(1);
			case 'h':
				print_help(argv[0]);
        exit(1);
      case 't':
        TAB = TRUE;
        break;
      case 'e':
        LINE_ENDS = TRUE;
        break;
      case 'n':
        LINE_NUM = TRUE;
        break;
      case 'A':
        ALL = TRUE;
        break;
			case '?':
				err(0,"unknown option '-%c'\n",optopt);
				exit(1);
			default:
				printf("Wot??\n");
				abort();

		}

	}

	char *fname;
	for(int i = optind; i < argc; i++) {
		fname = argv[i];
		if (strcmp(fname, "-") == 0) {
			print_stdout(stdin);
		}
		else {
			FILE *f = fopen(fname , "r");
			check_file(fname, f);
      
      if (TAB || LINE_NUM || LINE_ENDS || ALL) {
        adv_print(f);
      }
      else {
			  print_stdout(f);
      }
		}
	}

	return 0;
}

void print_version() {
	printf("Kitty (My ripoff of cat) %s\n",KAT_VERSION);
	printf("\nCopyright (C) None\n");
	printf("Written By: me\n");
}

void print_help(char *exec_name) {
	printf("Usage: %s [OPTION]... [FILE]...\n",exec_name);
	printf("Print contents of file(s) to stdout\n\n");

	printf("When no input or '-' is provided, prints from stdin\n");
	printf("OPTIONS:\n");
	printf("\t-t\t\tPrint tabs\n");
	printf("\t-e\t\tPrint line ends\n");
	printf("\t-n\t\tPrint line numbers\n");
	printf("\t-A\t\tsimilar to -e -t\n");
	printf("\t-v\t\tPrints version\n");
	printf("\t-h\t\tPrints this help\n");
}

void print_stdout(FILE *f) {

	err(f,"Failed to open file\n");

	int c;

	while((c = fgetc(f)) != EOF) {
		fputc(c,stdout);
	}

}

void adv_print(FILE *f)  {

	err(f,"Failed to open file\n");

  int c;
  int i;

  int numline = 0;
  if (LINE_NUM) {
    i = 1;
    numline = 1;
  }

  while((c = fgetc(f)) != EOF) {

    if (numline == 1) {
        printf("%6d ",i);
        numline = 0;
    }
  
    if(c == '\t' && (TAB || ALL)) {
      printf("^I");
    }
    else if (c == '\n') {

      if (LINE_ENDS || ALL) {
        printf("$");
        (LINE_NUM) ? 0 : printf("\n"); // 0 - NULL
      }
      if (LINE_NUM) {
        i++;
        fputc(c, stdout);
        numline = 1;
      }
      if(!LINE_NUM && !LINE_ENDS) {
        fputc(c, stdout); 
      }
    }
    else {
      fputc(c, stdout);
    }

  }

}

void check_file(char *file_path, FILE *f) {


	// checks if the file exists
	int exists = access(file_path, F_OK);
	err((exists == 0), "Unable to find file %s\n", file_path);

	// checks if the given path is a dir
	int isDir = isdirectory(file_path);	
	err((isDir == 0), "%s is a directory\n",file_path)

	// checks if the user has read acces to the file
	int read = access(file_path, R_OK);
	err((read == 0), "Unable to read file %s (Do you have the read access?)\n", file_path);


	// checks if the file pointer is null
	err(f, "Unable to open file %s\n", file_path);

}

int isdirectory(char *path) {
	struct stat statbuf;

	if (stat(path, &statbuf) == -1) {
		perror("while calling stat()");
		return -1;

	} else {
		return S_ISDIR(statbuf.st_mode);

	}
	   
}
