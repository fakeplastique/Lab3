// Bondar Ihor K-15 Variant 11

#ifndef MY_STRUCT_H
#define MY_STRUCT_H

#include <initializer_list>
#include <cstddef>

class MyStruct {
private:
    void addToIds(int value, int min_id);
    struct Block {
        static const size_t BLOCK_SIZE = 100;
        size_t size;

        Block();

        struct Element {
            long long value;
            size_t id;

            Element();
            Element(long long val, size_t elem_id);
        };

        Element data[BLOCK_SIZE];
    };

    static const size_t MAX_BLOCKS = 64;
    Block* blocks[MAX_BLOCKS];
    size_t num_blocks;
    size_t total_size;
    size_t next_id;

    void removeOldestElements();

public:
    using value_type = long long;
    using reference = value_type&;
    using const_reference = const value_type&;
    using size_type = size_t;
    
    class iterator {
    private:
        MyStruct* container;
        size_t block_idx;
        size_t elem_idx;
        
        friend class MyStruct;
        friend class const_iterator;
        iterator& operator--();
        iterator operator--(int);
        
    public:
        using value_type = MyStruct::value_type;
        using pointer = value_type*;
        using reference = value_type&;
        
        iterator();
        iterator(MyStruct* cont, size_t blk, size_t elem);
        
        reference operator*() const;
        pointer operator->() const;
        
        iterator& operator++();
        iterator operator++(int);
        
        bool operator==(const iterator& other) const;
        bool operator!=(const iterator& other) const;
    };
    
    class const_iterator {
    private:
        const MyStruct* container;
        size_t block_idx;
        size_t elem_idx;

        friend class MyStruct;
        const_iterator& operator--();
        const_iterator operator--(int);
    public:
        using value_type = MyStruct::value_type;
        using pointer = const value_type*;
        using reference = const value_type&;
        
        const_iterator();
        const_iterator(const MyStruct* cont, size_t blk, size_t elem);
        const_iterator(const iterator& it);
        
        reference operator*() const;
        pointer operator->() const;
        
        const_iterator& operator++();
        const_iterator operator++(int);
        
        bool operator==(const const_iterator& other) const;
        bool operator!=(const const_iterator& other) const;
    };
    
    MyStruct();
    MyStruct(std::initializer_list<value_type> init);
    MyStruct(const MyStruct& other);
    MyStruct(MyStruct&& other) noexcept;
    MyStruct& operator=(const MyStruct& other);
    MyStruct& operator=(MyStruct&& other) noexcept;
    ~MyStruct();
    
    size_type size() const;
    size_type capacity() const;
    bool empty() const;
    void shrink_to_fit();
    
    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;
    const_iterator cbegin() const;
    const_iterator cend() const;
    
    reference front();
    const_reference front() const;
    reference back();
    const_reference back() const;
    
    void push_back(const value_type& value);
    void push_front(const value_type& value);
    void pop_back();
    void pop_front();
    
    iterator insert(const_iterator pos, const value_type& value);
    
    iterator erase(const_iterator pos);
    
    void clear();
private:
    const_iterator findOldestElement() const;
    void removeOldestElements(const_iterator &pos);
};

#endif // MY_STRUCT_H