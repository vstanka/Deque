//https://github.com/vstanka/Deque.git

#ifndef DEQUE_DEQUE_H
#define DEQUE_DEQUE_H

#include <vector>
#include <iterator>

enum class ReallocType{
    INCREASE = 0,
    DECREASE = 1,
    STAY = 2
};

typedef unsigned long long ullint;
typedef long long int llint;

class Index{
    static const ullint MIN_CAPACITY = 2;
    ullint capacity_;
    ullint index;
public:
    Index(ullint capacity, llint index = 0) : capacity_(capacity + 1), index((capacity_ + index % llint(capacity_)) % capacity_){}
    Index& operator=(const Index& rIndex){
        this->capacity_ = rIndex.capacity_;
        this->index = rIndex.index;
        return *this;
    }
    explicit operator llint () const{
        return llint(index);
    }
    operator ullint() const{
        return index;
    }
    Index& operator+=(llint i){
        index = (index + capacity_ + i % llint(capacity_)) % capacity_;
        return *this;
    }
    Index& operator-=(llint i){
        index = (index + capacity_ - i % llint(capacity_)) % capacity_;
        return *this;
    }
    Index operator+(llint i) const {
        return (Index(*this)) += i;
    }
    Index operator-(llint i) const {
        return (Index(*this)) -= i;;
    }
    Index operator++(int){
        ullint temp = index;
        *this += 1;
        return Index(capacity_, temp);
    }
    Index& operator++(){
        return *this += 1;
    }
    Index operator--(int){
        ullint temp = index;
        *this -= 1;
        return Index(capacity_, temp);
    }
    Index& operator--(){
        return *this -= 1;
    }
    Index operator+(const Index& i) const {
        return (Index(*this)) += i.index;
    }
    Index operator-(const Index& i) const {
        return (Index(*this)) -= i.index;
    }

    bool operator==(const Index& rVI) const{
        return (rVI.capacity_ == this->capacity_) && (this->index == rVI.index);
    }
    bool operator!=(const Index& rVI) const{
        return !(operator==(rVI));
    }
    ~Index(){}
};

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

    llint difference(const Index& first, const Index& second) const {
        return llint(first - front_) - llint(second - front_);
    }

public:
    friend DequeIterator <value, value*, value&>;
    friend DequeIterator <value, const value*, crvalue&>;
    typedef DequeIterator <value, value *, value &> iterator;
    typedef DequeIterator <value, const value *, crvalue> const_iterator;
    typedef std::reverse_iterator <iterator> reverse_iterator;
    typedef std::reverse_iterator <const_iterator> const_reverse_iterator;


    Deque() : capacity_(MIN_CAPACITY), size_(0), array(new value[MIN_CAPACITY]), front_(MIN_CAPACITY, 0), back_(MIN_CAPACITY, 0){}

    Deque(const Deque &rdeq) : capacity_(rdeq.capacity_), size_(rdeq.size_), array(new value[capacity_]), front_(rdeq.front_), back_(rdeq.back_){
        for (ullint i = 0; i < capacity_; ++i)
            array[i] = rdeq.array[i];
    }

    Deque &operator=(const Deque &deq){
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
        ++size_;
    }
    void push_back(crvalue elem){

        reallocate();
        array[back_++] = elem;
        ++size_;
    }
    void pop_front(){
        reallocate();
        ++front_;
        --size_;
    }
    void pop_back(){
        reallocate();
        --back_;
        --size_;
    }

    value &front(){
        return array[front_];
    }
    value &back(){
        return array[back_ - 1ll];
    }

    crvalue front() const{
        return array[front_];
    }
    crvalue back() const{
        return array[back_ - 1ll];
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
    value &operator[](llint i){
        return array[front_ + i];
    }

    crvalue operator[](llint i) const{
        return array[front_ + i];
    }

    iterator begin(){
        return iterator(this, front_);
    }
    iterator end(){
        return iterator(this, back_);
    }

    const_iterator cbegin() const{
        return const_iterator(this, front_);
    }
    const_iterator cend() const{
        return const_iterator(this, back_);
    }
    const_iterator begin() const{
        return cbegin();
    }
    const_iterator end() const{
        return cend();
    }
    reverse_iterator rbegin(){
        return reverse_iterator(end());
    }
    reverse_iterator rend(){
        return reverse_iterator(begin());
    }
    const_reverse_iterator crbegin() const{
        return const_reverse_iterator(cend());
    }
    const_reverse_iterator crend() const{
        return const_reverse_iterator(cbegin());
    }
    const_reverse_iterator rbegin() const{
        return crbegin();
    }
    const_reverse_iterator rend() const{
        return crend();
    }

    ~Deque(){
        delete[] array;
    };
};

template <typename value_type, typename pointer, typename reference>
struct DequeIterator : public std::iterator <std::random_access_iterator_tag, value_type, ssize_t, pointer, reference> {
    typedef const Deque <value_type> DequeType;
    typedef ssize_t difference_type;
    typedef DequeIterator <value_type, pointer, reference> DequeIteratorType;
    typedef const DequeIteratorType &crefDequeIteratorType;
    DequeType *deque;
    Index index;
public:
    DequeIterator() : deque(nullptr), index(0){}

    DequeIterator(DequeType *deque, ullint index) : deque(deque), index(deque->capacity(), index){}

    DequeIterator(crefDequeIteratorType deqIter) : deque(deqIter.deque), index(deqIter.index){}

    DequeIteratorType &operator=(crefDequeIteratorType deqIter){
        this->deque = deqIter.deque;
        this->index = deqIter.index;
        return *this;
    }

    DequeIteratorType& operator+=(llint i){
        index += i;
        return *this;
    }

    DequeIteratorType& operator-=(llint i){
        index -= i;
        return *this;
    }

    DequeIteratorType& operator++(){
        return *this += 1;
    }

    DequeIteratorType operator++(int){
        DequeIteratorType temp(*this);
        *this += 1;
        return temp;
    }

    DequeIteratorType& operator--(){
        return *this -= 1;
    }

    DequeIteratorType operator--(int){
        DequeIteratorType temp(*this);
        *this -= 1;
        return temp;
    }

    DequeIteratorType operator+(long long i) const{
        return DequeIteratorType(*this) += i;
    }

    DequeIteratorType operator-(long long i) const{
        return DequeIteratorType(*this) -= i;
    }

    difference_type operator-(crefDequeIteratorType deqIter) const{
        return static_cast <difference_type>(deque->difference(this->index, deqIter.index));
    }

    bool operator<(crefDequeIteratorType deqIter) const{
        return (this->deque == deqIter.deque) && (deque->difference(this->index, deqIter.index) < 0);
    }
    bool operator>(crefDequeIteratorType deqIter) const{
        return (this->deque == deqIter.deque) && (deque->difference(this->index, deqIter.index) > 0);
    }
    bool operator==(crefDequeIteratorType deqIter) const{
        return (this->deque == deqIter.deque) && (this->index == deqIter.index);
    }
    bool operator!=(crefDequeIteratorType deqIter) const{
        return !(operator==(deqIter));
    }
    bool operator>=(crefDequeIteratorType deqIter) const{
        return (this->deque == deqIter.deque) && (deque->difference(this->index, deqIter.index) >= 0);;
    }
    bool operator<=(crefDequeIteratorType deqIter) const{
        return (this->deque == deqIter.deque) && (deque->difference(index, deqIter.index) <= 0);
    }

    reference operator*(){
        return const_cast<reference>(deque->array[index]);
    }
    pointer operator->(){
        return const_cast<pointer>(&(deque->array[index]));
    }
    reference operator[](llint i){
        return const_cast<reference>(deque->array[index + i]);
    }
};

template <typename value_type, typename pointer, typename reference>
DequeIterator <value_type, pointer, reference>
operator+(ullint i, DequeIterator <value_type, pointer, reference> deqIter){
    return deqIter + i;
}

#endif //DEQUE_DEQUE







































