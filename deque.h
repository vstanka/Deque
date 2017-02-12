#ifndef DEQUE_DEQUE_H
#define DEQUE_DEQUE_H

#include <vector>
#include <iterator>

enum class ReallocType{
    INCREASE = 0,
    DECREASE = 1,
    STAY = 2
};

class Index{
    typedef unsigned long long ullint;
    static const ullint  MIN_CAPACITY = 2;
    ullint capacity_;
    ullint index;
public:
    Index(ullint capacity, ullint index = 0) : capacity_(capacity), index(index){}
    Index& operator=(const Index& rIndex){
        this->capacity_ = rIndex.capacity_;
        this->index = rIndex.index;
        return *this;
    }
    operator ullint() const{
        return index;
    }
    Index operator+(ullint i) const {
        return Index(capacity_, (index + i % capacity_) % capacity_);
    }
    Index operator-(ullint i) const {
        ullint temp = capacity_;
        return Index(capacity_, (index + temp - i % temp) % temp);
    }
    const Index operator++(int){
        ullint temp = index++;
        index %= capacity_;
        return Index(capacity_, temp);
    }
    Index& operator++(){
        index = (index + 1) % capacity_;
        return *this;
    }
    const Index operator--(int){
        ullint temp = index;
        index += capacity_ - 1;
        index %= capacity_;
        return Index(capacity_, temp);
    }
    Index& operator--(){
        index = (index + capacity_ - 1) % capacity_;
        return *this;
    }
    Index& operator+=(ullint i){
        index = (index + i % capacity_) % capacity_;
        return *this;
    }
    Index& operator-=(ullint i){
        index = (index + capacity_ - i % capacity_) % capacity_;
        return *this;
    }

    bool operator==(const Index& rVI) const{
        return (rVI.capacity_ == this->capacity_) && (this->index == rVI.index);
    }
    bool operator!=(const Index& rVI) const{
        return !(operator==(rVI));
    }
    ~Index(){}
};
namespace Deque {
    typedef unsigned long long ullint;
    const ullint MIN_CAPACITY = 2;

    template <typename value_type, typename pointer, typename reference>
    class DequeIterator;

    template <class T>
    class Deque {
        typedef T value;
        typedef const T &crvalue;

        ullint capacity_;
        ullint size_;
        value *array;
        Index front_, back_; //elements of deque in [front_, back_)

        ReallocType needReallocate(){
            if (size_ == capacity_)
                return ReallocType::INCREASE;
            if (((capacity_ >> 2) > MIN_CAPACITY) && (size_ == (capacity_ >> 2)))
                return ReallocType::DECREASE;
            return ReallocType::STAY;
        }
        void reallocate(){
            ReallocType needIs = needReallocate();
            if (needIs == ReallocType::STAY)
                return;

            if (needReallocate() == ReallocType::INCREASE)
                capacity_ <<= 1;
            else
                capacity_ >>= 1;

            value *temp = array;
            array = new value[capacity_];

            Index j = front_;
            for (ullint i = 0; i < size_; ++i, ++j)
                array[i] = temp[j];
            delete[] temp;
            front_ = Index(capacity_, 0);
            back_ = Index(capacity_, size_);
        }

    public:
        typedef DequeIterator<value, value*, value&> iterator;
        typedef DequeIterator<value, const value*, crvalue> const_iterator;
        typedef std::reverse_iterator <iterator> reverse_iterator;
        typedef std::reverse_iterator <const_iterator> const_reverse_iterator;
        Deque() : size_(0), capacity_(MIN_CAPACITY), array(new value[capacity_]), front_(capacity_), back_(capacity_){}
        Deque(const Deque &rdeq) : size_(rdeq.size_), capacity_(rdeq.capacity_), array(new value[capacity_]),
                                   front_(rdeq.front_), back_(rdeq.back_){
            for (ullint i = 0; i < capacity_; ++i)
                array[i] = rdeq.array[i];
        }
        Deque& operator=(const Deque& deq){
            if (deq.array == this->array)
                return *this;
            this->size_ = deq.size_;
            this->capacity_ = deq.capacity_;
            delete[]array;

            this->array = new value[capacity_];
            for (size_t i = 0; i < capacity_; ++i)
                this->array[i] = deq.array[i];

            this->front_ = deq.front_;
            this->back_ = deq.back_;
            return *this;
        }
        void push_front(crvalue elem){
            reallocate();
            array[--front_] = elem;
            size_++;
        }
        void push_back(crvalue elem){
            reallocate();
            array[back_++] = elem;
            size_++;
        }
        void pop_front(){
            reallocate();
            front_++;
            size_--;
        }
        void pop_back(){
            reallocate();
            back_--;
            size_--;
        }
        value &front(){
            return array[front_];
        }
        value &back(){
            return array[back_];
        }
        crvalue front() const{
            return array[front_];
        }
        crvalue back() const{
            return array[back_];
        }

        ullint capacity() const{
            return capacity_;
        }
        ullint size() const{
            return size_;
        }
        bool empty() const{
            return size_ == 0;
        }

        value &operator[](ullint i){
            return array[front_ + i];
        }
        crvalue operator[](ullint i) const{
            return array[front_ + i];
        }

        iterator begin(){
            return iterator(this, 0);
        }
        iterator end(){
            return iterator(this, size_);
        }
        const_iterator cbegin() const {
            return const_iterator(this, 0);
        }
        const_iterator cend() const {
            return const_iterator(this, size_);
        }
        const_iterator begin() const {
            return cbegin();
        }
        const_iterator end() const {
            return cend();
        }

        reverse_iterator rbegin(){
            return reverse_iterator(begin());
        }
        reverse_iterator rend(){
            return reverse_iterator(end());
        }
        const_reverse_iterator crbegin() const {
            return const_reverse_iterator(cbegin());;
        }
        const_reverse_iterator crend() const {
            return const_reverse_iterator(cend());
        }
        const_reverse_iterator rbegin() const {
            return crbegin();
        }
        const_reverse_iterator rend() const {
            return crend();
        }

        ~Deque(){
            delete[] array;
        };
    };



    template <typename value_type, typename pointer, typename reference>
    class DequeIterator : public std::iterator <std::random_access_iterator_tag, value_type, ssize_t , pointer, reference>{
        typedef Deque<value_type> DequeType;
        typedef ssize_t difference_type;
        typedef DequeIterator<value_type, pointer, reference> DequeIteratorType;
        typedef const DequeIteratorType& crefDequeIteratorType;
        DequeType *deque;
        Index index;
    public:
        DequeIterator() : deque(nullptr), index(0) {}
        DequeIterator(DequeType* deque, ullint index) : deque(deque), index(deque->capacity(), index) {}
        DequeIterator(crefDequeIteratorType deqIter) : deque(deqIter.deque), index(deqIter.index) {}

        DequeIteratorType& operator=(crefDequeIteratorType deqIter){
            this->deque = deqIter.deque;
            this->index = deqIter.index;
            return *this;
        }

        DequeIteratorType& operator++(){
            index++;
            return *this;
        }
        DequeIteratorType operator++(int){
            DequeIteratorType temp = *this;
            index++;
            return temp;
        }
        DequeIteratorType& operator--(){
            index--;
            return *this;
        }
        DequeIteratorType operator--(int){
            DequeIteratorType temp = *this;
            index--;
            return temp;
        }

        DequeIteratorType& operator+=(ullint i){
            index += i;
            return *this;
        }
        DequeIteratorType& operator-=(ullint i){
            index -= i;
            return *this;
        }

        DequeIteratorType operator+(ullint i) const {
            return DequeIteratorType(deque, index + i);
        }
        DequeIteratorType operator-(ullint i) const {
            return DequeIteratorType(deque, index - i);
        }

        difference_type operator-(crefDequeIteratorType deqIter) const {
            return static_cast <difference_type>(this->index) - static_cast <difference_type>(deqIter.index);
        }

        bool operator<(crefDequeIteratorType deqIter) const {
            return (this->deque == deqIter.deque) && (this->index < deqIter.index);
        }
        bool operator>(crefDequeIteratorType deqIter) const {
            return (this->deque == deqIter.deque) && (this->index > deqIter.index);
        }
        bool operator==(crefDequeIteratorType deqIter) const {
            return (this->deque == deqIter.deque) && (this->index == deqIter.index);
        }
        bool operator!=(crefDequeIteratorType deqIter) const {
            return !(operator==(deqIter));
        }
        bool operator>=(crefDequeIteratorType deqIter) const {
            return (operator>(deqIter) || operator==(deqIter));
        }
        bool operator<=(crefDequeIteratorType deqIter) const {
            return (operator<(deqIter) || operator==(deqIter));
        }

        reference operator*(){
            return (*deque)[index];
        }
        pointer operator->(){
            return &((*deque)[index]);
        }

        reference operator[](ullint i){
            return (*deque)[index + i];
        }
    };

    template <typename value_type, typename pointer, typename reference>
    DequeIterator<value_type, pointer, reference> operator+ (ullint i, DequeIterator<value_type, pointer, reference> deqIter){
        return deqIter + i;
    }
}

#endif //DEQUE_DEQUE







































