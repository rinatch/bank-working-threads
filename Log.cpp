#include "Log.h"


Log::Log(const char* logfile) {
	log_file.open(logfile);
	pthread_mutex_init(&mutex, NULL);
	pthread_mutex_unlock(&mutex);
}


Log::~Log() {
	pthread_mutex_destroy(&mutex);
	log_file.close();
}


void Log::newAccountLog(int atmID, int account, int password, int money) {
	pthread_mutex_lock(&mutex);
	string passw = static_cast<ostringstream*>(&(ostringstream() << password))->str();
	string passSTR = string(4 - passw.length(), '0') + passw;
	log_file << atmID << ": New account id is " << account << " with password "
			<< passSTR << " and initial balance " << money
			<< endl;
	pthread_mutex_unlock(&mutex);
}


void Log::depositLog(int atmID, int account, int amount, int balance) {
	pthread_mutex_lock(&mutex);
	log_file << atmID << ": Account " << account << " new balance is "
			<< balance << " after " << amount << " $ was deposited" << endl;
	pthread_mutex_unlock(&mutex);
}


void Log::withdrawLog(int atmID, int account, int amount, int balance) {
	pthread_mutex_lock(&mutex);
	log_file << atmID << ": Account " << account << " new balance is "
			<< balance << " after " << amount << " $ was withdrew" << endl;
	pthread_mutex_unlock(&mutex);
}


void Log::checkBalanceLog(int atmID, int account, int balance) {
	pthread_mutex_lock(&mutex);
	log_file << atmID << ": Account " << account << " balance is " << balance
			<< endl;
	pthread_mutex_unlock(&mutex);
}

void Log::transferLog(int atmID, int account, int target,
		int amount, int account_bal, int target_bal) {
	pthread_mutex_lock(&mutex);
	log_file << atmID << ": Transfer " << amount << " from account " << account
			<< " to account " << target << " new account balance is "
			<< account_bal << " new target account balance is " << target_bal
			<< endl;
	pthread_mutex_unlock(&mutex);
}


void Log::commissionLog(int account, double percent, int amount) {
	pthread_mutex_lock(&mutex);
	log_file << "Bank: commissions of " << percent
			<< " % were charged, the bank gained " << amount
			<< " $ from account " << account << endl;
	pthread_mutex_unlock(&mutex);
}


void Log::error_noAccount(int atmID, int account) {
	pthread_mutex_lock(&mutex);
	log_file << "Error " << atmID << ": Your transaction failed - account id "
			<< account << " does not exist" << endl;
	pthread_mutex_unlock(&mutex);
}


void Log::error_lowBalance(int atmID, int account, int amount) {
	pthread_mutex_lock(&mutex);
	log_file << "Error " << atmID << ": Your transaction failed - account id "
			<< account << " balance is lower than " << amount << endl;
	pthread_mutex_unlock(&mutex);
}


void Log::error_accountExist(int atmID, int account) {
	pthread_mutex_lock(&mutex);
	log_file << "Error " << atmID
			<< ": Your transaction failed - account with the same id exists"
			<< endl;
	pthread_mutex_unlock(&mutex);
}


void Log::error_wrongPassword(int atmID, int account) {
	pthread_mutex_lock(&mutex);
	log_file << "Error " << atmID
			<< ": Your transaction failed - password for account id " << account
			<< " is incorrect" << endl;
	pthread_mutex_unlock(&mutex);
}

