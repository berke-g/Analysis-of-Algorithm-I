/*
Student name: Berke Gülçiçek
Student no:  150160054
Date: 19.01.2021
BLG 335 Project 3
*/

#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

struct Player { //only used in main function to pass player to tree
	string name;
	string team;
	int rebound;
	int assist;
	int point;
};

class Node {
public:
	bool isRed;
	Node* left;
	Node* right;
	Node* parent;
	string name;
	string team;
	int rebound;
	int assist;
	int point;
};

class RBTree {
public:
	Node* root;
	int reboundRecord; //max rebounds
	int assistRecord; //max assists
	int pointRecord; //max points
	string reboundLeader; //name of the rebound leader
	string assistLeader; //name of the assist leader
	string pointLeader; //name of the point leader

	RBTree() : root(NULL), reboundRecord(0), assistRecord(0), pointRecord(0) {};

	void insert(Player player); //creates and adds node to tree
	Node* search(string name); //search through tree using name
	void fixTree(Node* k); //used to fix the tree after insert operation
	void leftRotate(Node* n); //used to rotate the tree
	void rightRotate(Node* n); //used to rotate the tree
	void updateLeaders(Node* n); //find stat leaders after a season ends
	void printLeaders(); //print leaders
	void releaseNode(Node* n); //free tree pointers
	int height(Node* n); //returns height of a given node
	void preorder(Node* n); //recursive function to print the tree
	void printPreOrder(); //to print whole tree
};

//returns node if it exists, returns correct place to add a node if it doesnt
Node* RBTree::search(string search_key) {	
	Node *temp = root; 

	while (temp != NULL) { 
		if ((search_key).compare(temp->name) < 0) { 
			if (temp->left == NULL) {
				break; 
			} else {
				temp = temp->left; 
			}
		} else if ((temp->name).compare(search_key) == 0) { 
			break; 
		} else { 
			if (temp->right == NULL) {
				break; 
			} else {
				temp = temp->right; 
			}
		} 
	} 

	return temp; 
}

void RBTree::insert(Player player) {
	//create node
	Node* newnode = new Node();
	newnode->left = NULL;
	newnode->right = NULL;
	newnode->parent = NULL;
	newnode->isRed = true;
	newnode->name = player.name;
	newnode->team = player.team;
	newnode->rebound = player.rebound;
	newnode->assist = player.assist;
	newnode->point = player.point;


	if (root == NULL) { //if tree is empty
		newnode->isRed = false;
		root = newnode;
	} else {
		Node* temp = search(player.name);

		if (temp->name == player.name) { //player already exists, update stats
			temp->rebound += player.rebound;
			temp->assist += player.assist;
			temp->point += player.point;

			return;
		} 

		newnode->parent = temp;

		if ( (player.name).compare( newnode->parent->name ) < 0 ) { 
			temp->left = newnode;
		} else {
			temp->right = newnode;
		}
		
		fixTree(newnode);
	}
}

//fixes the tree if parent node is red after inserting a new node
void RBTree::fixTree(Node* k) {
	if (k == root) {
		k->isRed = false;
		return;
	}

	Node* k_parent = k->parent;
	Node* k_grandparent = k_parent->parent;
	Node* k_uncle;

	if (k_parent->isRed) { //we only fix the tree if parent is red

		bool parentIsRight;
		bool k_IsRight;

		if(k_parent == k_grandparent->right) { //if parent is right child
			k_uncle = k_grandparent->left;
			parentIsRight = true;
		} else if (k_parent == k_grandparent->left) { //if parent is left child
			k_uncle = k_grandparent->right;
			parentIsRight = false;
		}

		if (k_parent->left == k) {
			k_IsRight = false;
		} else if (k_parent->right == k) { 
			k_IsRight = true;
		}

		if (k_uncle != NULL && k_uncle->isRed) { //if both uncle and parent is red
			//make uncle and parent black, grandparent red
			k_uncle->isRed = false;
			k_parent->isRed = false;
			k_grandparent->isRed = true;
			fixTree(k_grandparent); 
		} else { //uncle is black or null
			if (parentIsRight && k_IsRight) {
				k_parent->isRed = false; //make parent black
				k_grandparent->isRed = true; //make grndparent red
				leftRotate(k_grandparent); //left rotate grandparent
			} else if (parentIsRight && !k_IsRight) { //TODO: is this correct?
				rightRotate(k_parent);
				k->isRed = false; //make k black
				k_grandparent->isRed = true; //make grndparent red
				leftRotate(k_grandparent); //left rotate grandparent
			} else if (!parentIsRight && k_IsRight) { //TODO: is this correct?
				leftRotate(k_parent);
				k->isRed = !(k->isRed);
				k_grandparent->isRed = !(k_grandparent->isRed);
				rightRotate(k_grandparent);
			} else if (!parentIsRight && !k_IsRight) {				
				k_parent->isRed = !(k_parent->isRed);
				k_grandparent->isRed = !(k_grandparent->isRed);
				rightRotate(k_grandparent);
			}
		}
	}


}

void RBTree::leftRotate(Node* n) {
	Node* y = n->right;
	n->right = y->left;

	if (y->left != NULL) {
		y->left->parent = n;
	}

	y->parent = n->parent;

	if (n->parent == NULL) {
		root = y;
	} else if (n == n->parent->left) {
		n->parent->left = y;
	} else {
		n->parent->right = y;
	}

	y->left = n;
	n->parent = y;
}

void RBTree::rightRotate(Node* n) {
	Node* y = n->left;
	n->left = y->right; 

	if(y->right != NULL) {
		y->right->parent = n;
	}

	y->parent = n->parent;

	if (n->parent == NULL) {
		root = y;
	} else if (n == n->parent->right) {
		n->parent->right = y;
	} else { 
		n->parent->left = y;
	}

	y->right = n;
	n->parent = y;
}

void RBTree::updateLeaders(Node* n) {
	if (n == NULL) {
		return;
	}

	if (n->assist > assistRecord) {
		assistRecord = n->assist;
		assistLeader = n->name;
	}

	if (n->rebound > reboundRecord) {
		reboundRecord = n->rebound;
		reboundLeader = n->name;
	}

	if (n->point > pointRecord) {
		pointRecord = n->point;
		pointLeader = n->name;
	}

	updateLeaders(n->left);
	updateLeaders(n->right);
}

void RBTree::printLeaders() {
	cout << "Max Points: " << pointRecord << " - Player Name: " << pointLeader << endl;
	cout << "Max Assists: " << assistRecord << " - Player Name: " << assistLeader << endl;
	cout << "Max Rebs: " << reboundRecord << " - Player Name: " << reboundLeader << endl;
}

void RBTree::releaseNode(Node* n) {
	if (n == NULL) {
		return;
	}

	releaseNode(n->left);
	releaseNode(n->right);

	delete n;
}

void RBTree::preorder(Node* n) {
	if (n == NULL) {
		return;
	}

	for (int i = 0; i < (height(root) - height(n)); i++) {
		cout << "-";
	}

	if (n->isRed) {
		cout << "(RED) ";
	} else {
		cout << "(BLACK) ";
	}

	cout << n->name << endl;

	preorder(n->left);
	preorder(n->right);
}

void RBTree::printPreOrder() {
	if (root == NULL) {
		cout << "Empty tree" << endl;
	} else {
		preorder(root);
	}
}

int RBTree::height(Node* n) {
	if (n == NULL) {
		return 0;
	} else {
		int left_height = height(n->left);
		int right_height = height(n->right);

		if (left_height > right_height) {
			return (left_height + 1);
		} else {
			return (right_height + 1);
		}
	}
}

int main(int argc, char* argv[]) {
	if (argc != 2) {
		cout << "Need one argument." << endl;
		exit(1);
	}

	string filename = argv[1];

	ifstream file;
	file.open(filename);

	if (!file) {
		cout << "File cannot be opened!";
		exit(1);
	}

	RBTree Tree;

	string line;
	string previous_line_season;
	bool startedReading = false;
	bool firstSeason = true;

	getline(file, line);

	while (getline(file, line)) {
		stringstream s(line);
		string word; 

		struct Player player;

		getline(s, word, ','); 
		string season = word;

		if (previous_line_season != season && startedReading) { //start of a new season
			cout << "End of the " << previous_line_season << " Season" << endl;
			Tree.updateLeaders(Tree.root);
			Tree.printLeaders();

			if (firstSeason) {
				Tree.printPreOrder();
			}

			firstSeason = false;

		}

		getline(s, word, ','); 
		player.name = word;

		getline(s, word, ','); 
		player.team = word;

		getline(s, word, ','); 
		player.rebound = stoi(word);

		getline(s, word, ','); 
		player.assist = stoi(word);

		getline(s, word, ','); 
		player.point = stoi(word);

		previous_line_season = season;

		Tree.insert(player);

		startedReading = true;

	}
	
	//for last season
	cout << "End of the " << previous_line_season << " Season" << endl;
	Tree.updateLeaders(Tree.root);
	Tree.printLeaders(); 
	

	Tree.releaseNode(Tree.root);

	return 0;
}
