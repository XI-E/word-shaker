#include <iostream>
#include <fstream>
#include <string.h>
#include <chrono>
#include "rlutil.h"

namespace sc = std::chrono;

//begin: Structure definitions
struct node
{
	char data;
	bool is_end_of_str;
	node *eq, *right;
	
	node()
	{
		data = '\0';
		is_end_of_str = false;
		eq = nullptr;
		right = nullptr;
	}
	
	~node()
	{
		delete eq;
		delete right;
	}
};

struct top_node
{
	char data;
	node *next;
	bool is_end_of_str;
	
	top_node()
	{
		data = '\0';
		next = nullptr;
		is_end_of_str = false;
	}
	
	~top_node()
	{
		delete next;
	}
};
//end: Structure definitions

using namespace std;

top_node root[26];
int freq[26] = {0}, total_chars;
void init();
bool input();
void insert(char*, node *&);
void generate(int curr_place, string word, node *p);
void print_words();

inline void print_spaces(int n) {for(int i = 0; i < n; i++) cout << ' ';}

int main()
{	
	sc::high_resolution_clock::time_point t1, t2;
	
	t1 = sc::high_resolution_clock::now();
	init();
	t2 = sc::high_resolution_clock::now();
	sc::duration<double> time_span = sc::duration_cast<sc::duration<double>>(t2 - t1);
	
	#ifdef TEST
	
	cout << "Initialisation: "; print_spaces(10); cout << time_span.count() << 's' <<endl;
	while(input())
	{
		t1 = sc::high_resolution_clock::now();
		print_words();
		t2 = sc::high_resolution_clock::now();
		time_span = sc::duration_cast<sc::duration<double>>(t2-t1);
		print_spaces(4); cout << time_span.count() << 's';
	}
	
	#else /* TEST */
	
	cout << "Time taken for initialisation: " << time_span.count() << "s\n";
	cout << "Press any key to continue"; getch();
	
	while(input())
	{
		cout << "\nPossible formed words :" << endl << endl;
		t1 = sc::high_resolution_clock::now();
		print_words();
		t2 = sc::high_resolution_clock::now();
		time_span = sc::duration_cast<sc::duration<double>>(t2 - t1);
		cout << endl << "Time taken for generation: " << time_span.count() << "s" <<endl;
		
		cout << endl;
		
		cout << "Press any key to continue"; getch();
	}
	
	#endif /* TEST */
}

void init()
{
	for(int i = 0; i < 26; i++)
	{
		root[i].data = (char)(i+'a');
		root[i].next = new node;
	}
	
	ifstream dict ("../dict.txt", ios_base::in);

	for(int i=0; i<200000; i++)
    {
        char h[35];
        dict.getline(h, 35);
        
        if(strncmp(h, "*", 1)== 0 || dict.eof())
        {
            break;
        }
        else
        {
        	int index = (int) (h[0] - 'a');
        	if(h[1] == '\0')
        	{
        		root[index].is_end_of_str = true;
        	}
        	else
        	{
     			insert(h+1, root[index].next);
     		}   	
        }
    }
    
    dict.close();
}

void insert(char h[], node *&p)
{	
	//If it is end node
	if(p->data == '\0')
	{
		p->data = h[0];
		
		if(h[1] == '\0')
		{
			p->is_end_of_str = true;
		}
		else
		{
			if(!p->eq)
			{
				p->eq = new node;
			}
			else
			{
				cerr << "End node pointing to something\n";
				p->eq = new node;
			}
			insert(h+1, p->eq);
		}
	}
	else
	{
		if(h[0] == p->data)
		{
			if(h[1] == '\0')
			{
				p->is_end_of_str = true;
			}
			else
			{
				if(!p->eq)
				{
					p->eq = new node;
				}
				insert(h+1, p->eq);
			}
		}
		else
		{
			//If p->right is a null pointer
			if(!p->right)
			{
				p->right = new node;
			}
			insert(h, p->right);
		}
	}
	
}

bool input()
{
	total_chars = 0;
	for(int i = 0; i < 26; i++)
	{
		freq[i] = 0;
	}

	#ifndef TEST
	rlutil::cls();	
	cout << "Enter letters in succession (Press enter to end input. Program will be terminated if no characters are entered) : \n";
	#endif

	for(int i=0; i<1;  ){
		char inp;
		
		inp = getch();
			if(inp == '\n' || inp == '\r'){
				break;
			}
			#ifdef TEST
			
			if(isalpha(inp))
			{
				inp = tolower(inp);
				freq[inp-97]++;
			}
			
			#else /* TEST */
			
            if(!isalpha(inp)){
            	
                cout << "\n" << inp <<" - Input skipped\n";  	 // Ignores any invalid input
            }
			else{
                cout << inp << ' ';
                inp = tolower(inp);
				freq[inp-97]++;
			}
			
			#endif /* TEST */
	}

	cout << "\n";

	for(int k=0; k<26; k++){
        total_chars += freq[k];
	}
	
	if(total_chars == 0)
	{
		return false;
	}

	return true;
}

void generate(int curr_place, string word, node *p) //Number of place is like in numbers: So, higher curr_place means more left place
{
	while(true)
	{
		char ch = p->data;
		int i = (int) (ch - 'a');
		if(freq[i] != 0)
		{
			if(curr_place == 1)
			{
				string temp = word + ch;
				if(p->is_end_of_str)
				{
					cout << temp << ' ';
				}
			}
			else if (p->eq) //p->eq should not be a null pointer
			{
				string temp = word + ch;
				freq[i]--;
				generate(curr_place - 1, temp, p->eq);
				freq[i]++;
			}
		}
		
		if (p->right) //If p->right is not a null pointer
		{
			p = p->right;
		}
		else
		{
			break;
		}
	}
}  


void print_words()
{
	for(int i = 0; i < 26; i++)
	{
		if(freq[i] != 0)
		{
			string s = "";
			s += (char)(i + 'a');
			if(root[i].is_end_of_str == true && total_chars == 1 )
			{
				cout << root[i].data << ' ';
			}
			else if(total_chars > 1)
			{
				freq[i]--;
				generate(total_chars - 1, s, root[i].next);
				freq[i]++;
			}
		}
	}
}
