#include "Bank.h"
extern bool flag;

//Bank C'tor
Bank::Bank(Log* log_file) : log_file(*log_file) {
	read_count = 0;
	pthread_mutex_init(&read_mutex_bank, NULL);
	pthread_mutex_init(&write_mutex_bank, NULL);
	pthread_mutex_unlock(&read_mutex_bank);
	pthread_mutex_unlock(&write_mutex_bank);
	bank_money = 0;
}

//Bank D'tor
Bank::~Bank() {
	pthread_mutex_destroy(&read_mutex_bank);
	pthread_mutex_destroy(&write_mutex_bank);
}
////////////////////////////////////////////////////// METHODS /////////////////////////////////////////////////////////
//lock read mutex
void Bank::lockAccountRead() {
	pthread_mutex_lock(&read_mutex_bank);
	read_count++;
	if (read_count == 1) {
		pthread_mutex_lock(&write_mutex_bank);
	}
	pthread_mutex_unlock(&read_mutex_bank);
}
//create new account
void Bank::new_account(int id, int account, int password, int money) {
	pthread_mutex_lock(&write_mutex_bank);
    map<int, Account>::iterator  it = accounts.find(account);
	if (it != accounts.end()) {	//account already exist
		log_file.error_accountExist(id, account);
		pthread_mutex_unlock(&write_mutex_bank);
		return;
	}
	//OK let's create the account
	sleep(1);
	Account new_account(account, password, money);
	accounts.insert(pair<int, Account>(account, new_account));
	//write a report about it
	log_file.newAccountLog(id, account, password, money);
	pthread_mutex_unlock(&write_mutex_bank);
}
void Bank::deposit(int id, int account, int password, int amount) {
	lockAccountRead();
    map<int, Account>::iterator  it = accounts.find(account);
	if (it == accounts.end()) {	//cannot find account
		log_file.error_noAccount(id, account);
		unlockAccountRead();
		return;
	}
	if ((*it).second.getPassword() != password) {	//wrong password
		log_file.error_wrongPassword(id, account);
		unlockAccountRead();
		return;
	}
	sleep(1);
	(*it).second.deposit(amount, true);
	log_file.depositLog(id, account, amount, (*it).second.getBalance());
	unlockAccountRead();
	return;

}

//unlock read mutex, same as lecs\tuts
void Bank::unlockAccountRead() {
    pthread_mutex_lock(&read_mutex_bank);
    read_count--;
    if (read_count == 0) {
        pthread_mutex_unlock(&write_mutex_bank);
    }
    pthread_mutex_unlock(&read_mutex_bank);
}
void Bank::withdraw(int id, int account, int password, int amount) {
	lockAccountRead();
    map<int, Account>::iterator  it = accounts.find(account);
	if (it == accounts.end()) {	//cannot find account
		log_file.error_noAccount(id, account);
		unlockAccountRead();
		return;
	}

	if ((*it).second.getPassword() != password) {	//wrong password
		log_file.error_wrongPassword(id, account);
		unlockAccountRead();
		return;
	}
	if ((*it).second.getBalance() < amount) {	//no money
		log_file.error_lowBalance(id, account, amount);
		unlockAccountRead();
		return;
	}
	sleep(1);
	(*it).second.withdraw(amount, true);
	log_file.withdrawLog(id, account, amount,
			(*it).second.getBalance());
	unlockAccountRead();
}

//checks the balance
void Bank::checkBalance(int id, int account, int password) {
	lockAccountRead();
    map<int, Account>::iterator  it = accounts.find(account);
	if (it == accounts.end()) {	//cant find account
		log_file.error_noAccount(id, account);
		unlockAccountRead();
		return;
	}
	if ((*it).second.getPassword() != password) {	//wrong password
		log_file.error_wrongPassword(id, account);
		unlockAccountRead();
		return;
	}
	sleep(1);
	log_file.checkBalanceLog(id, account, (*it).second.getBalance());
	unlockAccountRead();

}

//transfer money
void Bank::transfer(int id, int account, int password, int target, int amount) {
	if (account==target)
		return;
	lockAccountRead();
    map<int, Account>::iterator  it1 = accounts.find(account);
	if (it1 == accounts.end()) {	//account not found
		log_file.error_noAccount(id, account);
		unlockAccountRead();
		return;
	}
    map<int, Account>::iterator  it2 = accounts.find(target);
	if (it2 == accounts.end()) {	//second account not found
		log_file.error_noAccount(id, target);
		unlockAccountRead();
		return;
	}

	if (it1->second.getPassword() != password) {	//wrong password
		log_file.error_wrongPassword(id, account);
		unlockAccountRead();
		return;
	}
	sleep(1);
	pthread_mutex_lock(&(it1->second.write_lock));
	pthread_mutex_lock(&(it2->second.write_lock));
	if (!((*it1).second.withdraw(amount, false))) { //no money
		log_file.error_lowBalance(id, account, amount);
		pthread_mutex_unlock(&(it1->second.write_lock));
		pthread_mutex_unlock(&(it2->second.write_lock));
		unlockAccountRead();
		return;
	}
	//deposit legal
	(*it2).second.deposit(amount, false);
	int b1 = (*it1).second.getBalance();
	int b2 = (*it2).second.getBalance();
	log_file.transferLog(id, account, target, amount, b1, b2);
	pthread_mutex_unlock(&(it1->second.write_lock));
	pthread_mutex_unlock(&(it2->second.write_lock));
	unlockAccountRead();
}

//let's take commisions
void Bank::commissions() {
	pthread_mutex_lock(&write_mutex_bank);
	map<int, Account>::iterator it;
	double percent = (rand()%3 + 2);
	for (it = accounts.begin(); it != accounts.end(); it++) {
			int account_balance = (*it).second.getBalance();
			int commision_amount = (int) ((percent * account_balance) / 100);
			(*it).second.withdraw(commision_amount, true);
			bank_money += commision_amount;
			log_file.commissionLog((*it).first, percent, commision_amount);
	}
	pthread_mutex_unlock(&write_mutex_bank);
}

//printing all the details as requested in the pdf
void Bank::printBank() {
	pthread_mutex_lock(&write_mutex_bank);
	cout << "\033[2J" << "\033[1;1H";
	cout << "Current Bank Status" << endl;
	map<int, Account>::iterator it;
	for (it = accounts.begin(); it != accounts.end(); it++) {
		int passw = (*it).second.getPassword();
		string password = static_cast<ostringstream*>(&(ostringstream() << passw))->str();
		string passSTR = string(4 - password.length(), '0') + password;
		cout << "Account " << (*it).second.getNumAccount() << ": Balance - "
				<< (*it).second.getBalance() << " $ , Account Password - "
				<< passSTR << endl;
	}
	cout << "The Bank has " << bank_money << " $" << endl;
	pthread_mutex_unlock(&write_mutex_bank);

}

//take commissions
void* ChargeCommissions(void* arg) {
	while (!flag) {
		sleep(3);
		Bank* bank = (Bank*) arg;
		bank->commissions();
	}
	pthread_exit(NULL);
	return NULL;
}

//print bank
void * bankPrint(void * arg) {
	while (!flag) {
		usleep(500000);
		Bank* bank = (Bank*) arg;
		bank->printBank();
	}
	pthread_exit(NULL);
	return NULL;
}


