#ifndef _AVLNODE_H_
#define _AVLNODE_H_

#include "VaccinationData.h"

class AVLNode
{
private:
	AVLNode*			pRight;
	AVLNode*			pLeft;
	AVLNode*			pParent;
	VaccinationData*	pVaccinationData;
	int					mBF;	//balance factor

public:
	AVLNode(){
		pRight = NULL;
		pLeft = NULL;
		pParent = NULL;
		pVaccinationData = NULL;
		mBF = 0;
	};
	~AVLNode(){};

	void setRight(AVLNode* SN){pRight = SN;}
	void setLeft(AVLNode* SN){pLeft = SN;}
	void setVacData(VaccinationData* pVac){ pVaccinationData = pVac;}
	void setParent(AVLNode* node) { pParent = node; }
	void setBF(int a){mBF = a;}
	AVLNode*	getRight(){return pRight;}
	AVLNode*	getLeft(){return pLeft;}
	AVLNode* getParent() { return pParent; }

	VaccinationData*	getVacData(){return pVaccinationData;}
	int				getBF(){return mBF;}
};

#endif