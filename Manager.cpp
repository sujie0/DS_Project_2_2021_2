#include "Manager.h"
bool already_exsit_B_tree = false;
bool compare_A(VaccinationData* a, VaccinationData* b);
bool compare_B(VaccinationData* a, VaccinationData* b);

Manager::Manager(int bpOrder) {
	avl = new AVLTree;
	bp = new BpTree;

	/*Every time the program starts, erase the contents of the file*/
	std::ofstream ofs;
	ofs.open("log.txt", std::ofstream::out | std::ofstream::trunc);
	ofs.close();
}

Manager::~Manager() 
{
	delete avl;
	delete bp;
}

void Manager::run(const char* command_txt) 
{
	ifstream fin;
	fin.open(command_txt);
	if (!fin)
	{
		printErrorCode(100);
	}

	char cmd[32];

	while (!fin.eof())
	{
		fin.getline(cmd, 32);
		char* tmp = strtok(cmd, " ");

		if (!strcmp(cmd, "LOAD"))
		{
			tmp=strtok(NULL," ");
			if(tmp==NULL)
			{
				if (LOAD())
				{
					printSuccessCode("LOAD");
				}
				else
				{
					printErrorCode(100);
				}
			}
			else
				printErrorCode(100);
		}

		else if (!strcmp(cmd, "ADD"))
		{
			tmp = strtok(NULL, " ");
			if (tmp != NULL)	// If there are not enough command arguments, print error code
			{
				string userName = tmp;
				tmp = strtok(NULL, " ");
				if (tmp != NULL) // If there are not enough command arguments, print error code
				{
					string vaccinName = tmp;
					tmp = strtok(NULL, " ");
					if (tmp != NULL) // If there are not enough command arguments, print error code
					{
						int age = atoi(tmp);
						tmp = strtok(NULL, " ");
						if (tmp != NULL) // If there are not enough command arguments, print error code
						{
							string locationName = tmp;
							tmp=strtok(NULL," ");
							if(tmp==NULL)
							{	if (ADD(userName, vaccinName, age, locationName))
								{
									//nothing
								}
								else
									printErrorCode(300);
							}
							else
printErrorCode(300);
						}
						else
							printErrorCode(300);
					}
					else
						printErrorCode(300);
				}
				else
					printErrorCode(300);
			}
			else
				printErrorCode(300);
		}
		
		else if (!strcmp(cmd, "SEARCH_BP"))
		{
			tmp = strtok(NULL, " ");
			if (tmp == NULL)	// If there are not enough command arguments, print error code
				printErrorCode(400);
			else if (bp->empty())	//if B+ Tree is empty, print error code
				printErrorCode(400);
			else
			{
				char* tmp2 = strtok(NULL, " ");
				char* tmp3 = strtok(NULL, " ");
				if(tmp3==NULL)
				{	if (tmp2 == NULL)
					{
						if (SEARCH_BP(tmp))
						{
							//nothing
						}
						else
						{
							printErrorCode(400);
						}
					}
					else
					{
						if (SEARCH_BP(tmp,tmp2))
						{
							//nothing
						}
						else
						{
							printErrorCode(400);
						}
					}
				}
				else
					printErrorCode(400);	//ther are more command arguments
			}
		}

		else if (!strcmp(cmd, "VLOAD"))
		{
			tmp = strtok(NULL," ");
			if(tmp==NULL)
			{	if (avl->empty())	//if AVL Tree is empty, print error code
					printErrorCode(200);
				else if (VLOAD())
				{
					printSuccessCode("VLOAD");
				}
				else
				{
					printErrorCode(200);
				}
			}
			else
				printErrorCode(200);	//there are more command arguments
		}

		else if (!strcmp(cmd, "SEARCH_AVL"))
		{
			tmp = strtok(NULL, " ");
			if (tmp == NULL)	// If there are not enough command arguments, print error code
				printErrorCode(500);
			else if(avl->empty())	//if AVL Tree is empty, print error code
				printErrorCode(500);
			else
			{
				char* tmp2=strtok(NULL," ");
				if(tmp2==NULL)
				{	if (SEARCH_AVL(tmp))
					{
						//nothing
					}
					else
					{
						printErrorCode(500);
					}
				}
				else
					printErrorCode(500);	//there are more command arguments
			}
		}

		else if (!strcmp(cmd, "VPRINT"))
		{
			tmp = strtok(NULL, " ");
			if (tmp == NULL)	// If there are not enough command arguments, print error code
				printErrorCode(600);
			else if (avl->empty())	//if AVL Tree is empty, print error code
				printErrorCode(600);
			else
			{
				char* tmp2=strtok(NULL," ");
				if(tmp2==NULL)
				{	if (VPRINT(tmp))
					{
						//nothing
					}
					else
					{
						printErrorCode(600);
					}
				}
				else
					printErrorCode(600);	//there are more command arguments
			}
		}

		else if (!strcmp(cmd, "PRINT_BP"))
		{
			tmp=strtok(NULL," ");
			if(tmp==NULL)
			{	if (bp->empty())	//B+ Tree is empty, print error code
					printErrorCode(700);
				else
					bp->Print();
			}
			else
				printErrorCode(700);	//there are more command arguments
 		}

		else if (!strcmp(cmd, "EXIT"))
		{
			tmp=strtok(NULL," ");
			if(tmp==NULL)
			{	printSuccessCode("EXIT");
				return;
			}
			else
				printErrorCode(800);	//there are more command arguments
		}

		else
			printErrorCode(800);	//this command is not exsits
	}

	fin.close();
}

bool Manager::LOAD() 
{
	ofstream fout("log.txt", ofstream::app);
	if (already_exsit_B_tree)	//if B+ tree already exsit, return false
		return false;

	bool isEmpty;
	ifstream read;

	isEmpty = read.peek() == EOF;
	read.open("input_data.txt");

	if (read.good())	//read successful of data.txt
	{
		already_exsit_B_tree = true;

		char data[200];

		while (read.getline(data, sizeof(data)))
		{
			if (data == NULL)	//if File is empty, print error code
				printErrorCode(100);
			else 
			{
				VaccinationData* pNode = new VaccinationData;
			
				char* tmp = strtok(data, " ");

				if (tmp != NULL)
				{
					pNode->SetUserName(tmp);	//set user name

					tmp = strtok(NULL, " ");
					if (tmp != NULL)
					{
						pNode->SetVaccineName(tmp);	//set vaccine name

						tmp = strtok(NULL, " ");
						if (tmp != NULL)
						{
							int num = atoi(tmp);
							pNode->SetTimes(num);		//set time

							tmp = strtok(NULL, " ");
							if (tmp != NULL)
							{
								int age = atoi(tmp);
								pNode->SetAge(age);		//set age

								tmp = strtok(NULL, " ");
								if (tmp != NULL)
								{
									pNode->SetLocationName(tmp);	//set location

									
									bp->Insert(pNode);	//Insert VaccinationData Node in B+ Tree
								}
								else
									fout << "100" << endl;
							}
							else
								fout << "100" << endl;
						}
						else
							fout << "100" << endl;
					}
					else
						fout << "100" << endl;
				}
				else
					fout << "100" << endl;

			}
			
		}
		fout.close();
		return true;
	}
	else
		return false;
}

bool Manager::VLOAD() 
{
	if (avl->empty())	//if AVL Tree is empty, return false
		return false;

	avl->GetVector(Print_vector);
	
	return true;
}

bool Manager::ADD(string userName, string vaccinName, int age, string locationName) 
{
	if (avl->Search(userName))	// If the target to be added to B+tree is already a vaccine completion, return false
		return false;

	ofstream fout("log.txt", ofstream::app);

	/*store ADD data*/
	VaccinationData* pNode = new VaccinationData;
	pNode->SetUserName(userName);
	pNode->SetVaccineName(vaccinName);
	pNode->SetAge(age);
	pNode->SetLocationName(locationName);
	pNode->SetTimes(1);

	/*print ADD data*/
	fout << "=======ADD=======" << endl;
	fout << pNode->GetUserName() << " " << pNode->GetVaccineName() << " "
		<< pNode->GetAge() << " " << pNode->GetLocationName() << endl;
	fout << "==================" << endl << endl;
	fout.close();

	if(bp->Insert(pNode)==true)	//If not done with the vaccination
		return true;
	else //if done with the vaccination, Add to the AVL tree
	{
		string name = "Janssen";
		int num = vaccinName.compare(name);
		if (num == 0)
		{
			//The number of times Janssen inoculation has been completed is 1
			pNode->SetTimes(1);
		}
		else //else Vaccinaion, the number of times is 2
			pNode->SetTimes(2);

		avl->Insert(pNode);	//insert AVL Tree
	}

	return true;
}

bool Manager::SEARCH_BP(string name) 
{
	BpTreeNode* pNode = bp->searchDataNode(name);

	if (pNode == NULL)	//if search fail, return false
		return false;

	if (pNode!=NULL)	//search success
	{
		map<string, VaccinationData*>* curr = pNode->getDataMap();

		for (auto iter = curr->begin(); iter != curr->end(); ++iter) 
		{
			int num = iter->first.compare(name);
			if (num==0)
			{
				/*print search data*/
				ofstream fout("log.txt", ofstream::app);
				fout << "======SEARCH_BP======" << endl;
				fout << iter->first << " " << iter->second->GetVaccineName() << " "
					<< iter->second->GetTimes() << " " << iter->second->GetAge() << " "
					<< iter->second->GetLocationName() << endl;
				fout << "====================" << endl << endl;
				fout.close();
				return true;
			}
		}
		return false;
	}
	return false;
}

bool Manager::SEARCH_BP(string start, string end) 
{
	ofstream fout("log.txt", ofstream::app);

	BpTreeNode* pNode = bp->SearchRange(start, end);

	if (pNode == NULL)	//if search fail, return false
		return false;

	if (pNode != NULL)	//serarch success
	{
		map<string, VaccinationData*>* curr = pNode->getDataMap();
		fout << "======SEARCH_BP======" << endl;
		for (auto iter = curr->begin(); iter != curr->end(); ++iter)
		{	//print all search data
			fout << iter->first << " " << iter->second->GetVaccineName() << " "
				<< iter->second->GetTimes() << " " << iter->second->GetAge() << " "
				<< iter->second->GetLocationName() << endl;
		}
		fout << "====================" << endl << endl;
		
	}
	fout.close();
	return true;
}

bool Manager::SEARCH_AVL(string name) 
{
	VaccinationData* Node = avl->Search(name);

	if (Node==NULL)		//if search fail, retrun false
		return false;

	else //search success
	{
		//print search data
		ofstream fout("log.txt", ofstream::app);
		fout << "=======SEARCH_AVL=======" << endl;
		fout << Node->GetUserName() << " " << Node->GetVaccineName() << " "
			<< Node->GetTimes() << " " << Node->GetAge() << " " << Node->GetLocationName() << endl;
		fout << "======================" << endl << endl;

		fout.close();
		return true;
	}
}

bool Manager::VPRINT(string type_) 
{
	ofstream fout("log.txt", ofstream::app);
	if (avl->empty()||Print_vector.empty())
		return false;	//if AVL tree is empty or print_vector is empty, return false
	
	if (type_ == "A")	//In case of A type sort
	{
		sort(Print_vector.begin(), Print_vector.end(),compare_A);
	}
	else if (type_ == "B")	//In case of B type sort
	{
		sort(Print_vector.begin(), Print_vector.end(), compare_B);
	}
	else
		return false;

	fout << "=======VPRINT " <<type_<<"======="<< endl;
	for (int i = 0; i < Print_vector.size(); i++)
	{	//print sorted data
		fout << Print_vector[i]->GetUserName() << " " << Print_vector[i]->GetVaccineName()
			<< " " << Print_vector[i]->GetTimes() << " " << Print_vector[i]->GetAge()
			<< " " << Print_vector[i]->GetLocationName() << endl;
	}
	fout << "====================" << endl << endl;

	fout.close();
	return true;
}

void Manager::printErrorCode(int n) 
{
	ofstream fout("log.txt", ofstream::app);
	fout << "========== ERROR ==========" <<endl;
	fout << n << endl;
	fout << "=======================" << endl << endl;
	fout.close();
}

void Manager::printSuccessCode(string cmd)
{
	ofstream fout("log.txt", ofstream::app);
	fout << "==========" << cmd << "==========" << endl;
	fout << "Success" << endl;
	fout << "========================" << endl << endl;
	fout.close();
}

bool compare_A(VaccinationData* a, VaccinationData* b)
{
		if (a->GetVaccineName() == b->GetVaccineName())	//if have same vaccine name
		{
			if (a->GetAge() == b->GetAge())	//if have same age
			{	
				//Sort in ascending order based on the name
				return a->GetUserName() < b->GetUserName();
			}
			else //if have not same age
			{
				//Sort in ascending order based on the age
				return a->GetAge() < b->GetAge();
			}
		}
		else //if have not same vaccination name
		{
			//Sort in ascending order based on the vaccine name
			return a->GetVaccineName() < b->GetVaccineName();
		}
}

bool compare_B(VaccinationData* a, VaccinationData* b)
{
	if (a->GetLocationName() == b->GetLocationName())	//if have same loccaion name
	{
		if (a->GetAge() == b->GetAge())	//if have same age
		{
			//Sort in ascending order based on the name
			return a->GetUserName() < b->GetUserName();
		}
		else //if have not same age
		{
			//Sort by age in descending order
			return a->GetAge() > b->GetAge();
		}
	}
	else //if have not same loccaion name
	{
		//Sort in ascending order based on the location name
		return a->GetLocationName() < b->GetLocationName();
	}
}
