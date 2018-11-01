#include <iostream>
#include "database.h"
using namespace std;

int main()
{
    Database db("data/employees.txt");
    db.print();
    return 0;
}

