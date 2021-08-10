#include "Account.h"


//Account C'tor
Account::Account(int account, int password, int balance) {
	_NumAccount = account;
	_password = password;
	_balance = balance;
	reader_counter = 0;
	pthread_mutex_init(&read_lock, NULL);
	pthread_mutex_unlock(&read_lock);
	pthread_mutex_init(&write_lock, NULL);
	pthread_mutex_unlock(&write_lock);
}

//Account D'tor
Account::~Account() {
	pthread_mutex_destroy(&read_lock);
	pthread_mutex_destroy(&write_lock);
}
////////////////////////////////////////////////////// METHODS /////////////////////////////////////////////////////////
int Account::getNumAccount() {
	return _NumAccount;
}
int Account::getBalance() {
	return _balance;
}
int Account::getPassword() {
    return _password;
}
//deposit safely
void Account::deposit(int amount, bool locker) {
	if (locker){
		pthread_mutex_lock(&write_lock);
	}
	_balance += amount;
	if (locker)
		pthread_mutex_unlock(&write_lock);
}
//withdraw safely
bool Account::withdraw(int amount, bool locker) {
	if (locker){
		pthread_mutex_lock(&write_lock);
	}
	if (_balance < amount)
		return false;
	_balance -= amount;
	if (locker)
		pthread_mutex_unlock(&write_lock);
	return true;
}
// lock read mutex
void Account::lockread() {
	pthread_mutex_lock(&read_lock);
	reader_counter++;
	if (reader_counter == 1)
		pthread_mutex_lock(&write_lock);
	pthread_mutex_unlock(&read_lock);
}
// unlock read mutex
void Account::unlockread() {
	pthread_mutex_lock(&read_lock);
	reader_counter--;
	if (reader_counter == 0)
		pthread_mutex_unlock(&write_lock);
	pthread_mutex_unlock(&read_lock);
}

