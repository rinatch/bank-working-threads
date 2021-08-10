#ifndef BANK_H_
#define BANK_H_
#include <map>
#include <pthread.h>
#include "Log.h"
#include "Account.h"
#include <unistd.h>
#include <cstdio>
#include <string>

using namespace std;

void* ChargeCommissions(void* arg);
void* bankPrint(void* arg);

class Bank {
	map<int, Account> accounts;
	int read_count;
	int bank_money;
	pthread_mutex_t read_mutex_bank;
	pthread_mutex_t write_mutex_bank;
	Log& log_file;

public:
	Bank(Log* log_file);
	~Bank();
	/**
	 * creates new account
	 * @param id
	 * @param account
	 * @param password
	 * @param money
	 */
	void new_account(int id, int account, int password, int money);
	/**
	 * gets commissions
	 */
    void commissions();
    /**
     * prints all the details as requested in the pdf
     */
    void printBank();
    /**
     * locks read mutex
     */
    void lockAccountRead();
    /**
     * unlocks read mutex
     */
    void unlockAccountRead();
    /**
     * deposits money from account (int) with password (int)
     * @param id
     * @param account
     * @param password
     * @param amount
     */
	void deposit(int id, int account, int password, int amount);
	/**
	 * //withdraws money from account (int) with password (int)
	 * @param id
	 * @param account
	 * @param password
	 * @param amount
	 */
	void withdraw(int id, int account, int password, int amount);
	/**
	 * checks balance of account (int)
	 * @param id
	 * @param account
	 * @param password
	 */
	void checkBalance(int id, int account, int password);
	/**
	 * transfers money between two accounts
	 * @param id
	 * @param account
	 * @param password
	 * @param target
	 * @param amount
	 */
	void transfer(int id, int account, int password, int target, int amount);
};

#endif