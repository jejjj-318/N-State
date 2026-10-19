#ifndef N_STATE_HISTORY_TABLE_H
#define N_STATE_HISTORY_TABLE_H

#include <cstddef>
#include <memory>
#include <stdexcept>
#include <utility>

template<typename T>
class HistoryTable
{
public:
    HistoryTable();
    HistoryTable(const HistoryTable& other);
    ~HistoryTable();

    HistoryTable& operator=(const HistoryTable& other);

    void add(int key, const T& value);
    HistoryTable& operator<<(const T& value);

    bool remove(int key);
    void clear();

    std::size_t size() const;
    bool contains(int key) const;

    T& operator[](int key);
    const T& operator[](int key) const;

    bool operator==(const HistoryTable& other) const;
    HistoryTable operator&&(const HistoryTable& other) const;

private:
    struct Node
    {
        int key;
        T value;
        std::unique_ptr<Node> next;
    };

    static constexpr std::size_t BUCKET_COUNT = 32;

    static std::size_t bucketOf(int key);

    Node* findNode(int key) const;
    void copyFrom(const HistoryTable& other);

    std::unique_ptr<Node> buckets[BUCKET_COUNT];
    std::size_t itemCount;
};

template<typename T>
HistoryTable<T>::HistoryTable()
    : itemCount(0)
{
}

template<typename T>
HistoryTable<T>::HistoryTable(const HistoryTable& other)
    : itemCount(0)
{
    copyFrom(other);
}

template<typename T>
HistoryTable<T>::~HistoryTable() = default;

template<typename T>
HistoryTable<T>& HistoryTable<T>::operator=(
    const HistoryTable& other
)
{
    if (this == &other)
    {
        return *this;
    }
    clear();
    copyFrom(other);
    return *this;
}

template<typename T>
std::size_t HistoryTable<T>::bucketOf(int key)
{
    const int safe = key < 0 ? -key : key;
    return static_cast<std::size_t>(safe) % BUCKET_COUNT;
}

template<typename T>
typename HistoryTable<T>::Node*
HistoryTable<T>::findNode(int key) const
{
    Node* node = buckets[bucketOf(key)].get();
    while (node != nullptr)
    {
        if (node->key == key)
        {
            return node;
        }
        node = node->next.get();
    }
    return nullptr;
}

template<typename T>
void HistoryTable<T>::copyFrom(const HistoryTable& other)
{
    for (std::size_t i = 0; i < BUCKET_COUNT; ++i)
    {
        Node* node = other.buckets[i].get();
        while (node != nullptr)
        {
            add(node->key, node->value);
            node = node->next.get();
        }
    }
}

template<typename T>
void HistoryTable<T>::add(int key, const T& value)
{
    Node* existing = findNode(key);
    if (existing != nullptr)
    {
        existing->value = value;
        return;
    }

    const std::size_t index = bucketOf(key);
    auto node = std::make_unique<Node>();
    node->key = key;
    node->value = value;
    node->next = std::move(buckets[index]);
    buckets[index] = std::move(node);
    ++itemCount;
}

template<typename T>
HistoryTable<T>& HistoryTable<T>::operator<<(const T& value)
{
    add(value.getTurn(), value);
    return *this;
}

template<typename T>
bool HistoryTable<T>::remove(int key)
{
    auto& head = buckets[bucketOf(key)];
    Node* previous = nullptr;
    Node* current = head.get();

    while (current != nullptr)
    {
        if (current->key == key)
        {
            if (previous == nullptr)
            {
                head = std::move(current->next);
            }
            else
            {
                previous->next = std::move(current->next);
            }
            --itemCount;
            return true;
        }
        previous = current;
        current = current->next.get();
    }
    return false;
}

template<typename T>
void HistoryTable<T>::clear()
{
    for (std::size_t i = 0; i < BUCKET_COUNT; ++i)
    {
        buckets[i].reset();
    }
    itemCount = 0;
}

template<typename T>
std::size_t HistoryTable<T>::size() const
{
    return itemCount;
}

template<typename T>
bool HistoryTable<T>::contains(int key) const
{
    return findNode(key) != nullptr;
}

template<typename T>
T& HistoryTable<T>::operator[](int key)
{
    Node* node = findNode(key);
    if (node == nullptr)
    {
        throw std::out_of_range("history key not found");
    }
    return node->value;
}

template<typename T>
const T& HistoryTable<T>::operator[](int key) const
{
    const Node* node = findNode(key);
    if (node == nullptr)
    {
        throw std::out_of_range("history key not found");
    }
    return node->value;
}

template<typename T>
bool HistoryTable<T>::operator==(
    const HistoryTable& other
) const
{
    if (itemCount != other.itemCount)
    {
        return false;
    }
    for (std::size_t i = 0; i < BUCKET_COUNT; ++i)
    {
        Node* node = buckets[i].get();
        while (node != nullptr)
        {
            if (!other.contains(node->key))
            {
                return false;
            }
            if (!(node->value == other[node->key]))
            {
                return false;
            }
            node = node->next.get();
        }
    }
    return true;
}

template<typename T>
HistoryTable<T> HistoryTable<T>::operator&&(
    const HistoryTable& other
) const
{
    HistoryTable result;
    for (std::size_t i = 0; i < BUCKET_COUNT; ++i)
    {
        Node* node = buckets[i].get();
        while (node != nullptr)
        {
            if (other.contains(node->key) &&
                node->value == other[node->key])
            {
                result.add(node->key, node->value);
            }
            node = node->next.get();
        }
    }
    return result;
}

#endif
