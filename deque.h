#ifndef DEQUE_DEQUENEW_H
#define DEQUE_DEQUENEW_H

#include <iterator>
#include <algorithm>

enum class ReallocType{
    INCREASE = 0,
    DECREASE = 1,
    STAY = 2
};

template <typename value_type, typename pointer, typename reference>
class DequeIterator;

template <class value>
class Deque{
    typedef const value &crvalue;
    size_t capacity_;
    size_t size_;
    size_t front_, back_;
    value *array;

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

        size_t ex_capacity = capacity_;
        if (needIs == ReallocType::INCREASE)
            capacity_ <<= 1;
        else
            capacity_ >>= 1;

        value *temp = array;
        array = new value[capacity_];

        for (size_t i = 0, j = front_; i < size_; ++i, j = (j + 1) % ex_capacity)
            array[i] = temp[j];
        delete[] temp;
        front_ = 0;
        back_ = size_;
    }

public:
    static const size_t MIN_CAPACITY = 2;
    friend class DequeIterator <value, value*, value&>;
    friend class DequeIterator <value, const value*, const value&>;
    typedef DequeIterator <value, value *, value &> iterator;
    typedef DequeIterator <value, const value *, crvalue> const_iterator;
    typedef std::reverse_iterator <iterator> reverse_iterator;
    typedef std::reverse_iterator <const_iterator> const_reverse_iterator;

    Deque() : capacity_(MIN_CAPACITY), size_(0), front_(0), back_(0){
        array = new value[capacity_];
    }

    Deque(const Deque &deq) : size_(deq.size_), front_(deq.front_), back_(deq.back_){
        capacity_ = deq.capacity_;
        array = new value[capacity_];
        for (size_t i = 0; i < capacity_; ++i)
            array[i] = deq.array[i];
    }
    Deque &operator=(const Deque &deq){
        size_ = deq.size_;
        front_= deq.front_;
        back_ = deq.back_;
        if (capacity_ != deq.capacity_) {
            delete []array;
            array = new value[deq.capacity_];
            capacity_ = deq.capacity_;
        }
        for (size_t i = 0; i < capacity_; ++i)
            array[i] = deq.array[i];
        return *this;
    }

    void clear(){
        size_ = 0;
        front_ = 0;
        back_ = 0;
        capacity_ = MIN_CAPACITY;
        delete []array;
        array = new value[capacity_];
    }

    void push_front(crvalue elem){
        ++size_;
        front_ += capacity_ - 1;
        front_ %= capacity_;
        array[front_] = elem;
        reallocate();
    }
    void push_back(crvalue elem){
        ++size_;
        array[back_] = elem;
        ++back_;
        back_ %= capacity_;
        reallocate();
    }
    void pop_front(){
        if (empty())
            return;
        --size_;
        ++front_;
        front_ %= capacity_;
        reallocate();
    }
    void pop_back(){
        if (empty())
            return;
        --size_;
        back_ += capacity_ - 1;
        back_ %= capacity_;
        reallocate();
    }

    value &front(){
        return array[front_];
    }
    value &back(){
        return array[(back_ + capacity_ - 1) % capacity_];
    }

    crvalue front() const{
        return array[front_];
    }
    crvalue back() const{
        return array[(back_ + capacity_ - 1) % capacity_];
    }

    size_t capacity() const{
        return capacity_;
    }
    size_t size() const{
        return size_;
    }

    bool empty() const{
        return !size_;
    }
    value &operator[](ssize_t i){
        return array[(ssize_t(front_) + i % ssize_t(capacity_)) % capacity_];
    }

    crvalue operator[](ssize_t i) const{
        return array[(ssize_t(front_) + i % ssize_t(capacity_)) % capacity_];
    }
    iterator begin(){
        return iterator(this, front_);
    }
    iterator end(){
        return iterator(this, back_);
    }

    const_iterator cbegin() const {
        return const_iterator(this, front_);
    }
    const_iterator cend() const {
        return const_iterator(this, back_);
    }
    const_iterator begin() const {
        return cbegin();
    }
    const_iterator end() const {
        return cend();
    }
    reverse_iterator rbegin(){
        return reverse_iterator(end());
    }
    reverse_iterator rend(){
        return reverse_iterator(begin());
    }
    const_reverse_iterator crbegin() const{
        return const_reverse_iterator(end());
    }
    const_reverse_iterator crend() const{
        return const_reverse_iterator(begin());
    }
    const_reverse_iterator rbegin() const{
        return crbegin();
    }
    const_reverse_iterator rend() const{
        return crend();
    }

    ~Deque(){
        delete []array;
    }
};


template <typename value_type, typename pointer, typename reference>
struct DequeIterator : public std::iterator <std::random_access_iterator_tag, value_type, ssize_t, pointer, reference> {
    typedef const Deque <value_type> DequeType;
    typedef ssize_t difference_type;
    typedef DequeIterator <value_type, pointer, reference> DequeIteratorType;
    typedef const DequeIteratorType& crefDequeIteratorType;
    DequeType *deq;
    ssize_t index;
public:
    DequeIterator() : deq(nullptr), index(0){}

    DequeIterator(DequeType *deq, size_t index) : deq(deq), index(index){}

    DequeIterator(crefDequeIteratorType deqIt) : deq(deqIt.deq), index(deqIt.index){}

    DequeIteratorType& operator=(crefDequeIteratorType deqIter){
        deq = deqIter.deq;
        index = deqIter.index;
        return *this;
    }

    DequeIteratorType& operator+=(ssize_t i){
        index += ssize_t(deq->capacity_) + i % ssize_t(deq->capacity_);
        index %= deq->capacity_;
        return *this;
    }

    DequeIteratorType& operator-=(ssize_t i){
        return *this += -i;
    }

    DequeIteratorType& operator++(){
        return *this += 1;
    }

    DequeIteratorType operator++(int){
        DequeIteratorType it(*this);
        *this += 1;
        return it;
    }

    DequeIteratorType& operator--(){
        return *this -= 1;
    }

    DequeIteratorType operator--(int){
        DequeIteratorType it(*this);
        *this -= 1;
        return it;
    }

    DequeIteratorType operator+(ssize_t i) const {
        return DequeIteratorType(*this) += i;
    }

    DequeIteratorType operator-(ssize_t i) const {
        return DequeIteratorType(*this) -= i;
    }

    difference_type operator-(crefDequeIteratorType deqIter) const {
        //if (this->deq != deqIter.deq) {throw();}
        ssize_t firstLength = (deq->capacity_ + index - deq->front_) % deq->capacity_;
        ssize_t secondLength = (deq->capacity_ + deqIter.index - deq->front_) % deq->capacity_;
        return firstLength - difference_type(secondLength);
    }

    bool operator<(crefDequeIteratorType deqIter) const {
        //if (this->deq != deqIter.deq) {throw();}
        return *this - deqIter < 0;
    }
    bool operator>(crefDequeIteratorType deqIter) const {
        //if (this->deq != deqIter.deq) {throw();}
        return *this - deqIter > 0;
    }
    bool operator==(crefDequeIteratorType deqIter) const {
        //if (this->deq != deqIter.deq) {throw();}
        return index == deqIter.index;
    }
    bool operator!=(crefDequeIteratorType deqIter) const{
        //if (this->deq != deqIter.deq) {throw();}
        return !(*this == deqIter);
    }
    bool operator>=(crefDequeIteratorType deqIter) const{
        //if (this->deq != deqIter.deq) {throw();}
        return !(*this < deqIter);
    }
    bool operator<=(crefDequeIteratorType deqIter) const{
        //if (this->deq != deqIter.deq) {throw();}
        return !(*this > deqIter);
    }

    reference operator*(){
        return const_cast<reference>(deq->array[index]);
    }
    pointer operator->(){
        return const_cast<pointer>(&(deq->array[index]));
    }
    reference operator[](ssize_t i){
        size_t _index = (ssize_t(index) + i % ssize_t(deq->capacity_)) % deq->capacity_;
        return const_cast<reference>(*DequeIterator(deq, _index));
    }
};

template <typename value_type, typename pointer, typename reference>
DequeIterator <value_type, pointer, reference>
operator+(ssize_t i, DequeIterator <value_type, pointer, reference> deqIter){
    return deqIter + i;
};

#endif //DEQUE_DEQUENEW_H
