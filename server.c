#include <stdio.h>
#include <string.h>
#include"server.h"
/*
int main()
{
	recieveTransactionDataTest();
	isValidAccountTest();
	isBlockedAccountTest();
	isAmountAvailableTest();
	saveTransactionTest();
	return 0;
}
*/
ST_accountsDB_t accountsDB[255] = { { 10000.0, RUNNING, "5038621930765423" },
									{ 90000.0, RUNNING, "6432957813567019" },
									{  2000.0, RUNNING, "9037182034657910" },
									{ 50000.0, RUNNING, "3570641903215064" },
									{100000.0, BLOCKED, "8370291630574390" } };
ST_transaction_t transactionsDB[255];
uint32_t transactionNumber = 0;
uint8_t clientNumber;

/////////////////////////////////////////////////////    Main Functions    /////////////////////////////////////////////////////
EN_transState_t recieveTransactionData(ST_transaction_t* transData)
{
	ST_accountsDB_t account;
	if (isValidAccount(&transData->cardHolderData, &account) == ACCOUNT_NOT_FOUND)
	{
		printf("FRAUD_CARD\n");
		transData->transState = FRAUD_CARD;
		saveTransaction(transData);
		return FRAUD_CARD;
	}
	if (isBlockedAccount(&account) == BLOCKED_ACCOUNT)
	{
		printf("DECLINED_STOLEN_CARD\n");
		transData->transState = DECLINED_STOLEN_CARD;
		saveTransaction(transData);
		return DECLINED_STOLEN_CARD;
	}
	if (isAmountAvailable(&transData->terminalData, &account) == LOW_BALANCE)
	{
		printf("DECLINED_INSUFFECIENT_FUND\n");
		transData->transState = DECLINED_INSUFFECIENT_FUND;
		saveTransaction(transData);
		return DECLINED_INSUFFECIENT_FUND;
	}
	else
	{
		accountsDB[clientNumber].balance -= transData->terminalData.transAmount;
		printf("New balance = %f\n", accountsDB[clientNumber].balance);
	}
	transData->transState = APPROVED;
	if (saveTransaction(transData) == SAVING_FAILED)
		return INTERNAL_SERVER_ERROR;
	return APPROVED;
}

EN_serverError_t isValidAccount(ST_cardData_t* cardData, ST_accountsDB_t* accountReference)
{
	uint8_t exist = 0;
	for (int i = 0; i < 255; i++)
	{
		if (!strcmp((cardData->primaryAccountNumber), (accountsDB[i].primaryAccountNumber)))
		{
			exist = 1;
			clientNumber = i;
			break;
		}
	}
	if (exist == 1)
	{
		*accountReference = accountsDB[clientNumber];
		return SERVER_OK;
	}
	else
	{
		accountReference = NULL;
		return ACCOUNT_NOT_FOUND;
	}
}

EN_serverError_t isBlockedAccount(ST_accountsDB_t* accountRefrence)
{
	if (accountRefrence->state == RUNNING)
		return SERVER_OK;
	else
		return BLOCKED_ACCOUNT;
}

EN_serverError_t isAmountAvailable(ST_terminalData_t* termData, ST_accountsDB_t* accountRefrence)
{
	//printf("New balance = %d",)
	if (accountRefrence->balance >= termData->transAmount)
		return SERVER_OK;
	else
		return LOW_BALANCE;

}

EN_serverError_t saveTransaction(ST_transaction_t* transData)
{
	transData->transactionSequenceNumber = transactionNumber + 1;
	transactionsDB[transactionNumber] = *transData;
	transactionNumber++;
	listSavedTransactions();
	return SERVER_OK;
}

void listSavedTransactions(void)
{
	for (int i = 0; i < transactionNumber; i++)
	{
		printf("#########################\n");
		printf("Transaction Sequence Number : %d\n", transactionsDB[i].transactionSequenceNumber);
		printf("Transaction Date : %s\n", transactionsDB[i].terminalData.transactionDate);
		printf("Transaction Amount : %f\n", transactionsDB[i].terminalData.transAmount);
		printf("Transaction State : ");
		printTransState(transactionsDB[i].transState);
		printf("Terminal Max Amount : %f\n", transactionsDB[i].terminalData.maxTransAmount);
		printf("Cardholder Name : %s\n", transactionsDB[i].cardHolderData.cardHolderName);
		printf("PAN: %s\n", transactionsDB[i].cardHolderData.primaryAccountNumber);
		printf("Card Expiration Date : %s\n", transactionsDB[i].cardHolderData.cardExpirationDate);
		printf("#########################\n");
	}
}

/////////////////////////////////////////////////////    Print Functions    /////////////////////////////////////////////////////
void printTransState(EN_transState_t state)
{
	switch (state)
	{
	case APPROVED:
		printf("APPROVED\n");
		break;
	case DECLINED_INSUFFECIENT_FUND:
		printf("DECLINED_INSUFFECIENT_FUND\n");
		break;
	case DECLINED_STOLEN_CARD:
		printf("DECLINED_STOLEN_CARD\n");
		break;
	case FRAUD_CARD:
		printf("FRAUD_CARD\n");
		break;
	case INTERNAL_SERVER_ERROR:
		printf("INTERNAL_SERVER_ERROR\n");
		break;
	}
}

void printServerError(EN_serverError_t error)
{
	switch (error)
	{
	case SERVER_OK:
		printf("SERVER_OK\n");
		break;
	case SAVING_FAILED:
		printf("SAVING_FAILED\n");
		break;
	case TRANSACTION_NOT_FOUND:
		printf("TRANSACTION_NOT_FOUND\n");
		break;
	case ACCOUNT_NOT_FOUND:
		printf("ACCOUNT_NOT_FOUND\n");
		break;
	case LOW_BALANCE:
		printf("LOW_BALANCE\n");
		break;
	case BLOCKED_ACCOUNT:
		printf("BLOCKED_ACCOUNT\n");
		break;
	}
}

/////////////////////////////////////////////////////    Test Functions    /////////////////////////////////////////////////////
void recieveTransactionDataTest(void)
{
	EN_transState_t error;
	ST_terminalData_t terminal = {3000, 5000, "25/11/2022"};
	ST_cardData_t card = { "Mahmoud Ahmed Ibrahim", "5038621930765423", "10/25" };
	ST_transaction_t transaction = {card, terminal};

	printf("Tester Name : Mahmoud Ahmed\n");
	printf("Function Name: recieveTransactionData\n");

	printf("\nTest case 1: \n");
	printf("Input Data : PAN = 5038621930765423, TransAmount = 3000 \n");
	error = recieveTransactionData(&transaction);
	printf("Expected result : APPROVED\n");
	printf("Actual Result : ");
	printTransState(error);

	strcpy_s(transaction.cardHolderData.primaryAccountNumber, sizeof(transaction.cardHolderData.primaryAccountNumber), "6511346895137519");
	printf("\nTest case 2: \n");
	printf("Input Data : PAN = 6511346895137519, TransAmount = 3000 \n");
	error = recieveTransactionData(&transaction);
	printf("Expected result : FRAUD_CARD\n");
	printf("Actual Result : ");
	printTransState(error);

	strcpy_s(transaction.cardHolderData.primaryAccountNumber, sizeof(transaction.cardHolderData.primaryAccountNumber), "8370291630574390");
	printf("\nTest case 3: \n");
	printf("Input Data : PAN = 8370291630574390, TransAmount = 3000 \n");
	error = recieveTransactionData(&transaction);
	printf("Expected result : DECLINED_STOLEN_CARD\n");
	printf("Actual Result : ");
	printTransState(error);

	strcpy_s(transaction.cardHolderData.primaryAccountNumber, sizeof(transaction.cardHolderData.primaryAccountNumber), "9037182034657910");
	printf("\nTest case 4: \n");
	printf("Input Data : PAN = 9037182034657910, TransAmount = 3000 \n");
	error = recieveTransactionData(&transaction);
	printf("Expected result : DECLINED_INSUFFECIENT_FUND\n");
	printf("Actual Result : ");
	printTransState(error);
}


void isValidAccountTest(void)
{
	ST_accountsDB_t account;
	ST_cardData_t card;
	EN_serverError_t result;
	printf("\nTester Name : Mahmoud Ahmed\n");
	printf("Function Name: isValidAccount\n");

	printf("\nTest case 1: \n");
	printf("Input Data : 5038621930765423 \n");
	getCardPAN(&card);
	result = isValidAccount(&card, &account);
	printf("Expected Result : SERVER_OK account = { 10000.0, RUNNING, \"5038621930765423\" }\n");
	printf("Actual Result : ");
	printServerError(result);
	printf("account = {%f, %d, \"%s\" }\n", account.balance, account.state, account.primaryAccountNumber);

	printf("\nTest case 2: \n");
	printf("Input Data : 898937461543684 \n");
	getCardPAN(&card);
	result = isValidAccount(&card, &account);
	printf("Expected Result : ACCOUNT_NOT_FOUND \n");
	printf("Actual Result : ");
	printServerError(result);
}

void isBlockedAccountTest(void)
{
	printf("\nTester Name : Mahmoud Ahmed\n");
	printf("Function Name: isBlockedAccount\n");

	printf("\nTest case 1: \n");
	printf("Input Data : account[0] = { 10000.0, RUNNING, \"5038621930765423\" } \n");
	printf("Expected Result : SERVER_OK\n");
	printf("Actual Result : ");
	printServerError(isBlockedAccount(&accountsDB[0]));

	printf("\nTest case 2: \n");
	printf("Input Data : account[4] = {100000.0, BLOCKED, \"8370291630574390\" } \n");
	printf("Expected Result : BLOCKED_ACCOUNT\n");
	printf("Actual Result : ");
	printServerError(isBlockedAccount(&accountsDB[4]));
}

void isAmountAvailableTest(void)
{
	ST_terminalData_t terminal;
	printf("\nTester Name : Mahmoud Ahmed\n");
	printf("Function Name: isAmountAvailable\n");
	printf("Transaction Amount = 5000\n");
	getTransactionAmount(&terminal);

	printf("\nTest case 1: \n");
	printf("Input Data : account[2] = {  2000.0, RUNNING, \"9037182034657910\" } \n");
	printf("Expected Result : LOW_BALANCE\n");
	printf("Actual Result : ");
	printServerError(isAmountAvailable(&terminal, &accountsDB[2]));

	printf("\nTest case 2: \n");
	printf("Input Data : account[0] = { 10000.0, RUNNING, \"5038621930765423\" } \n");
	printf("Expected Result : SERVER_OK\n");
	printf("Actual Result : ");
	printServerError(isAmountAvailable(&terminal, &accountsDB[0]));
}

void saveTransactionTest(void)
{
	ST_cardData_t card_1 = { "Mahmoud Ahmed Ibrahim", "5038621930765423", "10/25"};
	ST_terminalData_t terminal_1 = {1000, 5000, "25/11/2022"};
	ST_transaction_t transaction_1 = {card_1, terminal_1, APPROVED };

	printf("\nTester Name : Mahmoud Ahmed\n");
	printf("Function Name: saveTransaction\n");

	printf("\nTest case 1: \n");
	saveTransaction(&transaction_1);

	ST_cardData_t card_2 = { "Mostafa Ahmed Ali", "6432957813567019", "09/26" };
	ST_terminalData_t terminal_2 = { 3000, 5000, "25/11/2022" };
	ST_transaction_t transaction_2 = { card_2, terminal_2, FRAUD_CARD };
	printf("\nTest case 2: \n");
	saveTransaction(&transaction_2);
}