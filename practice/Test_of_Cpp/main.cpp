#include <iostream>
#include <unordered_map>
#include <windows.h>
#include <Psapi.h>

//class Base
//{
//public:
//int t = 1;
//};
//class A : public virtual Base
//{
//};
//class B : public virtual Base
//{
//private:
//int x[100];
//};
//class C : public virtual Base
//{
//};
//class myTop : public A, public B, C
//{
//};
//union tu
//{
//int x;
//char y;
//};

//class cu
//{
//public:
//union
//{
//int a;
//char b;
//};
//int &geta() { return a; }
//};

class onefunction
{
private:
    int x = 0;

public:
    void add() { x++; }
};

class twofunction
{
private:
    int x;

public:
    twofunction() { this->x = 0; }
    twofunction(int x) { this->x = x; }
    void add() { this->x++; }
    void minus1() { this->x = this->x + 1; }
    void minus2() { this->x = this->x + 2; }
    void minus3() { this->x = this->x + 3; }
    void minus4() { this->x = this->x + 4; }
    void minus5() { this->x = this->x + 5; }
    void minus6() { this->x = this->x + 6; }
    void minus7() { this->x = this->x + 8; }
    void minus8() { this->x = this->x + 7; }
    void minus9() { this->x = this->x + 9; }
    void minus10() { this->x = this->x + 10; }
    void minus11() { this->x = this->x + 11; }
    void tminus1() { this->x = this->x + 12; }
    void tminus2() { this->x = this->x + 13; }
    void tminus3() { this->x = this->x + 14; }
    void tminus4() { this->x = this->x + 15; }
    void tminus5() { this->x = this->x + 111; }
    void tminus6() { this->x = this->x + 112; }
    void tminus7() { this->x = this->x + 1123; }
    void tminus8() { this->x = this->x + 1134; }
    void tminus9() { this->x = this->x + 1142; }
    void tminus10() { this->x = this->x + 1124; }
    void tminus11() { this->x = this->x + 1124; }
    void ttminus1() { this->x = this->x + 11245; }
    void ttminus2() { this->x = this->x + 1125; }
    void ttminus3() { this->x = this->x + 1251; }
    void ttminus4() { this->x = this->x + 1231; }
    void ttminus5() { this->x = this->x + 12341; }
    void ttminus6() { this->x = this->x + 151; }
    void ttminus7() { this->x = this->x + 221; }
    void ttminus8() { this->x = this->x + 142; }
    void ttminus9() { this->x = this->x + 142; }
    void ttminus10() { this->x = this->x + 121; }
    void ttminus11() { this->x = this->x + 1412; }
    int get() { return x; }
};

int main(int argc, char **argv)
{
    PROCESS_MEMORY_COUNTERS memCounter;
    BOOL result = K32GetProcessMemoryInfo(GetCurrentProcess(),
                                          &memCounter,
                                          sizeof(memCounter));
    float MB0 = (float)memCounter.WorkingSetSize / 1000000.f;

    //    bool t = false;
    //if (t)
    //{
    //myTop t;
    //Base base;
    //A a;
    //B b;
    //std::cout << "base size: " << sizeof(base) << "\nA size: " << sizeof(a) << "\nB size: " << sizeof(b) << "\nTop size: " << sizeof(t)
    //<< "\n t=" << t.t << std::endl;
    //tu mtu;
    //mtu.y = 1;
    //std::cout << "size is " << sizeof(mtu.y);
    //cu mcu;
    //int &x = mcu.geta();
    //x = 5;
    //std::cout << "should be 5: " << mcu.a << std::endl;
    //return 0;
    //}
    //bool test_size = true;
    //if (test_size)
    //{
    //std::unordered_map<int, int> empty;
    //std::unordered_map<int, int> someint;
    //for (size_t i = 0; i < 100; i++)
    //{
    //someint[i] = i;
    //}
    //std::cout
    //<< "size of empty: " << sizeof(empty)
    //<< "size of 10: " << sizeof(someint)
    //<< std::endl;
    //onefunction one;
    //twofunction two;
    //std::cout
    //<< "size of one function class: " << sizeof(one)
    //<< "size of two function class: " << sizeof(two)
    //<< std::endl;
    //}

    twofunction two;
    two.add();
    twofunction **twos = new twofunction *[100000];
    for (size_t i = 0; i < 100000; i++)
    {
        twos[i] = new twofunction(100);
        twos[i]->minus10();
        twos[i]->minus8();
        twos[i]->minus1();
        twos[i]->minus3();
        twos[i]->minus6();
        //std::cout << twos[i]->get() << std::endl;
    }

    result = K32GetProcessMemoryInfo(GetCurrentProcess(),
                                     &memCounter,
                                     sizeof(memCounter));
    float MB1 = (float)memCounter.WorkingSetSize / 1000000.f;
    std::cout
        << "WorkingSetSize in MB1: "
        << MB1 - MB0 << std::endl;

    onefunction **ones = new onefunction *[100000];
    for (size_t i = 0; i < 100000; i++)
    {
        ones[i] = new onefunction;
        ones[i]->add();
    }
    result = K32GetProcessMemoryInfo(GetCurrentProcess(),
                                     &memCounter,
                                     sizeof(memCounter));
    float MB2 = (float)memCounter.WorkingSetSize / 1000000.f;
    std::cout
        << "WorkingSetSize in MB2: "
        << MB2 - MB1 << std::endl;

    return 0;
}