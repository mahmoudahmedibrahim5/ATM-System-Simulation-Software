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
	printf("Enter Card Name : ");
	gets(cardData->cardHolderName);
	if ((strlen(cardData->cardHolderName) > 24) || (strlen(cardData->cardHolderName) < 20) || ((cardData->cardHolderName) == NULL))
		return WRONG_NAME;
	else
		return CARD_OK;
}

EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData)
{
	printf("Enter Expiry Date : ");
	gets(cardData->cardExpirationDate);//12/24
	uint8_t ExpirationMonth = ((uint8_t)(cardData->cardExpirationDate[1]) - 48) + ((uint8_t)(cardData->cardExpirationDate[0]) - 48) * 10;
	if ( ((strlen(cardData->cardExpirationDate) == 5) && ((cardData->cardExpirationDate) != NULL) && ExpirationMonth <= 12 && cardData->cardExpirationDate[2] == '/') )
		return CARD_OK;
	else
		return WRONG_EXP_DATE;
}

EN_cardError_t getCardPAN(ST_cardData_t* cardData)
{
	printf("Enter Card PAN : ");
	gets(cardData->primaryAccountNumber);
	if ((strlen(cardData->primaryAccountNumber) > 19) || (strlen(cardData->primaryAccountNumber) < 16) || ((cardData->primaryAccountNumber) == NULL))
		return WRONG_PAN;
	else
		return CARD_OK;
}

void printCardError(EN_cardError_t error)
{
	switch (error)
	{
	case CARD_OK:
		printf("CARD_OK");
		break;
	case WRONG_NAME:
		printf("WRONG_NAME");
		break;
	case WRONG_EXP_DATE:
		printf("WRONG_EXP_DATE");
	case WRONG_PAN:
		printf("WRONG_PAN");
		break;
	}
}

void getCardHolderNameTest(void)
{
	ST_cardData_t cardData;
	printf("Tester Name : Mahmoud Ahmed\n");
	printf("Function Name: getCardHolderName\n");

	printf("\nTest case 1: \n");
	printf("Input Data : Mahmoud Ahmed Ibrahim \n");
	printf("Expected Result : 0\n"); // CARD_OK
	printf("Actual Result : %d\n", getCardHolderName(&cardData));

	printf("\nTest case 2: \n");
	printf("Input Data : Mahmoud Ahmed Ibrahim Ahmed\n");
	printf("Expected Result : 1\n"); // WRONG_NAME ( because it exceeds 24 )
	printf("Actual Result : %d\n", getCardHolderName(&cardData));

	printf("\nTest case 3: \n");
	printf("Input Data : Mahmoud Ahmed\n");
	printf("Expected Result : 1\n"); // WRONG_NAME ( because it is less than 20 )
	printf("Actual Result : %d\n", getCardHolderName(&cardData));
}

void getCardExpiryDateTest(void)
{
	ST_cardData_t cardData;
	printf("\nTester Name : Mahmoud Ahmed\n");
	printf("Function Name: getCardExpiryDate\n");

	printf("\nTest case 1: \n");
	printf("Input Data : 02/22 \n");
	printf("Expected Result : 0\n"); // CARD_OK
	printf("Actual Result : %d\n", getCardExpiryDate(&cardData));

	printf("\nTest case 2: \n");
	printf("Input Data : 2/24\n");
	printf("Expected Result : 2\n"); // WRONG_EXP_DATE
	printf("Actual Result : %d\n", getCardExpiryDate(&cardData));

	printf("\nTest case 3: \n");
	printf("Input Data : 1/022\n");
	printf("Expected Result : 2\n"); // WRONG_EXP_DATE
	printf("Actual Result : %d\n", getCardExpiryDate(&cardData));

	printf("\nTest case 4: \n");
	printf("Input Data : 24/05\n");
	printf("Expected Result : 2\n"); // WRONG_EXP_DATE
	printf("Actual Result : %d\n", getCardExpiryDate(&cardData));
}

void getCardPANTest(void)
{
	ST_cardData_t cardData;
	printf("\nTester Name : Mahmoud Ahmed\n");
	printf("Function Name: getCardPAN\n");

	printf("\nTest case 1: \n");
	printf("Input Data : 5338956234781234 \n"); // 16 digit
	printf("Expected Result : 0\n"); // CARD_OK
	printf("Actual Result : %d\n", getCardPAN(&cardData));

	printf("\nTest case 2: \n");
	printf("Input Data : 53389562347812349034 \n"); // 20 digit
	printf("Expected Result : 3\n"); // WRONG_PAN
	printf("Actual Result : %d\n", getCardPAN(&cardData));

	printf("\nTest case 3: \n");
	printf("Input Data : 533895623478123 \n"); // 15 digit
	printf("Expected Result : 3\n"); // WRONG_PAN
	printf("Actual Result : %d\n", getCardPAN(&cardData));
}
