#include <stdio.h>
#include <string.h>
#include "card.h"
#include "terminal.h"

/*
int main()
{
	getCardHolderNameTest();
	getCardExpiryDateTest();
	getCardPANTest();
	return 0;
}
*/

EN_cardError_t getCardHolderName(ST_cardData_t* cardData)
{
	EN_cardError_t error = CARD_OK;
	uint8_t i; // counter for loop
	printf("Enter Card Name : ");
	gets(cardData->cardHolderName);
	/* First, we want to check name length */
	if ((strlen(cardData->cardHolderName) > 24) || (strlen(cardData->cardHolderName) < 20) || ((cardData->cardHolderName) == NULL))
		error = WRONG_NAME;
	else
	{
		/* We want to check that the input name characters are space or letters only */
		for (i = 0; i < strlen(cardData->cardHolderName); i++)
		{
			if (!((cardData->cardHolderName[i] == ' ') || ((cardData->cardHolderName[i] >= 'a') && (cardData->cardHolderName[i] <= 'z')) || ((cardData->cardHolderName[i] >= 'A') && (cardData->cardHolderName[i] <= 'Z') )))
				error = WRONG_NAME;
		}
	}
	return error;
}

EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData)
{
	EN_cardError_t error = CARD_OK;
	printf("Enter Expiry Date : ");
	gets(cardData->cardExpirationDate);
	uint8_t ExpirationMonth = ((cardData->cardExpirationDate[1]) - 48) + ((cardData->cardExpirationDate[0]) - 48) * 10;
	if ((strlen(cardData->cardExpirationDate) == 5) && ((cardData->cardExpirationDate) != NULL) && ExpirationMonth <= 12)
	{
		/* Check the format */
		if (cardData->cardExpirationDate[0] > '9' || cardData->cardExpirationDate[0] < '0') error = WRONG_EXP_DATE;
		if (cardData->cardExpirationDate[1] > '9' || cardData->cardExpirationDate[1] < '0') error = WRONG_EXP_DATE;
		if (cardData->cardExpirationDate[2] != '/') error = WRONG_EXP_DATE;
		if (cardData->cardExpirationDate[3] > '9' || cardData->cardExpirationDate[3] < '0') error = WRONG_EXP_DATE;
		if (cardData->cardExpirationDate[4] > '9' || cardData->cardExpirationDate[4] < '0') error = WRONG_EXP_DATE;
	}
	else
		error = WRONG_EXP_DATE;
	return error;
}

EN_cardError_t getCardPAN(ST_cardData_t* cardData)
{
	EN_cardError_t error = CARD_OK;
	uint8_t i; // counter for loop
	printf("Enter Card PAN : ");
	gets(cardData->primaryAccountNumber);
	if ((strlen(cardData->primaryAccountNumber) > 19) || (strlen(cardData->primaryAccountNumber) < 16) || ((cardData->primaryAccountNumber) == NULL))
		error = WRONG_PAN;
	else
	{
		for (i = 0; i < strlen(cardData->primaryAccountNumber); i++)
		{
			/* Make sure that all characters are numbers */
			if (cardData->primaryAccountNumber[i] < '0' || cardData->primaryAccountNumber[i] > '9')
				error = WRONG_PAN;
		}
	}
	return error;
}

void printCardError(EN_cardError_t error)
{
	switch (error)
	{
	case CARD_OK:
		printf("CARD_OK\n");
		break;
	case WRONG_NAME:
		printf("WRONG_NAME\n");
		break;
	case WRONG_EXP_DATE:
		printf("WRONG_EXP_DATE\n");
		break;
	case WRONG_PAN:
		printf("WRONG_PAN\n");
		break;
	}
}

void getCardHolderNameTest(void)
{
	ST_cardData_t cardData;
	EN_cardError_t error;
	printf("Tester Name : Mahmoud Ahmed\n");
	printf("Function Name: getCardHolderName\n");

	printf("\nTest case 1: \n");
	printf("Input Data : Mahmoud Ahmed Ibrahim \n");
	error = getCardHolderName(&cardData);
	printf("Expected Result : CARD_OK \n"); // CARD_OK
	printf("Actual Result : ");
	printCardError(error);

	printf("\nTest case 2: \n");
	printf("Input Data : Mahmoud Ahmed Ibrahim Ahmed\n");
	error = getCardHolderName(&cardData);
	printf("Expected Result : WRONG_NAME\n"); // WRONG_NAME ( because it exceeds 24 )
	printf("Actual Result : ");
	printCardError(error);

	printf("\nTest case 3: \n");
	printf("Input Data : 123456789012345678901\n");
	error = getCardHolderName(&cardData);
	printf("Expected Result : WRONG_NAME\n"); // WRONG_NAME ( because it contains numbers )
	printf("Actual Result : ");
	printCardError(error);

	printf("\nTest case 4: \n");
	printf("Input Data : asdfghjklqwertyuiopz12\n");
	error = getCardHolderName(&cardData);
	printf("Expected Result : WRONG_NAME\n"); // WRONG_NAME ( because it contains numbers )
	printf("Actual Result : ");
	printCardError(error);
}

void getCardExpiryDateTest(void)
{
	ST_cardData_t cardData;
	EN_cardError_t error;
	printf("\nTester Name : Mahmoud Ahmed\n");
	printf("Function Name: getCardExpiryDate\n");

	printf("\nTest case 1: \n");
	printf("Input Data : 02/22 \n");
	error = getCardExpiryDate(&cardData);
	printf("Expected Result : CARD_OK\n");
	printf("Actual Result : ");
	printCardError(error);

	printf("\nTest case 2: \n");
	printf("Input Data : 2/24\n");
	error = getCardExpiryDate(&cardData);
	printf("Expected Result : WRONG_EXP_DATE\n");
	printf("Actual Result : ");
	printCardError(error);

	printf("\nTest case 3: \n");
	printf("Input Data : a2/22\n");
	error = getCardExpiryDate(&cardData);
	printf("Expected Result : WRONG_EXP_DATE\n");
	printf("Actual Result : ");
	printCardError(error);

	printf("\nTest case 4: \n");
	printf("Input Data : 24/05\n");
	error = getCardExpiryDate(&cardData);
	printf("Expected Result : WRONG_EXP_DATE\n");
	printf("Actual Result : ");
	printCardError(error);
}

void getCardPANTest(void)
{
	ST_cardData_t cardData;
	EN_cardError_t error;
	printf("\nTester Name : Mahmoud Ahmed\n");
	printf("Function Name: getCardPAN\n");

	printf("\nTest case 1: \n");
	printf("Input Data : 5338956234781234 \n"); // 16 digit
	error = getCardPAN(&cardData);
	printf("Expected Result : CARD_OK\n");
	printf("Actual Result : ");
	printCardError(error);

	printf("\nTest case 2: \n");
	printf("Input Data : 53389562347812349034 \n"); // 20 digit
	error = getCardPAN(&cardData);
	printf("Expected Result : WRONG_PAN\n");
	printf("Actual Result : ");
	printCardError(error);

	printf("\nTest case 3: \n");
	printf("Input Data : asdfghjklqwertyuio \n");
	error = getCardPAN(&cardData);
	printf("Expected Result : WRONG_PAN\n");
	printf("Actual Result : ");
	printCardError(error); 

	printf("\nTest case 3: \n");
	printf("Input Data : 1234567890123456a \n");
	error = getCardPAN(&cardData);
	printf("Expected Result : WRONG_PAN\n"); 
	printf("Actual Result : ");
	printCardError(error);
}
