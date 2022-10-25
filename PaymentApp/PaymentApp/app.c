#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE  
#define _CRT_NONSTDC_NO_DEPRECATE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "app.h"
#include "card.h"
#include "terminal.h"
#include "server.h"

int main() {
	//getCardHolderNameTest();
	//getCardExpiryDateTest();
	//getCardPANTest();
	//getTransactionDateTest();
	//isCardExpriedTest();
	//getTransactionAmountTest();
	//isBelowMaxAmountTest();
	//setMaxAmountTest();
	//recieveTransactionDataTest();
	//isValidAccountTest();
	//isBlockedAccountTest();
	//isAmountAvailableTest();
	//saveTransactionTest();
	//listSavedTransactionsTest();
	appStart();

	return 0;
}

void appStart(void)
{
	ST_cardData_t card;
	ST_terminalData_t terminalData;
	ST_transaction_t transaction = {0};
	ST_accountsDB_t accountRefernce = {0};

	setMaxAmount(&terminalData, 10000.0);

	if (getCardHolderName(&card) != CARD_OK) 
	{
		printf("Invalid card name.\n");
		return;
	}

	if (getCardExpiryDate(&card) != CARD_OK)
	{
		printf("Invalid card expiration date.\n");
		return;
	}

	if (getCardPAN(&card) != CARD_OK)
	{
		printf("Invalid card PAN.\n");
		return;

	}

	if (getTransactionDate(&terminalData) != TERMINAL_OK)
	{
		printf("Invalid transaction date.\n");
		return;
	}

	if (isCardExpired(&card, &terminalData) != TERMINAL_OK)
	{
		printf("Your card is expired.\n");
		return;
	}

	if (getTransactionAmount(&terminalData) != TERMINAL_OK)
	{
		printf("Invalid amount.\n");
		return;
	}

	if (isBelowMaxAmount(&terminalData) != TERMINAL_OK)
	{
		printf("Declined amount exceeded limit.\n");
		return;
	}
	if (isValidAccount(&card, &accountRefernce) != SERVER_OK)
	{
		printf("Declined Invalid account.\n");
		return;
	}

	if (isAmountAvailable(&terminalData, &accountRefernce) != SERVER_OK)
	{
		printf("Declined insuffecient funds.\n");
		return;
	}
	
	transaction.cardHolderData = card;
	transaction.terminalData = terminalData;
	
	if (recieveTransactionData(&transaction) != APPROVED)
	{
		printf("Transaction Failed.\n");
		return;
	}

	if (saveTransaction(&transaction) != SERVER_OK)
	{
		printf("Transaction Failed.\n");
		return;
	}

	printf("Success: Transaction Saved.");
}

