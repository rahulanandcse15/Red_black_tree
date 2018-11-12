#include<bits/stdc++.h>
using namespace std;

struct Node{
	int data;
	bool col;
	Node* left=NULL;
	Node* right=NULL;
	Node* par=NULL;
};

Node *HEAD = NULL;

Node* Find(int val){
	Node *tmp = HEAD;
	Node *par = NULL;
	while(tmp != NULL)
	{
		int var = tmp->data;
		if(var == val)
			return par;
		par = tmp;
		tmp = (var > val)?tmp->left:tmp->right;
	}
	return par;
}
void details(Node *curr)
{
	if(curr->par == NULL) cout<<"HEAD ";
	else 
		cout<<curr->par->data<<" ";
	if(curr->left == NULL) cout<<"NULL ";
	else 
		cout<<curr->left->data<<" ";
	if(curr->right == NULL) cout<<"NULL ";
	else 
		cout<<curr->right->data<<" ";
}

void Inorder(Node* curr){
	if(curr->left != NULL){
		cout<<"L";
		Inorder(curr->left);
	}
	cout<<"C";
	cout<<(curr->data)<<" ("<<curr->col<<") ";
	 // cout<<"{";
	// details(curr);
	// cout<<"} ";
	if(curr->right != NULL){
		cout<<"R";
		Inorder(curr->right);
	}
}

void rotateRight(Node *curr){
	Node *parent = curr->par;
	Node *child = curr->left;
	curr->left = child->right;
	child->right = curr;
	if(parent != NULL){
		if(parent->left == curr) parent->left = child;
		else
			parent->right = child;
	}
	else
		HEAD = child;
	if(curr->left != NULL)
		curr->left->par = curr;
	curr->par = child;
	child->par = parent;
}

void rotateLeft(Node *curr){
	Node *parent = curr->par;
	Node *child = curr->right;
	curr->right = child->left;
	child->left = curr;
	if(parent != NULL){
		if(parent->left == curr) parent->left = child;
		else
			parent->right = child;
	}
	else
		HEAD = child;
	if(curr->right != NULL)
		curr->right->par = curr;
	curr->par = child;
	child->par = parent;
}

void colourRearrange(Node *parent, Node *child){
	Node *grandPar = parent->par;
	Node *grandGrandPar = grandPar->par;
	if(parent->data < grandPar->data){
		Node *uncle = grandPar->right;
		if(uncle != NULL && uncle->col == true)
		{
			uncle->col = false;
			parent->col = false;
			grandPar->col = true;
			if(grandGrandPar == NULL) grandPar->col = false;
			else{
					if(grandGrandPar->col == true)
						colourRearrange(grandGrandPar,grandPar);
				}
			return;
		}
		if(parent->right == child){
			rotateLeft(parent);
			Node *tmp = parent;
			parent = child;
			child = tmp;
		}
		rotateRight(grandPar);
		grandPar->col = 1;
		parent->col = 0;
	}
	else{
		Node *uncle = grandPar->left;
		if(uncle != NULL && uncle->col == true)
		{
			uncle->col = false;
			parent->col = false;
			grandPar->col = true;
			if(grandGrandPar == NULL) grandPar->col = false;
			else{
					if(grandGrandPar->col == true)
						colourRearrange(grandGrandPar,grandPar);
				}
			return;
		}
		if(parent->left == child){
			rotateRight(parent);
			Node *tmp = parent;
			parent = child;
			child = tmp;
		}
		rotateLeft(grandPar);
		grandPar->col = 1;
		parent->col = 0;
	}
}

void Insert(int val){
	if(HEAD == NULL){
		Node *newNode = new Node;
		newNode->data = val;
		newNode->col = false;
		newNode->par = NULL;
		HEAD = newNode;
	}
	else{
		Node *tmp = Find(val);
		if(tmp == NULL) return;
		if( (tmp->left != NULL && tmp->left->data == val) || (tmp->right != NULL && tmp->right->data == val) ) return;
		Node *newNode = new Node;
		newNode->data = val;
		newNode->col = true;
		newNode->par = tmp;
		if(val < (tmp->data))
			tmp->left = newNode;
		else
			tmp->right = newNode;

		if(tmp->col == true)
			colourRearrange(tmp, newNode);
		}
}

void DoubleBlackNode(Node *curr){
	cout<<"DoubleBlackNode "<<curr->data<<endl;
	Node *parent = curr->par;
	if(parent == NULL)
		return ;
	Node *sibling = (parent->left == curr)?parent->right:parent->left;

	if(parent->col == true)
	{
		if(sibling == NULL){
			parent->col = false;
			return;
		}
		else{
			Node *nephewL = sibling->left;
			Node *nephewR = sibling->right;
			if( (nephewL == NULL || nephewL->col == false) && (nephewR== NULL || nephewR->col == false) )
			{
				parent->col = false;
				sibling->col = true;
				return;
			}
			if(parent->left == curr)
				rotateLeft(parent);
			else
				rotateRight(parent);
		}
	}
	else{
		if(sibling == NULL)
			DoubleBlackNode(parent);
		else{
			if(sibling->col == false)
			{
				Node *nephewL = sibling->left;
				Node *nephewR = sibling->right;
				if( (nephewL == NULL || nephewL->col == false) && (nephewR== NULL || nephewR->col == false) ){
					sibling->col = true;
					DoubleBlackNode(parent);
				}
				else if(nephewL == NULL || nephewL->col == false){
					rotateRight(sibling);
					sibling->col = true;
					nephewR->col = false;
					DoubleBlackNode(curr);
				}
				else if(nephewR ==NULL || nephewR->col == false)
				{
					rotateLeft(sibling);
					sibling->col = true;
					nephewL->col = false;
					DoubleBlackNode(curr);
				}
				else{
					if(parent->left == curr){
						rotateLeft(parent);
						nephewR->col = false;
					}
					else{
						rotateRight(parent);
						nephewL->col = false;
					}
				}
			}
			else{
				if(parent->left == curr)
					rotateLeft(parent);
				else
					rotateRight(parent);
				parent->col == true;
				sibling->col == false;
				DoubleBlackNode(curr);
			}
		}
	}

}



void Delete(int val){
	if(HEAD == NULL) return;
	Node *tmp = Find(val);
	if(tmp == NULL && HEAD->data != val) return;
	if( tmp != NULL){
		if(tmp->data > val && tmp->left == NULL) return;
		if(tmp->data < val && tmp->right == NULL) return;
	}

	if(HEAD->data == val)
		tmp = HEAD;
	else
		tmp = (val > tmp->data)?tmp->right:tmp->left;

	if(tmp->left == NULL && tmp->right == NULL){
		Node *parent = tmp->par;
		if(parent == NULL){
			delete tmp;
			HEAD = NULL;
		}
		else if(parent->right != NULL && parent->left != NULL)
		{
			if(parent->left == tmp){
				parent->left = NULL;
			}
			else{
				parent->right = NULL;
			}
			delete tmp;
			return;
		}
		else{
			parent->left = parent->right = NULL;
			if(parent->col == true) 
				parent->col = false;
			else if(tmp->col == false) 
				DoubleBlackNode(parent);
			delete tmp;
		}
	}
	else if(tmp->left == NULL || tmp->right == NULL)
	{
		// cout<<"Case 2\n";
		Node *tmpNext = (tmp->left == NULL)?tmp->right:tmp->left;
		Node *parent = tmp->par;
		if(parent == NULL)
		{
			HEAD = tmpNext;
			tmpNext->par = NULL;
			tmpNext->col = false;
			delete tmp;
			return;
		}
		if(parent->left == tmp)
			parent->left = tmpNext;
		else
			parent->right = tmpNext;
		tmpNext->par = tmp->par;

		if(tmp->col == true)
			delete tmp;
		else if(tmpNext->col == true)
		{
			tmpNext->col = false;
			delete tmp;
		}
		else
		{
			delete tmp;
			DoubleBlackNode(tmpNext);
		}
	}
	else
	{
		// cout<<"Case 3 or 4\n";
		Node *parNext = tmp;
		Node *tmpNext = tmp->left;
		Node *parent = tmp->par;
		while(tmpNext->right != NULL){
			parNext = tmpNext;
			tmpNext = tmpNext->right;
		}
		int value = tmpNext->data;
		Delete(value);
		tmp->data = value;
	}
}


int main()
{
	/*
				1 2 3 4 5 6 7 8 9
	*/
	int arr[] = {5,6,3,7,1,4,2};
	for(int i=0;i<7;i++)
	{
		Insert(arr[i]);
	}
	Inorder(HEAD);
		cout<<endl;
	// cout<<"Delete starts ....."<<endl;
	// int del[] ={9,8,3,1};
	// for(int i=0;i<1;i++)
	// {
	//     cout<<"DEl "<<del[i]<<endl;
	// 	Delete(del[i]);
	// 	Inorder(HEAD);
	// 	cout<<endl;
	//  }

	return 0;
}
