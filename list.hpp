
#ifndef SJTU_LIST_HPP
#define SJTU_LIST_HPP

#include <cstddef>
#include <memory>
#include <stdexcept>

namespace sjtu {

template <typename T>
class list {
private:
    struct node {
        T* data;
        node* prev;
        node* next;
        
        node() : data(nullptr), prev(nullptr), next(nullptr) {}
        
        node(const T& value) : data(nullptr), prev(nullptr), next(nullptr) {
            data = static_cast<T*>(operator new(sizeof(T)));
            new (data) T(value);
        }
        
        ~node() {
            if (data) {
                data->~T();
                operator delete(data);
            }
        }
    };
    
    node* head;
    node* tail;
    size_t list_size;

    void initialize() {
        head = new node();
        tail = new node();
        head->next = tail;
        tail->prev = head;
        list_size = 0;
    }

    void clear_nodes() {
        while (head->next != tail) {
            node* temp = head->next;
            head->next = temp->next;
            temp->next->prev = head;
            delete temp;
        }
    }

public:
    class const_iterator;
    
    class iterator {
    private:
        node* current;
        
    public:
        iterator() : current(nullptr) {}
        iterator(node* n) : current(n) {}
        
        iterator operator++(int) {
            iterator old = *this;
            ++(*this);
            return old;
        }
        
        iterator& operator++() {
            if (current) {
                current = current->next;
            }
            return *this;
        }
        
        iterator operator--(int) {
            iterator old = *this;
            --(*this);
            return old;
        }
        
        iterator& operator--() {
            if (current) {
                current = current->prev;
            }
            return *this;
        }
        
        T& operator*() const {
            if (!current || !current->data) {
                throw std::runtime_error("Invalid iterator");
            }
            return *(current->data);
        }
        
        T* operator->() const {
            if (!current || !current->data) {
                throw std::runtime_error("Invalid iterator");
            }
            return current->data;
        }
        
        bool operator==(const iterator& rhs) const {
            return current == rhs.current;
        }
        
        bool operator!=(const iterator& rhs) const {
            return current != rhs.current;
        }
        
        bool operator==(const const_iterator& rhs) const {
            return current == rhs.current;
        }
        
        bool operator!=(const const_iterator& rhs) const {
            return current != rhs.current;
        }
        
        friend class const_iterator;
    };
    
    class const_iterator {
    private:
        const node* current;
        
    public:
        const_iterator() : current(nullptr) {}
        const_iterator(const node* n) : current(n) {}
        const_iterator(const iterator& it) : current(it.current) {}
        
        const_iterator operator++(int) {
            const_iterator old = *this;
            ++(*this);
            return old;
        }
        
        const_iterator& operator++() {
            if (current) {
                current = current->next;
            }
            return *this;
        }
        
        const_iterator operator--(int) {
            const_iterator old = *this;
            --(*this);
            return old;
        }
        
        const_iterator& operator--() {
            if (current) {
                current = current->prev;
            }
            return *this;
        }
        
        const T& operator*() const {
            if (!current || !current->data) {
                throw std::runtime_error("Invalid iterator");
            }
            return *(current->data);
        }
        
        const T* operator->() const {
            if (!current || !current->data) {
                throw std::runtime_error("Invalid iterator");
            }
            return current->data;
        }
        
        bool operator==(const const_iterator& rhs) const {
            return current == rhs.current;
        }
        
        bool operator!=(const const_iterator& rhs) const {
            return current != rhs.current;
        }
        
        bool operator==(const iterator& rhs) const {
            return current == rhs.current;
        }
        
        bool operator!=(const iterator& rhs) const {
            return current != rhs.current;
        }
    };

    list() {
        initialize();
    }
    
    list(const list& other) {
        initialize();
        for (const auto& item : other) {
            push_back(item);
        }
    }
    
    list& operator=(const list& other) {
        if (this != &other) {
            clear();
            for (const auto& item : other) {
                push_back(item);
            }
        }
        return *this;
    }
    
    ~list() {
        clear();
        delete head;
        delete tail;
    }
    
    const T& front() const {
        if (empty()) {
            throw std::runtime_error("List is empty");
        }
        return *(head->next->data);
    }
    
    T& front() {
        if (empty()) {
            throw std::runtime_error("List is empty");
        }
        return *(head->next->data);
    }
    
    const T& back() const {
        if (empty()) {
            throw std::runtime_error("List is empty");
        }
        return *(tail->prev->data);
    }
    
    T& back() {
        if (empty()) {
            throw std::runtime_error("List is empty");
        }
        return *(tail->prev->data);
    }
    
    iterator begin() {
        return iterator(head->next);
    }
    
    const_iterator cbegin() const {
        return const_iterator(head->next);
    }
    
    iterator end() {
        return iterator(tail);
    }
    
    const_iterator cend() const {
        return const_iterator(tail);
    }
    
    bool empty() const {
        return list_size == 0;
    }
    
    size_t size() const {
        return list_size;
    }
    
    void clear() {
        clear_nodes();
        list_size = 0;
    }
    
    iterator insert(iterator pos, const T& value) {
        node* pos_node = pos.current;
        if (!pos_node) {
            throw std::runtime_error("Invalid iterator");
        }
        
        node* new_node = new node(value);
        new_node->next = pos_node;
        new_node->prev = pos_node->prev;
        pos_node->prev->next = new_node;
        pos_node->prev = new_node;
        
        list_size++;
        return iterator(new_node);
    }
    
    iterator erase(iterator pos) {
        if (empty() || !pos.current || pos.current == tail) {
            throw std::runtime_error("Invalid iterator");
        }
        
        node* pos_node = pos.current;
        node* next_node = pos_node->next;
        
        pos_node->prev->next = pos_node->next;
        pos_node->next->prev = pos_node->prev;
        
        delete pos_node;
        list_size--;
        
        return iterator(next_node);
    }
    
    void push_back(const T& value) {
        insert(end(), value);
    }
    
    void pop_back() {
        if (empty()) {
            throw std::runtime_error("List is empty");
        }
        erase(--end());
    }
    
    void push_front(const T& value) {
        insert(begin(), value);
    }
    
    void pop_front() {
        if (empty()) {
            throw std::runtime_error("List is empty");
        }
        erase(begin());
    }
};

} // namespace sjtu

#endif // SJTU_LIST_HPP
