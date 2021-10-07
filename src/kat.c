/*
 * This project is libre, and licenced under the terms of the
 * DO WHAT THE FUCK YOU WANT TO PUBLIC LICENCE, version 3.1,
 * as published by dtf on July 2019. See the COPYING file or
 * https://ph.dtf.wtf/w/wtfpl/#version-3-1 for more details.
 */


#include <stdio.h>
#include <unistd.h> // for getopt
#include <getopt.h>
#include <stdlib.h> // for abort() exit() 
#include <string.h> // for strcmp()

//#include <sys/stat.h> // for dir check
#include <errno.h>

#include "KatConfig.h" // CMake config

// An error macro
#define err(A, ERRNUM, MSG, ...) if(!A || ERRNUM) {\
  fprintf(stderr,"[ERROR] " MSG ": %s\n", ##__VA_ARGS__, strerror(ERRNUM));\
  exit(1); }

// to fix few std C99 problem
int getopt(int argc, char *const argv[], const char *optstring);

void print_help(char *exec_name); 
void print_version();

void print_stdout(FILE *f,char *file_name); // prints the file to stdout
void adv_print(FILE *f,char *file_name); // for more options

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

  bool pstdin = FALSE; // print from stdin only once

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
        exit(0);
      case 'h':
        print_help(argv[0]);
        exit(0);
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

  // if args are given
  if (TAB || LINE_NUM || LINE_ENDS || ALL) {
    
    // print the files with the args
    if(optind < argc) {
      char *fname;

      while (optind < argc) {
        fname = argv[optind++];

        // - => Take input from stdin only once
        if ((strcmp(fname,"-")) == 0 && !pstdin) {
          adv_print(stdin,"stdin");
          pstdin = TRUE;
        }
        else {

          FILE *f = fopen(fname, "r");
          err(f,errno,"%s",fname);
          //check_file(fname, f);

          adv_print(f, fname);

          fclose(f);

        }

      }
    }
    // if no files are given, print from stdin
    else {
      adv_print(stdin,"stdin"); 
      err(0,errno, "stdin");
    }


  }
  // no options are given
  else {
    // print the files
    if (optind < argc) {
      char *fname;

      while(optind < argc) {
        fname = argv[optind++];

        if ((strcmp(fname,"-")) == 0 && !pstdin) {
          print_stdout(stdin, "stdin");
          err(0,errno,"stdin");
          pstdin = TRUE;
        }
        else {

          FILE *f = fopen(fname, "r");
          err(f,errno,"%s",fname);

          print_stdout(f, fname);

          fclose(f);

        }

      }
    } 
    // print from stdin
    else {
      print_stdout(stdin, "stdin");
      err(0, errno, "stdin");
    }

  }

  return 0;
}

void print_version() {
  printf("Kat(v" TO_STRING(KAT_VERSION)") - Print files to stdin\n");
  printf("\nCopyright(C) WTFPL 3.1\n");
  printf("This project is libre, and licenced under the terms of the\n"
         "DO WHAT THE FUCK YOU WANT TO PUBLIC LICENCE, version 3.1,\n"
         "as published by dtf on July 2019. See the COPYING file or\n"
         "https://ph.dtf.wtf/w/wtfpl/#version-3-1 for more details.\n");
  printf("\nWritten By: me\n");
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

void print_stdout(FILE *f,char *file_name) {

  int c;

  while(TRUE) {
    c = fgetc(f); // get char

    // test
    err(c,errno,"%s",file_name);
    
    // break at end of file
    if(c == EOF) break;

    // print the char
    fputc(c,stdout);
  }

}

void adv_print(FILE *f,char *file_name)  {

  int c;
  int i;

  int numline = 0; //to print number in front of a line if equal to 1
  if (LINE_NUM) {
    i = 1;
    numline = 1;
  }

  while(TRUE) {
    c = fgetc(f);

    err(c, errno, "%s",file_name);

    // break at end of file
    if(c == EOF) break;


    if (numline == 1) {
      printf("%6d\t",i);
      numline = 0;
    }

    if(c == '\t' && (TAB || ALL)) {
      printf("^I"); // tab char
    }
    else if (c == '\n') {

      if (LINE_ENDS || ALL) {
        printf("$");

        // do not print new line if LINE_NUMS is enabled
        // since LINE_NUMS prints \n 
        (numline == 1 || LINE_NUM == TRUE) ? 0 : printf("\n"); // 0 - NULL
      }
      if (LINE_NUM) {
        i++;
        //(!LINE_ENDS) ? 0 : fputc(c, stdout);
        (LINE_ENDS) ? 0 : printf("\n"); 
        numline = 1;
      }
      
      if((!LINE_NUM && !LINE_ENDS) && !ALL) {
        fputc(c, stdout);  // print the char if not in the start or ending
      }
      
    }
    else {
      fputc(c, stdout); // default case, just print the character
    }

  }

}

