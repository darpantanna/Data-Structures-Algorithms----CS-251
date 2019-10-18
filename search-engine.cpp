
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "search-engine.h"
#include "webcrawl.h"
#define maxL 3000

DictionaryType d;
//hrtime_t average;
int ct = 0;

SearchEngine::SearchEngine( int port, DictionaryType dictionaryType):
  MiniHTTPD(port)
{
  // Create dictionary of the indicated type
    if(dictionaryType == ArrayDictionaryType) {
        //ArrayDictionaryNode array = new ArrayDictionaryNode();
        _wordToURLList = new ArrayDictionary();
    }
    else if(dictionaryType == HashDictionaryType) {
        _wordToURLList = new HashDictionary();
    }
    else if(dictionaryType == AVLDictionaryType) {
        _wordToURLList = new AVLDictionary();
    }
    else if(dictionaryType == BinarySearchDictionaryType) {
        _wordToURLList = new BinarySearchDictionary();
    }
    else {
        _wordToURLList = NULL;
    }

    //_wordToURLList = new ArrayDictionary();

    d = dictionaryType;
    // Populate dictionary and sort it if necessary
    URLRecord ** r = new URLRecord * [1024];
    int i = 0;
    while(i < 1024) {
      r[i] = new URLRecord();
      i++;
    }

    FILE * fd = fopen("url.txt", "r"); // Read url.txt for urls
    char * c = new char [maxL];

    while(fgets(c, maxL, fd)) {
      if(strcmp(c, "\n") == 0) {
        continue;
      }
      else {
        char * tok = new char[maxL]; //Seperate index
        tok = strtok(c, " \n");
        int j = atoi(tok);

        tok = strtok(NULL, " \n"); //Seperate url
        char * u = new char[maxL];
        strcpy(u, tok);

        fgets(c, maxL, fd); //Seperate description
        char * d = new char[maxL];
        tok = strtok(c, "\n");
        strcpy(d, tok);

        r[j]->_url = strdup(u); //Store in urlArray
        r[j]->_description = strdup(d);
      }
    }

    delete c; //Free space
    fclose(fd);

    //GET words


    fd = fopen("word.txt", "r");
    char * w = new char[maxL];
    URLRecordList * begin = NULL;
    URLRecordList * previous = NULL;

    while(fgets(w, maxL, fd)) {
      if(strcmp(w, "\n") == 0) {
        continue;
      }
      else {
        begin = NULL;
        char * tok = new char[maxL];
        tok = strtok(w, " \n"); //Seperate word

        char * word = new char [maxL];
        strcpy(word, tok); //Copy word

        tok = strtok(NULL, " \n");
        // printf("Word: %s, tok = %s\n", word, tok);

        while(tok != NULL) {
          //Add word to word list
          int current = atoi(tok);
          URLRecordList * e = new URLRecordList();
          if(r[current]->_url == NULL) {
            continue;
          }

          e->_urlRecord = r[current];
          e->_next = NULL;
          // printf("Word: %s, URL = %s\n", word, e -> _urlRecord -> _url);

          if(begin == NULL) {
            begin = e;
            previous = e;
          }
          else if(previous != NULL) {
            previous->_next = e; // Set previous->next pointer to current
            previous = e;
          }

           // Set previous to current
          tok = strtok(NULL, " \n");
          // printf("Word: %s, tok = %s\n", word, tok);
        }

        _wordToURLList->addRecord(word, (URLRecordList *) begin); //Add to table
        // printf("Word: %s\n", word);
        URLRecordList* tmp = (URLRecordList*) begin;
        while(tmp != NULL)
        {
          // printf("Word: %s, URL: %s\n", word, tmp -> _urlRecord -> _url);
          tmp = tmp -> _next;
        }
        delete word; //Free memory for word
        delete tok; //delete buffer
      }
    }
    // delete c;
    fclose(fd);
}

int SearchEngine::check(char * &check) {
  if(!strncmp(check, "/search?word=", 13)) {
      check = check +  13;
      return 1;
  }
  return 0;
}

void
SearchEngine::dispatch( FILE * fout, const char * documentRequested)
{
  if (strcmp(documentRequested, "/")==0) {
    // Send initial form
    fprintf(fout, "<TITLE>CS251 Search</TITLE>\r\n");
    fprintf(fout, "<CENTER><H1><em>Boiler Search</em></H1>\n");
    fprintf(fout, "<H2>\n");
    fprintf(fout, "<FORM ACTION=\"search\">\n");
    fprintf(fout, "Search:\n");
    fprintf(fout, "<INPUT TYPE=\"text\" NAME=\"word\" MAXLENGTH=\"80\"><P>\n");
    fprintf(fout, "</H2>\n");
    fprintf(fout, "</FORM></CENTER>\n");
    return;
  }

  // TODO: The words to search in "documentRequested" are in the form
  // /search?word=a+b+c
  //
  // You need to separate the words before search
  // Search the words in the dictionary and find the URLs that
  // are common for all the words. Then print the URLs and descriptions
  // in HTML. Make the output look nicer.

  // Here the URLs printed are hardwired
  char  * args = new char[strlen(documentRequested) + 1];
  int ctr = 0;
  int lC = 0;
  //documentRequested has to have more have 13 characters to perform search
  if(strlen(documentRequested) < 13 ) {
    return;
  }
  strcpy(args, documentRequested + 13);


  const int nurls = 2;

  char ** words =  new char*[50];//"data structures";

  int i = 0;
  while(i < 50) {
    words[i] == NULL;
      i++;
  }

  char * word = strtok(args, "+");
  int n = 0;

/*  if(check(args) == 1) {
    for(int i = 0; )
  }*/

  while(word != NULL) {
    //word[n] = new char[25];
    words[n] = strdup(word);
    n++;
    word = strtok(NULL, "+");

  }

  char * q = new char [25*100]; //Create request
  strcpy(q, "");
  i = 0;
  while(i < n) {
    strcat(q, words[i]);
    if(i+1 != n) {
      strcat(q, " ");
    }
    i++;
  }
  /*const char * urls[] = {
    "http://www.cs.purdue.edu",
    "http://www.cs.purdue.edu/homes/cs251"
  };

  const char * description[] = {
    "Computer Science Department. Purdue University.",
    "CS251 Data Structures"
  };*/

  URLRecord ** l = new URLRecord * [500];
  int in = 0;
  int k = 0;
  int check = 0;
  int check2 = 1;
  // while(k < n) {
    // check = 0;
    // check2 = 1;
    URLRecordList * u = (URLRecordList *) _wordToURLList->findRecord(words[k]);
    // printf("Words[0] = %s\n", words[0]);
    URLRecordList * v = NULL;
    while(u != NULL) {
      check2 = 1;
      for(int j = 1; j < n; j++) {
        // check = 0;
        v = (URLRecordList *) _wordToURLList->findRecord(words[j]);
        // printf("Words[%d] = %s\n", j, words[j]);
        while(v != NULL)
        {
          if(strcmp(v -> _urlRecord -> _url, u ->_urlRecord -> _url ) == 0) {
            check = 1;
          }
          v = v -> _next;
        }
        if(check == 0)
        {
          check2 = 0;
        }
        check = 0;
      }
      if(check2 == 1) {
        l[in++] = u->_urlRecord;
      }
      check = 0;
      check2 = 1;
      u = u->_next;
    }
    // k++;
  // }
  // for(k = 0; k < in; k++)
  // {
  //   printf("URL: %s\n", l[k] -> _url);
  // }
  //hrtime_t t1;
  //hrtime_t t2;
/* k = 0;
  check = 0;
  while(k < in) {
    check = 0;
    int s = 0;
    while(s < n) {
      URLRecordList * u = (URLRecordList *)_wordToURLList->findRecord(words[s]);
      while(u != NULL) {
        if(u->_urlRecord == l[i]) {
          check = 1;
        }
        u = u->_next;
      }
      if(check == 0) {
        l[k] = NULL;
      }
      s++;
    }
    k++;
  }

*/
	/*if (d == ArrayDictionaryType)
		fprintf(fout, "<h3>Dictionary Used: Array Dictionary</h3>\n");
	else if (d == HashDictionaryType)
		fprintf(fout, "<h3>Dictionary Used: Hash Dictionary</h3>\n");
	else if (d == AVLDictionaryType)
		fprintf(fout, "<h3>Dictionary Used: AVL Dictionary</h3>\n");
	else if (d == BinarySearchDictionaryType)
		fprintf(fout, "<h3>Dictionary Used: Binary Search Dictionary</h3>\n");
	else
		fprintf(fout, "<h3>Dictionary Used: Unknown Dictionary</h3>\n");*/



    fprintf( stderr, "Search for words: \"%s\"\n", q);

    fprintf( fout, "<TITLE>Search Results</TITLE>\r\n");
    fprintf( fout, "<H1> <Center><em>Boiler Search</em></H1>\n");
    fprintf( fout, "<H2> Search Results for \"%s\"</center></H2>\n", q);
    printf("\nin: %d\n\n", in);

    for ( int i = 0; i < in; i++ ) {
      if(l[i] == NULL) {
        continue;
      }
      printf("Check\n");
      fprintf( fout, "<h3>%d. <a href=\"%s\">%s</a><h3>\n", i+1, l[i]->_url, l[i]->_url);
      fprintf( fout, "<blockquote>%s<p></blockquote>\n", l[i]->_description );
    }

  // Add search form at the end
  fprintf(fout, "<HR><H2>\n");
  fprintf(fout, "<FORM ACTION=\"search\">\n");
  fprintf(fout, "Search:\n");
  fprintf(fout, "<INPUT TYPE=\"text\" NAME=\"word\" MAXLENGTH=\"80\"><P>\n");
  fprintf(fout, "</H2>\n");
  fprintf(fout, "</FORM>\n");
}

void
printUsage()
{
  const char * usage =
    "Usage: search-engine port (array | hash | avl | bsearch)\n"
    "  It starts a search engine at this port using the\n"
    "  data structure indicated. Port has to be larger than 1024.\n";

  fprintf(stderr, "%s", usage);
}

int main(int argc, char ** argv)
{
  if (argc < 3) {
    printUsage();
    return 1;
  }

  // Get port
  int port;
  sscanf( argv[1], "%d", &port);

  // Get DictionaryType
  const char * d = argv[2];
  DictionaryType dictionaryType;
  if (!strcmp(d, "array")) {
    dictionaryType = ArrayDictionaryType;
  }
  else if (!strcmp(d, "hash")) {
    dictionaryType = HashDictionaryType;
  }
  else if (!strcmp(d, "avl")) {
    dictionaryType = AVLDictionaryType;
  }
  else if (!strcmp(d, "bsearch")) {
    dictionaryType = BinarySearchDictionaryType;
  }
  else {
    printUsage();
    return 0;
  }

  SearchEngine httpd(port, dictionaryType);

  httpd.run();

  return 0;
}
