#include <iostream>
#include <utility>

//Последовательный контейнер 
struct ConsistentContainer {
    int* data; // Указатель на массив 
    int size; // Текущее количество элементов
    int maxsize; // Максимальный размер массива

    ConsistentContainer() : data(nullptr), size(0), maxsize(0) {}
    
    // Перемещающий конструктор
    ConsistentContainer(ConsistentContainer&& rvalue) noexcept
    : data(std::move(rvalue.data)), size(rvalue.size), maxsize(rvalue.maxsize) {
        rvalue.data = nullptr; // Освобождаем указатель у другого объекта
        rvalue.size = 0;
        rvalue.maxsize = 0;
    }

    // Перемещающий оператор присваивания
    ConsistentContainer& operator=(ConsistentContainer&& rvalue) noexcept {
        if (this != &rvalue) { 
        data = rvalue.data;
        size = rvalue.size;
        maxsize = rvalue.maxsize;
        rvalue.data = nullptr; // Освобождаем указатель у другого объекта
        rvalue.size = 0;
        rvalue.maxsize = 0;
        }
        return *this;
    }

    // Функция для увеличения емкости
    void moresize() {
        
        maxsize = maxsize == 0 ? 1 : static_cast<int>(maxsize * 1.5); 
        int* newData = new int[maxsize];
        for (int i = 0; i < size; ++i) {
            newData[i] = data[i]; // Копируем старые данные в новую память
        }
        delete[] data; 
        data = newData; // Перенаправляем указатель на новую память
        
    }

    // Функция для уменьшения емкости до фактического размера
    void lesssize() {
        
        int* newData = new int[size]; // Выделяем новую память по размеру фактических данных
        for (int i = 0; i < size; ++i) {
            newData[i] = data[i]; // Копируем данные
        }
        delete[] data; 
        data = newData; 
        maxsize = size; // Обновляем емкость
    }

    // Добавление элемента в конец
    void push_back(int value) {
        if (size == maxsize) {
            std::cout<< "Увеличение ёмкости"<< std::endl;
            moresize(); // Изменяем размер, если емкость заполнена
            
        }
        maxsize++;
        data[size++] = value; // Добавляем элемент и увеличиваем размер
        
    }
    // Добавление элемента в начало
    void push_front(int value) {
        if (size == maxsize) {
            std::cout<< "Увеличение ёмкости"<< std::endl;
            moresize(); // Изменяем размер, если емкость заполнена
        }
        // Сдвигаем все элементы на один вправо
        for (int i = size; i > 0; --i) {
            data[i] = data[i - 1];
        }
        data[0] = value; // Вставляем новый элемент в начало
        maxsize++;
        ++size;
        
    }

    // Добавление элемента в указанный индекс
    void insert(int index, int value) {
        if (index < 0 || index > size) {
            throw std::out_of_range("Индекс вне диапазона"); // Исключение для недопустимого индекса
        }
        if (size == maxsize) {
            std::cout<< "Увеличение ёмкости"<< std::endl;
            moresize(); // Изменяем размер, если емкость заполнена
        }
        // Сдвигаем элементы вправо, начиная с указанного индекса
        for (int i = size; i > index; --i) {
            data[i] = data[i - 1];
        }
        data[index] = value; // Вставляем значение в указанный индекс
        ++size;
        maxsize++;
    }

    // Удаление элемента по индексу 
    void erase(int index) { 
        if (index < 0 || index >= size) { 
            throw std::out_of_range("Индекс вне диапазона"); // Исключение для недопустимого индекса 
        } 
        for (int i = index; i < size - 1; ++i) { 
            data[i] = data[i + 1]; // Сдвинаем элементы влево 
        } 
        --size; // Уменьшаем размер 

        // Проверяем, нужно ли уменьшить емкость
        if (size < maxsize / 2 && maxsize > 1) { 
            std::cout<< "Уминьшение ёмкости"<< std::endl;
            lesssize(); // Уменьшаем емкость до необходимого размера
        }
    }
    // Получение размера контейнера
    int getSize() const {
        return size; // Возвращаем текущее количество элементов
    }

    int getmaxsize() const {
        return maxsize; // Возвращаем текущее количество элементов
    }

    // Вывод содержимого контейнера
    void print() const {
    for (int i = 0; i < size; ++i) {
        std::cout << data[i] << " ";
    }
    std::cout << std::endl;
    }

    // Оператор [] для доступа к элементам по индексу (новый)
    int& operator[](int index) {
        if (index < 0 || index >= size) {      
            throw std::out_of_range("Индекс вне диапазона");
        }
        return data[index];// Возвращаем элемент по индексу
    }

    // Структура итератора для ConsistentContainer
    struct Iterator {
        int* ptr;

        // Конструктор
        Iterator(int* ptr) : ptr(ptr) {}

        // Оператор разыменования
        int& operator*() {
            if (ptr==nullptr) {
                throw std::out_of_range("Индекс вне диапазона"); 
            }
            return *ptr;
        }
        int& get() {
            if (ptr == nullptr) {
                throw std::out_of_range("Индекс вне диапазона"); 
            }
            return *ptr;
        }

        // Оператор сравнения (для проверки конца итерации)
        bool operator!=(const Iterator& rvalue) {
            return ptr != rvalue.ptr;
        }

        // Перемещение итератора на следующий элемент
        Iterator& operator++() {
            ptr++;
            return *this;
        }

    };
    // Возвращает итератор на начало контейнера
    Iterator begin() {
        return Iterator(data);
    }

    // Возвращает итератор на конец контейнера
    Iterator end() {
        return Iterator(data + size);
    }

    // Деструктор
    ~ConsistentContainer() {
    //delete[] data;
    }
};


// Класс для спискового контейнера (связь через указатели)
// Двусвязный список, где каждый элемент хранит ссылку на предыдущий и следующий
class DoubleLinkedList {
private:
    struct Node {
        int value;
        Node* next;
        Node* prev;

        Node(int value) : value(value), next(nullptr), prev(nullptr) {}
    };

    Node* head;
    Node* tail;
    int size;

public:
    // Конструктор
    DoubleLinkedList() : head(nullptr), tail(nullptr), size(0) {}

    // Деструктор
    ~DoubleLinkedList() {
        while (head != nullptr) {
            Node* next = head->next;
            delete head;
            head = next;
        }
    }

    // Перемещающий конструктор
    DoubleLinkedList(DoubleLinkedList&& rvalue) noexcept
    : head(std::move(rvalue.head)), tail(std::move(rvalue.tail)), size(rvalue.size){
        rvalue.head = nullptr;
        rvalue.tail = nullptr;
        rvalue.size = 0;
    }

    // Перемещающий оператор присваивания
    DoubleLinkedList& operator=(DoubleLinkedList&& rvalue) noexcept {
        if (this != &rvalue) {
        // Освобождение текущих ресурсов
        while (head != nullptr) {
            Node* next = head->next;
            delete head;
            head = next;
        }

        head = rvalue.head;
        tail = rvalue.tail;
        size = rvalue.size;

        rvalue.head = nullptr;
        rvalue.tail = nullptr;
        rvalue.size = 0;
        }
        return *this;
    }
    
    // Добавление элемента в конец
    void push_back(int value) {
        Node* newNode = new Node(value);
        if (head == nullptr) {
            head = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        ++size;
    }

    // Добавление элемента в начало
    void push_front(int value) {
        Node* newNode = new Node(value);
        if (head == nullptr) {
            head = newNode;
            tail = newNode;
        } else {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
        ++size;
    }

    // Добавление элемента по индексу
    void insert(int index, int value) {
        if (index < 0 || index > size) {
            throw std::out_of_range("Индекс вне допустимого диапазона");
        }
        if (index == 0) {
            push_front(value);
            return;
        } else if (index == size) {
            push_back(value);
            return;
        }

        Node* newNode = new Node(value);
        Node* current = head;
        for (int i = 0; i < index; ++i) {
            current = current->next;
        }

        newNode->next = current;
        newNode->prev = current->prev;
        current->prev->next = newNode;
        current->prev = newNode;

        ++size;
    }

    // Удаление элемента по индексу
    void erase(int index) {
        if (index < 0 || index >= size) {
            throw std::out_of_range("Индекс вне допустимого диапазона");
        }
        Node* current = head;
        for (int i = 0; i < index; ++i) {
            current = current->next;
        }

        if (current->prev) {
            current->prev->next = current->next;
        } else {
            head = current->next; // Удаление головного элемента
        }

        if (current->next) {
            current->next->prev = current->prev;
        } else {
            tail = current->prev; // Удаление хвостового элемента
        }

        delete current;
        --size;
    }

    // Получение размера контейнера
    int getSize() const {
        return size;
    }

    // Вывод содержимого контейнера
    void print() const {
        Node* current = head;
        while (current != nullptr) {
            std::cout << current->value << " ";
            current = current->next;
        }
        std::cout << std::endl;
    }

    // Оператор [] для доступа к элементам по индексу
    int& operator[](int index) {
        if (index < 0 || index >= size) {
            throw std::out_of_range("Индекс вне диапазона");
        }
        Node* current = head;
        for (int i = 0; i < index; ++i) {
            current = current->next;
        }
        return current->value;
    }
    // Структура итератора для DoubleLinkedList
    struct Iterator {
        Node* ptr;

        // Конструктор
        Iterator(Node* ptr) : ptr(ptr) {}

        // Оператор разыменования
        int operator*() {
            if (ptr == nullptr) {
                throw std::out_of_range("Индекс вне диапазона");
            }
            return ptr->value;
        }

        // Оператор сравнения (для проверки конца итерации)
        bool operator!=(const Iterator& rvalue) {
            return ptr != rvalue.ptr;
        }

        // Перемещение итератора на следующий элемент
        Iterator& operator++() {
            ptr = ptr->next;
            return *this;
        }

        int& get() {
            if (ptr == nullptr) {
                throw std::out_of_range("Индекс вне диапазона");
            }
            return ptr->value;
        }
    };

    // Возвращает итератор на начало контейнера
    Iterator begin() {
        return Iterator(head);
    }

    // Возвращает итератор на конец контейнера
    Iterator end() {
        return Iterator(tail);
    }
};

class SinglyLinkedList {
private:
   struct Node {
        int value;
        Node* next;

        Node(int value) : value(value), next(nullptr) {}
    };

    Node* head;
    int size;

public:
    // Конструктор
    SinglyLinkedList() : head(nullptr), size(0) {}

    // Деструктор
    ~SinglyLinkedList() {
        while (head != nullptr) {
            Node* next = head->next;
            delete head;
            head = next;
        }
    }

    // Перемещающий конструктор
    SinglyLinkedList(SinglyLinkedList&& rvalue) noexcept:  head(rvalue.head), size(rvalue.size) {
        rvalue.head = nullptr;
        rvalue.size = 0;
    }

    // Перемещающий оператор присваивания
    SinglyLinkedList& operator=(SinglyLinkedList&& rvalue) noexcept {
        if (this != &rvalue) {
            while (head != nullptr) {
                Node* temp = head;
                head = head->next;
                delete temp;
            }
            head = rvalue.head;
            size = rvalue.size;

            rvalue.head = nullptr;
            rvalue.size = 0;
        }
        return *this;
    }    

    // Добавление элемента в конец
    void push_back(int value) {
        Node* newNode = new Node(value);
        if (head == nullptr) {
            head = newNode;
        } else {
            Node* current = head;
            while (current->next != nullptr) {
                current = current->next;
            }
            current->next = newNode;
        }
        ++size;
    }

    // Добавление элемента в начало
    void push_front(int value) {
        Node* newNode = new Node(value);
        newNode->next = head;
        head = newNode;
        ++size;
    }


    // Метод для вставки элемента по индексу
    void insert(int index, int value) {
        if (index < 0 || index > size) {
            throw std::out_of_range("Индекс вне допустимого диапазона");
        }
        if (index == 0) {
            push_front(value);
            return;
        }
        if (index == size) {
            push_back(value);
            return;
        }

        Node* newNode = new Node(value);
        Node* current = head;
        for (int i = 0; i < index - 1; ++i) {
            current = current->next;
        }
        newNode->next = current->next;
        current->next = newNode;
        ++size;
    }

    // Удаление элемента по индексу
    void erase(int index) {
        if (index < 0 || index >= size) {
            throw std::out_of_range("Индекс вне допустимого диапазона");
        }
        Node* current = head;
        if (index == 0) {
            head = current->next;
            delete current;
        } else {
            Node* prev = nullptr;
            for (int i = 0; i < index; ++i) {
                prev = current;
                current = current->next;
            }
            prev->next = current->next;
            delete current;
        }
        --size;
    }

    // Получение размера контейнера
    int getSize() const {
        return size;
    }

    // Вывод содержимого контейнера
    void print() const {
        Node* current = head;
        while (current != nullptr) {
            std::cout << current->value << " ";
            current = current->next;
        }
        std::cout << std::endl;
    }

    // Оператор [] для доступа к элементам по индексу
    int& operator[](int index) {
        if (index < 0 || index >= size) {
            throw std::out_of_range("Индекс вне диапазона");
        }
        Node* current = head;
        for (int i = 0; i < index; ++i) {
            current = current->next;
        }
        return current->value;
    }

    // Структура итератора для SinglyLinkedList
    struct Iterator {
        Node* ptr;

        // Конструктор
        Iterator(Node* ptr) : ptr(ptr) {}

        // Оператор разыменования
        int operator*() {
            if (ptr == nullptr) {
                throw std::out_of_range("Индекс вне диапазона");
            }            
            return ptr->value;
        }

        // Оператор сравнения (для проверки конца итерации)
        bool operator!=(const Iterator& rvalue) {
            return ptr != rvalue.ptr;
        }

        // Перемещение итератора на следующий элемент
        Iterator& operator++() {
            ptr = ptr->next;
            return *this;
        }
        int& get() {
            if (ptr == nullptr) {
                throw std::out_of_range("Индекс вне диапазона");
            }
            return ptr->value;
        }
    };
    // Возвращает итератор на начало контейнера
    Iterator begin() {
        return Iterator(head);
    }

    // Возвращает итератор на конец контейнера
    Iterator end() {
        return Iterator(nullptr);
    }
};

int main() {
// Создание объектов контейнеров
    ConsistentContainer vec;
    DoubleLinkedList Double_lst;
    SinglyLinkedList Singl_lst;

    // Тестирование контейнера ConsistentContainer
    std::cout << "ConsistentContainer:" << std::endl;

    // Добавление элементов
    for (int i = 0; i < 10; ++i) {
        vec.push_back(i);
    }

    std::cout << "Контейнер: ";
    vec.print();
    std::cout << "Размер: " << vec.getSize() << std::endl;

    // Удаление элементов
    vec.erase(2); // Удаление третьего элемента
    vec.erase(3); // Удаление пятого элемента
    vec.erase(4); // Удаление седьмого элемента

    // Вывод содержимого после удаления
    std::cout << "Удаление 3, 5, 7 элементов: ";
    vec.print();

    // Добавление элемента в начало
    vec.push_front(10);
    std::cout << "Добавление элемента в начало: ";
    vec.print();

    // Вставка в середину
    vec.insert(vec.getSize() / 2, 20); // Вставка в середину
    std::cout << "Добавление элемента в середину: ";
    vec.print();

    // Добавление элемента в конец
    vec.push_back(30);
    
    // Вывод содержимого после добавления в конец
    std::cout << "Добавление элемента в конец: ";
    vec.print();
    
    std::cout << std::endl;

    
    // Тестирование контейнера DoubleLinkedList
    std::cout << "DoubleLinkedList:" << std::endl;

    for (int i = 0; i < 10; ++i) {
        Double_lst.push_back(i);
    }

    std::cout << "Контейнер: ";
    Double_lst.print();
    std::cout << "Размер: " << Double_lst.getSize() << std::endl;

    Double_lst.erase(2);
    Double_lst.erase(3);
    Double_lst.erase(4);

    std::cout << "Удаление 3, 5, 7 элементов: ";
    Double_lst.print();

    Double_lst.push_front(10);
    std::cout << "Добавление элемента в начало: ";
    Double_lst.print();

    Double_lst.insert(Double_lst.getSize() / 2, 20);
    std::cout << "Добавление элемента в середину: ";
    Double_lst.print();

    Double_lst.push_back(30);
    
    std::cout << "Добавление элемента в конец: ";
    Double_lst.print();
    
    std::cout << std::endl;


    
   // Тестирование контейнера SinglyLinkedList
   std::cout << "SinglyLinkedList:" << std::endl;

   for (int i = 0; i < 10; ++i) {
       Singl_lst.push_back(i);
   }

   std::cout << "Контейнер: ";
   Singl_lst.print();
   std::cout << "Размер: " << Singl_lst.getSize() << std::endl;

   Singl_lst.erase(2);
   Singl_lst.erase(3);
   Singl_lst.erase(4);

   std::cout << "Удаление 3, 5, 7 элементов: ";
   Singl_lst.print();

   Singl_lst.push_front(10);
   std::cout << "Добавление элемента в начало: ";
   Singl_lst.print();

   Singl_lst.insert(Singl_lst.getSize() / 2, 20);
   std::cout << "Добавление элемента в середину: ";
   Singl_lst.print();

   Singl_lst.push_back(30);
   
   std::cout << "Добавление элемента в конец: ";
   Singl_lst.print();
   
   std::cout << std::endl;


    
   // Демонстрация семантики перемещения для ConsistentContainer
   {
       std::cout << "Демонстрация семантики перемещения для ConsistentContainer:\n";
       ConsistentContainer test;
       test.push_back(1);
       test.push_back(2);
       test.push_back(3);
       test.push_back(4);
       test.push_back(5);
       test.push_back(6);
       test.push_back(7);
       test.push_back(8);
       
       std::cout << "test не пуст: ";
       test.print();
       test.erase(0);
       test.erase(1);
       test.erase(1);
       test.erase(1);
       test.erase(1);
       
       
       test.print();


       ConsistentContainer moved_container = std::move(test); 
       std::cout << "Содержимое moved_container после перемещения: ";
       moved_container.print();

       std::cout << "Содержимое test после перемещения: ";
       test.print(); 

       if (test.getSize() == 0) {
           std::cout << "test успешно перемещен и теперь он не существует." << std::endl;
       } else {
           std::cout << "Ошибка: test существует!" << std::endl;
       }
       std::cout << std::endl;
   }

   
   // Демонстрация семантики перемещения для DoubleLinkedList
   {
       std::cout << "Демонстрация семантики перемещения для DoubleLinkedList:\n";
       DoubleLinkedList test;
       test.push_back(1);
       test.push_back(2);
       test.push_back(3);
       test.push_back(4);
       std::cout << "test не пуст: ";
       test.print();

       DoubleLinkedList moved_container = std::move(test); 
       std::cout << "Содержимое moved_container после перемещения: ";
       moved_container.print();

       std::cout << "Содержимое test после перемещения: ";
       test.print(); 

       if (test.getSize() == 0) {
           std::cout << "test успешно перемещен и теперь он не существует." << std::endl;
       } else {
           std::cout << "Ошибка: test существует!" << std::endl;
       }
       std::cout << std::endl;
   }

   
   // Демонстрация семантики перемещения для SinglyLinkedList
   {
     	std::cout << "Демонстрация семантики перемещения для SinglyLinkedList:\n";
     	SinglyLinkedList test;
     	test.push_back(1);
     	test.push_back(2);
     	test.push_back(3);
     	test.push_back(4);
     	std::cout << "testне пуст: ";
     	test.print();


     	SinglyLinkedList moved_container = std::move(test); 
     	std::cout << "Содержимое moved_container после перемещения: ";
     	moved_container.print();

        std::cout << "Содержимое testпосле перемещения: ";
        test.print(); 

     	if (test.getSize() == 0) {
         	std::cout << "test успешно перемещен и теперь он не существует." << std::endl;
     	} else {
         	std::cout << "Ошибка: test существует!" << std::endl;
     	}
     	std::cout << std::endl;
}}