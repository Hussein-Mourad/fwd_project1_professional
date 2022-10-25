#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE  
#define _CRT_NONSTDC_NO_DEPRECATE
#include "server.h"
#include "card.h"
#include "terminal.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

extern ST_accountsDB_t accountsDB[255] = {
	{2000.0, RUNNING, "8989374615436851"},
	{100000.0, RUNNING, "5807007076043875"},
	{20000.0, BLOCKED, "7301765304658102553"},
	{6000.0, RUNNING, "602988817196047097"},
	{1990.0, BLOCKED, "12090007180724491"},
};

ST_transaction_t transactionsDB[255];

uint32_t transactionIndex = 0;


EN_transState_t recieveTransactionData (ST_transaction_t* transData)
{
	int accountIndex = -1;
	int accountsSize = sizeof(accountsDB) / sizeof(ST_accountsDB_t);

	// Find the account
	for (int i = 0; i < accountsSize; i++)
	{
		if (strcmp(transData->cardHolderData.primaryAccountNumber, accountsDB[i].primaryAccountNumber) == 0)
		{
			accountIndex = i;
			break;
		}
	}
	
	if (accountIndex == -1)
		transData->transState = FRAUD_CARD;
	else if (transData->terminalData.transAmount > accountsDB[accountIndex].balance)
		transData->transState = DECLINED_INSUFFECIENT_FUND;
	else if (accountsDB[accountIndex].state == BLOCKED)
		transData->transState = DECLINED_STOLEN_CARD;
	else if (transData->terminalData.transAmount <= accountsDB[accountIndex].balance)
	{
		accountsDB[accountIndex].balance -= transData->terminalData.transAmount;
		transData->transState = APPROVED;
	}
	else
		transData->transState = INTERNAL_SERVER_ERROR;

	return transData->transState;
}

EN_serverError_t isValidAccount(ST_cardData_t* cardData, ST_accountsDB_t* accountRefrence)
{
	int accountIndex = -1;
	int accountsSize = sizeof(accountsDB) / sizeof(ST_accountsDB_t);

	// Find the account
	for (int i = 0; i < accountsSize; i++)
	{
		if (strcmp(cardData->primaryAccountNumber, accountsDB[i].primaryAccountNumber) == 0)
		{
			accountIndex = i;
			break;
		}
	}
	if (accountIndex == -1)
	{
		accountRefrence = NULL;
		return ACCOUNT_NOT_FOUND;
	}
	else
	{
		*accountRefrence = accountsDB[accountIndex];
		return SERVER_OK;
	}
}

EN_serverError_t isBlockedAccount(ST_accountsDB_t* accountRefrence)
{
	if (accountRefrence->state == BLOCKED)
		return BLOCKED_ACCOUNT;
	else
		return SERVER_OK;
}

EN_serverError_t isAmountAvailable(ST_terminalData_t* termData, ST_accountsDB_t* accountRefrence)
{
	if (termData->transAmount > accountRefrence->balance)
		return LOW_BALANCE;
	else
		return SERVER_OK;
}

EN_serverError_t saveTransaction(ST_transaction_t* transData)
{
	transData->transactionSequenceNumber = transactionIndex;
	transactionsDB[transData->transactionSequenceNumber] = *transData;
	transactionIndex++;
	listSavedTransactions();
	return SERVER_OK;
}

void listSavedTransactions(void)
{
	const char* TRANS_STATES[] = 
	{
		"APPROVED",
		"DECLINED_INSUFFECIENT_FUND",
		"DECLINED_STOLEN_CARD",
		"FRAUD_CARD",
		"INTERNAL_SERVER_ERROR"
	};
	printf("\n");
	for (uint32_t i = 0; i < transactionIndex; i++)
	{
		ST_transaction_t* transaction;
		transaction = &transactionsDB[i];
		printf("#########################\n");
		printf("Transaction Sequence Number: %lu\n", transaction->transactionSequenceNumber);
		printf("Transaction Date: %s\n", transaction->terminalData.transactionDate);
		printf("Transaction Amount: %f\n", transaction->terminalData.transAmount);
		printf("Transaction State: %s\n", TRANS_STATES[transaction->transState]);
		printf("Transaction Max Amount: %f\n", transaction->terminalData.maxTransAmount);
		printf("Cardholder Name: %s\n", transaction->cardHolderData.cardHolderName);
		printf("PAN: %s\n", transaction->cardHolderData.primaryAccountNumber);
		printf("Card Expiration Date: %s\n", transaction->cardHolderData.cardExpirationDate);
		printf("#########################\n");
	}
	printf("\n");
}

void recieveTransactionDataTest(void)
{	
	ST_cardData_t cardData = {"hussein mourad kassem", "8989374615436851", "09/26"};
	ST_terminalData_t termData = {1000, 1000, "20/10/2022"};
	ST_transaction_t transData = {cardData, termData, RUNNING, 0};

	
	printf("Tester Name: Hussein Mourad Kassem\n");
	printf("Function Name: recieveTransactionData\n");

	printf("Test Case 1: \n");
	printf("Input Data: \n");
	printf("Expected Result: 0 (SERVER_OK)\n");
	printf("Actual Result: %d\n", recieveTransactionData(&transData));

	termData.transAmount = 10000;
	transData.terminalData = termData;

	printf("Test Case 2: \n");
	printf("Input Data: No suffecient fund\n");
	printf("Expected Result: 1 (DECLINED_INSUFFECIENT_FUND)\n");
	printf("Actual Result: %d\n", recieveTransactionData(&transData));

	strcpy(cardData.primaryAccountNumber, "549857489574574549");
	transData.cardHolderData = cardData;

	printf("Test Case 3: \n");
	printf("Input Data: Card not found\n");
	printf("Expected Result: 3 (FRAUD_CARD)\n");
	printf("Actual Result: %d\n", recieveTransactionData(&transData));

	strcpy(cardData.primaryAccountNumber, "7301765304658102553");
	transData.cardHolderData = cardData;

	printf("Test Case 4: \n");
	printf("Input Data: Stolen Card\n");
	printf("Expected Result: 2 (DECLINED_STOLEN_CARD)\n");
	printf("Actual Result: %d\n", recieveTransactionData(&transData));
}

void isValidAccountTest(void)
{
	ST_cardData_t cardData = { "hussein mourad kassem", "8989374615436851", "09/26" };
	ST_terminalData_t termData = { 1000, 1000, "20/10/2022" };
	ST_transaction_t transData = { cardData, termData, RUNNING, 0 };
	ST_accountsDB_t accountReference;


	printf("Tester Name: Hussein Mourad Kassem\n");
	printf("Function Name: isValidAccount\n");

	printf("Test Case 1: \n");
	printf("Input Data: \n");
	printf("Expected Result: 0 (SERVER_OK)\n");
	printf("Actual Result: %d\n", isValidAccount(&cardData, &accountReference));

	strcpy(cardData.primaryAccountNumber, "549857489574574549");
	transData.cardHolderData = cardData;

	printf("Test Case 2: \n");
	printf("Input Data: Invalid Account\n");
	printf("Expected Result: 3 (ACCOUNT_NOT_FOUND)\n");
	printf("Actual Result: %d\n", isValidAccount(&cardData, &accountReference));
}

void isBlockedAccountTest(void)
{
	ST_accountsDB_t accountReference = accountsDB[0];

	printf("Tester Name: Hussein Mourad Kassem\n");
	printf("Function Name: isBlockedAccount\n");

	printf("Test Case 1: \n");
	printf("Input Data: \n");
	printf("Expected Result: 0 (SERVER_OK)\n");
	printf("Actual Result: %d\n", isBlockedAccount(&accountReference));

	accountReference = accountsDB[2];

	printf("Test Case 2: \n");
	printf("Input Data: Invalid Account\n");
	printf("Expected Result: 5 (BLOCKED_ACCOUNT)\n");
	printf("Actual Result: %d\n", isBlockedAccount(&accountReference));
}

void isAmountAvailableTest(void)
{
	ST_terminalData_t termData = { 1000, 1000, "20/10/2022" };
	ST_accountsDB_t accountReference = accountsDB[0];

	printf("Tester Name: Hussein Mourad Kassem\n");
	printf("Function Name: isAmountAvailable\n");

	printf("Test Case 1: \n");
	printf("Input Data: \n");
	printf("Expected Result: 0 (SERVER_OK)\n");
	printf("Actual Result: %d\n", isAmountAvailable(&termData, &accountReference));

	termData.transAmount = 20000;

	printf("Test Case 2: \n");
	printf("Input Data: low balance\n");
	printf("Expected Result: 4 (LOW_BALANCE)\n");
	printf("Actual Result: %d\n", isAmountAvailable(&termData, &accountReference));
}

void saveTransactionTest(void)
{
	ST_cardData_t cardData = { "hussein mourad kassem", "8989374615436851", "09/26" };
	ST_terminalData_t termData = { 1000, 1000, "20/10/2022" };
	ST_transaction_t transData = { cardData, termData, RUNNING, 0 };

	printf("Tester Name: Hussein Mourad Kassem\n");
	printf("Function Name: saveTransaction\n");

	printf("Test Case 1: \n");
	printf("Input Data: \n");
	printf("Expected Result: 0 (SERVER_OK)\n");
	printf("Actual Result: %d\n", saveTransaction(&transData));
}

void listSavedTransactionsTest(void)
{
	ST_cardData_t cardData = { "hussein mourad kassem", "8989374615436851", "09/26" };
	ST_terminalData_t termData = { 1000, 1000, "20/10/2022" };
	ST_transaction_t transData = { cardData, termData, RUNNING, 0 };
	transData.transactionSequenceNumber = transactionIndex;
	transactionsDB[transData.transactionSequenceNumber] = transData;
	transactionIndex++;

	ST_cardData_t cardData1 = { "hussein mourad kassem", "7301765304652553", "07/27" };
	ST_terminalData_t termData1 = { 300, 100, "01/08/2022" };
	ST_transaction_t transData1 = { cardData1, termData1, RUNNING, 2 };
	transData1.transactionSequenceNumber = transactionIndex;
	transactionsDB[transData1.transactionSequenceNumber] = transData1;
	transactionIndex++;

	printf("Tester Name: Hussein Mourad Kassem\n");
	printf("Function Name: listSavedTransactions\n");

	printf("Test Case 1:\n");
	printf("Input Data: Adding two transactions\n");
	printf("Expected Result: \n");
	printf("Actual Result:\n");
	listSavedTransactions();
}
