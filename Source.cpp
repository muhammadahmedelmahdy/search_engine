#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include<sstream>
#include<unordered_map>
#include"BSTwords.h"
#include"webpage.h"
using namespace std;
vector<webpage> readdata(string fileofname,string fileofimpressions)
{
	vector<webpage>mypages;
	webpage page;
	string s1;
	ifstream fin,fin2,fin3;
	BSTwords* keywords=NULL;
	bool firsttime = true;
	fin.open(fileofname);
	getline(fin, s1, ',');
	page.setname(s1);
	int index = 0;
	while (!fin.eof())
	{
		getline(fin, s1, ',');
		if ((s1[0] == '\n'))
		{
			
			page.setwords(keywords);
			page.originalindex = index;
			mypages.push_back(page);
			index++;
			page.setname(s1);
			keywords = NULL;
			firsttime=true;
		}
		else
		{
			keywords = keywords->insert(keywords, s1);
		}
	}
	string z;
	double s3; int j = 0;
	fin3.open(fileofimpressions);
	while (!fin3.eof())
	{
		fin3 >> s3;
		mypages[j].setimpressions(s3);
		j++;
	}
	return mypages;
}

int indexofstring(string word, vector<webpage> pages)
{
	for (int i = 0; i < pages.size(); i++)
	{
		if (("\n" + word == pages[i].getname())||(word==pages[i].getname()))
			return i;
	}
}
int** generategraph(vector<webpage> pages)
{
	int i = 0,j=0; string z;
	int** graph = new int* [pages.size()];
	for (int a = 0; a < pages.size(); a++)
	{
		graph[a] = new int[pages.size()];
	}
	for (int b = 0; b < pages.size(); b++)
	{
		for (int c = 0; c < pages.size(); c++)
			graph[b][c] = 0;
	}
	ifstream file;
	file.open("D:/AUC Books/Fall 21/analysis lab/lab project/graph.csv");
	while (!file.eof())
	{
		getline(file, z, ',');
		
		if (z[0] == '\n')
		{
			
			i++;

		}
		else
		{
			j = indexofstring(z, pages);
		}
		graph[i][j] = 1;
	}
	
	return graph;
}
vector<string> split(string str, char limiter) {
	vector<string> internal;
	stringstream ss(str);  
	string x;

	while (getline(ss, x, limiter)) {
		internal.push_back(x);
	}

	return internal;
}
vector<int> search_query(string z, vector<webpage> pages)
{
	bool andor, bothwords = false;
	vector<int> indices;
	BSTwords x;
	bool orand = false;
	bool found=false;
	int indexofandor=0;
	vector<string> word;
	string firstword="", secondword="";
	for (int i = 0; i < pages.size(); i++)
	{
		if (x.wordfound(pages[i].getwords(), z))
		{
			indices.push_back(i);
		}
	}
	if (indices.size() > 0)
		found = true;
	if (!found)
	{
		if (z[0] == '"')
		{
			string sub = z.substr(1, z.size() - 2);
			for (int i = 0; i < pages.size(); i++)
			{
				if (x.wordfound(pages[i].getwords(), sub))
					indices.push_back(i);
			}
		}
		else
		{
			word = split(z, ' ');
			for (int i = 0; i < word.size(); i++)
			{
				if ((word[i] == "OR") || (word[i] == "Or") || (word[i] == "or"))
				{
					orand = false;
					indexofandor = i;
					break;
				}
				else
				{
					if ((word[i] == "AND") || (word[i] == "And") || (word[i] == "and"))
					{
						orand = true;
						indexofandor = i;
						break;
					}
					else
						orand = false;
				}
			}
			for (int i = 0; i < indexofandor; i++)
			{
				if(i==indexofandor-1)
					firstword += word[i];
				else
					firstword += word[i]+" ";
				
			}
				
			for (int i = indexofandor+1; i < word.size(); i++)
			{
				if (i == word.size() - 1)
					secondword += word[i];
				else
					secondword += word[i]+" ";
			}
				
			if (!orand)
			{
				for (int i = 0; i < pages.size(); i++)
				{
					if ((x.wordfound(pages[i].getwords(), firstword)) || ((x.wordfound(pages[i].getwords(), secondword))))
					{
						indices.push_back(i);
					}
				}
			}
			else
			{
				for (int i = 0; i < pages.size(); i++)
				{
					if ((x.wordfound(pages[i].getwords(), firstword)) && ((x.wordfound(pages[i].getwords(), secondword))))
					{
						indices.push_back(i);
					}
				}
			}
	}
	

	}
	
		return indices;
}
void calculaterank(int** graph, vector<webpage>&pages)
{
	const double d = 0.7;
	double sum = 0.0, rank=0, count=0, index;
	for (int i = 0; i < pages.size(); i++)
	{
		sum = 0;
		for (int j = 0; j < pages.size(); j++)
		{
			rank = 0;
			if (graph[i][j] == 1)
			{
				count=0;
				for (int a = 0; a < pages.size(); a++)
				{
					if (graph[a][j] == 1)
					{
						count++;
						
					}
				}
				rank = pages[j].getrank() / count;
			}
			sum += rank;
		}
		sum = 1 - d + d * (sum);
		pages[i].updaterank(sum);
	}
}
void swap(webpage& xp, webpage& yp)
{
	webpage temp = xp;
	xp = yp;
	yp = temp;
}
void heapify(vector<webpage>& arr, int n, int i)
{
	int largest = i;
	int l = 2 * i + 1;
	int r = 2 * i + 2;
	if (l < n && arr[l].calculatescore() > arr[largest].calculatescore())
		largest = l;
	if (r < n && arr[r].calculatescore() > arr[largest].calculatescore())
		largest = r;
	if (largest != i) {
		swap(arr[i], arr[largest]
		);
		heapify(arr, n, largest);
	}
}
void build_heap(vector<webpage>& arr, int n)	
{
	for (int i = n / 2 - 1; i >= 0; i--)
		heapify(arr, n, i);
}
void heap_sort(vector<webpage>& arr, int n)
{
	int k = arr.size() - 1;

	build_heap(arr, n);
	for (int i = n - 1; i >= 0; i--) {
		swap(arr[0], arr[i]
		);
		heapify(arr, i, 0);
	}
	for (int i = 0; i < arr.size()/2; i++)
	{
		swap(arr[i], arr[k]);
		k--;
	}
}
void updateimpressionsfile(string filename,vector<webpage>pages)
{
	ofstream fout;
	fout.open(filename);
	for (int i = 0; i < pages.size(); i++)
	{
		if (i == pages.size() - 1)
		{
			fout << pages[i].getimpressions();
		}
		else
			fout << pages[i].getimpressions() << endl;
		
	}
	fout.close();
}
void searchengine()
{
	vector<webpage>filtered;
	vector<webpage> mypages;
	vector<int> indices;
	int choice,secondchoice,thirdchoice,webchoice,fourthchoice,last,x=1; int** graph;
	string z;
	mypages = readdata("D:/AUC Books/Fall 21/analysis lab/lab project/website names.csv", "D:/AUC Books/Fall 21/analysis lab/lab project/impressions file.csv");
	graph = generategraph(mypages);
	cout << "Welcome to our search engine " << endl;
	cout << "press 1 to start " << endl;
	cin >> choice;
	system("cls");
	while (choice == 1)
	{
		system("cls");
		calculaterank(graph, mypages);
		cout <<"Welcome!"<<endl<<"1-New Search" << endl << "2- Exit" << endl;
		cin >> secondchoice;
		if (secondchoice == 1)
		{
			filtered.clear();
			system("cls");
			cout << "enter the word you want to search about " << endl;
			cin.ignore();
			getline(cin, z);
			indices = search_query(z, mypages);
			if (indices.size() == 0)
			{
				cout << "No results found" << endl;
				cout  << "1-New Search" << endl << "2- Exit" << endl;
				cin >> last;
				if (last == 2)
				{
					choice = 0;
				}
			}
			else
			{
				for (int i = 0; i < indices.size(); i++)
				{
					filtered.push_back(mypages[indices[i]]);
				}
				heap_sort(filtered, filtered.size());
				x = 1;
				while (x == 1)
				{
					system("cls");
					cout << "The results are " << endl;
					for (int i = 0; i < filtered.size(); i++)
					{
						cout << i + 1 << "-" << filtered[i].getname() << endl;
					}
					for (int i = 0; i < filtered.size(); i++)
					{
						mypages[filtered[i].originalindex].updateimpressions();
					}
					cout << "1-choose your website" << endl << "2-New search" << endl << "3-Exit" << endl;
					cin >> thirdchoice;
					if (thirdchoice == 1)
					{
						cout << "Enter the number of the website, please" << endl;
						cin >> webchoice;
						system("cls");
						cout << "You are now viewing " << mypages[filtered[webchoice - 1].originalindex].getname() << endl;
						
						mypages[filtered[webchoice - 1].originalindex].updateCTR();
						updateimpressionsfile("D:/AUC Books/Fall 21/analysis lab/lab project/impressions file.csv", mypages);
						cout << "1-Back to results" << endl << "2-New search" << endl << "3-Exit" << endl;
						cin >> fourthchoice;
						if (fourthchoice == 2)
						{
							x = 0;
						}
						else
						{
							if (fourthchoice == 3)
							{

								choice = 0;
								x = 0;
							}
						}

					}
					if (thirdchoice == 2)
						x = 0;
					if (thirdchoice == 3)
					{
						x = 0;
						choice = 0;
					}
					
				}


			}
		
			

		}
		else
		{
			choice = 0;
		}
	}
}
int main()
{
	searchengine();
}
