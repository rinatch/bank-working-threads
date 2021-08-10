#ifndef LOG_H_
#define LOG_H_

#include <stdlib.h>
#include <iostream>
#include <pthread.h>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>

using namespace std;

class Log {
public:
	Log(const char* logfile);
	Log();
	~Log();
	void newAccountLog(int atmID, int account, int password, int money);
	void depositLog(int atmID, int account, int amount, int balance);
	void withdrawLog(int atmID, int account, int amount, int balance);
	void checkBalanceLog(int atmID, int account, int balance);
	void transferLog(int atmID, int account, int target_account,
			int amount, int account_bal, int target_bal);
	void commissionLog(int account, double percent, int amount);
	
	void error_noAccount(int atmID, int account);
	void error_lowBalance(int atmID, int account, int amount);
	void error_accountExist(int atmID, int account);
	void error_wrongPassword(int atmID, int account);

private:
	pthread_mutex_t mutex;
	ofstream log_file;
};
#endif
