// Bondar Ihor K-15 Variant 11

#include "mystruct.h"
#include <iostream>
#include <cmath>

MyStruct::Block::Block() : size(0) {
}

MyStruct::Block::Element::Element() : value(0), id(0) {}

MyStruct::Block::Element::Element(long long val, size_t elem_id) : value(val), id(elem_id) {}

MyStruct::iterator::iterator() : container(nullptr), block_idx(0), elem_idx(0) {}

MyStruct::iterator::iterator(MyStruct* cont, size_t blk, size_t elem)
    : container(cont), block_idx(blk), elem_idx(elem) {}

MyStruct::reference MyStruct::iterator::operator*() const {
    return container->blocks[block_idx]->data[elem_idx].value;
}

MyStruct::iterator::pointer MyStruct::iterator::operator->() const {
    return &(container->blocks[block_idx]->data[elem_idx].value);
}

MyStruct::iterator& MyStruct::iterator::operator++() {
    ++elem_idx;

    if (elem_idx >= container->blocks[block_idx]->size) {
        do {
            ++block_idx;
            elem_idx = 0;
        } while (block_idx < container->num_blocks &&
                 container->blocks[block_idx]->size == 0);
    }

    return *this;
}

MyStruct::iterator MyStruct::iterator::operator++(int) {
    iterator tmp = *this;
    ++(*this);
    return tmp;
}

MyStruct::iterator& MyStruct::iterator::operator--() {
    if (elem_idx == 0) {
        size_t prev_block = block_idx;
        do {
            if (prev_block == 0) {
                throw std::out_of_range("Iterator decremented beyond begin()");
            }
            --prev_block;
        } while (container->blocks[prev_block]->size == 0);

        block_idx = prev_block;
        elem_idx = container->blocks[block_idx]->size - 1;
    } else {
        --elem_idx;
    }

    return *this;
}

MyStruct::iterator MyStruct::iterator::operator--(int) {
    iterator tmp = *this;
    --(*this);
    return tmp;
}

bool MyStruct::iterator::operator==(const iterator& other) const {
    return container == other.container &&
           block_idx == other.block_idx &&
           elem_idx == other.elem_idx;
}

bool MyStruct::iterator::operator!=(const iterator& other) const {
    return !(*this == other);
}

MyStruct::const_iterator::const_iterator() : container(nullptr), block_idx(0), elem_idx(0) {}

MyStruct::const_iterator::const_iterator(const MyStruct* cont, size_t blk, size_t elem)
    : container(cont), block_idx(blk), elem_idx(elem) {}

MyStruct::const_iterator::const_iterator(const iterator& it)
    : container(it.container), block_idx(it.block_idx), elem_idx(it.elem_idx) {}

MyStruct::const_reference MyStruct::const_iterator::operator*() const {
    return container->blocks[block_idx]->data[elem_idx].value;
}

MyStruct::const_iterator::pointer MyStruct::const_iterator::operator->() const {
    return &(container->blocks[block_idx]->data[elem_idx].value);
}

MyStruct::const_iterator& MyStruct::const_iterator::operator++() {
    ++elem_idx;

    if (elem_idx >= container->blocks[block_idx]->size) {
        do {
            ++block_idx;
            elem_idx = 0;
        } while (block_idx < container->num_blocks &&
                 container->blocks[block_idx]->size == 0);
    }

    return *this;
}

MyStruct::const_iterator MyStruct::const_iterator::operator++(int) {
    const_iterator tmp = *this;
    ++(*this);
    return tmp;
}

MyStruct::const_iterator& MyStruct::const_iterator::operator--() {
    if (elem_idx == 0) {
        size_t prev_block = block_idx;
        do {
            if (prev_block == 0) {
                throw std::out_of_range("Iterator decremented beyond begin()");
            }
            --prev_block;
        } while (container->blocks[prev_block]->size == 0);

        block_idx = prev_block;
        elem_idx = container->blocks[block_idx]->size - 1;
    } else {
        --elem_idx;
    }

    return *this;
}

MyStruct::const_iterator MyStruct::const_iterator::operator--(int) {
    const_iterator tmp = *this;
    --(*this);
    return tmp;
}

bool MyStruct::const_iterator::operator==(const const_iterator& other) const {
    return container == other.container &&
           block_idx == other.block_idx &&
           elem_idx == other.elem_idx;
}

bool MyStruct::const_iterator::operator!=(const const_iterator& other) const {
    return !(*this == other);
}

MyStruct::MyStruct() : num_blocks(1), total_size(0), next_id(0) {
    blocks[0] = new Block();
}

MyStruct::MyStruct(std::initializer_list<value_type> init) : MyStruct() {
    for (const auto& value : init) {
        push_back(value);
    }
}

MyStruct::MyStruct(const MyStruct& other)
    : num_blocks(other.num_blocks), total_size(other.total_size), next_id(other.next_id) {
    for (size_t i = 0; i < num_blocks; ++i) {
        blocks[i] = new Block();
        blocks[i]->size = other.blocks[i]->size;

        for (size_t j = 0; j < blocks[i]->size; ++j) {
            blocks[i]->data[j] = other.blocks[i]->data[j];
        }
    }
}

MyStruct::MyStruct(MyStruct&& other) noexcept
    : num_blocks(other.num_blocks), total_size(other.total_size), next_id(other.next_id) {
    for (size_t i = 0; i < num_blocks; ++i) {
        blocks[i] = other.blocks[i];
        other.blocks[i] = nullptr;
    }

    other.num_blocks = 0;
    other.total_size = 0;
    other.next_id = 0;
}

MyStruct& MyStruct::operator=(const MyStruct& other) {
    if (this != &other) {
        clear();

        num_blocks = other.num_blocks;
        total_size = other.total_size;
        next_id = other.next_id;

        for (size_t i = 0; i < num_blocks; ++i) {
            blocks[i] = new Block();
            blocks[i]->size = other.blocks[i]->size;

            for (size_t j = 0; j < blocks[i]->size; ++j) {
                blocks[i]->data[j] = other.blocks[i]->data[j];
            }
        }
    }
    return *this;
}

MyStruct& MyStruct::operator=(MyStruct&& other) noexcept {
    if (this != &other) {
        clear();

        num_blocks = other.num_blocks;
        total_size = other.total_size;
        next_id = other.next_id;

        for (size_t i = 0; i < num_blocks; ++i) {
            blocks[i] = other.blocks[i];
            other.blocks[i] = nullptr;
        }

        other.num_blocks = 0;
        other.total_size = 0;
        other.next_id = 0;
    }
    return *this;
}

MyStruct::~MyStruct() {
    clear();
}

MyStruct::size_type MyStruct::size() const {
    return total_size;
}

MyStruct::size_type MyStruct::capacity() const {
    return num_blocks * Block::BLOCK_SIZE;
}

bool MyStruct::empty() const {
    return total_size == 0;
}

void MyStruct::shrink_to_fit() {
    for (int i = 0; i < num_blocks; ++i) {
        if (blocks[i]->size == 0)
            delete [] blocks[i];
    }
}

MyStruct::iterator MyStruct::begin() {
    if (empty()) return end();

    return iterator(this,0,0);
}

MyStruct::iterator MyStruct::end() {
    return iterator(this, num_blocks, 0);
}

MyStruct::const_iterator MyStruct::begin() const {
    if (empty()) return end();

    return const_iterator(this, 0, 0);
}

MyStruct::const_iterator MyStruct::end() const {
    return const_iterator(this, num_blocks, 0);
}

MyStruct::const_iterator MyStruct::cbegin() const {
    return begin();
}

MyStruct::const_iterator MyStruct::cend() const {
    return end();
}

MyStruct::reference MyStruct::front() {
    if (empty())
        throw std::out_of_range("Container is empty");

    for (size_t i = 0; i < num_blocks; ++i) {
        if (blocks[i]->size > 0) {
            return blocks[i]->data[0].value;
        }
    }

    throw std::out_of_range("Container is empty");
}

MyStruct::const_reference MyStruct::front() const {
    if (empty())
        throw std::out_of_range("Container is empty");

    for (size_t i = 0; i < num_blocks; ++i) {
        if (blocks[i]->size > 0) {
            return blocks[i]->data[0].value;
        }
    }

    throw std::out_of_range("Container is empty");
}

MyStruct::reference MyStruct::back() {
    if (empty())
        throw std::out_of_range("Container is empty");

    for (int i = num_blocks - 1; i >= 0; --i) {
        if (blocks[i]->size > 0) {
            return blocks[i]->data[blocks[i]->size - 1].value;
        }
    }

    throw std::out_of_range("Container is empty");
}

MyStruct::const_reference MyStruct::back() const {
    if (empty())
        throw std::out_of_range("Container is empty");

    for (int i = num_blocks - 1; i >= 0; --i) {
        if (blocks[i]->size > 0) {
            return blocks[i]->data[blocks[i]->size - 1].value;
        }
    }

    throw std::out_of_range("Container is empty");
}

void MyStruct::push_back(const value_type& value) {
    if (capacity() == total_size) {
        if (num_blocks < MAX_BLOCKS) {
            blocks[num_blocks] = new Block();
            num_blocks++;
        } else {
            removeOldestElements();
        }
    }
    size_type block_idx = total_size / Block::BLOCK_SIZE;
    size_type elem_idx = total_size % Block::BLOCK_SIZE;
    blocks[block_idx]->data[elem_idx].value = value;
    blocks[block_idx]->data[elem_idx].id = total_size;
    blocks[block_idx]->size++;
    ++total_size;
}

void MyStruct::push_front(const value_type& value) {
    insert(cbegin(), value);
}

void MyStruct::pop_back() {
    if (empty()) return;
    erase(--cend());
}

void MyStruct::pop_front() {
    if (!empty()) {
        erase(cbegin());
    }
}

MyStruct::iterator MyStruct::insert(const_iterator pos, const value_type& value) {
    if (capacity() == total_size) {
        if (num_blocks < MAX_BLOCKS) {
            blocks[num_blocks] = new Block();
            num_blocks++;
        } else {
            removeOldestElements(pos);
        }
    }

    if (empty()) {
        blocks[0]->data[0] = Block::Element(value, 0);
        blocks[0]->size = 1;
        total_size = 1;
        return begin();
    }

    size_t insert_pos = 0;

    if (pos == cend()) {
        insert_pos = total_size;
    } else {
        for (size_t i = 0; i < pos.block_idx; ++i) {
            insert_pos += blocks[i]->size;
        }
        insert_pos += pos.elem_idx;
    }
    if (insert_pos == total_size) {
        push_back(value);
        for (int i = num_blocks - 1; i >= 0; --i) {
            if (blocks[i]->size > 0) {
                return iterator(this, i, blocks[i]->size - 1);
            }
        }
        return end();
    }

    size_t last_block = 0;
    for (int i = num_blocks - 1; i >= 0; --i) {
        if (blocks[i]->size > 0) {
            last_block = i;
            break;
        }
    }

    if (blocks[last_block]->size < Block::BLOCK_SIZE) {
        blocks[last_block]->size++;
    } else if (last_block + 1 < num_blocks) {
        blocks[last_block + 1]->size = 1;
    }

    Block::Element* temp = new Block::Element[total_size - insert_pos];

    size_t temp_idx = 0;
    for (size_t i = insert_pos; i < total_size; ++i) {
        size_t curr_block = 0;
        size_t curr_idx = i;

        for (size_t b = 0; b < num_blocks; ++b) {
            if (curr_idx < blocks[b]->size) {
                curr_block = b;
                break;
            }
            curr_idx -= blocks[b]->size;
        }

        temp[temp_idx++] = blocks[curr_block]->data[curr_idx];
    }


    for (size_t i = 0; i < temp_idx; ++i) {
        size_t target_pos = insert_pos + i + 1;

        size_t target_block = 0;
        size_t target_idx = target_pos;

        for (size_t b = 0; b < num_blocks; ++b) {
            if (target_idx < blocks[b]->size) {
                target_block = b;
                break;
            }
            target_idx -= blocks[b]->size;
        }

        blocks[target_block]->data[target_idx] = temp[i];
    }

    delete[] temp;


    size_t insert_block = 0;
    size_t insert_idx = insert_pos;

    for (size_t b = 0; b < num_blocks; ++b) {
        if (insert_idx < blocks[b]->size) {
            insert_block = b;
            break;
        }
        insert_idx -= blocks[b]->size;
    }

    blocks[insert_block]->data[insert_idx] = Block::Element(value, total_size);

    total_size++;

    return iterator(this, insert_block, insert_idx);
}

MyStruct::iterator MyStruct::erase(const_iterator pos) {
    if (empty() || pos == cend()) return end();

    size_t block_idx = pos.block_idx;
    size_t elem_idx = pos.elem_idx;

    int min_id = blocks[block_idx]->data[elem_idx].id;
    addToIds(-1,min_id);

    size_t global_idx = 0;
    for (size_t b = 0; b < block_idx; ++b) {
        global_idx += blocks[b]->size;
    }
    global_idx += elem_idx;


    for (size_t i = global_idx + 1; i < total_size; ++i) {
        size_t curr_block = 0, curr_idx = i;
        while (curr_idx >= blocks[curr_block]->size) {
            curr_idx -= blocks[curr_block]->size;
            ++curr_block;
        }

        size_t prev_block = 0, prev_idx = i - 1;
        while (prev_idx >= blocks[prev_block]->size) {
            prev_idx -= blocks[prev_block]->size;
            ++prev_block;
        }

        blocks[prev_block]->data[prev_idx] = blocks[curr_block]->data[curr_idx];
    }

    for (int b = num_blocks - 1; b >= 0; --b) {
        if (blocks[b]->size > 0) {
            --blocks[b]->size;
            break;
        }
    }


    --total_size;

    if (elem_idx < blocks[block_idx]->size) {
        return iterator(this, block_idx, elem_idx);
    } else {
        for (size_t b = block_idx + 1; b < num_blocks; ++b) {
            if (blocks[b]->size > 0) {
                return iterator(this, b, 0);
            }
        }
        return end();
    }
}
void MyStruct::clear() {
    Block* new_block = new Block();

    for (size_t i = 0; i < num_blocks; ++i) {
        delete blocks[i];
    }

    blocks[0] = new_block;

    num_blocks = 1;
    total_size = 0;
}

void MyStruct::addToIds(int value, int min_id) {
     for (int i = 0; i < num_blocks; ++i) {
         for (int j = 0; j < blocks[i]->size; ++j) {
             MyStruct::Block::Element *elem = &blocks[i]->data[j];
             if (elem->id >= min_id) {
                 elem->id += value;
             }
         }
     }
}

MyStruct::const_iterator MyStruct::findOldestElement() const {
    size_t block_idx;
    size_t elem_idx;
    for (int i = 0; i < num_blocks; ++i) {
        for (int j = 0; j < blocks[i]->size; ++j) {
            if (blocks[i]->data[j].id == 0) {
                block_idx = i;
                elem_idx = j;
                break;
            }
        }
    }
    return const_iterator(this, block_idx, elem_idx);
}

void MyStruct::removeOldestElements() {
    int to_remove = std::ceil(total_size * 0.05);
    while (to_remove > 0) {
        erase(findOldestElement());
        --to_remove;
    }
}

void MyStruct::removeOldestElements(const_iterator &pos) {
    int to_remove = std::ceil(total_size * 0.05);
    while (to_remove > 0) {
        const_iterator it = findOldestElement();
        if (it.block_idx < pos.block_idx) {
            --pos;
        } else if (it.block_idx == pos.block_idx && it.elem_idx <    pos.elem_idx) {
            --pos;
        }
        erase(it);
        --to_remove;
    }
}
