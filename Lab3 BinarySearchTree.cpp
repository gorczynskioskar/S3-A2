#include <iostream>
#include <locale.h>
#include <string>
#include <sstream>
#include <cstdlib>

struct object {
	int a;
	char b;
};
std::ostream& operator<<(std::ostream& os, const object& obj) {
	os << "a: " << obj.a << ", b: " << obj.b;
	return os;
}

template <typename T> class BinarySearchTree {
	struct Node {
		unsigned int ID;
		T data;
		Node* parent;
		Node* left;
		Node* right;
	};
	Node* root;
	unsigned int size;
	unsigned int lastAssignedID;
public:
	BinarySearchTree() {
		root = nullptr;
		size = 0;
		lastAssignedID = -1;
	}
	void addElement(T object, int(*cmp)(const T&, const T&)) {
		if (root == nullptr) {
			root = new Node{ lastAssignedID+1, object, nullptr, nullptr, nullptr };
			lastAssignedID++;
			size=1;
			return;
		}
		else {
			Node* current = root;
			while (true) {
				if (cmp(object, current->data) < 0) {
					if (current->left == nullptr) {
						current->left = new Node{ lastAssignedID+1, object, current, nullptr, nullptr };
						lastAssignedID++;
						size++;
						return;
					}
					else {
						current = current->left;
					}
				}
				else {
					if (current->right == nullptr) {
						current->right = new Node{ lastAssignedID + 1, object, current, nullptr, nullptr };
						lastAssignedID++;
						size++;
						return;
					}
					else {
						current = current->right;
					}
				}
			}
		}
	}
	T findElement(T object, int(*cmp)(const T&, const T&)) {
		Node* current = root;
		while (current != nullptr) {
			if (cmp(object, current->data) == 0) {
				return current->data;
			}
			else if (cmp(object, current->data) < 0) {
				current = current->left;
			}
			else {
				current = current->right;
			}
		}
	}
	void findAndDeleteElement(T object, int(*cmp)(const T&, const T&)) {
		Node* current = root;
		Node* parent = nullptr;
		bool isLeftChild = false;
		// Find element to delete
		while (current != nullptr) {
			int cmpResult = cmp(object, current->data);
			if (cmpResult == 0) break;
			parent = current;
			if(cmpResult < 0) {
				current = current->left;
				isLeftChild = true;
			}
			else {
				current = current->right;
				isLeftChild = false;
			}
		}
		if (current == nullptr) return;
		// Case 1: Node has no children
		if (current->left == nullptr && current->right == nullptr) {
			if (current == root) {
				root = nullptr;
				delete current;
			}
			else if (isLeftChild) {
				parent->left = nullptr;
				delete current;
			}
			else {
				parent->right = nullptr;
				delete current;
			}
		}
		// Case 2: Node has one child
		else if (current->left == nullptr || current->right == nullptr) {
			Node* child = (current->left != nullptr) ? current->left : current->right;
			if (current == root) {
				root = child;
				child->parent = nullptr;
				delete current;
			}
			else if (isLeftChild) {
				parent->left = child;
				child->parent = parent;
				delete current;
			}
			else {
				parent->right = child;
				child->parent = parent;
				delete current;
			}
		}
		// Case 3: Node has two children
		else {
			Node* successorParent = current;
			Node* successor = current->right;
			while (successor->left != nullptr) {
				successorParent = successor;
				successor = successor->left;
			}
			current->data = successor->data;
			if (successorParent->left == successor) {
				successorParent->left = successor->right;
				if (successor->right != nullptr) {
					successor->right->parent = successorParent;
				}
			}
			else {
				successorParent->right = successor->right;
				if (successor->right != nullptr) {
					successor->right->parent = successorParent;
				}
			}
			delete successor;
		}
		size--;
	}
	void preOrder(std::ostream& oss, Node* root = root, int maxLevels = -1) {
		Node* current = root;
		if (current == nullptr || maxLevels == 0) return;
		oss << "(" << current->ID << ": " << "p: " << ((current->parent != nullptr) ? std::to_string(current->parent->ID) : "NULL") << ", l: " << ((current->left != nullptr) ? std::to_string(current->left->ID) : "NULL") << ", r: " << ((current->right != nullptr) ? std::to_string(current->right->ID) : "NULL") << "], data: " << current->data << ")\n";
		preOrder(oss, current->left, maxLevels - 1);
		preOrder(oss, current->right, maxLevels - 1);
	}
	void inOrder(std::ostream& oss, Node* root = root, int maxLevels = -1) {
		Node* current = root;
		if (current == nullptr) return;
		inOrder(oss, current->left, maxLevels - 1);
		oss << "(" << current->ID << ": " << "p: " << current->parent->ID << ", l: " << current->left->ID << ", r: " << current->right->ID << "], data: " << current->data << ")\n";
		inOrder(oss, current->right, maxLevels - 1);
	}
	void clearNode(Node* node) {
		Node* current = node;
		if (current == nullptr) return;
		clearNode(current->left);
		clearNode(current->right);
		delete current;
	}
	void clear() {
		clearNode(root);
		root = nullptr;
		size = 0;
		lastAssignedID = -1;
	}
	unsigned int getHeight(Node* node) {
		int height = 0;
		Node* current = node;
		if (current == nullptr) return height;
		height++;
		return height+std::max(getHeight(current->left), getHeight(current->right));
	}
	std::string toString(int maxLevels = 5) {
		std::ostringstream oss;
		oss << "BST:\n";
		oss << "size: " << size << "\n";
		oss << "height: " << getHeight(root) << "\n{\n";
		preOrder(oss, root, maxLevels);
		oss << "}\n";
		return oss.str();
	}
};
int dataComparator(const object& a, const object& b) {
	int diff;
	diff = a.a - b.a;
	if (diff != 0) {
		return diff;
	}
	else return a.b - b.b;
}
int main()
{
	const int maxOrder = 7;
	BinarySearchTree<object> BST;
	for (int o = 1; o <= maxOrder; o++) {
		const int n = pow(10, o);
		clock_t start = clock();
		for (int i = 0; i < n; i++) {
			object newElement{ rand() % 100000, rand() % 26 + 'a' };
			BST.addElement(newElement, dataComparator);
		}
		clock_t end = clock();
		double diff = double(end - start) / CLOCKS_PER_SEC * 1000;
		std::cout << "----------------------------------------\n";
		std::cout << "Added " << n << " elements in " << diff << " ms.\n";
		std::cout << "Time per element: " << diff / n << " ms.\n";
		std::cout << BST.toString(3);
		const int m = pow(10, 4);
		int hits = 0;
		start = clock();
		for (int i = 0; i < m; i++) {
			object newElement{ rand() % 100000, rand() % 26 + 'a' };
			object result = BST.findElement(newElement, dataComparator);
			if (dataComparator(result, newElement) == 0) {
				hits++;
			}
		}
		end = clock();
		diff = double(end - start) / CLOCKS_PER_SEC * 1000;
		std::cout << "Searched " << m << " elements in " << diff << " ms.\n";
		std::cout << "Found " << hits << " elements.\n";
		std::cout << "Time per element: " << diff / m << " ms.\n";
		std::cout << "----------------------------------------\n\n\n";
		BST.clear();
	}
	system("pause");
	return 0;
}
