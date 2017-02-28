#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include <ctype.h>
#include "rlutil.h"

typedef long long ll;

using namespace std;

struct dict_word{
    char word[100];
};

dict_word enigma[99999];

int aff[26] = {0};        // aff[i] = affinity of letter (char) 97+i i.e. a-z
int start[26] = {0};      // start[i] = line where letter first appears (char) 97+i i.e. a-z
int max_letters = 0;      // no. of letters in word



int spool(int = 0, string = "", string = "");
void check(string = '\0');
void init(void);

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

		spool();

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

void check(string a){
    for(int i=start[(int) a.at(0) - 97]; i<start[((int) a.at(0) + 1) - 97]; i++){
        if(enigma[i].word[0] == '\0'){
            break;
        }
        else{
            if(strcmp(enigma[i].word, a.c_str()) == 0){
                cout << a << ' ';
            }
        }
    }
}

void init(void){
    ifstream dict ("dict.txt", ios_base::in);

    char current = 'a';

    for(int i=0; i<99999; i++){
        string h;
        getline(dict, h);

        if(h.at(0) == (current+1)){
            start[++current-97] = i;
        }

        if(strncmp(h.c_str(), "*", 1)== 0 || dict.eof()){
            break;
        }
        else{
            strcpy(enigma[i].word, h.c_str());
        }
    }
}
