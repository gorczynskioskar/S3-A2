#include <iostream>
#include <sstream>
#include <string>
#include <random>
struct object {
    double key;
    char field1;
    bool operator==(const object& other) const {
        return key == other.key && field1 == other.field1;
	}
};
std::ostream& operator<<(std::ostream& os, const object& obj) {
    os << "{" << obj.key << ", " << obj.field1<<"} ";
    return os;
}
template <typename T> class linkedList {
private:
    struct Node {
        T data;
        Node* next;
		Node* prev;
    };
    Node* head = nullptr;
    Node* tail = nullptr;
    size_t length = 0;
public:
    size_t getLength() {
        return length;
    }
    void addTail(T object) {
        auto newTail = new Node;
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
    T popHead() {
        if (length == 0) throw std::out_of_range("List is empty");
        T object = head->data;
        Node* oldHead = head;
        head = head->next;
        delete oldHead;
        length--;
        if (length == 0) {
            tail = nullptr;
        }
        return object;
	}
    void insertionSort(int(*cmp)(const T&, const T&)) {
        if (length < 2) return;
        Node* current = head->next;
        while (current != nullptr) {
            T key = current->data;
            Node* prevNode = current->prev;
            while (prevNode != nullptr && cmp(prevNode->data, key)>0) {
                prevNode->next->data = prevNode->data;
                prevNode = prevNode->prev;
            }
            if (prevNode == nullptr) {
                head->data = key;
            }
            else {
                prevNode->next->data = key;
            }
            current = current->next;
		}
    }
    void delTail() {
        if (length == 0) {
            std::cout << "Lista jest pusta!";
            return;
        }
        else if (length == 1) {
            delete tail;
            head = nullptr;
            tail = nullptr;
        }
        else {
            tail->prev->next = nullptr;
            auto temp = tail;
            tail = tail->prev;
            delete temp;
        }
        length--;
        return;
    }
    void clearList() {
        size_t len = getLength();
        for (unsigned i = 0; i < len; i++) {
            delTail();
        }
    }
    ~linkedList() {
        clearList();
	}
};

template <typename T> class BinaryHeap {
    class DynamicArray {
        unsigned int maxCapacity;
        unsigned int currentSize;
        T* array;
    public:
        DynamicArray() {
            maxCapacity = 1;
            currentSize = 0;
            array = new T[maxCapacity];
        }
        unsigned int size() const {
            return currentSize;
        }
        T& operator[](unsigned int index) {
            if (index >= currentSize) {
                throw std::out_of_range("Index out of range");
            }
            return array[index];
        }
        void pop_back() {
            if (currentSize > 0) currentSize--;
            else throw std::out_of_range("Array is empty");
        }
        void add(T element) {
            if (currentSize >= maxCapacity) {
                maxCapacity *= 2;
                T* newArray = new T[maxCapacity];
                for (unsigned int i = 0; i < currentSize; i++) {
                    newArray[i] = array[i];
                }
                delete[] array;
                array = newArray;
            }
            array[currentSize] = element;
            currentSize++;
        }
        void swap(unsigned int index1, unsigned int index2) {
            if (index1 >= currentSize || index2 >= currentSize) {
                throw std::out_of_range("Index out of range");
            }
            if (index1 == index2) return;
            T temp = array[index1];
            array[index1] = array[index2];
            array[index2] = temp;
        }
        void clear() {
            delete[] array;
            maxCapacity = 1;
            currentSize = 0;
            array = new T[maxCapacity];
        }
        void importData(T* data, unsigned int size) {
            delete[] array;
            maxCapacity = size;
            currentSize = size;
            array = new T[maxCapacity];
            for (unsigned int i = 0; i < size; i++) {
                array[i] = data[i];
            }
		}
        void setSize(unsigned int n) {
			currentSize = n;
        }
        T* exportData() {
            return array;
		}
        ~DynamicArray() {
            delete[] array;
        }
    };
    DynamicArray array;
	bool bottom_up;
	int(*cmp)(const T&, const T&);
public:
	BinaryHeap(T* data, unsigned int size, int(*cmp)(const T&, const T&), bool bottom_up) {
		array.importData(data, size);
		array.setSize(size);
		this->bottom_up = bottom_up;
        this->cmp = cmp;
		if (bottom_up) fix_bottom_up();
    }
    void add(T object) {
        array.add(object);
        unsigned int index = array.size() - 1;
        if(!bottom_up) heap_up(index);
    }
    void heap_up(unsigned int index) {
        while (index > 0) {
            unsigned int parent_index = (index - 1) / 2;
            if (cmp(array[index], array[parent_index]) > 0) {
                array.swap(index, parent_index);
                index = parent_index;
            }
            else {
                break;
            }
        }
    }
    void heap_down(unsigned int index) {
        while (true) {
            if (2 * index + 1 >= array.size()) {
                break;
            }
            if (2 * index + 2 >= array.size()) {
                if (cmp(array[index], array[2 * index + 1]) < 0) {
                    array.swap(index, 2 * index + 1);
                    index = 2 * index + 1;
                }
                else {
                    break;
                }
                continue;
            }
            if (cmp(array[2 * index + 1], array[2 * index + 2]) > 0) {
                if (cmp(array[index], array[2 * index + 1]) < 0) {
                    array.swap(index, 2 * index + 1);
                    index = 2 * index + 1;
                }
                else {
                    break;
                }
            }
            else {
                if (cmp(array[index], array[2 * index + 2]) < 0) {
                    array.swap(index, 2 * index + 2);
                    index = 2 * index + 2;
                }
                else {
                    break;
                }
            }
        }
    }
    T delete_root() {
        if (array.size() == 0) throw std::out_of_range("Heap is empty");
        T object = array[0];
        array[0] = array[array.size() - 1];
        array.pop_back();
        if (array.size() > 0) heap_down(0);
        return object;
    }
    void clear() {
        array.clear();
    }
    T pop_back() {
        T object = array[array.size() - 1];
		array.pop_back();
        return object;
    }
    void sort() {
        unsigned int originalSize = array.size();
        for (unsigned int i = 0; i < originalSize; i++) {
            array.swap(0, array.size() - 1);
            pop_back();
			if (array.size() > 0) heap_down(0);
        }
		array.setSize(originalSize);
	}
    T* getData() {
        return array.exportData();
    }
    void fix_bottom_up() {
        if (!bottom_up) return;
        for (int i = array.size() / 2 - 1; i >= 0; i--) {
            heap_down(i);
		}
    }
    void fix_top_down() {
        if (bottom_up) return;
        for (int i = 1; i < array.size(); i++) {
            heap_up(i);
        }
	}
};
void counting_sort(int* data, unsigned int size, unsigned int max) {
    int* counts = new int[max + 1];
    for (int i = 0; i <= max; i++) {
        counts[i] = 0;
    }
    for (int i = 0; i < size; i++) {
        counts[data[i]]++;
    }
    unsigned int j = 0;
    for (int i = 0; i <= max; i++) {
        while (counts[i] != 0) {
            data[j] = i;
            counts[i]--;
            j++;
        }
    }
	delete[] counts;
}
int objectComparator(const object& a, const object& b) {
    if (a.key < b.key) return -1;
    if (a.key > b.key) return 1;
	return 0;
}
template <typename T> void bucket_sort(T* data, unsigned int size, int(*cmp)(const T&, const T&)) {
    const int No_OF_BUCKETS = size;
    linkedList<T>* buckets = new linkedList<T>[No_OF_BUCKETS];
    for (int i = 0; i < size; i++) {
        int bucketIndex = data[i].key * No_OF_BUCKETS;
        buckets[bucketIndex].addTail(data[i]);
    }
    for (int i = 0; i < No_OF_BUCKETS; i++) {
        buckets[i].insertionSort(cmp);
    }
    int j = 0;
    for (int i = 0; i < No_OF_BUCKETS; i++) {
        while (buckets[i].getLength() > 0) {
            data[j++] = buckets[i].popHead();
        }
    }
    delete[] buckets;
}
int intComparator(const int& a, const int& b) {
    return a - b;
}
void bucket_sort(int* data, unsigned int size) {
    const unsigned int No_OF_BUCKETS = size;
    int max = data[0];
    for (int i = 1; i < size; i++) {
        if (data[i] > max) {
            max = data[i];
        }
    }
    linkedList<int>* buckets = new linkedList<int>[No_OF_BUCKETS];
    for (int i = 0; i < size; i++) {
        unsigned long long numerator = static_cast<unsigned long long>(data[i]) * No_OF_BUCKETS;
        unsigned long long denominator = static_cast<unsigned long long>(max) + 1ULL;
        unsigned int bucketIndex = static_cast<unsigned int>(numerator / denominator);
        if (bucketIndex >= No_OF_BUCKETS) bucketIndex = No_OF_BUCKETS - 1;
        buckets[bucketIndex].addTail(data[i]);
    }
    for (int i = 0; i < No_OF_BUCKETS; i++) {
       buckets[i].insertionSort(intComparator);
    }
    int j = 0;
    for (int i = 0; i < No_OF_BUCKETS; i++) {
        while (buckets[i].getLength() > 0) {
            data[j] = buckets[i].popHead();
			j++;
        }
    }
    delete[] buckets;
}

int generateRandomInt(int min, int max) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(min, max);
    return distrib(gen);
}
template<typename T> bool areEqual(const T* array1, const T* array2, unsigned int size) {
    for(int i=0;i<size;i++) {
        if(array1[i]!=array2[i]) return false;
	}
	return true;
}
int main_ints() {
    const int ORDER = 7;
    const int m = pow(10, 7);
    for (int o = 1; o <= ORDER; o++) {
        const int n = pow(10, o);
        int* array1 = new int[n];
        for (int i = 0; i < n; i++) {
            array1[i] = generateRandomInt(0, m - 1);
        }
        std::cout << "Array before sorting:\n";
        for (int i = 0; i < 10; i++) {
            if (i == 0) std::cout << array1[i];
            else std::cout << " " << array1[i];
        }
        int* array2 = new int[n];
        int* array3 = new int[n];
        memcpy(array2, array1, n * sizeof(int));
        memcpy(array3, array1, n * sizeof(int));

        clock_t start = clock();
        counting_sort(array1, n, m);
        clock_t end = clock();
        double diff = double(end - start) / CLOCKS_PER_SEC * 1000;
        std::cout << "\nCounting sort took " << diff << " ms to sort " << n << " elements.\n";
        std::cout << "Time per element: " << diff / n << " ms.\n";
        std::cout << "Array after sorting:\n";
        for (int i = 0; i < 10; i++) {
            if (i == 0) std::cout << array1[i];
            else std::cout << " " << array1[i];
        }
		BinaryHeap<int> BH(array2, n, intComparator, true);
		delete[] array2;
        start = clock();
        BH.sort();
        end = clock();
        diff = double(end - start) / CLOCKS_PER_SEC * 1000;
        std::cout << "\nHeap sort took " << diff << " ms to sort " << n << " elements.\n";
        std::cout << "Time per element: " << diff / n << " ms.\n";
        std::cout << "Array after sorting:\n";
		array2 = BH.getData();
        for (int i = 0; i < 10; i++) {
            if (i == 0) std::cout << array2[i];
            else std::cout << " " << array2[i];
        }
        start = clock();
        bucket_sort(array3, n);
        end = clock();
        diff = double(end - start) / CLOCKS_PER_SEC * 1000;
        std::cout << "\nBucket sort took " << diff << " ms to sort " << n << " elements.\n";
        std::cout << "Time per element: " << diff / n << " ms.\n";
        std::cout << "Array after sorting:\n";
        for (int i = 0; i < 10; i++) {
            if (i == 0) std::cout << array3[i];
            else std::cout << " " << array3[i];
        }
		if ((areEqual<int>(array1, array2, n)) && (areEqual<int>(array1, array3, n)))
            std::cout << "\nSorting results are identical.\n";
        else
            std::cout << "\nSorting results differ!\n";
        std::cout << "\n===================================\n";

        delete[] array1;
        delete[] array3;
    }
    return 0;
}
int main_objects() {
    const int ORDER = 7;
    const int m_double = (double)pow(2, 30);
    for (int o = 1; o <= ORDER; o++) {
        const int n = pow(10, o);
        object* array1 = new object[n];
        for (int i = 0; i < n; i++) {
			object newObject;
			newObject.key = (double)generateRandomInt(0, m_double - 1) / m_double;
			newObject.field1 = 'a' + (char)(generateRandomInt(0, 25));
            array1[i] = newObject;
        }
        std::cout << "Array before sorting:\n";
        for (int i = 0; i < 10; i++) {
            if (i == 0) std::cout << array1[i];
            else std::cout << " " << array1[i];
        }
        object* array2 = new object[n];
        memcpy(array2, array1, n * sizeof(object));

        BinaryHeap<object> BH(array1, n, objectComparator, true);
		delete[] array1;
        clock_t start = clock();
        BH.sort();
        clock_t end = clock();
        double diff = double(end - start) / CLOCKS_PER_SEC * 1000;
        std::cout << "\nHeap sort took " << diff << " ms to sort " << n << " elements.\n";
        std::cout << "Time per element: " << diff / n << " ms.\n";
        std::cout << "Array after sorting:\n";
        array1 = BH.getData();
        for (int i = 0; i < 10; i++) {
            if (i == 0) std::cout << array1[i];
            else std::cout << " " << array1[i];
        }
        start = clock();
        bucket_sort<object>(array2, n, objectComparator);
        end = clock();
        diff = double(end - start) / CLOCKS_PER_SEC * 1000;
        std::cout << "\nBucket sort took " << diff << " ms to sort " << n << " elements.\n";
        std::cout << "Time per element: " << diff / n << " ms.\n";
        std::cout << "Array after sorting:\n";
        for (int i = 0; i < 10; i++) {
            if (i == 0) std::cout << array2[i];
            else std::cout << " " << array2[i];
        }
        if (areEqual<object>(array1, array2, n))
            std::cout << "\nSorting results are identical.\n";
        else
            std::cout << "\nSorting results differ!\n";
        std::cout << "\n===================================\n";

        delete[] array2;
    }
    return 0;
}
int main() {
    main_objects();
    system("pause");
}