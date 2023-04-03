#include "BpTree.h"
#include <fstream>

bool BpTree::Insert(VaccinationData* newData)
{
	if (root != NULL)
	{
		BpTreeNode* pNode = searchDataNode(newData->GetUserName());

		/*If the user already exists, the number of vaccinations increases*/
		if (pNode != NULL)
		{
			map<string, VaccinationData*>* curr = pNode->getDataMap();

			for (auto iter = curr->begin(); iter != curr->end(); ++iter)
			{
				int num = iter->first.compare(newData->GetUserName());
				if (num == 0)
				{
					iter->second->SetTimesInc(); //Increase the number of vaccinations
					string VaccinName = "Janssen";
					int num2 = iter->second->GetVaccineName().compare(VaccinName);
					if (num2 == 0)	//In case of Janssen
					{
						if (iter->second->GetTimes() == 1)
							return false;	//insert AVL Tree
					}
					else //In the case of a vaccine other than Janssen
					{
						if (iter->second->GetTimes() == 2)
							return false;	//insert AVL Tree
					}
					return true;
				}
			}	
		}
	}

	if (root == NULL)
	{	//Insert into an empty tree
		root = new BpTreeDataNode;
		root->insertDataMap(newData->GetUserName(), newData);
		return true;
	}
	else if (!root->getMostLeftChild())	//If there's only one root node in the tree
	{
		root->insertDataMap(newData->GetUserName(), newData);

		if (root->getDataMap()->size() == order) //if data's num is 3,
		{
			//Need to split
			splitDataNode(root);

		}
	}
	else //If there are more than one node in the tree
	{ //Find a node to insert new data
		BpTreeNode* ptr = root;
		int num;
		num = ptr->getIndexMap()->begin()->first.compare(newData->GetUserName());

		while (ptr->getMostLeftChild())
		{ //Compare without distinction between upper and lower case letters
			num = ptr->getIndexMap()->begin()->first.compare(newData->GetUserName());
			
			if (num == 0)	//case) name is same 
			{
				//nothing
			}
			else if (num > 0)
			{
				if (!ptr->getMostLeftChild())
				{
					break;
				}
				ptr = ptr->getMostLeftChild();


			}
			else
			{
				if (ptr->getIndexMap()->size() == 1)
				{
					ptr = ptr->getIndexMap()->begin()->second;
				}
				else if (ptr->getIndexMap()->size() == 2)
				{
					num = ptr->getIndexMap()->rbegin()->first.compare(newData->GetUserName());
					if (num == 0)	//case) name is same 
					{
						//nothing
					}
					else if (num > 0)
					{
						ptr = ptr->getIndexMap()->begin()->second;
					}
					else
					{
						ptr = ptr->getIndexMap()->rbegin()->second;
					}
				}
			}
		}
		ptr->insertDataMap(newData->GetUserName(), newData);	//insert one user name, and one 
		if (ptr->getDataMap()->size() == order) //Need to split
			splitDataNode(ptr);
	}
	int num = newData->GetVaccineName().compare("Janssen");
	if (num == 0)
	{
		if(newData->GetTimes()==1)
			return false;
	}
	return true;
}

BpTreeNode * BpTree::searchDataNode(string n) 
{
	BpTreeNode* pNode = root;

	while (pNode->getMostLeftChild())
	{
		pNode = pNode->getMostLeftChild();
	}


	map<string, VaccinationData*>* curr = pNode->getDataMap();

	if (empty())	//if Bp Tree is empty
		return NULL;

	while (pNode)
	{
		curr = pNode->getDataMap();
		for (auto iter = curr->begin(); iter != curr->end(); ++iter) {
			int num = iter->first.compare(n);
			if (num == 0)
				return pNode;
		}
		pNode = pNode->getNext();
	}
	return NULL;
}

void BpTree::splitDataNode(BpTreeNode* pDataNode)
{	//pDataNode split

	/*Save the first data of the map separately*/
	auto iter = pDataNode->getDataMap()->rbegin();
	VaccinationData* last_data = new VaccinationData;
	last_data->SetAge(iter->second->GetAge());
	last_data->SetUserName(iter->second->GetUserName());
	last_data->SetLocationName(iter->second->GetLocationName());
	last_data->SetTimes(iter->second->GetTimes());
	last_data->SetVaccineName(iter->second->GetVaccineName());
	pDataNode->deleteMap(last_data->GetUserName());

	/*Save the second data of the map separately*/
	iter = pDataNode->getDataMap()->rbegin();
	VaccinationData* middle_data = new VaccinationData;
	middle_data->SetAge(iter->second->GetAge());
	middle_data->SetUserName(iter->second->GetUserName());
	middle_data->SetLocationName(iter->second->GetLocationName());
	middle_data->SetTimes(iter->second->GetTimes());
	middle_data->SetVaccineName(iter->second->GetVaccineName());
	pDataNode->deleteMap(middle_data->GetUserName());

	BpTreeDataNode* pRight = new BpTreeDataNode;
	pRight->insertDataMap(middle_data->GetUserName(), middle_data);
	pRight->insertDataMap(last_data->GetUserName(), last_data);

	/*The index node to insert the split data into*/
	BpTreeNode* tmpRoot = new BpTreeIndexNode;
	if (pDataNode != root)
	{
		tmpRoot = pDataNode->getParent();
	}

	/*insert split node*/
	tmpRoot->insertIndexMap(pRight->getDataMap()->begin()->first, pRight);

	//Parent setting
	pDataNode->setParent(tmpRoot);
	pRight->setParent(tmpRoot);

	//data Node linked list connection
	if (pDataNode->getNext())
	{
		pRight->setNext(pDataNode->getNext());
		pRight->getNext()->setPrev(pRight);
	}
	pDataNode->setNext(pRight);
	pRight->setPrev(pDataNode);

	if (pDataNode == root)
	{
		//Most Left Child setting
		tmpRoot->setMostLeftChild(pDataNode);
		root = tmpRoot;
	}

	if (tmpRoot->getIndexMap()->size() == order)
	{	//index node split
		splitIndexNode(tmpRoot);
	}

}

void BpTree::splitIndexNode(BpTreeNode* pIndexNode)
{	//Index Node split

	/*Save the first data of the map separately*/
	BpTreeIndexNode* pRight = new BpTreeIndexNode;
	pRight->insertIndexMap(pIndexNode->getIndexMap()->rbegin()->first, pIndexNode->getIndexMap()->rbegin()->second);
	pIndexNode->deleteMap(pIndexNode->getIndexMap()->rbegin()->first);

	/*The index node to insert the split data into*/
	BpTreeNode* tmpRoot = new BpTreeIndexNode;
	if (pIndexNode != root)
	{
		tmpRoot = pIndexNode->getParent();
	}
	pRight->setMostLeftChild(pIndexNode->getIndexMap()->rbegin()->second);
	tmpRoot->insertIndexMap(pIndexNode->getIndexMap()->rbegin()->first, pRight);
	pIndexNode->deleteMap(pIndexNode->getIndexMap()->rbegin()->first);

	//Most left child setting
	if (pIndexNode == root)
	{
		tmpRoot->setMostLeftChild(pIndexNode);
		pIndexNode->setParent(tmpRoot);
		root = tmpRoot;
	}

	//parent setting
	pRight->getMostLeftChild()->setParent(pRight);
	pRight->setParent(tmpRoot);
	pRight->getIndexMap()->begin()->second->setParent(pRight);

	if (tmpRoot->getIndexMap()->size() == order)
		splitIndexNode(tmpRoot);	//Index Node split
}

bool BpTree::exceedDataNode(BpTreeNode* pDataNode) {
	return true;
}

bool BpTree::exceedIndexNode(BpTreeNode* pIndexNode) {
	return true;
}

BpTreeNode* BpTree::SearchRange(string start, string end)
{
	if (empty())	//if Bp Tree is empty, return NULL
		return NULL;

	bool find = false;	//To detect if data has been found

	/*Find the first node in the linked list of data nodes*/
	BpTreeNode* pNode = root;
	while (pNode->getMostLeftChild())
	{
		pNode = pNode->getMostLeftChild();
	}
	
	BpTreeNode* resultNode = new BpTreeDataNode;	//Node to store the found data

	map<string, VaccinationData*>* curr = pNode->getDataMap();

	while (pNode)
	{
		curr = pNode->getDataMap();

		/*compare all map data of node*/
		for (auto iter = curr->begin(); iter != curr->end(); ++iter) 
		{
			if (start.c_str()[0] <= iter->first.c_str()[0] && iter->first.c_str()[0] <= end.c_str()[0])
			{
				resultNode->insertDataMap(iter->first, iter->second);
				find = true;
			}
		}
		
		pNode = pNode->getNext();	//go to next node
	}

	if (find==true)	//If found, reutrn data
		return resultNode;
	else
		return NULL;

	
}

void BpTree::Print() 
{
	if (empty())	//B+ Tree is empty, print error code
		return;

	ofstream fout("log.txt", ofstream::app);
	
	/*Find the first node in the linked list of data nodes*/
	BpTreeNode* pNode = root;
	while (pNode->getMostLeftChild())
	{
		pNode = pNode->getMostLeftChild();
	}

	map<string, VaccinationData*>* curr = pNode->getDataMap();

	fout << "=========PRINT_BP=========" << endl;
	while (pNode)
	{
		curr = pNode->getDataMap();
		/*print all map data of node*/
		for (auto iter = curr->begin(); iter != curr->end(); ++iter) {
			fout << iter->first << " " << iter->second->GetVaccineName() << " "
				<< iter->second->GetTimes() << " " << iter->second->GetAge() << " "
				<< iter->second->GetLocationName() << endl;
		}
		pNode = pNode->getNext();	//go to next node
	}
	fout << "========================" << endl << endl;
	fout.close();

}

bool BpTree::empty()	//B+ Tree is empty, return true
{
	if (root == nullptr)
		return true;
	else
		return false;
}