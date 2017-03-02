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

struct dict_word{
    char word[100];
};

dict_word enigma[200000];

int aff[26] = {0};        // aff[i] = affinity of letter (char) 97+i i.e. a-z
int start[27] = {0};      // start[i] = line where letter first appears (char) 97+i i.e. a-z. Last element stores EOF
int max_letters = 0;      // no. of letters in word



int spool(int = 0, string = "", string = "");
void check(string = '\0');
void init(void);
bool mycheck(string a, ll start_pt, ll end_pt);

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

		cout << "\bPossible formed words :" << endl << endl;

		//COMMENTED CODE BELOW IS FOR PRINTING TIME ELAPSED
	
		//clock_t t1 = clock();
		spool();
		//clock_t t2 = clock();
		//float diff = ((float)t2-(float)t1)/CLOCKS_PER_SEC;
    	//cout<< endl << diff<<endl;

		cout << endl << endl;

	return 0;
}

int spool(int rc, string m, string mn){            	/* rc = run-count, signifies (n+1)th recursion
                                                       of function, printing (n+1)th digit
                                                    */
	if(rc==max_letters){
        check(m);
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

void check(string a)
{
	ll total = start[26];
	bool b = mycheck(a, 0, total-1);
	if(b)
	{
		cout << a << ' ';
	}
}


//s1 < s2 : < 0
bool mycheck(string a, ll start, ll end)
{
	if(start > end)
	{
		return false;
	}
	ll mid_pt = (start + end)/2;
	int result = strcmp(a.c_str(), enigma[mid_pt].word);
	if(result == 0)
	{
		return true;
	}
	else if(result < 0)
	{
		return mycheck(a, start, mid_pt - 1);
	}
	else
	{
		return mycheck(a, mid_pt + 1, end);
	}
}


void init(void){
    ifstream dict ("dict.txt", ios_base::in);

    char current = 'a';

    for(int i=0; i<200000; i++){
        string h;
        getline(dict, h);

        if(h.at(0) == (current+1)){
            start[++current-97] = i;
        }

        if(strncmp(h.c_str(), "*", 1)== 0 || dict.eof()){
            start[26] = i;
            break;
        }
        else{
            strcpy(enigma[i].word, h.c_str());
        }
    }
}
