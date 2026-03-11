#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <numeric>
#include <algorithm>

struct object {
	int a;
	char b;
};

std::ostream& operator<<(std::ostream& os, const object& obj) {
	os << "a = " << obj.a << ", b = " << obj.b;
	return os;
}

template <typename T> class RedBlackTree {
private:
	struct Node {
		T data;
		Node* parent;
		Node* left;
		Node* right;
		bool isRed;
		unsigned int ID;
		Node* uncle() {
			if (parent == nullptr || parent->parent == nullptr) return nullptr;
			return (parent == parent->parent->left) ? parent->parent->right : parent->parent->left;
		}
		Node* grandParent() {
			if (parent == nullptr || parent->parent == nullptr) return nullptr;
			return parent->parent;
		}
	};
	Node* root;
	unsigned int size;
	unsigned int lastAssignedID;

public:
	RedBlackTree() {
		root = nullptr;
		size = 0;
		lastAssignedID = -1;
	}
	T find(T data, int(*cmp)(const T&, const T&)) {
		Node* current = root;
		while (current != nullptr) {
			if (cmp(data, current->data) == 0) {
				return current->data;
			}
			else if (cmp(data, current->data) < 0) {
				current = current->left;
			}
			else if (cmp(data, current->data) > 0) {
				current = current->right;
			}
		}
	}
	void preOrder(std::ostream& os, Node* node, int maxLevels = -1) {
		Node* current = node;
		if (current == nullptr || maxLevels == 0) return;
		os << "(" << current->ID << ": [" << ((current->isRed) ? "red, p: " : "black, p: ") << ((current->parent != nullptr) ? std::to_string(current->parent->ID) : "NULL") << ", l: " << ((current->left != nullptr) ? std::to_string(current->left->ID) : "NULL") << ", r: " << ((current->right != nullptr) ? std::to_string(current->right->ID) : "NULL") << "], data: " << current->data << ")\n";
		preOrder(os, current->left, maxLevels - 1);
		preOrder(os, current->right, maxLevels - 1);
	}
	void inOrder(std::ostream& os, Node* node = root, int maxLevels = -1) {
		Node* current = node;
		if (current == nullptr || maxLevels == 0) return;
		inOrder(os, current->left, maxLevels - 1);
		os << "(" << current->ID << ": [" << ((current->isRed) ? "red, p: " : "black, p: ") << ((current->parent != nullptr) ? std::to_string(current->parent->ID) : "NULL") << ", l: " << ((current->left != nullptr) ? std::to_string(current->left->ID) : "NULL") << ", r: " << ((current->right != nullptr) ? std::to_string(current->right->ID) : "NULL") << "], data: " << current->data << ")\n";
		inOrder(os, current->right, maxLevels - 1);
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
		return height + std::max(getHeight(current->left), getHeight(current->right));
	}
	void add(T data, int(*cmp)(const T&, const T&)) {
		if (root == nullptr) {
			root = new Node{ data, nullptr, nullptr, nullptr, false, lastAssignedID + 1 };
			lastAssignedID++;
			size++;
			return;
		}
		else {
			Node* current = root;
			while (true) {
				if (cmp(data, current->data) < 0) {
					if (current->left == nullptr) {
						current->left = new Node{ data, current, nullptr, nullptr, true, lastAssignedID + 1 };
						lastAssignedID++;
						size++;
						current = current->left;
						fixViolations(current);
						return;
					}
					else {
						current = current->left;
					}
				}
				else {
					if (current->right == nullptr) {
						current->right = new Node{ data, current, nullptr, nullptr, true, lastAssignedID + 1 };
						lastAssignedID++;
						size++;
						current = current->right;
						fixViolations(current);
						return;
					}
					else {
						current = current->right;
					}
				}
			}
		}
	}
	void fixViolations(Node* node) {
		Node* current = node;
		if (current == nullptr) return;

		while (current != root && current->parent != nullptr && current->parent->isRed) {
			Node* parent = current->parent;
			Node* grandParent = current->grandParent();
			if (grandParent == nullptr) break;
			Node* uncle = current->uncle();
			if (uncle != nullptr && uncle->isRed) {
				parent->isRed = false;
				uncle->isRed = false;
				grandParent->isRed = true;
				current = grandParent;
			}
			else {
				//L
				if (parent == grandParent->left) {
					//LR
					if (current == parent->right) {
						rotateLeft(parent, current);
						parent = current;
					}
					// LL
					rotateRight(grandParent, parent);
					parent->isRed = false;
					grandParent->isRed = true;
				}
				//R
				else {
					// RL
					if (current == parent->left) {
						rotateRight(parent, current);
						parent = current;
					}
					// RR
					rotateLeft(grandParent, parent);
					parent->isRed = false;
					grandParent->isRed = true;
				}
				break;
			}
		}
		if (root != nullptr) root->isRed = false;
	}
	std::string toString(int maxLevels = 5) {
		std::ostringstream oss;
		oss << "Red-Black Tree:\nsize: " << size << "\nheight: " << getHeight(root) << "\n{\n";
		preOrder(oss, root, maxLevels);
		oss << "}\n";
		return oss.str();
	}
	void rotateLeft(Node* parent, Node* child) {
		if (parent == nullptr || child == nullptr) return;
		parent->right = child->left;
		if (child->left != nullptr) child->left->parent = parent;
		child->parent = parent->parent;
		if (parent->parent == nullptr) {
			root = child;
		}
		else if (parent == parent->parent->left) {
			parent->parent->left = child;
		}
		else {
			parent->parent->right = child;
		}
		child->left = parent;
		parent->parent = child;
	}
	void rotateRight(Node* parent, Node* child) {
		if (parent == nullptr || child == nullptr) return;
		parent->left = child->right;
		if (child->right != nullptr) child->right->parent = parent;
		child->parent = parent->parent;
		if (parent->parent == nullptr) {
			root = child;
		}
		else if (parent == parent->parent->right) {
			parent->parent->right = child;
		}
		else {
			parent->parent->left = child;
		}
		child->right = parent;
		parent->parent = child;
	}
	Node* getRoot(){
		if (root == nullptr) return nullptr;
		return root;
	}
	T get_root_data() {
		return root->data;
	}
};

int intComparator(const int& a, const int& b) {
	return a - b;
}
int dataComparator(const object& a, const object& b) {
	int diff;
	diff = a.a - b.a;
	if (diff != 0) {
		return diff;
	}
	else return a.b - b.b;
}
void test()
{
	std::vector<int> v(8);
	std::iota(v.begin(), v.end(), 0);
	std::map<int, int> heights, root_data;
	do
	{
		RedBlackTree<int> t;
		for (int i : v)
			t.add(i, intComparator);
		++heights[t.getHeight(t.getRoot())];
		++root_data[t.get_root_data()];
	} while (std::next_permutation(v.begin(), v.end()));
	for (auto [height, cnt] : heights)
		std::cout << height << ": " << cnt << std::endl;
	std::cout << "***" << std::endl;
	for (auto [data, cnt] : root_data)
		std::cout << data << ": " << cnt << std::endl;
}
int main()
{
	test();
	const int MAX = 7;
	RedBlackTree<object> RBT;
	for (int o = 1; o <= MAX; o++) {
		const int n = pow(10, o);
		clock_t start = clock();
		for (int i = 0;i < n;i++) {
			object newObject{ rand() % 100000, rand() % 26 + 'a' };
			RBT.add(newObject, dataComparator);
		}
		clock_t end = clock();
		double diff = double(end - start) / CLOCKS_PER_SEC * 1000;
		std::cout << "----------------------------------------\n";
		std::cout << "Added " << n << " elements in " << diff << " ms.\n";
		std::cout << "Time per element: " << diff / n << " ms.\n";
		std::cout << RBT.toString();
		const int m = pow(10, 4);
		int hits = 0;
		start = clock();
		for (int i = 0;i < m;i++) {
			object newObject{ rand() % 100000, rand() % 26 + 'a' };
			object result = RBT.find(newObject, dataComparator);
			if (dataComparator(result, newObject) == 0)  hits++;
		}
		end = clock();
		diff = double(end - start) / CLOCKS_PER_SEC * 1000;
		std::cout << "Searched " << m << " elements in " << diff << " ms.\n";
		std::cout << "Time per element: " << diff / m << " ms.\n";
		std::cout << "Found " << hits << " elements.\n";
		std::cout << "----------------------------------------\n\n\n";
		RBT.clear();
	}
	system("pause");
	return 0;
}