#include <stdio.h>
#include <unistd.h> // for getopt
#include <getopt.h>
#include <stdlib.h> // for abort() exit() 
#include <string.h> // for strcmp()

#include <sys/stat.h> // for dir check

// An error macro
#define err(A, MSG, ...) if(!A) {\
	fprintf(stderr,"[ERROR] " MSG, ##__VA_ARGS__);\
	exit(1); }

#define KAT_VERSION "0.7"

// to fix few std C99 problem
int getopt(int argc, char *const argv[], const char *optstring);

void print_help(char *exec_name); 
void print_version();

int file_func(char *fname); // this will call the below 2 func
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

	int c;

	while(1) {

    static struct option long_option[] = {
    
      {"version", no_argument, NULL, 'v'},
      {"help", no_argument, NULL, 'h'},
      {"show-ends", no_argument, NULL, 'e'},
      {"show-tabs", no_argument, NULL, 't'},
      {"show-all", no_argument, NULL, 'A'},
      {"number", no_argument, NULL, 'n'},
      {NULL, 0, NULL, 0}
    
    };

    int option_index = 0;

    c = getopt_long(argc, argv, "hentAv", long_option, &option_index);

    if (c == -1)
      break;

		switch(c) {

      case 0:
        if (long_option[option_index].flag != 0)
          break;
        printf("Option: %s", long_option[option_index].name);
        if (optarg)
         printf(" With arg %s", optarg);
       printf("\n");
       break; 

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
        break;
			default:
				printf("Wot??\n");
				abort();

		}

	}

  if (optind < argc) {
	  char *fname;
    bool pstdin = FALSE; // print from stdin only once

    while(optind < argc) {
    
      fname = argv[optind++];

      if ((strcmp(fname, "-") == 0) && !pstdin) {
      
        print_stdout(stdin);

      }
      else {
        FILE *f = fopen(fname, "r");
        check_file(fname, f);

        if (TAB || LINE_NUM || LINE_ENDS || ALL) {
          adv_print(f);
        }
        else {
          print_stdout(f);   
        }

        fclose(f);
      
      }


    }

  }
  else {
    print_stdout(stdin);
    return 0;
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

  int numline = 0; //to print number in front of a line if equal to 1
  if (LINE_NUM) {
    i = 1;
    numline = 1;
  }

  while((c = fgetc(f)) != EOF) {

    if (numline == 1) {
        printf("%6d\t",i);
        numline = 0;
    }
  
    if(c == '\t' && (TAB || ALL)) {
      printf("^I");
    }
    else if (c == '\n') {

      if (LINE_ENDS || ALL) {
        printf("$");

        // do not print new line if LINE_NUMS is enabled
        // since LINE_NUMS prints \n 
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
      fputc(c, stdout); // default case, just print the character
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
