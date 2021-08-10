#include "ATM.h"


//commands of atm
void ATM::handle_actions(string buffer) {
	char CommandType;
	stringstream Command(buffer);
	int password,account,amount,transferAmount;

	Command >> CommandType >> account >> password >> amount >> transferAmount;
	if (CommandType == 'O') {	//Open account
		bank->new_account(id, account, password, amount);
		return;
	}
	else if (CommandType == 'D') {	//Deposit to account
		bank->deposit(id, account, password, amount);
		return;
	} else if (CommandType == 'W') {		//Withdraw from account
		bank->withdraw(id, account, password, amount);
		return;
	} else if (CommandType == 'T') {		//Transfer money
        bank->transfer(id, account, password, amount, transferAmount);
        return;
    }
	else if (CommandType == 'B') {		//Bdock balance
		bank->checkBalance(id, account, password);
		return;
	}
}

//let's execute the '.txt' file
void* atmTransactionsFunc(void* arg) {
	ATM* atm = (ATM*) (arg);
	ifstream atm_stream(atm->atm_txt.c_str());
	string buffer;
	while (getline(atm_stream, buffer)) {
		usleep(100000);
		atm->handle_actions(buffer);
	}
	pthread_exit(NULL);
	return NULL;
}
