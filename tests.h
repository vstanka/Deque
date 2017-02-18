#include <deque>
using std::deque;
#ifndef DEQUE_TESTS_H_H
#define DEQUE_TESTS_H_H


#include "deque.h"
#include "gtest/gtest.h"
#include <vector>
#include <string>
#include <climits>
#include <iostream>

using std::cout;
using std::endl;

using std::vector;

enum DequeOperation{
    PushBack,
    PushFront,
    PopBack,
    PopFront
};

DequeOperation randomPush(){
    return (rand() % 2 ? PushBack : PushFront);
}
DequeOperation randomPop(){
    return (rand() % 2 ? PopBack : PopFront);
}
DequeOperation randomOperation(){
    switch(rand() % 4){
        case 0:
            return PushBack;
        case 1:
            return PushFront;
        case 2:
            return PopBack;
        case 3:
            return PopFront;
    }
}





template <class value>
value random();

template <>
ssize_t random(){
    return ssize_t(rand() % ULLONG_MAX) - ULLONG_MAX / 2;
}
template <>
size_t random(){
    return rand() % ULLONG_MAX;
}

const size_t STRING_SIZE = 15;
const std::string SYMBOLS = "qwertyuiopasdfghjklzxcvbnm1234567890";
template <>
std::string random(){
    std::string s ="";
    for (size_t i = 0; i < STRING_SIZE; ++i){
        s += SYMBOLS[rand() % SYMBOLS.size()];
    }
    return s;
};

const size_t CHECK_SIZE = 1024;

template <class value>
class CheckerTest : public ::testing::Test{
protected:
    typedef std::pair<DequeOperation, value> operation;
    vector <operation> opers;
    deque <value> deq;
    Deque <value> Deq;

public:
    typename Deque<value>::iterator Diterator;
    CheckerTest(){}

    void check(){
        ASSERT_EQ(deq.size(), Deq.size()) << "Differnt Sizes";
        for (size_t i = 0; i < deq.size(); ++i)
            ASSERT_EQ(deq[i], Deq[i]) << "i = " << i;
    }
    void push_back(const value& t){
        deq.push_back(t);
        Deq.push_back(t);
        opers.push_back(operation(PushBack, t));
    }
    void push_front(const value& t){
        deq.push_front(t);
        Deq.push_front(t);
        opers.push_back(operation(PushFront, t));
    }
    void pop_back(){
        deq.pop_back();
        Deq.pop_back();
        opers.push_back(operation(PopBack, value()));
    }

    void changeValue(size_t i, const value& t){
        deq[i] = t;
        Deq[i] = t;
    }

    void pop_front(){
        deq.pop_front();
        Deq.pop_front();
        opers.push_back(operation(PopFront, value()));
    }

    void RandomPush(){
        value temp;
        if (randomPush() == PushBack)
            push_back(random<value>());
        else
            push_front(random<value>());
    }

    void RandomPop(){
        if (randomPop() == PopBack)
            pop_back();
        else
            pop_front();
    }

    void fillRandom(size_t size = CHECK_SIZE){
        for (size_t i = 0; i < size; ++i)
            RandomPush();
    }

    void RandomPushPop(size_t size = CHECK_SIZE){
        size_t countPush = rand() % size + size + 1;
        size_t countPop = countPush - size;
        for (size_t i = 0; i < countPop; ++i){
            --countPush;
            RandomPush();
        }
        while (countPop && countPush){
            if (rand() % 2){
                --countPush;
                RandomPush();
            }else{
                --countPop;
                RandomPop();
            }
        }
        while (countPop){
            --countPop;
            RandomPop();
        }
        while (countPush){
            --countPush;
            RandomPush();
        }
    }
    void clear(){
        deq.clear();
        Deq.clear();
    }

    virtual ~CheckerTest(){}
};


typedef ::testing::Types <size_t, ssize_t, std::string> Implemintations;

TYPED_TEST_CASE(CheckerTest, Implemintations);


// size() and operator[] test in  Checker::check();


TYPED_TEST(CheckerTest, FullSizelPushBackCheck){
    for (size_t i = 0; i < CHECK_SIZE; ++i)
        this->push_back(random<TypeParam>());
    this->check();
}
TYPED_TEST(CheckerTest, RandomSizePushBackCheck){
    for (size_t i = 0; i < rand() % (CHECK_SIZE - 10) + 10; ++i)
        this->push_back(random<TypeParam>());
    this->check();
}
TYPED_TEST(CheckerTest, FullSizePushFrontCheck){
    for (size_t i = 0; i < CHECK_SIZE; ++i)
        this->push_front(random<TypeParam>());
    this->check();
}
TYPED_TEST(CheckerTest, RandomSizePushFrontCheck){
    for (size_t i = 0; i < rand() % (CHECK_SIZE - 10) + 10; ++i)
        this->push_front(random<TypeParam>());
    this->check();
}
TYPED_TEST(CheckerTest, FullSizePushBackAndFrontCheck){
    this->fillRandom();
    this->check();
}
TYPED_TEST(CheckerTest, RandomSizePushBackAndFrontCheck){
    this->fillRandom((CHECK_SIZE - 10) + 10);
    this->check();
}

TYPED_TEST(CheckerTest, FullSizelPopBackCheck){
    this->fillRandom();
    for (size_t i = 0; i < CHECK_SIZE; ++i)
        this->pop_back();
    this->check();
}
TYPED_TEST(CheckerTest, RandomSizePopBackCheck){
    this->fillRandom();
    for (size_t i = 0; i < rand() % (CHECK_SIZE - 10) + 10; ++i)
        this->pop_back();
    this->check();
}
TYPED_TEST(CheckerTest, FullSizePopFrontCheck){
    this->fillRandom();
    for (size_t i = 0; i < CHECK_SIZE; ++i)
        this->pop_front();
    this->check();
}
TYPED_TEST(CheckerTest, RandomSizePopFrontCheck){
    this->fillRandom();
    for (size_t i = 0; i < rand() % (CHECK_SIZE - 10) + 10; ++i)
        this->pop_front();
    this->check();
}
TYPED_TEST(CheckerTest, FullSizePopBackAndFrontCheck){
    this->fillRandom();
    for (size_t i = 0; i < CHECK_SIZE; ++i)
        this->RandomPop();
    this->check();
}
TYPED_TEST(CheckerTest, RandomSizePopBackAndFrontCheck){
    size_t temp = 10 + rand() % (CHECK_SIZE - 10);
    this->fillRandom(temp);
    for (size_t i = 0; i < temp; ++i)
        this->RandomPop();
    this->check();
}

TYPED_TEST(CheckerTest, FrontBack){
    this->RandomPushPop();
    for (size_t i = 0; i < this->deq.size(); ++i){
        ASSERT_EQ(this->deq.front(), this->Deq.front()) << "Fronts not equals";
        ASSERT_EQ(this->deq.back(), this->Deq.back()) << "Back not equals";
        this->RandomPop();
    }
}

TYPED_TEST(CheckerTest, NonConstSquarBraces){
    this->RandomPushPop();
    for (size_t i = 0; i < this->deq.size() / 10; ++i){
        this->changeValue(rand() % this->deq.size(), random<TypeParam>());
    }
    this->check();
}

TYPED_TEST(CheckerTest, BeginEnd){
    this->RandomPushPop();
    for (size_t i = 0; i < this->deq.size(); ++i){
        ASSERT_EQ(this->deq.rbegin() - this->deq.rend(), this->Deq.rbegin() - this->Deq.rend());
        ASSERT_EQ(this->deq.rend() - this->deq.rbegin(), this->Deq.rend() - this->Deq.rbegin());
        ASSERT_EQ(this->deq.begin() - this->deq.end(), this->Deq.begin() - this->Deq.end());
        ASSERT_EQ(this->deq.end() - this->deq.begin(), this->Deq.end() - this->Deq.begin());
        this->RandomPop();
    }
}




#endif //DEQUE_TESTS_H_H
