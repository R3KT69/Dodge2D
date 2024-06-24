#include <random>
#include <algorithm>

using namespace std;

int random(int domain, int range)
{
    random_device rd;
    mt19937 gen(rd());

    uniform_real_distribution<> dis(domain, range);

    int x = dis(gen);



    return x;
}
