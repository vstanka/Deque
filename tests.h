#ifndef DEQUE_TESTS_H_H
#define DEQUE_TESTS_H_H

#include "gtest/gtest.h"
#include "deque.h"


#include <algorithm>
#include <vector>
#include <queue>
#include <ctime>
#include <climits>

using std::vector;
using std::pair;

const int COUNT_ELEMENTS = 1 << 13;
const double timeO1 = 6 * 1e-06;

inline double currentTime() {
    return static_cast<double>(clock()) / CLOCKS_PER_SEC;
}

void fillVector(vector <size_t> &array){
    array.clear();
    for (size_t i = 0; i < COUNT_ELEMENTS; ++i)
        array.push_back(rand() % ULLONG_MAX);
}

void fillVectorDifference(vector <size_t> &array){
    array.clear();
    size_t temp;
    bool b;
    for (size_t i = 0; i < COUNT_ELEMENTS; ++i) {
        do {
            b = true;
            temp = rand() % ULLONG_MAX;
            for (size_t j = 0; (b) && (j < i); ++j)
                b = temp != array[j];
        } while (!b);
        array.push_back(temp);
    }
}

class Checker : public ::testing::Test{
    double alltime, t_time;
public:
    vector <size_t> array;
    Deque <size_t> *deq;
    Deque <size_t> *deq2;
    void timeOperationStart(){
        t_time = currentTime();
    }
    void timeOperationEnd(){
        alltime += currentTime() - t_time;
    }

    double getAlltimeOperations(){
        return alltime;
    }

    void fillDeque(const vector <size_t> &array){
        for (size_t i = 0; i < array.size(); ++i)
            deq->push_back(array[i]);
    }
    void fillDeque(size_t count = COUNT_ELEMENTS){
        for (size_t i = 0; i < count; ++i)
            deq->push_back(rand() % ULLONG_MAX);
    }
    void randomPushAndPop(){
        size_t countOperations = rand() % 401  + 100;
        size_t frontOrBack; //1 - front, 0 - back
        size_t pushOrPop; //1 - push, 0 - pop
        for (size_t i = 0, value; i < countOperations; ++i){
            pushOrPop = (deq->size() < 100 ? 1 : (deq->size() == COUNT_ELEMENTS ? 0 : rand() % 2));
            frontOrBack = rand() % 2;
            size_t temp = (pushOrPop << 1) + frontOrBack;
            switch (temp){
                case 0:
                    array.pop_back();
                    deq->pop_back();
                    break;
                case 1:
                    array.erase(array.begin(), array.begin() + 1);
                    deq->pop_front();
                    break;
                case 2:
                    value = rand() % ULLONG_MAX;
                    array.push_back(value);
                    deq->push_back(value);
                    break;
                case 3:
                    value = rand() % ULLONG_MAX;
                    array.insert(array.begin(), value);
                    deq->push_front(value);
                    break;
                default:
                    ASSERT_TRUE(true) << "bad code this";
                    break;
            }
        }
        ASSERT_EQ(deq->size(), array.size()) << "bad sizes";
    }

protected:
    void SetUp(){
        alltime = t_time = 0;
        deq = new Deque <size_t>();
    }
    void TearDown(){
        delete deq;
    }
};

class PairChecker : public ::testing::Test{
    double alltime, t_time;
public:
    Deque<std::pair<size_t, size_t>> *deq;
    void timeOperationStart(){
        t_time = currentTime();
    }
    void timeOperationEnd(){
        alltime += currentTime() - t_time;
    }

    double getAlltimeOperations(){
        return alltime;
    }

    void fillDeque(){
        for (size_t i = 0; i < COUNT_ELEMENTS; ++i)
            deq->push_back({rand() % ULLONG_MAX, rand() % ULLONG_MAX});
    }
protected:
    void SetUp(){
        alltime = t_time = 0;
        deq = new Deque <std::pair<size_t, size_t>>();
    }
    void TearDown(){
        delete deq;
    }
};



//TEST_F(Checker, FrontPushFront){
//    size_t temp;
//    for (size_t i = 0; i < COUNT_ELEMENTS; ++i){
//        temp = rand() % ULLONG_MAX;
//        timeOperationStart();
//        deq->push_front(temp);
//        timeOperationEnd();
//        ASSERT_EQ(temp, deq->front());
//    }
//    ASSERT_EQ(deq->size(), COUNT_ELEMENTS);
//    ASSERT_LE(getAlltimeOperations() / COUNT_ELEMENTS, timeO1);
//}
//TEST_F(Checker, BackPushBack){
//    size_t temp;
//    for (size_t i = 0; i < COUNT_ELEMENTS; ++i){
//        temp = rand() % ULLONG_MAX;
//        timeOperationStart();
//        deq->push_back(temp);
//        timeOperationEnd();
//        ASSERT_EQ(temp, deq->back());
//    }
//    ASSERT_EQ(deq->size(), COUNT_ELEMENTS);
//    ASSERT_LE(getAlltimeOperations() / COUNT_ELEMENTS, timeO1);
//}
//TEST_F(Checker, PopFront){
//    fillDeque();
//    for (size_t i = 0; i < COUNT_ELEMENTS; ++i){
//        timeOperationStart();
//        deq->pop_front();
//        timeOperationEnd();
//    }
//    ASSERT_EQ(deq->size(), 0);
//    ASSERT_LE(getAlltimeOperations() / COUNT_ELEMENTS, timeO1);
//}
//TEST_F(Checker, PopBack){
//    fillDeque();
//    for (size_t i = 0; i < COUNT_ELEMENTS; ++i){
//        timeOperationStart();
//        deq->pop_back();
//        timeOperationEnd();
//    }
//    ASSERT_EQ(deq->size(), 0);
//    ASSERT_LE(getAlltimeOperations() / COUNT_ELEMENTS, timeO1);
//}
//
//TEST_F(Checker, PushAndPopFrontAndBack){
//    size_t frontOrBack; //1 - front, 0 - back
//    size_t pushOrPop; //1 - push, 0 - pop
//    for (size_t i = 0; i < COUNT_ELEMENTS; ++i){
//        pushOrPop = (!deq->size() ? 1 : (deq->size() == COUNT_ELEMENTS ? 0 : rand() % 2));
//        frontOrBack = rand() % 2;
//        size_t temp = (pushOrPop << 1) + frontOrBack;
//        switch (temp){
//            case 0:
//                timeOperationStart();
//                deq->pop_back();
//                timeOperationEnd();
//                break;
//            case 1:
//                timeOperationStart();
//                deq->pop_front();
//                timeOperationEnd();
//                break;
//            case 2:
//                timeOperationStart();
//                deq->push_back(rand() % ULLONG_MAX);
//                timeOperationEnd();
//                break;
//            case 3:
//                timeOperationStart();
//                deq->push_front(rand() % ULLONG_MAX);
//                timeOperationEnd();
//                break;
//            default:
//                ASSERT_TRUE(true);
//                break;
//        }
//    }
//    ASSERT_LE(getAlltimeOperations() /COUNT_ELEMENTS, timeO1);
//}
TEST_F(Checker, ConstOperatorSquareBraces){
    fillVectorDifference(array);
    fillDeque(array);
    randomPushAndPop();
    size_t temp;
    for (size_t i = 0; i < deq->size(); ++i) {
        timeOperationStart();
        (*deq)[i];
        timeOperationEnd();
        ASSERT_EQ(array[i], (*deq)[i]) << "bad values";
    }
    ASSERT_LE(getAlltimeOperations() / deq->size(), timeO1);
}
TEST_F(Checker, OperatorSquareBraces){
    randomPushAndPop();
    size_t countAssigment = rand() % 401 + 100;
    for (size_t i = 0, value, index; i < countAssigment; ++i){
        index = rand() % array.size();
        value = rand() % ULLONG_MAX;
        array[index] = value;
        timeOperationStart();
        (*deq)[index] = value;
        timeOperationEnd();
        ASSERT_EQ(array[index], (*deq)[index]);
    }
    ASSERT_LE(getAlltimeOperations() / countAssigment, timeO1);
}


TEST_F(Checker, DequeIteratorBegin){
    fillVectorDifference(array);
    fillDeque(array);
    Deque<size_t>::iterator it;
    for (size_t i = 0; i < array.size(); ++i){
        timeOperationStart();
        it = deq->begin();
        timeOperationEnd();
        ASSERT_EQ(*it, array[i]);
        deq->pop_front();
    }
    ASSERT_LE(getAlltimeOperations() / array.size(), timeO1);
}

TEST_F(Checker, DequeIteratorEquival){
    fillVectorDifference(array);
    fillDeque(array);
    Deque<size_t>::iterator it;
    bool resultRelation;
    for (size_t i = 0; i < array.size(); ++i){
        it = deq->begin();
        timeOperationStart();
        resultRelation = it == deq->end() - deq->size();
        timeOperationEnd();
        deq->pop_front();
        ASSERT_TRUE(resultRelation);
    }
}

TEST_F(Checker, DequeIteratorLoop){
    fillDeque();
    Deque<size_t>::iterator tempFPost, FrontPostfix, tempFPre,
            FrontPrefix = FrontPostfix = deq->begin();
    Deque<size_t>::iterator tempBPost, BackPostfix, tempBPre,
            BackPrefix = BackPostfix = deq->end();
    for (size_t i = 0; i < deq->size(); ++i){
        timeOperationStart();
        tempFPre = ++FrontPrefix;
        tempFPost = FrontPostfix++;
        tempBPost = BackPostfix--;
        tempBPre = --BackPrefix;
        timeOperationEnd();
        ASSERT_NE(tempFPre, tempFPost);
        ASSERT_NE(tempBPost, tempBPre);
    }
    ASSERT_EQ(FrontPostfix, deq->end());
    ASSERT_EQ(FrontPrefix, deq->end());
    ASSERT_EQ(BackPostfix, deq->begin());
    ASSERT_EQ(BackPrefix, deq->begin());
    ASSERT_LE(getAlltimeOperations() / (deq->size() * 4), timeO1);
}
TEST_F(Checker, DequeIteratorPlus){
    fillVectorDifference(array);
    fillDeque(array);
    Deque<size_t>::iterator NumberIterator, IteratorNumber, it;
    for (size_t i = 0; i <= array.size(); ++i){
        timeOperationStart();
        NumberIterator = i + deq->begin();
        IteratorNumber = deq->begin() + i;
        timeOperationEnd();
        it = deq->begin();
        for (size_t j = 0; j < i; ++j)
            ++it;
        ASSERT_EQ(NumberIterator, IteratorNumber);
        ASSERT_EQ(IteratorNumber, it);
    }
    ASSERT_LE(getAlltimeOperations() / (array.size() + 1) / 2, timeO1);
}

TEST_F(Checker, DequeIteratorMinusIterator){
    fillDeque();
    Deque<size_t>::iterator it = deq->begin();
    ssize_t difference1, difference2, difference3, difference4;
    for (ssize_t i = 0; i < deq->size(); ++i, ++it){
        timeOperationStart();
        difference1 = it - deq->begin();
        difference2 = deq->begin() - it;
        difference3 = it - deq->end();
        difference4 = deq->end() - it;
        timeOperationEnd();
        ASSERT_EQ(difference1, i);
        ASSERT_EQ(difference2, -i);
        ASSERT_EQ(difference3, i - ssize_t(deq->size()));
        ASSERT_EQ(difference4, deq->size() - i);
    }
    ASSERT_LE(getAlltimeOperations() / (deq->size() * 4), timeO1);
}
TEST_F(Checker, DequeIteratorMinusNumber){
    fillDeque();
    Deque<size_t>::iterator IteratorNumber, it;
    for (size_t i = 0; i <= deq->size(); ++i){
        timeOperationStart();
        IteratorNumber = deq->end() - i;
        timeOperationEnd();
        ASSERT_EQ(IteratorNumber, deq->begin() + deq->size() - i);
    }
    ASSERT_LE(getAlltimeOperations() / (deq->size() + 1) / 2, timeO1);
}

TEST_F(Checker, DequeIteratorMinusPlusEqually){
    fillDeque();
    for(size_t i = 0; i < deq->size(); ++i){
        Deque<size_t>::iterator itBegin, itEnd;
        itBegin = deq->begin();
        itEnd = deq->end();
        timeOperationStart();
        itBegin += i;
        itEnd -= i;
        timeOperationEnd();
        ASSERT_EQ(itBegin, deq->begin() + i);
        ASSERT_EQ(itEnd, deq->end() - i);
    }
    ASSERT_LE(getAlltimeOperations() / (deq->size() * 2), timeO1);
}

TEST_F(Checker, Size){
    size_t size_ = rand()% (COUNT_ELEMENTS / 2 + 1) + COUNT_ELEMENTS / 2;
    fillDeque(size_);
    timeOperationStart();
    deq->size();
    timeOperationEnd();
    ASSERT_EQ(size_, deq->size());
    ASSERT_LE(getAlltimeOperations(), timeO1);
}

//TEST_F(Checker, DequeIteratorRelations){
//    fillDeque();
//    bool GT, GE, LT, LE;
//    Deque<size_t>::iterator it = deq->begin(), temp1, temp2;
//    for (size_t i = 0; i < deq->size(); ++i){
//        for (size_t j = i; j < deq->size(); ++j){
//            temp1 = it + i;
//            temp2 = it + j;
//            timeOperationStart();
//            GT = temp1 > temp2;
//            LT = temp1 < temp2;
//            GE = temp1 >= temp2;
//            LE = temp1 <= temp2;
//            timeOperationEnd();
//            ASSERT_EQ(GT, i > j);
//            ASSERT_EQ(LT, i < j);
//            ASSERT_EQ(GE, i >= j);
//            ASSERT_EQ(LE, i <= j);
//        }
//    }
//    ASSERT_LE(getAlltimeOperations() / (deq->size() * (deq->size() + 1) / 2), timeO1);
//}

TEST_F(Checker, DequeIteratorDereference){
    fillVector(array);
    fillDeque(array);
    Deque<size_t>::iterator it = deq->begin();
    for (size_t i = 0; i < deq->size(); ++i, ++it){
        timeOperationStart();
        *it;
        timeOperationEnd();
        ASSERT_EQ(*it, array[i]);
    }
    ASSERT_LE(getAlltimeOperations() / deq->size(), timeO1);
}

TEST_F(Checker, DequeIteratorOperatorSquareBraces){
    fillVector(array);
    fillDeque(array);
    randomPushAndPop();
    Deque<size_t>::iterator it = deq->begin();
    for (size_t i = 0; i < deq->size(); ++i){
        timeOperationStart();
        it[i];
        timeOperationEnd();
        ASSERT_EQ(it[i], array[i]);
    }
    Deque<size_t>::reverse_iterator rit = deq->rbegin();
    for (size_t i = 0; i < deq->size(); ++i){
        timeOperationStart();
        rit[i];
        timeOperationEnd();
        ASSERT_EQ(rit[i], array[array.size() - 1 - i]);
    }
    ASSERT_LE(getAlltimeOperations() / deq->size() * 0.5, timeO1);
}

TEST_F(PairChecker, DequeIteratorRefDereference){
    fillDeque();
    Deque<std::pair<size_t, size_t>>::iterator it = deq->begin();
    for (size_t i = 0; i < deq->size(); ++i, ++it){
        timeOperationStart();
        it->first;
        timeOperationEnd();
        ASSERT_EQ(it->first, it[0].first);
        ASSERT_EQ(it->second, it[0].second);
    }
    ASSERT_LE(getAlltimeOperations() / deq->size(), timeO1);
}







#endif //DEQUE_TESTS_H_H
