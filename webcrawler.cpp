
#include "webcrawler.h"
#include "string"
// Add your implementation here
//int isIn;
using std::string;
int TableSize = 2039;
char description[500];
int dIndex;

// Create a web crawler object with the initial urls
WebCrawler::WebCrawler(int maxUrls, int nurlRoots, const char ** urlRoots) {
	_urlArray = new URLRecord[maxUrls];
	_headURL = 0;
	_tailURL = 0;
	_maxUrls = maxUrls;
	_urlToUrlRecord = new HashTableTemplate<int>();
	_wordToURLRecordList = new HashTableTemplate<URLRecordList*>();

	int count = 0;
	while(urlRoots[count] != NULL) {
		URLRecord * record = new URLRecord();
		record->_url = strdup(urlRoots[count]);
		_urlArray[_tailURL] = *record;
		_tailURL++;
		_urlToUrlRecord->insertItem(record->_url, count);
		//a
	//printf("%s\n", urlRoots[count]);
	count++;
	}
}

//crawl the web
void
WebCrawler::crawl() {
	int count;
	while(_headURL < _tailURL) {
		char * current = _urlArray[_headURL]._url;
		char * buff = fetchHTML(current, &count);
			if(buff == NULL) {
			continue;
		}

			dIndex = 0;
			parse(buff, count);
			if(dIndex == 0) {
				//Ignore
			} else {
				description[dIndex-1] = '\0';
			}
			_urlArray[_headURL]._description = strdup(description);
			//printf("%s\n", description );
			//word = NULL;
			//desc = NULL;
			_headURL++;

		}
}

// Write array of URLs and descriptions to file
void
WebCrawler::writeURLFile(const char * urlFileName) {
	FILE * fd = fopen(urlFileName, "w");
	int i = 0;
	while(i <_tailURL) {
		fprintf(fd, "URL %d: %s\n", i, _urlArray[i]._url);
		if(_urlArray[i]._description == NULL) {
			fprintf(fd, "%s\n", "NO DESCRIPTION");
		}
		else {
			fprintf(fd, "%s\n", _urlArray[i]._description);
		}
		fprintf(fd, "\n");
		i++;
	}
	fclose(fd);
}

// Write list of words with their urls to file
void
WebCrawler::writeWordFile(const char *wordFileName) {
		int i = 0;
		while(i  < _tailURL) {
			char * buffer = _urlArray[i]._description;
			char * word;
			word = strtok(buffer, "., ");
			while(word != NULL) {
				//printf("%s\n", word);
				//Add word;
				URLRecordList * record;
				URLRecordList * prevRecord;
				if(_wordToURLRecordList->find(word, &record)) {
						while(record != NULL && record->_urlRecordIndex != i) {
							prevRecord = record;
							record = record->_next;
						}
						if(record == NULL) {
							 record = new URLRecordList();
							 record ->_urlRecordIndex=i;
							 record ->_next = NULL;
							 prevRecord->_next = record;
						}
				} else {
					record = new URLRecordList();
					record ->_urlRecordIndex=i;
					record ->_next = NULL;
					_wordToURLRecordList->insertItem(word, record);
				}



				word = strtok(NULL, "., ");
			}
			i++;

		}
		FILE * fd = fopen(wordFileName, "w");
		HashTableTemplateIterator<URLRecordList *> iterator(_wordToURLRecordList);
		const char * key;
		URLRecordList * record;
		while(iterator.next(key, record)) {
				fprintf(fd, "\n%s ", key);
				while(record != NULL) {
						fprintf(fd," %d", record->_urlRecordIndex);
						record = record->_next;
				}

		}

		fclose(fd);
}

void
WebCrawler::onContentFound(char ch) {
	if(dIndex < 500 && ch != '\n') {
		description[dIndex] =ch;
		dIndex++;
	}
}
void
WebCrawler::onAnchorFound(char * url) {
	//printf("%s\n", url);
	//Check if actual url
	if(_tailURL == _maxUrls) {
		 return;
	}
	if (!string(url).substr(0,4).compare("http")==0) {
		return;
	}

	//It is a url
	//Add it to the array
	//if it does not exist
	int * num = new int();
	if(_urlToUrlRecord->find(url, num)) {
		//url already exists
		return;
	}
	URLRecord * record = new URLRecord();
	record->_url = strdup(url);
	_urlArray[_tailURL] = *record;
	_urlToUrlRecord->insertItem(record->_url, _tailURL);
	printf("%s\n", url);
	_tailURL++;
	return;
}

int
main(int argc, char **argv) {
	if(argc <=1) {
		fprintf(stderr, "  Usage: webcrawler [-u <maxUrls] url-list\n");
		exit(1);
	}
	int isIn = 0;
	int maxUrls = 1000;
	int begin = 1;
	const char ** urlRoots;

	if(strcmp("-u", argv[1]) == 0) {
		maxUrls = atoi(argv[2]);
		begin = 3;
	}

	urlRoots = new const char *[(argc - begin)];
	for(int i = begin; i < argc; i++) {
		urlRoots[i - begin] = argv[i];
	}
	WebCrawler wc(maxUrls, argc-begin, urlRoots);

	wc.crawl();

	wc.writeURLFile("url.txt");
	wc.writeWordFile("word.txt");
}
