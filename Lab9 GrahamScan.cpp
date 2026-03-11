#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <cmath>
#include <sstream>

struct GraphNode {
    long double x;
    long double y;
    bool operator==(const GraphNode& other) const {
        return x == other.x && y == other.y;
	}
};
int orientation(const GraphNode& P0, const GraphNode& P1, const GraphNode& P2) {
    long double angle = P0.x * (P1.y - P2.y) + P1.x * (P2.y - P0.y) + P2.x * (P0.y - P1.y);
    if (angle == 0) {
        long double distA = (P1.x - P0.x) * (P1.x - P0.x) + (P1.y - P0.y) * (P1.y - P0.y);
        long double distB = (P2.x - P0.x) * (P2.x - P0.x) + (P2.y - P0.y) * (P2.y - P0.y);
        if (distA == distB) return 0;
		return (distA < distB) ? 1 : -1;
    }
    return (angle > 0) ? 1 : -1;
}
int polarAngleCompare(const GraphNode& pivot, const GraphNode& A, const GraphNode& B) {
	long double angleA = atan2(A.y - pivot.y, A.x - pivot.x);
	long double angleB = atan2(B.y - pivot.y, B.x - pivot.x);
    if (angleA == angleB) {
        long double distA = (A.x - pivot.x) * (A.x - pivot.x) + (A.y - pivot.y) * (A.y - pivot.y);
        long double distB = (B.x - pivot.x) * (B.x - pivot.x) + (B.y - pivot.y) * (B.y - pivot.y);
        if (distA == distB) return 0;
        return (distA < distB) ? 1 : -1;
	}
	return (angleA < angleB) ? 1 : -1;
}
std::ostream& operator<<(std::ostream& os, const GraphNode& node) {
    os << "{x: " << node.x << ", y: " << node.y << "}";
    return os;
}
void merge(GraphNode arr[], int left, int mid, int right, GraphNode pivot) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    GraphNode* L = new GraphNode[n1];
    GraphNode* R = new GraphNode[n2];
    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];
    int i = 0;
    int j = 0;
    int k = left;
    while (i < n1 && j < n2) {
        if (polarAngleCompare(pivot, L[i], R[j]) >= 0) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
    delete[] L;
    delete[] R;
}
void mergeSort(GraphNode arr[], int left, int right, GraphNode pivot) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid, pivot);
        mergeSort(arr, mid + 1, right, pivot);
        merge(arr, left, mid, right, pivot);
    }
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
	void deleteReference(T object) {
        Node* current = head;
        while (current != nullptr) {
            if (current->data == object) {
                if (current->prev != nullptr) {
                    current->prev->next = current->next;
                }
                else {
                    head = current->next;
                }
                if (current->next != nullptr) {
                    current->next->prev = current->prev;
                }
                else {
                    tail = current->prev;
                }
                delete current;
                length--;
                return;
            }
            current = current->next;
		}
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
    void display() {
        Node* current = head;
        while (current != nullptr) {
            std::cout << current->data << " ";
            current = current->next;
        }
    }
    void importFromTxt(const char* filename) {
        std::ifstream file(filename, std::ios::in);
        if (!file.is_open()) {
            std::cerr << "Error opening file: " << filename << std::endl;
            return;
        }
        int nodeCount;
        file >> nodeCount;
        for (unsigned int i = 0; i < nodeCount; ++i) {
            double x, y;
            file >> x >> y;
            addTail(T{x, y});
        }
    }
    void importFromArray(T arr[], size_t arrLength) {
        for (size_t i = 0; i < arrLength; i++) {
            addTail(arr[i]);
        }
	}
    T findMinY() {
        T minY = head->data;
        Node* current = head->next;
        while (current) {
            if (current->data.y < minY.y || (current->data.y == minY.y && current->data.x < minY.x)) minY = current->data;
            current = current->next;
        }
        return minY;
    }
    T* toArray() {
        Node* current = head;
        T* array = new T[length];
        for (int i = 0; i < length; i++) {
            array[i] = current->data;
            current = current->next;
        }
        return array;
    }
    T popTail() {
        if (length == 0) throw std::out_of_range("List is empty");
        T object = tail->data;
        Node* oldTail = tail;
        tail = tail->prev;
        delete oldTail;
        length--;
        if (length == 0) head = nullptr;
        else tail->next = nullptr;
        return object;
    }
    T& peekTail() {
        if (length == 0) throw std::out_of_range("List is empty");
        return tail->data;
    }
    T& peekSecondTail() {
        if (length < 2) throw std::out_of_range("Not enough elements");
        return tail->prev->data;
    }
	T at(size_t index) {
        if (index >= length) throw std::out_of_range("Index out of range");
        Node* current = head;
        for (size_t i = 0; i < index; i++) {
            current = current->next;
        }
        return current->data;
    }
    unsigned int objectAt(T object) {
		Node* current = head;
        for (unsigned int i = 0; i < length; i++) {
            if (current->data == object) {
                return i;
            }
            current = current->next;
        }
        throw std::out_of_range("Object not found");
    }
};
linkedList<int> GrahamScan(linkedList<GraphNode> nodes, clock_t& start_loop, clock_t& end_loop, clock_t& start_sort, clock_t& end_sort) {
    linkedList<int> hull;
    GraphNode minY = nodes.findMinY();
    hull.addTail(nodes.objectAt(minY));
    linkedList<GraphNode> sortedNodes;
    size_t n = nodes.getLength();
    GraphNode* nodeArray = (n>0) ? nodes.toArray() : nullptr;
    if (n > 0) {
		start_sort = clock();
        mergeSort(nodeArray, 0, n - 1, minY);
		end_sort = clock();
        for (size_t i = 0; i < n; i++) {
			sortedNodes.addTail(nodeArray[i]);
            }
    }
    delete[] nodeArray;
	sortedNodes.deleteReference(minY);
	start_loop = clock();
    for (size_t i = 0; i < sortedNodes.getLength(); i++) {
        GraphNode currentPoint = sortedNodes.at(i);
        while (hull.getLength() >= 2) {
            GraphNode top = nodes.at(hull.peekTail());
            GraphNode nextToTop = nodes.at(hull.peekSecondTail());
            if (orientation(nextToTop, top, currentPoint) != 1) {
                hull.popTail();
            }
            else {
                break;
            }
        }
        std::cout << i << "\n";
        hull.addTail(nodes.objectAt(currentPoint));
	}
	end_loop = clock();
    return hull;
}
int main()
{

    linkedList<GraphNode> nodes;
	std::ostringstream filename;
	clock_t start_sort, start_loop,end_sort, end_loop;
    for (int i = 1; i <= 5; i++) {
		filename.str("");
        filename << "points" << i << ".txt";
        nodes.importFromTxt(filename.str().c_str());
        linkedList<int> hull = GrahamScan(nodes, start_loop, end_loop, start_sort, end_sort);
        std::cout << "Convex Hull for " << filename.str() << ":\n";
		std::cout << "Hull contains " << hull.getLength() << " points.\n";
		std::cout << "Indexes of these points: ";
        hull.display();
        double diff_loop = double(end_loop - start_loop) / CLOCKS_PER_SEC * 1000;
        double diff_sort = double(end_sort - start_sort) / CLOCKS_PER_SEC * 1000;
		std::cout << "\nSorting time: " << diff_sort << " ms\n";
		std::cout << "Graham's scan time: " << diff_loop << " ms\n";
        std::cout << "-------------------------\n";
        nodes.clearList();
    }
    return 0;
}