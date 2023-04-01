#include "AVLTree.h"
#pragma warning(disable: 4996)
#include <cstring>
#include <queue>

bool AVLTree::Insert(VaccinationData* pVac)
{
	AVLNode* pNew = new AVLNode;
	AVLNode* tmpRoot = NULL;
	AVLNode* rotateNode = NULL;
	
	int d = 0;

	/*set new node's information*/
	pNew->setVacData(pVac);
	
	if (root == NULL) //If there's no Node in the tree, insert it to the root
	{
		root = pNew;
		return true;
	}
	else
	{
		AVLNode* ptr = root;
		int num = 0;

		while (ptr != NULL)
		{ //Compare without distinction between upper and lower case letters
				
			num = strcasecmp(pNew->getVacData()->GetUserName().c_str(), ptr->getVacData()->GetUserName().c_str());

			//Set left Node or Right Node
			if (num < 0)
			{
				tmpRoot = ptr;
				ptr = ptr->getLeft();
			}
			else
			{
				tmpRoot = ptr;
				ptr = ptr->getRight();
			}
		}
		num = strcasecmp(pNew->getVacData()->GetUserName().c_str(), tmpRoot->getVacData()->GetUserName().c_str());
		
		//Set left Node or Right Node
		if (num < 0)
		{
			pNew->setParent(tmpRoot);
			pNew->setBF(0);
			tmpRoot->setLeft(pNew);
		}
		else if (num > 0)
		{
			pNew->setParent(tmpRoot);
			pNew->setBF(0);
			tmpRoot->setRight(pNew);
		}

			
		/*calculte bf*/
		if (tmpRoot->getLeft() && tmpRoot->getRight())
		{ //If tmpRoot have both children
			tmpRoot->setBF(0);
			return true;	//No rotation, no need to calculate the above node bf
		}
		else //If tmpRoot have only one children
		{ //Need to calculate bf
			ptr = pNew;
			while (ptr)
			{
				if (ptr == tmpRoot->getRight())
				{
					d = -1;
					d += tmpRoot->getBF();
					tmpRoot->setBF(d);
				}
				else if (ptr == tmpRoot->getLeft())
				{
					d = 1;
					d += tmpRoot->getBF();
					tmpRoot->setBF(d);
				}

				if (tmpRoot->getBF() == 2 || tmpRoot->getBF() == -2)
				{
					rotateNode = tmpRoot;
					break;	//don't have to calculate bf anymore, need rotation
				}
				else if (tmpRoot->getBF() == 0)
					break;	//don't have to calculate bf anymore, don't need rotation
				else
				{
					if (tmpRoot == root)
						break;
					else
					{
						ptr = tmpRoot;
						tmpRoot = ptr->getParent();
					}
				}
			}
		}
	}
		
	/*Lotation*/
	if (rotateNode)	//Need to rotate
	{	
		if (rotateNode->getBF() == 2) //left imbalance
		{
			tmpRoot = rotateNode->getLeft();

			/*LL*/
			if (tmpRoot->getBF() == 1)
			{
				rotateNode->setLeft(tmpRoot->getRight());
				tmpRoot->setRight(rotateNode);

				tmpRoot->setParent(rotateNode->getParent());
				rotateNode->setParent(tmpRoot);

				//If the node to rotate is root, reset the root after rotation
				if (rotateNode == root)
					root = tmpRoot;
				else
				{
					if (rotateNode == tmpRoot->getParent()->getLeft())
						tmpRoot->getParent()->setLeft(tmpRoot);
					else if(rotateNode == tmpRoot->getParent()->getRight())
						tmpRoot->getParent()->setRight(tmpRoot);
				}

				//calculate bf again
				rotateNode->setBF(0);
				tmpRoot->setBF(0);
			}

			/*LR*/
			else
			{
				AVLNode* c = tmpRoot->getRight();
				tmpRoot->setRight(c->getLeft());
				rotateNode->setLeft(c->getRight());
				c->setLeft(tmpRoot);
				c->setRight(rotateNode);

				c->setParent(rotateNode->getParent());
				tmpRoot->setParent(c);
				rotateNode->setParent(c);

				switch (c->getBF())
				{
				case 0:
					tmpRoot->setBF(0); rotateNode->setBF(0);
					break;
				case 1:
					rotateNode->setBF(-1); tmpRoot->setBF(0);
					break;
				case -1:
					tmpRoot->setBF(1); rotateNode->setBF(0);
					break;
				}
				c->setBF(0);

				//If the node to rotate is root, reset the root after rotation
				if (rotateNode == root)
					root = c;
				else
				{
					if (rotateNode == c->getParent()->getLeft())
						c->getParent()->setLeft(c);
					else if (rotateNode == c->getParent()->getRight())
						c->getParent()->setRight(c);
				}
			}
		}

		else //right imbalance
		{
			tmpRoot = rotateNode->getRight();

			/*RR*/
			if (tmpRoot->getBF() == -1)
			{
				rotateNode->setRight(tmpRoot->getLeft());
				tmpRoot->setLeft(rotateNode);

				tmpRoot->setParent(rotateNode->getParent());
				rotateNode->setParent(tmpRoot);

				//If the node to rotate is root, reset the root after rotation
				if (rotateNode == root)
					root = tmpRoot;
				else
				{
					if (rotateNode == tmpRoot->getParent()->getLeft())
						tmpRoot->getParent()->setLeft(tmpRoot);
					else if (rotateNode == tmpRoot->getParent()->getRight())
						tmpRoot->getParent()->setRight(tmpRoot);
				}

				//calculate bf again
				rotateNode->setBF(0);
				tmpRoot->setBF(0);
			}

			/*RL*/
			else
			{
				AVLNode* c = tmpRoot->getLeft();
				tmpRoot->setLeft(c->getRight());
				rotateNode->setRight(c->getLeft());
				c->setRight(tmpRoot);
				c->setLeft(rotateNode);

				c->setParent(rotateNode->getParent());
				tmpRoot->setParent(c);
				rotateNode->setParent(c);

				switch (c->getBF())
				{
				case 0:
					tmpRoot->setBF(0); rotateNode->setBF(0);
					break;
				case 1:
					tmpRoot->setBF(-1); rotateNode->setBF(0);
					break;
				case -1:
					rotateNode->setBF(1); tmpRoot->setBF(0);
					break;
				}
				c->setBF(0);

				//If the node to rotate is root, reset the root after rotation
				if (rotateNode == root)
					root = c;
				else
				{
					if (rotateNode == c->getParent()->getLeft())
						c->getParent()->setLeft(c);
					else if (rotateNode == c->getParent()->getRight())
						c->getParent()->setRight(c);
				}
			}
		}
	}
	return true;
}

void AVLTree::GetVector(vector<VaccinationData*>& v)
{
	if (!v.empty())	//If Print_vecotr exists, erase it
		v.clear();

	queue<AVLNode*> Q;
	Q.push(root);
	Q.push(NULL);
	while (!Q.empty())
	{ //Queue is not empty, print BST to level order
		AVLNode* pNode = Q.front();
		Q.pop();
		if (pNode == NULL)
		{
			if (!Q.empty())
				Q.push(NULL);
		}
		else
		{
			v.push_back(pNode->getVacData());
			if ((pNode->getLeft()) != NULL)
				Q.push(pNode->getLeft());
			if ((pNode->getRight()) != NULL)
				Q.push(pNode->getRight());
		}
	}

}

bool AVLTree::empty()
{
	if (root == NULL)
		return true;	//if AVL Tree is empty
	else
		return false;
}

VaccinationData* AVLTree::Search(string name)
{
	AVLNode* ptr = root;
	int num;

	while (ptr != nullptr)
	{ //Compare without distinction between upper and lower case letters
		num = strcasecmp(ptr->getVacData()->GetUserName().c_str(), name.c_str());
		if (num==0)	//If find the corresponding name case
		{
			return ptr->getVacData();
		}
		else if (num > 0)
			ptr = ptr->getLeft();
		else if(num<0)
			ptr = ptr->getRight();
	}
	return NULL;
}
