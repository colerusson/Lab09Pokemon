#ifndef SET_H
#define SET_H
#include "SetInterface.h"
#include "Deque.h"
#include "Pair.h"
#include "HashMap.h"
#include <string>
using namespace std;

template<typename T>
class Set : public SetInterface<T> {				//Use bst here to create set
private:
	struct Node {		//Node struct nested within the BST template class
		struct Node* left_;		//left and right nodes to use in a tree
		struct Node* right_;
		T data_;
		int level_;

		Node(T data, Node* left = NULL, Node* right = NULL) :		//constructor for struct node, passing in data and setting left and right to NULL
			data_(data), left_(left), right_(right), level_(0) {}
		~Node() = default;

		friend ostream& operator<<(ostream& os, const struct Node* node) {		//friend operator to print out data
			os << node->data_;
			return os;
		}
	};
	Node* root;		//head or root node of the tree

	bool addNode(Node*& node, const T& data) {		//insert recursive function which is called from the public section of BST class
		if (node == NULL) {
			node = new Node(data);
			return true;
		}
		if (data < node->data_) return addNode(node->left_, data);
		if (data > node->data_) return addNode(node->right_, data);
		return false;
	}

	bool find(Node* node, const T& value) {		//find recursive function which is called from the public section of BST class
		if (node == NULL) return false;
		if (node->data_ == value) return true;
		if (value < node->data_) return find(node->left_, value);
		return find(node->right_, value);
	}

	bool deleteTree(Node* node) {		//clear tree recursive function which is called from the public section of BST class
		if (node == NULL) return false;
		deleteTree(node->left_);
		deleteTree(node->right_);
		delete node;
		root = NULL;
		return true;
	}

	size_t sizeTree(Node* node) {		//size recursive function which is called from the public section of BST class
		if (node == NULL) {
			return 0;
		}
		else {
			return(sizeTree(node->left_) + 1 + sizeTree(node->right_));
		}
	}

	void copyTree(Node*& nodeCopy, Node* nodeOther) const {	//copy recursive function which is called from the public section of BST class
		if (nodeOther == NULL) {
			nodeCopy = NULL;
		}
		else {
			nodeCopy = new Node(nodeOther->data_);
			copyTree(nodeCopy->left_, nodeOther->left_);
			copyTree(nodeCopy->right_, nodeOther->right_);
		}
	}

public:
	Set() : root(NULL) {}
	virtual ~Set() {
		clear();			
	}

	virtual bool insert(const T& item) {		//recurse insertion function which calls addNode for the set
		return addNode(root, item);
	}

	virtual void clear() {		//free up the dynamically allocated memory
		deleteTree(root);
	}

	virtual size_t size() {			//retunr the size of the set
		return sizeTree(root);
	}

	virtual size_t count(const T& item) {		//return one if item is found, else 0
		size_t one = 1;
		size_t zero = 0;
		if (find(root, item) == true) {
			return one;
		}
		else return zero;
	}

	void infixLevel(Node* root, stringstream& out) const {		//function which handles level order output of the tree
		if (root == NULL) return;
		infixLevel(root->left_, out);
		out << root->data_ << ",";
		infixLevel(root->right_, out);
	}

	virtual string toString() const {			//toString function which outputs infix level of the set
		stringstream out;
		if (root == NULL) out << " empty";
		else {
			infixLevel(root, out);
		}
		string newLine = out.str();
		newLine = newLine.substr(0, newLine.size() - 1);
		return newLine;
	}

	friend ostream& operator<<(ostream& os, const Set<T>& set) {
		os << set.toString();
		return os;
	}

	Set<T>(const Set<T>& other) {		//deep copy constructor for the class, which calls the recursive copyTree function in private
		if (other.root == NULL) {
			this->root = NULL;
		}
		else {
			copyTree(this->root, other.root);
		}
		return;
	}

	Set<T>& operator=(Set<T>& other) {		//deep assignment operator, which calls the deep copy constructor 
		Set<T> newTree(other);
		swap(newTree.root, root);
		return *this;
	}

	class Iterator {		//nested iterator class to output level order tree with iterator
	friend class Set;
	private:
		Node* node_;
		Deque<Node*> iterStack;		//has-a deque class to use as a stack object to push elements of the bst into a circular array stack
	public:
		Iterator(Node* root) : node_(root) {		//Iterator constructor, setting Node* equal to root
			Node* node_ = root;
			while (node_ != NULL) {			//pushing each item of the bst into the stack of Nodes 
				iterStack.push_back(node_);
				node_ = node_->left_;
			}
		}
		~Iterator() = default;

		Node* curr() {		//returns the node that the iterator is pointing to at the top of the stack
			return iterStack.back();
		}

		void next() {		//moves the iterator node to the next item in the stack, and removes the previous one
			Node* curr = iterStack.back()->right_;
			iterStack.pop_back();
			while (curr != NULL) {
				iterStack.push_back(curr);
				curr = curr->left_;
			}
		}

		Node* nextItem() {			//get the next item that the iterator will point to 
			Node* next = iterStack.back()->right_;
			return next;
		}

		bool isEnd() {			//returns if we are at the end of our stack
			return !(iterStack.size());
		}

		size_t iterSize() {		//return the size of the stack with an iterator 
			return iterStack.size();
		}
	};

	Iterator begin() { return Iterator(this->root); }
};
#endif // SET_H