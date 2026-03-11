#include <iostream>
#include <string>
#include <time.h>
#include <locale.h>
#include <sstream>

struct object {
    int a;
    char b;
    bool operator>(const object& other) const {
        if (a != other.a) {
            return a > other.a;
        }
        return b > other.b;
	}
};
std::ostream& operator<<(std::ostream& os, const object& obj) {
    os << "{a: " << obj.a << ", b: '" << obj.b << "'}";
    return os;
}
template <typename T> class DynamicArray {
    unsigned int maxCapacity;
    unsigned int currentSize;
    T* array;
public:
    DynamicArray() {
        maxCapacity = 1;
        currentSize = 0;
        array = new T[maxCapacity];
	}
    unsigned int size() {
        return currentSize;
	}
	//a) dodanie elementu na koniec tablicy
    void addElement(T element) {
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
	//b) zwrócenie danych i-tego elementu
    T* getElement(unsigned int index) {
        if(index >= currentSize){
            throw std::out_of_range("Index out of range");
		}
		return &array[index];
    }
	//c) podmiana danych i-tego elementu
    void setElement(unsigned int index, T element) {
        if (index >= currentSize) {
			throw std::out_of_range("Index out of range");
        }
		array[index] = element;
    }
	//d) czyszczenie tablicy
    void clear(){
        delete[] array;
        maxCapacity = 1;
        currentSize = 0;
        array = new T[maxCapacity];
	}
    //e) zwrócenie napisowej reprezentacji tablicy
    std::string toString() {
		std::ostringstream oss;
        oss << "Maksymalna pojemnoœæ: " <<maxCapacity <<"\nAktualny rozmiar: " <<currentSize;
		oss<<"\nTrzy pierwsze elementy: [";
        for (unsigned int i = 0; i < 3; i++) {
            oss << array[i];
            if (i < 2 ) {
                oss<< ", ";
            }
        }
		oss<< "]\nTrzy ostatnie elementy: [";
        for (unsigned int i = currentSize-3; i < currentSize; i++) {
			oss << array[i];
            if (i < currentSize - 1) {
                oss<< ", ";
            }
        }
        oss<< "]\n";
		return oss.str();
    }
	//f) b¹belkowe sortowanie tablicy
    void bubbleSort(){
        for (unsigned int i = 0; i < currentSize - 1; i++) {
            for (unsigned int j = 0; j < currentSize - i - 1; j++) {
                if (array[j] > array[j + 1]) {
                    T temp = array[j];
                    array[j] = array[j + 1];
                    array[j + 1] = temp;
                }
            }
        }
	}
};

int main()
{
    setlocale(LC_CTYPE, "polish");
	DynamicArray<object> DA;
    const int order = 7;
    const int n = pow(10, order);
    double maxTime = 0.0;
    clock_t start = clock();
    for (int i = 0;i < n;i++) {
        object obj{
            rand() % 100,
            'a' + rand() % 26
        };
		clock_t startElement = clock();
		DA.addElement(obj);
		clock_t endElement = clock();
		double timeElement = double(endElement - startElement) / CLOCKS_PER_SEC * 1000;
		if (timeElement > maxTime) {
			maxTime = timeElement;
			std::cout << "Nowy najgorszy czas dodania elementu: " << maxTime << "ms dla i = " << i << std::endl;
    }
}
    clock_t end = clock();
    double time = double(end - start) / CLOCKS_PER_SEC *1000;
	std::cout << "Tablica ma " << DA.size() << " elementow." << std::endl;
	std::cout << "Zaczyna siê na adresie: " << DA.getElement(0) << std::endl;
    std::cout << "Czas dodania " << n << " elementow: " << time << "ms" << std::endl;
	std::cout<<DA.toString();
	DA.clear();
	system("pause");
    return 0;
}
