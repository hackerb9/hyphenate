#define _GNU_SOURCE		/* GNU basename() and asprintf() */
#include <string.h>
#include <stdlib.h>		/* wcstombs(), etc */
#include <stdio.h>
#include <ctype.h>
#include <locale.h>		/* Allow use of UTF-8, etc */
#include <wchar.h>		/* Wide characters, mbrinit, mbsrtowc */
#include <wctype.h>		/* For wtolower() */

#include "hyphen.h"

#define BUFSIZE 1000

char *progname;			/* short program name, set by main */
void help() {
  printf("Usage:\n"); 
  printf("%s [-o | -d | -n] <word ...>\n", progname);
  printf("\t-o = use old algorithm (without non-standard hyphenation)\n");
  printf("\t-d = hyphenation with listing of the possible hyphenations\n");
  printf("\t-n = print hyphenation vector\n");
}

/* get a pointer to the nth 8-bit or UTF-8 character of the word */
char * hindex(char * word, int n, int utf8) {
  int j = 0;
  while (j < n) {
    j++;
    word++;
    while (utf8 && ((((unsigned char) *word) >> 6) == 2)) word++;
  }
  return word;
}

/* list possible hyphenations with -d option (example for the usage of the hyphenate2() function) */
void single_hyphenations(char * word, char * hyphen, char ** rep, int * pos, int * cut, int utf8) {
  int i, k, j = 0;
  char r;
  for (i = 0; (i + 1) < strlen(word); i++) {
    if (utf8 && ((((unsigned char) word[i]) >> 6) == 2)) continue;
    if ((hyphen[j] & 1)) {
      if (rep && rep[j]) {
	k = hindex(word, j - pos[j] + 1, utf8) - word;
	r = word[k];
	word[k] = 0;
	printf(" - %s%s", word, rep[j]);
	word[k] = r;
	printf("%s\n", hindex(word + k, cut[j], utf8));
      } else {
	k = hindex(word, j + 1, utf8) - word;
	r = word[k];
	word[k] = 0;
	printf(" - %s=", word);
	word[k] = r;
	printf("%s\n", word + k);
      }
    }
    j++;
  }
}

int 
main(int argc, char** argv)
{
  HyphenDict *dict;
  int k, i, j;
  int  nHyphCount;
  char *hyphens;
  char *lcword;
  char *hyphword;
  char hword[BUFSIZE * 2];
  int arg = 1;
  int optd = 1;
  int optn = 0;
  int optdd = 0;
  char ** rep;
  int * pos;
  int * cut;

  /* what name to show for usage message */
  progname=strdup(basename(argv[0]));

  /* first parse the command line options */
  if (argv[arg]) {
    if (strcmp(argv[arg], "-o") == 0) {
      optd = 0;
      arg++;
    }
    if (strcmp(argv[arg], "-n") == 0) {
      optn = 1;
      arg++;
    }
    if (argv[arg] && strcmp(argv[arg], "-d") == 0) {
      optd = 1;
      optdd = 1;
      arg++;
    }
  }

  if (!argv[arg]) {
    help();
    exit(1);
  }


  char *locale;
  locale=setlocale(LC_CTYPE, ""); /* Allow multibyte characters, UTF-8 */

  /* Find default dictionary file based on locale */
  if (locale==NULL || strlen(locale)==0) {
    locale=strdup("en_US");	/* Default to US English */
  }
  else {
    locale=strdup(locale);
  }
  char *end=strstr(locale, "."); /* Find period in "en_US.UTF-8" */
  if (end) *end='\0';		 /* Truncate to just "en_US" */

  char *dictfile;
  asprintf(&dictfile, "/usr/share/hyphen/hyph_%s.dic", locale);

  /* load the hyphenation dictionary */  
  if ((dict = hnj_hyphen_load(dictfile)) == NULL) {
    /* First guess failed. Try again. */
    fprintf(stderr,
	    "Could not load dictionary file based on locale \"%s\"\n",
	    locale);
    perror(dictfile);
    free(dictfile);
    asprintf(&dictfile, "/usr/share/hyphen/hyph_%s.dic", "en_US");
    fprintf(stderr, "Falling back to \"%s\"\n", dictfile);
    if ((dict = hnj_hyphen_load(dictfile)) == NULL) {
      /* Nope */
      fprintf(stderr,
	      "Couldn't load that, either. Please install hyphen-en-us\n");
      perror(dictfile);
      fflush(stderr);
      exit(1);
    }
  }


  wchar_t word[BUFSIZE];
  while (arg<argc) {
    const char *src = argv[arg++];
    mbstate_t state = { 0 };

    /* libhyphen works with multibyte (UTF-8) strings, but to
       lowercase we need to convert to a wide-character string. */
    if (mbsrtowcs(word, &src, BUFSIZE, &state) == -1) {
      perror(src);
    }
    
    k = wcslen(word);		/* Length as wide-character string.  */

    /* Chomp */
    if (k && word[k - 1] == '\n') word[k - 1] = '\0';
    if (k >=2 && word[k - 2] == '\r') word[k-- - 2] = '\0';

    /* Lowercase the wide-character string */
    for (i = 0; i < k; ++i) {
      word[i] = towlower(word[i]);
    }

    /* Convert wide-character back to multibyte string */
    k=wcstombs(NULL,word,0);	      /* Now k is length as multibyte string */
    lcword = (char *) malloc(k+1); 
    if (wcstombs(lcword, word, k+1) == -1) {
      perror("wcstombs could not convert back to multibyte string.");
    }

    /* Set aside a buffer to hold hyphen information */
    hyphens = (char *)malloc(k+5);			 /* Why 5? */

    /* now actually try to hyphenate the word */
    rep = NULL;
    pos = NULL;
    cut = NULL;
    hword[0] = '\0';

    if ((!optd && hnj_hyphen_hyphenate(dict, lcword, k, hyphens)) ||
	(optd && hnj_hyphen_hyphenate2(dict, lcword, k, hyphens, hword, &rep, &pos, &cut))) {
      free(hyphens);
      free(lcword);
      fprintf(stderr, "hyphenation error\n");
      exit(1);
    }

    if (optn) fprintf(stderr, "%s\n", hyphens); 

    if (!optd) {
      /* now create a new char string showing hyphenation positions */
      /* count the hyphens and allocate space for the new hypehanted string */
      nHyphCount = 0;
      for (i = 0; i < k; i++)
	if (hyphens[i]&1)
	  nHyphCount++;
      hyphword = (char *) malloc(k+1+nHyphCount);
      j = 0;
      for (i = 0; i < k; i++) {
	hyphword[j++] = word[i];
	if (hyphens[i]&1) {
	  hyphword[j++] = '-';
	}
      }
      hyphword[j] = '\0';
      fprintf(stdout,"%s\n",hyphword);
      fflush(stdout);
      free(hyphword);
    } else {
      fprintf(stdout,"%s\n", hword);


      if (optdd) single_hyphenations(lcword, hyphens, rep, pos, cut, dict->utf8);
      if (rep) {
	for (i = 0; i < k - 1; i++) {
	  if (rep[i]) free(rep[i]);
	}
	free(rep);
	free(pos);
	free(cut);
      }
    }
    free(hyphens);
    free(lcword);
  }

  hnj_hyphen_free(dict);
  return 0;
}
