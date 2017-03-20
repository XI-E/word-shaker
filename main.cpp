#include <iostream>
#include <fstream>
#include <string.h>
#include <chrono>
#include <thread>
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
int freq[10][26] = {0}, total_chars;
void init();
bool input();
void insert(char*, node *&);
void generate(unsigned thread_num, int curr_place, string word, node *p);
void print_words(unsigned thread_num); //Prints words with starting character index in range [start, end]
void controller(); //Generates threads and waits for them to complete
unsigned int num_threads;

inline void print_spaces(int n) {for(int i = 0; i < n; i++) cout << ' ';}

int main()
{	
	sc::high_resolution_clock::time_point t1, t2;
	
	t1 = sc::high_resolution_clock::now();
	init();
	t2 = sc::high_resolution_clock::now();
	sc::duration<double> time_span = sc::duration_cast<sc::duration<double>>(t2 - t1);
	
	num_threads = std::thread::hardware_concurrency();
	
	#ifdef TEST
	
	cout << "Initialisation: " << time_span.count() << 's' << endl;
	cout << "Threads: " << num_threads << endl;
	while(input())
	{
		t1 = sc::high_resolution_clock::now();
		controller();
		t2 = sc::high_resolution_clock::now();
		time_span = sc::duration_cast<sc::duration<double>>(t2-t1);
		print_spaces(4); cout << time_span.count() << 's';
	}
	
	#else /* TEST */
	
	cout << "Time taken for initialisation: " << time_span.count() << "s\n";
	cout << "Number of threads: " << num_threads << endl;
	cout << "Press any key to continue"; getch();
	
	while(input())
	{
		cout << "\nPossible formed words :" << endl << endl;
		t1 = sc::high_resolution_clock::now();
		controller();
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
	
	ifstream dict ("dict.txt", ios_base::in);

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
	for(unsigned i = 0; i < num_threads; i++)
	{
		for(unsigned j = 0; j < 26; j++)
		{
			freq[i][j] = 0;
		}
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
				for(unsigned k = 0; k < num_threads; k++)
					freq[k][inp-97]++;
			}
			
			#else /* TEST */
			
            if(!isalpha(inp)){
            	
                cout << "\n" << inp <<" - Input skipped\n";  	 // Ignores any invalid input
            }
			else{
                cout << inp << ' ';
                inp = tolower(inp);
                for(unsigned k = 0; k < num_threads; k++)
					freq[k][inp-97]++;
			}
			
			#endif /* TEST */
	}

	cout << "\n";

	for(int k=0; k<26; k++){
        total_chars += freq[0][k];
	}
	
	if(total_chars == 0)
	{
		return false;
	}

	return true;
}

void generate(unsigned thread_num, int curr_place, string word, node *p) //Number of place is like in numbers: So, higher curr_place means more left place
{
	while(true)
	{
		char ch = p->data;
		int i = (int) (ch - 'a');
		if(freq[thread_num][i] != 0)
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
				freq[thread_num][i]--;
				generate(thread_num, curr_place - 1, temp, p->eq);
				freq[thread_num][i]++;
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

void controller()
{
	std::thread threads[num_threads];
	
	for(unsigned i = 0; i < num_threads; i++)
	{
		threads[i] = std::thread(print_words, i);
	}
	
	for(unsigned i = 0; i < num_threads; i++)
	{
		threads[i].join();
	}
}

void print_words(unsigned thread_num)
{

	int start = (26 / num_threads) * thread_num,
		end = (26 / num_threads) * (thread_num + 1) - 1;
		
	if(thread_num == num_threads - 1)
	{
		end = 26 - 1;
	}
	
	for(int i = start; i <= end; i++)
	{
		if(freq[thread_num][i] != 0)
		{
			string s = "";
			s += (char)(i + 'a');
			if(root[i].is_end_of_str == true && total_chars == 1 )
			{
				cout << root[i].data << ' ';
			}
			else if(total_chars > 1)
			{
				freq[thread_num][i]--;
				generate(thread_num, total_chars - 1, s, root[i].next);
				freq[thread_num][i]++;
			}
		}
	}
}
