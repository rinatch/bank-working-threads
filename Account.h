#ifndef ACCOUNT_H_
#define ACCOUNT_H_
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <pthread.h>
#include <unistd.h>


using namespace std;
class Account
{
private:
    int _balance;
    int _NumAccount;
    int _password;
    int reader_counter;
    pthread_mutex_t read_lock;
public:
	Account(int account, int password, int balance);
	~Account();
	/**
	 *
	 * @return number of account (int)
	 */
	int getNumAccount();
	/**
	 *
	 * @return password of the account (int)
	 */
	int getPassword();
	/**
	 *
	 * @return balance of the account (int)
	 */
	int getBalance();
	/**
	 * deposits from the this (the account) money (amount)
	 * @param amount
	 * @param locker
	 */
	void deposit(int amount, bool locker);
	/**
	 * withdraws from the this (the account) money (amount)
	 * @param amount
	 * @param locker
	 * @return
	 */
	bool withdraw(int amount, bool locker);
	/**
	 * locks for reading
	 */
	void lockread();
	/**
	 * unlocks after we finish reading
	 */
	void unlockread();
	pthread_mutex_t write_lock;
};

#endif
