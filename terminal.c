#include <stdio.h>
#include <string.h>
#include "terminal.h"
/*
int main()
{
	getTransactionDateTest();
	isCardExpriedTest();
	getTransactionAmountTest();
	setMaxAmountTest();
	isBelowMaxAmountTest();
	isValidCardPANTest();
	return 0;
}
*/
EN_terminalError_t getTransactionDate(ST_terminalData_t* termData)
{
	printf("Enter Transaction Date : ");
	gets(termData->transactionDate);//03/11/2024
	uint8_t TransactionMonth = ((termData->transactionDate[4]) - 48) + ((termData->transactionDate[3]) - 48) * 10;
	if ((strlen(termData->transactionDate) == 10) && ((termData->transactionDate) != NULL) && TransactionMonth <= 12 && termData->transactionDate[2] == '/' && termData->transactionDate[5] == '/')
		return TERMINAL_OK;
	else
		return WRONG_DATE;
}

EN_terminalError_t isCardExpired(ST_cardData_t* cardData, ST_terminalData_t* termData)
{
	uint8_t ExpirationMonth = ((cardData->cardExpirationDate[1]) - 48 ) + ( (cardData->cardExpirationDate[0]) - 48 ) * 10;
	uint8_t ExpirationYear = ( (cardData->cardExpirationDate[4]) - 48 ) + ( (cardData->cardExpirationDate[3]) - 48 ) * 10;
	
	uint8_t TransactionMonth = ((termData->transactionDate[4]) - 48) + ((termData->transactionDate[3]) - 48) * 10;
	uint8_t TransactionYear = ((termData->transactionDate[9]) - 48) + ((termData->transactionDate[8]) - 48) * 10;
	//printf("\nExpMon %d ExpYear %d\nTranMon %d TranYear %d\n", ExpirationMonth, ExpirationYear, TransactionMonth, TransactionYear);
	
	if (TransactionYear < ExpirationYear)
		return TERMINAL_OK;
	else if ( (TransactionYear == ExpirationYear) && (TransactionMonth < ExpirationMonth) )
		return TERMINAL_OK;
	else
		return EXPIRED_CARD;
}

EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData)
{
	printf("Enter Transaction Amount : ");
	scanf_s("%f",&(termData->transAmount));
	if (termData->transAmount > 0)
		return TERMINAL_OK;
	else
		return INVALID_AMOUNT;
}

EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData)
{
	if (termData->transAmount > termData->maxTransAmount)
		return EXCEED_MAX_AMOUNT;
	else
		return TERMINAL_OK;
}

EN_terminalError_t setMaxAmount(ST_terminalData_t* termData, float maxAmount)
{
	termData->maxTransAmount = maxAmount;
	if (termData->maxTransAmount > 0)
		return TERMINAL_OK;
	else
		return INVALID_MAX_AMOUNT;
}

EN_terminalError_t isValidCardPAN(ST_cardData_t* cardData)
{
	uint8_t sum = 0;
	uint8_t num, controlDigit, actualDigit;
	for (int i = 0; i < strlen(cardData->primaryAccountNumber)-1; i++)
	{
		num = (cardData->primaryAccountNumber[i]) - 48;
		if (i % 2 == 0)
		{
			num *= 2;
			if (num < 10)
				sum += num;
			else
				sum += (num % 10) + 1;
		}
		else
			sum += num;
	}
	controlDigit = (10 - (sum % 10)) % 10;
	actualDigit = (cardData->primaryAccountNumber[strlen(cardData->primaryAccountNumber) - 1]) - 48;
	if (actualDigit == controlDigit)
		return TERMINAL_OK;
	else
		return INVALID_CARD;
}

void getTransactionDateTest(void)
{
	ST_terminalData_t termData;
	uint8_t result;
	printf("Tester Name : Mahmoud Ahmed\n");
	printf("Function Name: getTransactionDate\n");

	printf("\nTest case 1: \n");
	printf("Input Data : 14/11/2022 \n");
	result = getTransactionDate(&termData);
	printf("Expected Result : 0\n"); // TERMINAL_OK
	printf("Actual Result : %d\n", result);

	printf("\nTest case 2: \n");
	printf("Input Data : 1/1/2022\n");
	result = getTransactionDate(&termData);
	printf("Expected Result : 1\n"); // WRONG_DATE ( because it less than 10 )
	printf("Actual Result : %d\n", result);

	printf("\nTest case 3: \n");
	printf("Input Data : 2022/14/11\n");
	result = getTransactionDate(&termData);
	printf("Expected Result : 1\n"); // WRONG_DATE ( because it is Wrong format )
	printf("Actual Result : %d\n", result);

	printf("\nTest case 4: \n");
	printf("Input Data : 04/25/2025\n");
	result = getTransactionDate(&termData);
	printf("Expected Result : 1\n"); // WRONG_DATE ( because it is Wrong format )
	printf("Actual Result : %d\n", result);
}

void isCardExpriedTest(void)
{
	ST_cardData_t cardData;
	ST_terminalData_t termData;
	printf("\nTester Name : Mahmoud Ahmed\n");
	printf("Function Name: isCardExpried\n");
	printf("Expiration Date  10/24\n");
	getCardExpiryDate(&cardData); // 10/24
	
	printf("\nTest case 1: \n");
	printf("Input Data : 14/11/2022 \n");
	getTransactionDate(&termData);
	printf("Expected Result : 0\n"); // TERMINAL_OK
	printf("Actual Result : %d\n", isCardExpired(&cardData, &termData));

	printf("\nTest case 2: \n");
	printf("Input Data : 01/09/2025 \n");
	getTransactionDate(&termData);
	printf("Expected Result : 2\n"); // EXPIRED_CARD
	printf("Actual Result : %d\n", isCardExpired(&cardData, &termData));
	
	printf("\nTest case 3: \n");
	printf("Input Data : 01/10/2024 \n");
	getTransactionDate(&termData);
	printf("Expected Result : 2\n"); // EXPIRED_CARD
	printf("Actual Result : %d\n", isCardExpired(&cardData, &termData));
	
}

void getTransactionAmountTest(void)
{
	ST_terminalData_t termData;
	uint8_t result;
	printf("\nTester Name : Mahmoud Ahmed\n");
	printf("Function Name: getTransactionAmount\n");

	printf("\nTest case 1: \n");
	printf("Input Data : 100 \n");
	result = getTransactionAmount(&termData);
	printf("Expected Result : 0\n"); // TERMINAL_OK
	printf("Actual Result : %d\n", result);

	printf("\nTest case 2: \n");
	printf("Input Data : 0 \n");
	result = getTransactionAmount(&termData);
	printf("Expected Result : 4\n"); // INVALID_AMOUNT
	printf("Actual Result : %d\n", result);

	printf("\nTest case 3: \n");
	printf("Input Data : -5 \n");
	result = getTransactionAmount(&termData);
	printf("Expected Result : 4\n"); // INVALID_AMOUNT
	printf("Actual Result : %d\n", result);
}

void setMaxAmountTest(void)
{
	ST_terminalData_t termData;
	printf("\nTester Name : Mahmoud Ahmed\n");
	printf("Function Name: setMaxAmount\n");

	printf("\nTest case 1: \n");
	printf("Input Data : 100 \n");
	printf("Expected Result : 0\n"); // TERMINAL_OK
	printf("Actual Result : %d\n", setMaxAmount(&termData, 100));

	printf("\nTest case 2: \n");
	printf("Input Data : 0 \n");
	printf("Expected Result : 6\n"); // INVALID_MAX_AMOUNT
	printf("Actual Result : %d\n", setMaxAmount(&termData, 0));

	printf("\nTest case 3: \n");
	printf("Input Data : -5 \n");
	printf("Expected Result : 6\n"); // INVALID_MAX_AMOUNT
	printf("Actual Result : %d\n", setMaxAmount(&termData, -5));
}

void isBelowMaxAmountTest(void)
{
	ST_terminalData_t termData;
	printf("\nTester Name : Mahmoud Ahmed\n");
	printf("Function Name: isBelowMaxAmount\n");
	setMaxAmount(&termData, 500);
	printf("Max amount = 500\n");

	printf("\nTest case 1: \n");
	printf("Input Data : 100 \n");
	getTransactionAmount(&termData);
	printf("Expected Result : 0\n"); // TERMINAL_OK
	printf("Actual Result : %d\n", isBelowMaxAmount(&termData));

	printf("\nTest case 2: \n");
	printf("Input Data : 500 \n");
	getTransactionAmount(&termData);
	printf("Expected Result : 0\n"); // TERMINAL_OK
	printf("Actual Result : %d\n", isBelowMaxAmount(&termData));

	printf("\nTest case 3: \n");
	printf("Input Data : 1000 \n");
	getTransactionAmount(&termData);
	printf("Expected Result : 5\n"); // EXCEED_MAX_AMOUNT
	printf("Actual Result : %d\n", isBelowMaxAmount(&termData));
}

void isValidCardPANTest(void)
{
	ST_cardData_t cardData;
	uint8_t result;
	printf("\nTester Name : Mahmoud Ahmed\n");
	printf("Function Name: isValidCardPAN\n");

	printf("\nTest case 1: \n");
	printf("Input Data : 12345674 \n");
	getCardPAN(&cardData);
	result = isValidCardPAN(&cardData);
	printf("Expected Result : 0\n"); // CARD_OK
	printf("Actual Result : %d\n", result);

	printf("\nTest case 2: \n");
	printf("Input Data : 13245675 \n");
	getCardPAN(&cardData);
	result = isValidCardPAN(&cardData);
	printf("Expected Result : 0\n"); // CARD_OK
	printf("Actual Result : %d\n", result);

	printf("\nTest case 3: \n");
	printf("Input Data : 13245674 \n");
	getCardPAN(&cardData);
	result = isValidCardPAN(&cardData);
	printf("Expected Result : 3\n"); // INVALID_CARD
	printf("Actual Result : %d\n", result);

}
