#include <iostream.h>
#include <fstream.h>
#include <string.h>
#include <stddef.h>
#include <ctype.h>
//#include <chrono>
#include <conio.h>

//namespace sc = std::chrono;

//begin: Structure definitions
struct node
{
	char data;
	int is_end_of_str;
	node *eq, *right;

	node()
	{
		data = '\0';
		is_end_of_str = 0;
		eq = NULL;
		right = NULL;
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
	int is_end_of_str;

	top_node()
	{
		data = '\0';
		next = NULL;
		is_end_of_str = 0;
	}

	~top_node()
	{
		delete next;
	}
};
//end: Structure definitions

top_node root[26];
int freq[26] = {0}, total_chars;
void init();
int input();
void insert(char*, node *&);
void generate(int curr_place, char* word, node *p);
void print_words();
//char * appendchar(char *, char);

inline void print_spaces(int n) {for(int i = 0; i < n; i++) cout << ' ';}

int main()
{
	//sc::high_resolution_clock::time_point t1, t2;

	//t1 = sc::high_resolution_clock::now();
	init();
	//t2 = sc::high_resolution_clock::now();
	//sc::duration<double> time_span = sc::duration_cast<sc::duration<double>>(t2 - t1);

	#ifdef TEST

	//cout << "Initialisation: "; print_spaces(10); cout << time_span.count() << 's' <<endl;
	cout << "Initialisation done."

	while(input() != 0)
	{
		//t1 = sc::high_resolution_clock::now();
		print_words();
		//t2 = sc::high_resolution_clock::now();
		//time_span = sc::duration_cast<sc::duration<double>>(t2-t1);
		//print_spaces(4); cout << time_span.count() << 's';
	}

	#else /* TEST */

	//cout << "Time taken for initialisation: " << time_span.count() << "s\n";
	cout << "Press any key to continue"; getch();

	while(input())
	{
		cout << "\nPossible formed words :" << endl << endl;
		//t1 = sc::high_resolution_clock::now();
		print_words();
		//t2 = sc::high_resolution_clock::now();
		//time_span = sc::duration_cast<sc::duration<double>>(t2 - t1);
		//cout << endl << "Time taken for generation: " << time_span.count() << "s" <<endl;

		cout << endl;

		cout << "Press any key to continue"; getch();
	}

	#endif /* TEST */

	return 0;
}

void init()
{
	for(int i = 0; i < 26; i++)
	{
		root[i].data = (char)(i+'a');
		root[i].next = new node;
	}

	ifstream dict ("dict.txt", ios::in);

	for(long j=0; j<200000; j++)
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
				root[index].is_end_of_str++;
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
			p->is_end_of_str++;
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
				p->is_end_of_str++;
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

int input()
{
	total_chars = 0;
	for(int i = 0; i < 26; i++)
	{
		freq[i] = 0;
	}

	#ifndef TEST
	clrscr();
	cout << "Enter letters in succession (Press enter to end input. Program will be terminated if no characters are entered) : \n";
	#endif

	for(int o=0; o<1;  ){
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
		return 0;
	}

	return 1;
}

void generate(int curr_place, char word[], node *p) //Number of place is like in numbers: So, higher curr_place means more left place
{
	while(1)
	{
		char ch = p->data;
		int i = (int) (ch - 'a');
		if(freq[i] != 0)
		{
			if(curr_place == 1)
			{
				char a[] = {ch, (char) 0};
				char* temp = strcat(word, a);
				if(p->is_end_of_str != 0)
				{
					cout << temp << ' ';
				}
			}
			else if (p->eq) //p->eq should not be a null pointer
			{
				char a[] = {ch, (char) 0};
				char* temp = strcat(word, a);
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
			char s[35] = "";
			char a[] = {(char)(i + 'a'), (char) 0};
			strcpy(s, strcat(s, a));
			if(root[i].is_end_of_str != 0 && total_chars == 1 )
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

//char * appendchar(char inpstr[], char inpchar){
//	char* temp = new char[strlen(inpstr+2)];
//	strcpy(temp, inpstr);
//	temp[strlen(inpstr + 1)] = inpchar;
//	return temp;
//}
