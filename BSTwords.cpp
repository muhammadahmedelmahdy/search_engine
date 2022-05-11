#include "BSTwords.h"
BSTwords::BSTwords(string word)
{
	this->word = word;
	left = NULL;
	right = NULL;
}
BSTwords::BSTwords()
{
	word = " ";
	left = NULL;
	right = NULL;

}
BSTwords* BSTwords::insert(BSTwords* root, string value)
{
	if (!root)
	{
		return new BSTwords(value);
	}
	
	if (value > root->word)
	{
		root->right = insert(root->right, value);
	}
	else
	{
		root->left = insert(root->left, value);
	}

	return root;
}
bool BSTwords::wordfound(BSTwords* root, string word)
{
	if (root == NULL)
		return false;

	if (root->word == word)
	{
		return true;
	}

	if (root->word < word)
		return wordfound(root->right, word);

	if (root->word > word)
		return wordfound(root->left, word);
	return false;
}
void BSTwords::deletetree(BSTwords* node)
{
	if (node == NULL) return;

	deletetree(node->left);
	deletetree(node->right);

	delete node;
}
void BSTwords::display(BSTwords* root)
{
	
	if (root != NULL)
	{
		if(root->left!=NULL)
			display(root->left);
		
		cout << root->word << " ";

		if (root->right != NULL)
			display(root->right);
		//display(root->right);
	}
}