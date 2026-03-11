#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <cmath>
#include <sstream>
#include <random>

struct object {
	int a;
	char b;
};
std::ostream& operator<<(std::ostream& os, const object& obj) {
	os << "a = " << obj.a << ", b = " << obj.b;
	return os;
}

template <typename T> class HashTable {
	struct Pair {
		std::string key;
		T data;
	};

	class linkedList {
	public:
		struct Node {
			Pair data;
			Node* next = nullptr;
			Node* prev = nullptr;
		};
	private:
		Node* head = nullptr;
		Node* tail = nullptr;
		size_t length = 0;
	public:
		bool isEmpty() {
			return length == 0;
		}
		size_t getLength() {
			return length;
		}
		void addTail(Pair object) {
			Node* newTail = new Node;
			newTail->data = object;
			newTail->next = nullptr;
			if (length == 0) {
				newTail->prev = nullptr;
				head = newTail;
			}
			else {
				tail->next = newTail;
				newTail->prev = tail;
			}
			tail = newTail;
			length++;
			return;
		}
		void delHead() {
			if (length == 0) {
				std::cerr << "List is empty!";
				return;
			}
			else if (length == 1) {
				delete head;
				head = nullptr;
				tail = nullptr;
			}
			else {
				head->next->prev = nullptr;
				Node* temp = head;
				head = head->next;
				delete temp;
			}
			length--;
			return;
		}
		void delTail() {
			if (length == 0) {
				std::cerr << "List is empty!";
				return;
			}
			else if (length == 1) {
				delete tail;
				head = nullptr;
				tail = nullptr;
			}
			else {
				tail->prev->next = nullptr;
				Node* temp = tail;
				tail = tail->prev;
				delete temp;
			}
			length--;
			return;
		}
		Pair* findByKey(const std::string& key) {
			Node* current = head;
			while (current) {
				if (current->data.key == key) {
					return &(current->data);
				}
				current = current->next;
			}
			return nullptr;
		}
		int findByKeyAndDelete(const std::string& key) {
			auto current = head;
			while (current) {
				if (current->data.key == key) {
					if (current == head) {
						delHead();
						return 1;
					}
					else if (current == tail) {
						delTail();
						return 1;
					}
					else {
						current->prev->next = current->next;
						current->next->prev = current->prev;
						delete current;
						--length;
						return 1;
					}
				}
				current = current->next;
			}
			return 0;
		}
		void clearList() {
			size_t len = getLength();
			for (unsigned i = 0; i < len; i++) {
				delTail();
			}
		}
		std::string toString(unsigned int limit) {
			std::stringstream ss;
			Node* current = head;
			unsigned int count = 0;
			while (current && count < limit) {
				ss << "[" << current->data.key << ": " << current->data.data << "] -> ";
				current = current->next;
				count++;
			}
			if (length == 0) ss << "Empty\n";
			else ss << ". Total elements: " << length <<"\n";
			return ss.str();
		}
		Node* operator[](unsigned int index) {
			if (index >= length) {
				throw std::out_of_range("Index out of range!");
				return nullptr;
			}
			Node* current = head;
			for (unsigned int i = 0; i < index; i++) {
				current = current->next;
			}
			return current;
		}
		~linkedList() {
			clearList();
		}
	};
	class DynamicArray {
		unsigned int size;
		unsigned int occupiedLists;
		linkedList* array;
	public:
		DynamicArray& operator=(DynamicArray&& other) noexcept {
			if (this != &other) {
				delete[] array;
				size = other.size;
				occupiedLists = other.occupiedLists;
				array = other.array;
				other.array = nullptr;
				other.size = 0;
				other.occupiedLists = 0;
			}
			return *this;
		}
		DynamicArray() {
			size = 1;
			occupiedLists = 0;
			array = new linkedList[size];
		}
		DynamicArray(unsigned int n) {
			size = n;
			occupiedLists = 0;
			array = new linkedList[size];
		}
		void incrementOccupiedLists() {
			occupiedLists++;
		}
		unsigned int getOccupiedLists() {
			return occupiedLists;
		}
		unsigned int getSize() const {
			return size;
		}
		linkedList& operator[](unsigned int index) {
			if (index >= size) {
				throw std::out_of_range("Index out of range");
			}
			return array[index];
		}
		void clear() {
			if (array != nullptr) {
				for (unsigned int i = 0; i < size; ++i) {
					array[i].clearList();
				}
				delete[] array;
				array = nullptr;
			}
			size = 1;
			occupiedLists = 0;
			array = new linkedList[size];
		}
		std::string toString() {
			std::stringstream ss;
			for (unsigned int i = 0; i < size; i++) {
				ss << "Index " << i << ": " << array[i].toString() << "\n";
			}
			return ss.str();
		}
		~DynamicArray() {
			delete[] array;
		}
	};
	float maxLoad = 0.75;
	unsigned int elements = 0;
	DynamicArray array = DynamicArray();
	float currentLoad() {
		return (float)array.getOccupiedLists() / (float)array.getSize();
	}
	unsigned int largestListLength = 0;
public:
	unsigned int hash(const std::string& s, unsigned int arraySize) {
		unsigned int key = 0;
		for (int i = 1; i <= s.length(); i++) {
			key += s[i - 1] * pow(31, s.length() - i);
		}
		return (key % arraySize);
	}
	void add(const std::string& key, const T& data) {
		if (currentLoad() >= maxLoad) {
			rehash();
		}
		unsigned int index = hash(key, array.getSize());
		if (array[index].isEmpty()) array.incrementOccupiedLists();
		Pair newPair(key, data);
		array[index].addTail(newPair);
		unsigned int length = array[index].getLength();
		if (length > largestListLength) {
			largestListLength = length;
		}
		elements++;
	}
	Pair* find(const std::string& key) {
		unsigned int index = hash(key, array.getSize());
		return array[index].findByKey(key);
	}
	int remove(const std::string& key) {
		unsigned int index = hash(key, array.getSize());
		return array[index].findByKeyAndDelete(key);
	}
	void clear() {
		array.clear();
		elements = 0;
		largestListLength = 0;
	}
	std::string toString() {
		std::stringstream ss;
		ss << "HashTable:\n";
		for (unsigned int i = 0; i < array.getSize() && i < 5; i++) {
			ss << "Index " << i << ": ";
			ss << array[i].toString();
			if (i == 4 || i == (array.getSize() - 1)) ss << "...\n";
		}
		return ss.str();
	}
	void rehash() {
		std::cout << "Rehashing from size " << array.getSize() << " to size " << array.getSize() * 2 << "\n";
		DynamicArray newArray = DynamicArray(array.getSize()*2);
		for (unsigned int i = 0; i < array.getSize(); i++) {
			linkedList& currentList = array[i];
			if (currentList.isEmpty()) continue;
			typename linkedList::Node* currentNode = currentList[0];
			for (size_t j = 0; j < currentList.getLength(); j++) {
				Pair currentPair = currentNode->data;
				unsigned int newIndex = hash(currentPair.key, newArray.getSize());
				if (newArray[newIndex].isEmpty()) newArray.incrementOccupiedLists();
				newArray[newIndex].addTail(currentPair);
				currentNode = currentNode->next;
			}
		}
		array = std::move(newArray);
	}
	std::string stats() {
		std::stringstream ss;
		ss << "HashTable stats:\n";
		ss << "Total elements: " << elements << "\n";
		ss << "Array size: " << array.getSize() << "\n";
		ss << "Occupied lists: " << array.getOccupiedLists() << "\n";
		ss << "Load factor: " << currentLoad() << "\n";
		ss << "Largest list length: " << largestListLength << "\n";
		return ss.str();
	}
};
static std::string generateRandomKey(int length) {
	std::string key;
	key.reserve(length);
	static std::mt19937 rng{ std::random_device{}() };
	static std::uniform_int_distribution<int> dist(0, 25);
	for (int i = 0; i < length; i++) {
		key.push_back(static_cast<char>('a' + dist(rng)));
	}
	return key;
}

int main()
{
	HashTable<object> HT;
	const int MAX_ORDER = 7;
	for (int i = 1; i <= MAX_ORDER; i++) {
		const int n = pow(10, i);
		clock_t start = clock();
		for (int j = 0; j < n; j++) {
			object obj = { rand()%1000, 'a' + (rand() % 26)};
			HT.add(generateRandomKey(6), obj);
		}
		clock_t end = clock();
		double diff = double(end - start) / CLOCKS_PER_SEC * 1000;
		std::cout << "Added " << n << " elements in " << diff << " ms.\n";
		std::cout << "Time per element: " << diff / n << " ms.\n";
		//std::cout << HT.toString();

		const int m = pow(10, 4);
		int hits = 0;
		start = clock();
		for (int i = 0; i < m; i++) {
			if(HT.find(generateRandomKey(6)) != nullptr) hits++;
		}
		end = clock();
		diff = double(end - start) / CLOCKS_PER_SEC * 1000;
		std::cout << "Searched for " << m << " elements in " << diff << " ms.\n";
		std::cout << "Time per element: " << diff / m << " ms.\n";
		std::cout << "Got " << hits << " hits.\n";
		std::cout << HT.stats();
		std::cout << "----------------------------------------\n";
		HT.clear();
	}
	system("pause");
	return 0;
}