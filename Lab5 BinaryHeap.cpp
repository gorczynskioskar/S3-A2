#include <iostream>
#include <sstream>
#include <stdexcept>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <utility>

struct object {
    int a;
    char b;
};
std::ostream& operator<<(std::ostream& os, const object& obj) {
    os << "{a: " << obj.a << ", b: '" << obj.b << "'}";
    return os;
}

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
        T getElement(unsigned int index) {
            if (index >= currentSize) {
                throw std::out_of_range("Index out of range");
            }
            return array[index];
        }
        void setElement(unsigned int index, T element) {
            if (index >= currentSize) {
                throw std::out_of_range("Index out of range");
            }
            array[index] = element;
        }
        void clear() {
            delete[] array;
            maxCapacity = 1;
            currentSize = 0;
            array = new T[maxCapacity];
        }
        ~DynamicArray() {
            delete[] array;
        }
    };
    DynamicArray array;
public:
    void add(T object, int(*cmp)(const T&, const T&)) {
        array.add(object);
        unsigned int index = array.size() - 1;
        heap_up(index, cmp);
    }
    void heap_up(unsigned int index, int(*cmp)(const T&, const T&)) {
        while(index>0){
            unsigned int parent_index = (index - 1) / 2;
            if(cmp(array[index], array[parent_index]) > 0){
                array.swap(index, parent_index);
                index = parent_index;
            } 
            else {
                break;
            }
		}
    }
    void heap_down(unsigned int index, int(*cmp)(const T&, const T&)) {
        while (true) {
            if (2*index + 1 >= array.size()) {
                break;
			}
			if (2 * index + 2 >= array.size()) {
                if(cmp(array[index], array[2*index + 1]) < 0){
                    array.swap(index, 2*index + 1);
                    index = 2*index + 1;
                } 
                else {
                    break;
                }
                continue;
            }
            if(cmp(array[2*index+1], array[2*index + 2]) > 0){
                if(cmp(array[index], array[2*index + 1]) < 0){
                    array.swap(index, 2*index + 1);
                    index = 2*index + 1;
                } 
                else {
                    break;
                }
            }
            else {
                if(cmp(array[index], array[2*index + 2]) < 0){
                    array.swap(index, 2*index + 2);
                    index = 2*index + 2;
                } 
                else {
                    break;
                }
			}
        }
    }
    T delete_root(int(*cmp)(const T&, const T&)) {
		if (array.size() == 0) throw std::out_of_range("Heap is empty");
        T object = array[0];
        array[0] = array[array.size() - 1];
        array.pop_back();
		if (array.size() > 0) heap_down(0, cmp);
        return object;
    }
    void clear() {
        array.clear();
    }
    void traverse_pre_order(unsigned int index, std::ostream& os, int max_levels) {
        if (index >= array.size() || max_levels <= 0) return;
        os << array[index] << " ";
        traverse_pre_order(2 * index + 1, os, max_levels - 1);
        traverse_pre_order(2 * index + 2, os, max_levels - 1);
    }
    std::string to_string(int max_levels = 5) {
		std::stringstream oss;
        if (array.size() == 0) return "BinaryHeap is empty!\n";
		oss << "BinaryHeap with " << array.size() << " elements:\n";
		traverse_pre_order(0, oss, max_levels);
		return oss.str();
    }
};
int dataComparator(const object& a, const object& b) {
    int diff = a.a - b.a;
    if (diff != 0) return diff;
    return a.b - b.b;
}
int main()
{
    BinaryHeap<object> BH;
    const int order = 7;
	clock_t start, end;
    for(int i=1;i<=order;i++){
        int n = pow(10, i);
		start = clock();
		for (int j = 0; j < n; j++) {
            object obj{rand()%1000, char('A' + rand()%26)};
            BH.add(obj, dataComparator);
        }
		end = clock();
		double diff = double(end - start) / CLOCKS_PER_SEC * 1000;
		std::cout << "Added " << n << " elements in " << diff << " ms\n";
		std::cout << "Time per element: " << diff / n << " ms\n";
		std::cout << BH.to_string(5) << std::endl;
		start = clock();
        for (int i = 0; i < n; i++) {
			object obj = BH.delete_root(dataComparator);
			if(i==(n/2)) std::cout << "Deleted root: " << obj << std::endl;
        }
		end = clock();
		diff = double(end - start) / CLOCKS_PER_SEC * 1000;
		std::cout << "Deleted " << n << " elements in " << diff << " ms\n";
		std::cout << "Time per element: " << diff / n << " ms\n";
		std::cout << "Heap after deletions:\n" << BH.to_string(5) << std::endl;
		BH.clear();
    }
	system("pause");
	return 0;
}