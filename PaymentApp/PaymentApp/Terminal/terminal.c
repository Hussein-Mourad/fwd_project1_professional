#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE  
#define _CRT_NONSTDC_NO_DEPRECATE
#include "../Card/card.h"
#include "terminal.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

EN_terminalError_t getTransactionDate(ST_terminalData_t* termData)
{
	// There is an extra character for \m
	uint8_t date[12] = { 0 };
	char delim[] = "/";
	printf("Enter transaction date (DD/MM/YYYY): ");
	fgets(date, 12, stdin);
	date[strcspn(date, "\n")] = 0;

	if ((date == NULL) || (date[2] != '/') || (date[5] != '/') || (strlen(date) != 10))
		return WRONG_DATE;

	strcpy(termData->transactionDate, date);

	// Check for the day
	char* token = strtok(date, delim);
	if (token == 0)
		return WRONG_DATE;
	int day = atoi(token);
	if ((day < 1) || (day > 31))
		return WRONG_DATE;

	// Check for the month
	token = strtok(0, delim);
	if (token == 0)
		return WRONG_DATE;
	int month = atoi(token);
	if ((month < 1) || (month > 12))
		return WRONG_DATE;

	// Check for the year
	token = strtok(0, delim);
	if (token == 0)
		return WRONG_DATE;
	int year = atoi(token);
	if ((year < 1) || (year > 9999))
		return WRONG_DATE;

	return TERMINAL_OK;
}

EN_terminalError_t isCardExpired(ST_cardData_t* cardData, ST_terminalData_t* termData)
{
	char buffer[3] = { 0 };

	memcpy(buffer, &cardData->cardExpirationDate[0], 2); // Get month from card Data
	buffer[2] = '\0';
	int card_month = atoi(buffer);

	memcpy(buffer, &termData->transactionDate[3], 2); // Get month from terminal Data
	buffer[2] = '\0';
	int terminal_month = atoi(buffer);

	memcpy(buffer, &cardData->cardExpirationDate[3], 2); // Get month from card Data
	buffer[2] = '\0';
	int card_year = atoi(buffer);

	memcpy(buffer, &termData->transactionDate[8], 2); // Get month from terminal Data
	buffer[2] = '\0';
	int terminal_year = atoi(buffer);

	if ((terminal_year < card_year) || 
		(terminal_month <= card_month) && (terminal_year <= card_year))
		return TERMINAL_OK;

	return EXPIRED_CARD;
}

EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData)
{
	float amount;
	printf("Enter transaction amount: ");
	if (scanf("%f", &amount) != 1)
		return INVALID_AMOUNT;
	
	if (amount <= 0)
		return INVALID_AMOUNT;
	termData->transAmount = amount;
	return TERMINAL_OK;
}

EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData)
{
	if (termData->transAmount > termData->maxTransAmount)
		return EXCEED_MAX_AMOUNT;

	return TERMINAL_OK;
}

EN_terminalError_t setMaxAmount(ST_terminalData_t* termData, float maxAmount)
{
	if (maxAmount <= 0)
		return INVALID_MAX_AMOUNT;

	termData->maxTransAmount = maxAmount;
	return TERMINAL_OK;
}

void getTransactionDateTest(void)
{
	ST_terminalData_t termData;
	printf("Tester Name: Hussein Mourad Kassem\n");
	printf("Function Name: getTransactionDate\n");
	printf("Test Case 1: \n");
	printf("Input Data: 05/10/2022\n");
	printf("Expected Result: 0 (TERMINAL_OK)\n");
	printf("Actual Result: %d\n", getTransactionDate(&termData));
	printf("Test Case 2: \n");
	printf("Input Data: 5/1/22\n");
	printf("Expected Result: 1 (WRONG_DATE)\n");
	printf("Actual Result: %d\n", getTransactionDate(&termData));
	printf("Test Case 3: \n");
	printf("Input Data: 5\n");
	printf("Expected Result: 1 (WRONG_DATE)\n");
	printf("Actual Result: %d\n", getTransactionDate(&termData));
}

void isCardExpriedTest(void)
{
	ST_terminalData_t termData = { 0, 0, "10/10/2020" };
	ST_cardData_t card = { "hussein mourad kassem", "8989374615436851", "10/22" };


	printf("Tester Name: Hussein Mourad Kassem\n");
	printf("Function Name: isCardExpried\n");
	printf("Test Case 1: \n");
	printf("Input Data: card expire date 10/22, transaction expire date 10/10/2020\n");
	printf("Expected Result: 0 (TERMINAL_OK)\n");
	printf("Actual Result: %d\n", isCardExpired(&card, &termData));

	strcpy(card.cardExpirationDate, "03/19");
	strcpy(termData.transactionDate, "01/12/2012");

	printf("Test Case 2: \n");
	printf("Input Data:  card expire date 03/19, transaction expire date 01/12/2012\n");
	printf("Expected Result: 0 (TERMINAL_OK)\n");
	printf("Actual Result: %d\n", isCardExpired(&card, &termData));

	strcpy(card.cardExpirationDate, "04/12");
	strcpy(termData.transactionDate, "05/10/2022");

	printf("Test Case 3: \n");
	printf("Input Data:  card expire date 04/12, transaction expire date 05/10/2022\n");
	printf("Expected Result: 2 (EXPIRED_CARD)\n");
	printf("Actual Result: %d\n", isCardExpired(&card, &termData));

	strcpy(card.cardExpirationDate, "10/22");
	strcpy(termData.transactionDate, "05/10/2022");

	printf("Test Case 3: \n");
	printf("Input Data:  card expire date 10/22, transaction expire date 05/10/2022\n");
	printf("Expected Result: 0 (TERMINAL_OK)\n");
	printf("Actual Result: %d\n", isCardExpired(&card, &termData));
}

void getTransactionAmountTest(void)
{
	ST_terminalData_t termData;

	printf("Tester Name: Hussein Mourad Kassem\n");
	printf("Function Name: getTransactionAmount\n");

	printf("Test Case 1: \n");
	printf("Input Data: 5000\n");
	printf("Expected Result: 0 (TERMINAL_OK)\n");
	printf("Actual Result: %d\n", getTransactionAmount(&termData));

	printf("Test Case 2: \n");
	printf("Input Data: 0\n");
	printf("Expected Result: 4 (INVALID_AMOUNT)\n");
	printf("Actual Result: %d\n", getTransactionAmount(&termData));

	printf("Test Case 3: \n");
	printf("Input Data: -1 \n");
	printf("Expected Result: 4 (INVALID_AMOUNT)\n");
	printf("Actual Result: %d\n", getTransactionAmount(&termData));
}

void isBelowMaxAmountTest(void)
{
	ST_terminalData_t termData= {5000, 10000, ""};

	printf("Tester Name: Hussein Mourad Kassem\n");
	printf("Function Name: isBelowMaxAmount\n");

	printf("Test Case 1: \n");
	printf("Input Data: amount 5000, max amount 10000\n");
	printf("Expected Result: 0 (TERMINAL_OK)\n");
	printf("Actual Result: %d\n", isBelowMaxAmount(&termData));

	termData.transAmount = 1000;
	termData.maxTransAmount = 100;

	printf("Test Case 2: \n");
	printf("Input Data: amount 1000, max amount 100\n");
	printf("Expected Result: 5 (EXCEED_MAX_AMOUNT)\n");
	printf("Actual Result: %d\n", isBelowMaxAmount(&termData));

	termData.transAmount = 4000;
	termData.maxTransAmount = 10;

	printf("Test Case 3: \n");
	printf("Input Data: amount 4000, max amount 10 \n");
	printf("Expected Result: 5 (EXCEED_MAX_AMOUNT)\n");
	printf("Actual Result: %d\n", isBelowMaxAmount(&termData));
}

void setMaxAmountTest(void)
{
	ST_terminalData_t termData = { 0 };

	printf("Tester Name: Hussein Mourad Kassem\n");
	printf("Function Name: setMaxAmount\n");

	printf("Test Case 1: \n");
	printf("Input Data: 1000\n");
	printf("Expected Result: 0 (TERMINAL_OK)\n");
	printf("Actual Result: %d\n", setMaxAmount(&termData, 1000));

	printf("Test Case 2: \n");
	printf("Input Data: -1\n");
	printf("Expected Result: 6 (INVALID_MAX_AMOUNT)\n");
	printf("Actual Result: %d\n", setMaxAmount(&termData,-1));

	printf("Test Case 3: \n");
	printf("Input Data: 0\n");
	printf("Expected Result: 6 (INVALID_MAX_AMOUNT)\n");
	printf("Actual Result: %d\n", setMaxAmount(&termData, 0));
}
