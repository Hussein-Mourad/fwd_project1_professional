#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE  
#define _CRT_NONSTDC_NO_DEPRECATE
#include "card.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

EN_cardError_t getCardHolderName(ST_cardData_t* cardData)
{
	// There is an extra charater for gets to put \n
	char name[26] = { 0 };
	printf("Enter Card holder name: ");
	fgets(name, 26, stdin);
	name[strcspn(name, "\n")] = 0; // removes \n from the string

	if ((name == NULL) || (strlen(name) < 20) || (strlen(name) > 24))
		return WRONG_NAME;
	else 
	{
		strcpy(cardData->cardHolderName, name);
		return CARD_OK;
	}
}

EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData)
{
	// There is an extra charater for gets to put \n
	char date[7] = { 0 };
	char delimiter[] = "/";
	printf("Enter expiration date (MM/YY): ");
	fgets(date, 7, stdin);
	date[strcspn(date, "\n")] = 0; // removes \n from the string

	if ((date == NULL) || (strlen(date) != 5) || (date[2] != '/'))
		return WRONG_EXP_DATE;

	strcpy(cardData->cardExpirationDate, date);

	char* token = strtok(date, delimiter); // splits the string by delimiter "/"
	// Check the correct range for month and year
	if (token == 0) 
		return WRONG_EXP_DATE;
	int month = atoi(token);
	if ((month < 1) || (month > 12)) 
		return WRONG_EXP_DATE;

	token = strtok(0, delimiter);
	if (token == 0)
		return WRONG_EXP_DATE;
	int year = atoi(token);
	if ((year < 0) || (year > 99)) 
		return WRONG_EXP_DATE;

	return CARD_OK;
}

EN_cardError_t getCardPAN(ST_cardData_t* cardData)
{
	// There is an extra character for \n
	uint8_t panNumber[21] = { 0 };
	printf("Enter you pan number: ");
	fgets(panNumber, 21, stdin);
	panNumber[strcspn(panNumber, "\n")] = 0; // Removes extra \n

	// Check for wrong input
	if ((panNumber == NULL) || (strlen(panNumber) < 16) || (strlen(panNumber) > 19))
		return WRONG_PAN;
	strcpy(cardData->primaryAccountNumber, panNumber);
	return CARD_OK;
}

void getCardHolderNameTest(void) {
	ST_cardData_t card;
	printf("Tester Name: Hussein Mourad Kassem\n");
	printf("Function Name: getCardHolderName\n");
	printf("Test Case 1: \n");
	printf("Input Data: hussein mourad kassem\n");
	printf("Expected Result: 0 (CARD_OK)\n");
	printf("Actual Result: %d\n", getCardHolderName(&card));
	printf("Test Case 2: \n");
	printf("Input Data: hussein mourad\n");
	printf("Expected Result: 1 (WRONG_NAME)\n");
	printf("Actual Result: %d\n", getCardHolderName(&card));
	printf("Test Case 3: \n");
	printf("Input Data: idklfjdfkljdfkldjfdklfjdfdklfjdfklaa\n");
	printf("Expected Result: 1 (WRONG_NAME)\n");
	printf("Actual Result: %d\n", getCardHolderName(&card));
}

void getCardExpiryDateTest(void){
	ST_cardData_t card;
	printf("Tester Name: Hussein Mourad Kassem\n");
	printf("Function Name: getCardExpiryDate\n");
	printf("Test Case 1: \n");
	printf("Input Data: 05/25\n");
	printf("Expected Result: 0 (CARD_OK)\n");
	printf("Actual Result: %d\n", getCardExpiryDate(&card));
	printf("Test Case 2: \n");
	printf("Input Data: 30/25\n");
	printf("Expected Result: 2 (WRONG_EXP_DATE)\n");
	printf("Actual Result: %d\n", getCardExpiryDate(&card));
	printf("Test Case 3: \n");
	printf("Input Data: 20\n");
	printf("Expected Result: 2 (WRONG_EXP_DATE)\n");
	printf("Actual Result: %d\n", getCardExpiryDate(&card));
	printf("Test Case 4: \n");
	printf("Input Data: 20434390\n");
	printf("Expected Result: 2 (WRONG_EXP_DATE)\n");
	printf("Actual Result: %d\n", getCardExpiryDate(&card));
}

void getCardPANTest(void)
{
	ST_cardData_t card;
	printf("Tester Name: Hussein Mourad Kassem\n");
	printf("Function Name: getCardPAN\n");
	printf("Test Case 1: \n");
	printf("Input Data: 8989374615436851\n");
	printf("Expected Result: 0 (CARD_OK)\n");
	printf("Actual Result: %d\n", getCardPAN(&card));
	printf("Test Case 2: \n");
	printf("Input Data: 3894u349\n");
	printf("Expected Result: (3) WRONG_PAN\n");
	printf("Actual Result: %d\n", getCardPAN(&card));
	printf("Test Case 3: \n");
	printf("Input Data: 38l94u3dfkjsdlfjsdsdlj49\n");
	printf("Expected Result: (3) WRONG_PAN\n");
	printf("Actual Result: %d\n", getCardPAN(&card));
}