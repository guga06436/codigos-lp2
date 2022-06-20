#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <thread>
#include <mutex>

#define ITERATIONS 9999

class ContaCorrente
{
    int idCliente;
    double saldo;
    std::mutex m;

public:
    ContaCorrente(int i, double f)
    {
        idCliente = i;
        saldo = f;
    }

    double getSaldo()
    {
        return saldo;
    }

    void addSaldo(double f)
    {
        m.lock();
        saldo += f;
        m.unlock();
    }
};

ContaCorrente cc1(1, 1000.0);

void operacoes1()
{
    int counter = 0;
    while (counter < ITERATIONS + 1)
    {
        cc1.addSaldo(100.0);
        printf("[c] %lf\n", cc1.getSaldo());
        counter++;
    }
}

void operacoes2()
{
    int counter = 0;
    while (counter < ITERATIONS)
    {
        cc1.addSaldo(-100.0);
        printf("[d] %lf\n", cc1.getSaldo());
        counter++;
    }
}

int main(void)
{
    std::thread t1(operacoes1), t2(operacoes2), t3(operacoes1), t4(operacoes2);

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    printf("Saldo final da conta: %f\n", cc1.getSaldo());

    return 0;
}