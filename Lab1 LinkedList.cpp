#include <iostream>
#include <locale.h>
#include <string>
#include <time.h>

struct container {
	int field_1;
	char field_2;
};
template <typename T> class linkedList {
private:
	struct Node {
		T data; //obiekt typu T lub wskaźnik do obiektu typu T
		Node* next; //wskaźnik na następny element
		Node* prev; //wskaźnik na poprzedni element
	};
	Node* head; //wskaźnik na pierwszy element
	Node* tail; //wskaźnik na ostatni element
	size_t length; //długość listy
public:
	size_t getLength() {
		return length;
	}
	void addHead(T object) {
		auto newHead = new Node;
		newHead->data = object;
		newHead->prev = nullptr;
		if (length == 0) {
			newHead->next = nullptr;
			tail = newHead;
		}
		else {
			newHead->next = head;
		}
		head = newHead;
		length++;
		return;
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
	void delHead() {
		if (length == 0) {
			std::cout << "Lista jest pusta!";
			return;
		}
		else if (length == 1) {
			delete head;
		}
		else {
			head->next->prev = nullptr;
			auto temp = head;
			head = head->next;
			delete temp;
		}
		length--;
		return;
	}
	void delTail() {
		if (length == 0) {
			std::cout << "Lista jest pusta!";
			return;
		}
		else if (length == 1) {
			delete tail;
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
	 T getElementData(size_t index) {
		auto current = head;
		for(size_t i=0; i<index; i++) {
			current = current->next;
		}
		return current->data;
	}
	 T* getNodeAddress(size_t index) {
		 auto current = head;
		 for (size_t i = 0; i < index; i++){
			 current = current->next;
		 }
		 return &(current->data);
	 }
	void changeElementData(size_t index, T newData) {
		if (index >= length) {
			std::cout << "Index poza zakresem!";
			return;
		}
		auto current = head;
		for (unsigned i = 0; i < index; i++) {
			current = current->next;
		}
		current->data = newData;
		return;
	}
	
	void clearList() {
		size_t len = getLength();
		for(unsigned i=0; i<len; i++) {
			delTail();
		}
		return;
	}
};


int main()
{
	linkedList<int>* intList = new linkedList<int>();
	setlocale(LC_ALL, "pl_PL");
	linkedList<container>* ll = new linkedList<container>();
	int userChoice = 1;
	while (userChoice != 0) {
		std::cout << "Menu:\n";
		std::cout << "1. Wstaw nowy element na początku listy.\n";
		std::cout << "2. Wstaw nowy element na końcu listy.\n";
		std::cout << "3. Usuń ostatni element listy.\n";
		std::cout << "4. Usuń pierwszy element listy.\n";
		std::cout << "5. Zwróc dane i-tego elementu listy.\n";
		std::cout << "6. Podmień dane i-tego elementu listy.\n";
		std::cout << "7. Wyszukaj (wg danych lub komparatora).\n";
		std::cout << "8. Wyszukaj i usuń (wg danych lub komparatora).\n";
		std::cout << "9. Dodaj nowy element z wymuszeniem porządku.\n";
		std::cout << "10. Wyczyść listę.\n";
		std::cout << "11. Wyświetl reprezentację listy.\n";
		std::cout << "0. Wyjście\n";
		std::cout << "Wybierz opcję: ";
		std::cin >> userChoice;
		switch (userChoice) {
			case 1: {
				auto newElement = new container;
				std::cout << "Podaj wartość pola 1 (int): ";
				std::cin >> newElement->field_1;
				std::cout << "Podaj wartość pola 2 (char): ";
				std::cin >> newElement->field_2;
				clock_t start = clock();
				ll->addHead(*newElement);
				clock_t end = clock();
				std::cout << "Czas wykonania operacji: " << (double)(end - start) / (double)CLOCKS_PER_SEC * 100 << " milisekund\n";
				system("pause");
				system("cls");
				break;
			}
			case 2: {
				auto newElement = new container;
				std::cout << "Podaj wartość pola 1 (int): ";
				std::cin >> newElement->field_1;
				std::cout << "Podaj wartość pola 2 (char): ";
				std::cin >> newElement->field_2;
				clock_t start = clock();
				ll->addTail(*newElement);
				clock_t end = clock();
				std::cout << "Czas wykonania operacji: " << (double)(end - start) / (double)CLOCKS_PER_SEC * 100 << " milisekund\n";
				system("pause");
				system("cls");
				break;
			}
			case 3: {
				clock_t start = clock();
				ll->delTail();
				clock_t end = clock();
				std::cout << "Czas wykonania operacji: " << (double)(end - start) / (double)CLOCKS_PER_SEC * 100 << " milisekund\n";
				system("pause");
				system("cls");
				break;
			}
			case 4: {
				clock_t start = clock();
				ll->delHead();
				clock_t end = clock();
				std::cout << "Czas wykonania operacji: " << (double)(end - start) / (double)CLOCKS_PER_SEC * 100 << " milisekund\n";
				system("pause");
				system("cls");
				break;
			}
			case 5: {
				unsigned index;
				std::cout << "Podaj indeks elementu do wyświetlenia: ";
				std::cin >> index;
				if(index>= ll->getLength()) {
					std::cout << "Index poza zakresem!\n";
					system("pause");
					system("cls");
					break;
				}
				clock_t start = clock();
				auto object = ll->getElementData(index);
				clock_t end = clock();
				std::cout << "Element " << index << ": pole_1 = " << object.field_1 << ", pole_2 = " << object.field_2 << "\n";
				std::cout << "Czas wykonania operacji: " << (double)(end - start) / (double)CLOCKS_PER_SEC * 100 << " milisekund\n";
				system("pause");
				system("cls");
				break;
			}
			case 6: {
				unsigned index;
				std::cout << "Podaj indeks elementu do podmiany: ";
				std::cin >> index;
				auto newElement = new container;
				std::cout << "Podaj nową wartość pola 1 (int): ";
				std::cin >> newElement->field_1;
				std::cout << "Podaj nową wartość pola 2 (char): ";
				std::cin >> newElement->field_2;
				clock_t start = clock();
				ll->changeElementData(index, *newElement);
				clock_t end = clock();
				std::cout << "Czas wykonania operacji: " << (double)(end - start) / (double)CLOCKS_PER_SEC * 100 << " milisekund\n";
				system("pause");
				system("cls");
				break;
			}
			case 7: {
				auto element = new container;
				std::cout << "Podaj wartość pola 1 (int) szukanego elementu: ";
				std::cin >> element->field_1;
				std::cout << "Podaj wartość pola 2 (char) szukanego elementu: ";
				std::cin >> element->field_2;
				clock_t start = clock();
				//ll->searchElement(*element);
				clock_t end = clock();
				std::cout << "Czas wykonania operacji: " << (double)(end - start) / (double)CLOCKS_PER_SEC * 100 << " milisekund\n";
				system("pause");
				system("cls");
				break;
			}
			case 8: {
				auto element = new container;
				std::cout << "Podaj wartość pola 1 (int) szukanego elementu: ";
				std::cin >> element->field_1;
				std::cout << "Podaj wartość pola 2 (char) szukanego elementu: ";
				std::cin >> element->field_2;
				clock_t start = clock();
				//ll->searchAndDeleteElement(*element);
				clock_t end = clock();
				std::cout << "Czas wykonania operacji: " << (double)(end - start) / (double)CLOCKS_PER_SEC * 100 << " milisekund\n";
				system("pause");
				system("cls");
				break;
			}
			case 9: {
				auto newElement = new container;
				std::cout << "Podaj wartość pola 1 (int): ";
				std::cin >> newElement->field_1;
				std::cout << "Podaj wartość pola 2 (char): ";
				std::cin >> newElement->field_2;
				clock_t start = clock();
				//ll->addInOrder(*newElement);
				clock_t end = clock();
				std::cout << "Czas wykonania operacji: " << (double)(end - start) / (double)CLOCKS_PER_SEC * 100 << " milisekund\n";
				system("pause");
				system("cls");
				break;
			}
			case 10: {
				clock_t start = clock();
				ll->clearList();
				clock_t end = clock();
				std::cout << "Czas wykonania operacji: " << (double)(end - start) / (double)CLOCKS_PER_SEC * 100 << " milisekund\n";
				system("pause");
				system("cls");
				break;
			}
			case 11: {
				size_t len = ll->getLength();
				if (len == 0) {
					std::cout << "Lista jest pusta!\n";
				}
				else {
					std::cout << "Lista zawiera " << len << " elementów:\n";
					clock_t start = clock();
					for (size_t i = 0; i < len; i++) {
						auto object = ll->getElementData(i);
						std::cout << "Element " << i << ": pole_1 = " << object.field_1 << ", pole_2 = " << object.field_2 << ". Adres węzła w pamięci: " << ll->getNodeAddress(i) << "\n";
					}
					clock_t end = clock();
					std::cout << "Czas wykonania operacji: " << (double)(end - start) / (double)CLOCKS_PER_SEC * 100 << " milisekund\n";
				}
				system("pause");
				system("cls");
				break;
			}
			case 0: {
				system("cls");
				std::cout << "Dziękuję za skorzystanie z programu.\n";
				return 0;
			default: {
				std::cout << "Nieprawidłowy wybór!\n";
				system("pause");
				system("cls");
				break;
				}
			}
		}
	}

}
