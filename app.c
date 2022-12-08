#include <stdio.h>
#include <string.h>
#include "app.h"


int main()
{
	printf("########################## WELCOME ##########################\n");
	printf("Max Allowed Amount = 5000\n");
	appStart();
	return 0;
}


void appStart(void)
{
	ST_cardData_t client;
	EN_cardError_t cardError;
	cardError = getCardHolderName(&client);
	if (cardError != CARD_OK)
	{
		printCardError(cardError);
		return;
	}
	cardError = getCardExpiryDate(&client);
	if (cardError != CARD_OK)
	{
		printCardError(cardError);
		return;
	}
	cardError = getCardPAN(&client);
	if (cardError != CARD_OK)
	{
		printCardError(cardError);
		return;
	}
	if (isValidCardPAN(&client) == INVALID_CARD)
	{
		printf("INVALID_CARD");
		return;
	}

	ST_terminalData_t terminal;
	getTransactionDate(&terminal);
	if (isCardExpired(&client, &terminal) == EXPIRED_CARD)
	{
		printf("EXPIRED_CARD");
		return;
	}
	
	setMaxAmount(&terminal, 5000);
	getTransactionAmount(&terminal);
	if (isBelowMaxAmount(&terminal) == EXCEED_MAX_AMOUNT)
	{
		printf("EXCEED_MAX_AMOUNT");
		return;
	}
	
	ST_transaction_t transaction;
	transaction.cardHolderData = client;
	transaction.terminalData = terminal;
	recieveTransactionData(&transaction);
}
