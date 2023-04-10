#pragma once
#include "VaccinationData.h"
#include "AVLTree.h"
#include "BpTree.h"
#include <string>
#include <cstring>
#include <fstream>
#include <algorithm>
#pragma warning(disable: 4996)

using namespace std;



class Manager{
private:
	AVLTree		*avl;	// AVL tree
	BpTree		*bp;		// B+ tree
	vector<VaccinationData*> Print_vector;
	//ifstream fin;
	//ofstream fout;

public:
	Manager(int bpOrder); //b+ node map size == bpOrder
	~Manager(); //memory free only on newly allocated avl and bp

	void	run(const char * command_txt);
	bool	LOAD();
	bool	VLOAD();
	bool	ADD(string userName, string vaccinName, int age, string locationName);
	bool	SEARCH_BP(string name);
	bool	SEARCH_BP(string start, string end);
	bool	SEARCH_AVL(string name);
	bool	VPRINT(string type_);
	void	printErrorCode(int n);
	void	printSuccessCode(string cmd);
};

