#ifndef ATM_H_
#define ATM_H_
#include <map>
#include <pthread.h>
#include "Bank.h"
#include <unistd.h>
#include <cstdio>
#include <string>

using namespace std;

void* atmTransactionsFunc(void* arg);

class ATM {

public:
	void handle_actions(string actions);
	int id;
	string atm_txt;
	Bank* bank;
};

#endif