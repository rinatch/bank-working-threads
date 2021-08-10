#include "Log.h"
#include "Bank.h"
#include "ATM.h"
#include <pthread.h>


bool flag = false;

int main(int argc, char** argv) {
	if (argc < 3) {
		fprintf(stderr, "illegal arguments\n");
		exit(1);
	}
	int AtmsNum = atoi(argv[1]);
	if (argc - 2 != AtmsNum) { //is there correct num of '.txt'?
		fprintf(stderr, "illegal arguments\n");
		exit(1);
	}
	Log outLog("log.txt");
	Bank bank(&outLog); //created a bank
	pthread_t bank_commisson, printBank, ATMthread[AtmsNum];
	pthread_create(&bank_commisson, NULL, ChargeCommissions, (void*) &bank);
	pthread_create(&printBank, NULL, bankPrint, (void*) &bank);
	ATM* atms = new ATM[AtmsNum]; //create atms
	//initialize atms
	for (int i = 0; i < AtmsNum; i++) {
		atms[i].id = i + 1;
		atms[i].atm_txt = argv[i + 2];
		atms[i].bank = &bank;
		pthread_create(&ATMthread[i], NULL, atmTransactionsFunc, &atms[i]);

	}
	//activate the atms
	for (int i = 0; i < AtmsNum; ++i) {
		pthread_join(ATMthread[i], NULL);
	}
	flag = true;
	pthread_join(bank_commisson, NULL);
	pthread_join(printBank, NULL);
	pthread_cancel(bank_commisson);
	pthread_cancel(printBank);
	delete[] atms;
	return 0;
}
