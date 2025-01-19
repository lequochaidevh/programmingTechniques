#include <stdio.h>
#include <stdarg.h>
#include"debug_macro.h"// 
#include <string.h>
#include <ctype.h>

#define MAX_LINK_LENGTH (100)
#define MAX_NUM_LINK (20)

#define N_DUMMY_LINKS (10)
#define N_DUMMY_RESULT_SITE (3)

//
// Dummy result link List from google 
//
const char* DUMMY_RESULT_LINKS[] =
{
    #include"resultLinks.txt"
}; 

//
// Dummy of many site have sample source
//
const char* DUMMY_HAS_SOURCE_CODE_SITE[]=
{
    "www.stackoverflow.com",
    "www.kipalog.com",
    "www.facebook1.com"
};

//
// seach for key word in google, This will return the result, which is link list.
//


void search_google(char* keyword, char result[MAX_NUM_LINK][MAX_LINK_LENGTH], int *nResults)
{
    int i = 0;
    int numbResult = 0;

    for (i = 0; i < N_DUMMY_LINKS; i++)
    {
    	log_debug("Search google : (keyword=%s) START \n", keyword);
    	if(strstr(DUMMY_RESULT_LINKS[i], keyword) != NULL) {
    	        strcpy(result[numbResult], DUMMY_RESULT_LINKS[i]);
    	        numbResult++;
    	}
    }
    *nResults = numbResult;
    log_debug("Search google : (nResults=%d) END \n", *nResults);

}



//
// run source sample by a link pagrameter
//
int runSource(char* link)
{
    int retVal = 0;
    int i;
    for (i = 0; i < N_DUMMY_RESULT_SITE; i++)
    {
        if (strcmp(link, DUMMY_HAS_SOURCE_CODE_SITE[i]) == 0)
    	{
        	log_debug("OK, It works!!!! At: %d \n", i+1);
        	retVal ++;
    	}
    	else
    	{
    	    log_err("Hmm, Bad source (link=%s) \n",link);
    	    //retVal = 0;
    	}
    }
    

    return retVal;
}

//
// Check link have sample source.
//
int hasSampleSource(char* link)
{
    int i = 0;
    int retVal = 0;

    log_debug("Check source (link=%s) START \n",link);
    for (i = 0; i < N_DUMMY_RESULT_SITE; i++)
    {
        if (strstr(DUMMY_HAS_SOURCE_CODE_SITE[i],link) != NULL)
        {
            retVal = 1; 
            printf("Have source (link=%s) END \n",link);
            break;
        }
    }
    

    return retVal;
}


int main(int argc, char* argv[])
{
    char *keyword;
    char results[MAX_NUM_LINK][MAX_LINK_LENGTH];
    int nResults;
    int i = 0;

    // Check pagram
    if (2 != argc)
    {
        log_err("Invalid parameter, usage: %s <keyword>\n",argv[0]);
        return 0;
    }

    // Get keyword
    keyword = argv[1];

    // Find in google
    search_google(&keyword[0], results, &nResults);
    
    if (nResults > 0)// If have results
    {
        // find throuth out link result
        for (i = 0; i < nResults; i++)
        {
            log_debug("Try (link=%s) \n",results[i]);
            if (hasSampleSource(results[i])) // Check: have sample source? 
            {
                // Run source sample
                runSource(results[i]);
            }
            else
            {
                log_err("No source in link :%s\n", results[i]);
            }
        }
    }
    else{
        log_err("Keyword is so difficult (keyword=%s) \n", keyword);
    }

    return 0;
}

/*
	gcc program.c
	./a.out facebook
*/


