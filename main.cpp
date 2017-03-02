#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include <string.h>
#include <ctype.h>
#include "rlutil.h"
#include <time.h>

typedef long long ll;

using namespace std;

string words[31][30000]; //words[number_of_characters][]
int start_pts[31][26]; //Stores starting point(index)

int aff[26] = {0};        // aff[i] = affinity of letter (char) 97+i i.e. a-z
int max_letters = 0;      // no. of letters in word

int spool(int = 0, string = "", string = "");
void init(void);
bool check(string a, ll start_pt, ll end_pt, string dict[]);

int main(void){

	rlutil::cls();

		//////////////////////////////////
		//	Input

        init();

		start:
		cout << "Enter letters in succession (press enter to end input) : \n";

		for(int i=0; i<1;  ){
			char inp;
			inp = getch();
				if(inp == '\n' || inp == '\r'){
					break;
				}
                if(!isalpha(inp)){
                    cout << "\n" << inp <<" - Input skipped\n";  	 // Ignores any invalid input
                }
				else{
                    cout << inp << ' ';
                    inp = tolower(inp);
					aff[inp-97]++;
				}
		}

		cout << "\n ";

		for(int k=0; k<26; k++){
            max_letters += aff[k];
		}

        if(max_letters==0){
            cout << "No valid input\n";
            getch();
            goto start;
        }

		cout << "\nPossible formed words :" << endl << endl;

	
		clock_t t1 = clock();
		spool();
		clock_t t2 = clock();
		float diff = ((float)t2-(float)t1)/CLOCKS_PER_SEC;
    	cout<< endl << "Time taken for generation: " << diff << "s" <<endl;
		
		cout << endl << endl;

	return 0;
}

int spool(int rc, string m, string mn){            	/* rc = run-count, signifies (n+1)th recursion
                                                       of function, printing (n+1)th digit
                                                    */
	if(rc==max_letters)
	{
        int len = strlen(m.c_str());
        
        char start_char = m[0];
        int index_start_char = start_pts[len-1][(int)(start_char-'a')];
        int index_next_char = start_pts[len-1][(int)(start_char-'a' + 1)];
        
        bool b = check(m, index_start_char, index_next_char - 1, words[len-1]);
        if(b)
        {
        	cout << m << ' ';
        }
	}
	else{
		for(int t = 0; t<26; t++){
			if(aff[t]!=0){
				aff[t]--;
				mn = m;
				m += (char)(t+97);
				rc++;

				rc = spool(rc, m, mn);
				m = mn;                             /* Returns the number to value before printing next digit
                                                            For example,
                                                                for letters A B C D
                                                                passes AB
                                                                prints ABC
                                                                might need to print ABD as well,
                                                                so, returns AB to next recursion
                                                    */

				aff[t]++;
			}
		}
	}
	rc--;                                           // When function completes, returns to previous recursion with lesser run count
	return rc;
}

/*void check(string a, string dict[])
{
	ll total = start[26];
	bool b = check(a, 0, total-1,);
	if(b)
	{
		cout << a << ' ';
	}
}*/


bool check(string a, ll start, ll end, string dict[])
{
	if(start > end)
	{
		return false;
	}
	ll mid_pt = (start + end)/2;
	int result = strcmp(a.c_str(), dict[mid_pt].c_str());
	if(result == 0)
	{
		return true;
	}
	else if(result < 0)
	{
		return check(a, start, mid_pt - 1, dict);
	}
	else
	{
		return check(a, mid_pt + 1, end, dict);
	}
}


void init(void)
{
    ifstream dict ("dict.txt", ios_base::in);
    
    ll words_index[30000];    
    for(int i = 0; i < 30000; i++)
    {
    	words_index[i] = 0;
    }
    
    char current[31];
    for(int i = 0; i < 31; i++)
    {
    	start_pts[i][0] = 0;
    	current[i] = 'a';
    }
	
    for(int i=0; i<200000; i++)
    {
        string h;
        getline(dict, h);
        
        int len = strlen(h.c_str());
        
        if(h[0] == (char) (current[len-1]+1))
        {
        	current[len-1]++;
        	start_pts[len-1][(int) (current[len-1] - 'a')] = words_index[len - 1];
        }

        if(strncmp(h.c_str(), "*", 1)== 0 || dict.eof())
        {
            break;
        }
        else
        {
            words[len-1][words_index[len-1]] = h;
            words_index[len-1]++;
        }
    }
}
