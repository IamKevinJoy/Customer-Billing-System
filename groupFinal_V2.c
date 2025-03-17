/**********
C&Linux Final: Group Project

Programmers: Carlos Vega, Bao Nguyen, Kevin Joy
Due: 12/13/2022

1) Customer Billing System:
It should be able to take input from the user on how many watts of
electricity are used, plus gas used, and combine the cost and give out
a bill. Bill needs to be stored in a file for future references and could
be looked at by customer or the billing company. Data should be displayed
like a table when requested. Your program should be able to print the
sorted data.
**********/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

// Define cost per unit of measurement
#define WATTS 0.19
#define THERMS 0.95

// Define global variables
int status, prevType, searchType, credLen;
float month, year, gas, electricity, cost;
char pause, user[15], pass[15], fileName[30];
bool overrideKey = false;

/// Function prototypes
int newBill(float year, float month, FILE *file, bool overrideKey);    // Create a new bill and store it in a file
void prevBill(int prevType, FILE *file);    // Acesses previous bills from data file
void wipeData(FILE *file);    // Wipes all data in file
int commands();    // Prints commands for user
void swap(float *xp, float *yp);    // Swaps bill information
void sorting(FILE *file);    // Sorts bills in data file
int fileLength(FILE *file, int status);    // Calculates the length of the banking file
void bubbleSort(float array[][5], int arrayLen);    // Sorts data using bubble sort
bool binarySearch(FILE *file, int searchType, float year, float month);    // Searches dates in the file
void billCheck(FILE *file, float year, float month);    // Checks if a data is already exist
void printYear(FILE *file, float inputYear);    // Prints every bill in a year
void overWrite(FILE *file, float inputYear, float inputMonth);    // Overwrites date in file
void login(char* fileName);
int loginCommands();


int main(void)
{
    // Login menu
    login(fileName);
    printf("%s  ????\n", fileName);

    // Prompts user for a command
    status = commands();

    FILE *file;
    // Checks if file can be opened
    if ((file = fopen(fileName, "a+")) == NULL){
            puts("ERROR: File could not be opened");
    }
    else {
        // While program is runnning
        while(status != 0)
        {
            file = fopen(fileName, "a+");

            // If user wants enter a new bill
            if(status == 1) {
                // Header for new bill
                system("cls");
                printf("=================================\n");
                printf("%24s", "NEW ENERGY BILL\n");
                printf("=================================\n");
                printf("*Year must be valid (1950-9999)\n");
                printf("*Month must be valid (1-12)\n\n");


                // Ask user for date
                printf("Enter the YEAR of bill: ");
                scanf("%f", &year);
                while((year < 1950) || (9999 < year)){
                    printf(" Invalid Year\n");
                    printf("Enter the YEAR of bill: ");
                    scanf("%f", &year);
                }
                printf("Enter the MONTH of bill: ");
                scanf("%f", &month);
                while((month < 1) || (12 < month)) {
                    printf(" Invalid Month\n");
                    printf("Enter the MONTH of bill: ");
                    scanf("%f", &month);
                }

                // Creates new bill
                newBill(year, month, file, false);

                // Closes and reopens file to update
                fclose(file);
                fopen(fileName, "a+");
            }

            // If user wants to see a previous bill
            else if(status == 2) {
                // Header for previous bills
                system("cls");
                printf("=================================\n");
                printf("%27s", "ENERGY BILL ARCHIVE\n");
                printf("=================================\n\n");

                // Prints User Commands
                printf("Enter 1 to view bill of a specific date\n"
                        "Enter 2 to view all bills of a specific year\n"
                        "Enter 0 to return to main menu\n:");
                scanf("%d", &prevType);

                // Sorts bills in file and checks previous bills
                sorting(file);
                prevBill(prevType, file);
            }

            // If user wants to wipe data
            else if(status == 3) {
                wipeData(file);

                // Header for file wipe
                system("cls");
                printf("File successfully wiped!\n");
                printf("Enter any key to continue: ");
                scanf("%s", &pause);
            }

            // Invalid command
            else {
                printf("Invalid Command\n\n");
            }

            // Prompt user for another command
            status = commands();
        }
    }
    // Close the file
    system("cls");
    printf("THANK YOU FOR USING OUR PROGRAM. PLS DONATE MONEY FOR COLLEGE TUITION");
    fclose(file);
}


int newBill(float year, float month, FILE *file, bool overrideKey) {
    int overwriteCheck;

    // If date does not exists in file OR If override is permitted
    if((binarySearch(file, 1, year, month) == false) || (overrideKey == true)) {
        // Enter gas and electricity
        puts("");
        printf("Enter how much gas used: ");
        scanf("%f", &gas);
        printf("Enter watts of electricty used: ");
        scanf("%f", &electricity);

        // Calculate Costs
        cost = (gas * THERMS) + (electricity * WATTS);

        // Print and Store Costs
        printf("The cost of electricity and gas is: $%.2f \n\n", cost);
        fprintf(file, "%4.0f %2.0f %.2f %.2f %.2f\n", year, month, gas, electricity, cost);

        // Pause
        printf("Enter any key to continue: ");
        scanf("%s", &pause);
    }
    // If date already exists in the file
    else{
        // Asks if user wants to overwrite date
        printf("\n");
        printf("File already exists!\n"
                "Do you want to overide the existing bill? (1 for yes) (0 for no)\n");
        scanf("%d", &overwriteCheck);

        while((overwriteCheck != 1) && (overwriteCheck != 0)){
            printf("Invalid input\n");
            scanf("%d", &overwriteCheck);
        }

        // If user wants to over write the date
        if(overwriteCheck == 1){
            overWrite(file, year, month);
        }
    }
}


void prevBill(int prevType, FILE *file) {
    // Search for specific bill
    if(prevType == 1) {
        system("cls");
        // Prints header
        printf("=================================\n");
        printf("%26s", "BILL SEARCH BY DATE\n");
        printf("=================================\n");
        printf("*Year must be valid (1950-9999)\n");
        printf("*Month must be valid (1-12)\n\n");

        // Gets year and month of bill from user
        printf("Enter the YEAR of bill: ");
        scanf("%f", &year);
        while((year < 1950) || (9999 < year)){
            printf(" Invalid Year\n");
            printf("Enter the YEAR of bill: ");
            scanf("%f", &year);
        }
        printf("Enter the MONTH of bill: ");
        scanf("%f", &month);
        while((month < 1) || (12 < month)) {
            printf(" Invalid Month\n");
            printf("Enter the MONTH of bill: ");
            scanf("%f", &month);
        }
        binarySearch(file, 0, year, month);
    }
    // Displays all bills of a specific year
    else if(prevType == 2) {
        system("cls");
        //Prints header
        printf("=================================\n");
        printf("%26s", "BILL SEARCH BY YEAR\n");
        printf("=================================\n");
        printf("*Year must be valid (1950-9999)\n\n");

        // Gets year from user
        printf("Enter the YEAR of bill: ");
        scanf("%f", &year);
        while((year < 1950) || (9999 < year)){
            printf(" Invalid Year\n");
            printf("Enter the YEAR of bill: ");
            scanf("%f", &year);
        }
        printYear(file, year);
    }
    else {
        printf("Invalid Command");
    }
}


void wipeData(FILE *file) {
    // Opens file as 'write' to delete all data
    fopen(fileName, "w");

    // Reopens file for appending
    fclose(file);
    fopen(fileName, "a+");
}


int commands() {
    system("cls");
    // Header for Program
    printf("=================================\n");
    printf("%27s","ENERGY BILLING SYSTEM\n");
    printf("=================================\n");
    printf("%24s", ":Main Menu:\n\n");

    // Prompts user for command
    printf("Enter 1 to enter the new bill.\n"
        "Enter 2 to see the previous bills.\n"
        "Enter 3 to wipe the file.\n"
        "Enter 0 to exit the program.\n:");
    scanf("%d", &status);
    printf("\n");

    // Returns user's command
    return status;
}


void overWrite(FILE * file, float inputYear, float inputMonth) {
    // Initialize lengths and array for data
    int arrayLen = fileLength(file, 1);
    float billData[arrayLen-1][5];
    int index = 0;

    rewind(file);
    // Stores banking data from file to array
    while(index < arrayLen) {
        fscanf(file, "%4f %2f %f %f %f\n", &year, &month, &gas, &electricity, &cost);
        if ((year != inputYear) && (month != inputMonth)) {
            billData[index][0] = year;
            billData[index][1] = month;
            billData[index][2] = gas;
            billData[index][3] = electricity;
            billData[index][4] = cost;
            index++;
        }
    } 

    // Wipes old data from the file
    wipeData(file);

    // Prints data back into the file
    for(int index = 0; index < (arrayLen - 1); index++) {
        fprintf(file, "%4.0f %2.0f %.2f %.2f %.2f\n", billData[index][0], billData[index][1], billData[index][2],
                                                    billData[index][3], billData[index][4]);                                          
    }

    // Creates a new bill for overwriting
    newBill(inputYear, inputMonth, file, true);
}


bool binarySearch(FILE *file, int searchType, float inputyear, float inputmonth) {
    // Initialize lengths and array for data
    int arrayLen = fileLength(file, 1);
    float billData[arrayLen][5];

    // Initialize variables for searching
    int low = 0; 
    int high = fileLength(file, 1);   
    int mid = arrayLen / 2;
    
    rewind(file);
    // Stores banking data from file to array
    for(int index = 0; index < arrayLen; index++) {
        fscanf(file, "%4f %2f %f %f %f\n", &year, &month, &gas, &electricity, &cost);
        billData[index][0] = year;
        billData[index][1] = month;
        billData[index][2] = gas;
        billData[index][3] = electricity;
        billData[index][4] = cost;
    }

    // Search Algorithm
    while (low <= high) {
        // If date is matches
        if((billData[mid][0] == inputyear) && (billData[mid][1] == inputmonth)) {
            // If searchType is set to search
            if(searchType == 0) {
                printf("\n+-------------------------------------+\n");
                printf("| Year | Month | Gas | Electr |  Cost |\n");
                printf("|------|-------|-----|--------|-------|\n");
                printf("| %4.0f | %5.0f | %3.0f | %6.0f | %5.2f |\n", billData[mid][0], 
                                                                billData[mid][1], billData[mid][2],
                                                                billData[mid][3], billData[mid][4]);
                printf("+-------------------------------------+\n") ;    
                break;          
            }
            // If searchType is set to check
            else if(searchType == 1){                                        
                return true;
            }
        }
        // If year matches but month doesn't match
        else if((billData[mid][0] == inputyear) && (billData[mid][1] < inputmonth)) {
            // If month doesn't match and is less then desired month
            if (billData[mid][1] < inputmonth) {
                low = mid + 1;
                mid = (low + high)/2;
            }
            // If month doesn't match and is more then desired month
            else {
                high = mid - 1;
                mid = (low + high)/2;
            }
        }
        // If year doesn't match and is less then desired year
        else if(billData[mid][0] < inputyear) {
            low = mid + 1;
            mid = (low + high)/2;
        }
        // If year doesn't match and is more then desired year
        else {
            high = mid - 1;
            mid = (low + high)/2;
        }
        // If date not found
        if(low > high){
            if(searchType == 0) {
                printf("The date is not in the data!\n");
            }
            else if(searchType == 1) {
                return false;
            }
        }
    }
    // Pause
    printf("\nEnter any key to continue: ");
    scanf("%s", &pause);
}


void printYear(FILE *file, float inputYear){
    //initialize the array
    float billData[12][5];
    int arrayLen = fileLength(file, 1);
    int arrIndex = 0;
    float totalCost = 0;

    rewind(file);
    //stores the bill data into the array billData
    for(int index = 0; index < arrayLen; index++) {
        fscanf(file, "%4f %2f %f %f %f\n", &year, &month, &gas, &electricity, &cost);
        
        if(year == inputYear){
            billData[arrIndex][0] = year;
            billData[arrIndex][1] = month;
            billData[arrIndex][2] = gas;
            billData[arrIndex][3] = electricity;
            billData[arrIndex][4] = cost;
            arrIndex++;
        }
    }

    // Prints data from year to user
    printf("\n+-------------------------------------+\n");
    printf("| Year | Month | Gas | Electr |  Cost |\n");
    printf("|------|-------|-----|--------|-------|\n");   
    for(int index = 0; index < arrIndex; index++) {
        if(billData[index][0]  != 0) {
            printf("| %4.0f | %5.0f | %3.0f | %6.0f | %5.2f |\n", billData[index][0], 
                                                    billData[index][1], billData[index][2],
                                                    billData[index][3], billData[index][4]);
            totalCost += billData[index][4];                                         
        } 
    }
    printf("+-------------------------------------+\n") ; 
    printf("               | Total Cost: %8.2f |\n", totalCost);
    printf("               +----------------------+\n");

    // Pause
    printf("\nEnter any key to continue: ");
    scanf("%s", &pause);
}


                                            //////////////// FUNCTIONS FOR SORTING ////////////////
// Swaps data from one index with another
void swap(float *xp, float *yp) {
    float temp = *xp;
    *xp = *yp;
    *yp = temp;
}

// Bubble sort algorithm for sortng bills
void bubbleSort(float array[][5], int arrayLen) {
    for(int i = 0; i < arrayLen; i++) {
        for(int j = 0; j < arrayLen-1; j++) {
            // If year is greater than year of next row
            if(array[j][0] == array[j+1][0]) {
                if(array[j][1] > array[j + 1][1]) {
                    for(int k = 0; k < 5; k++)
                        swap(&array[j][k], &array[j+1][k]);
                }
            }
            else if(array[j][0] > array[j + 1][0]) {
                // For each column of data
                for(int k = 0; k < 5; k++)
                    swap(&array[j][k], &array[j+1][k]);
            }
        }
    }
}

// Accepts bill data from a file
void sorting(FILE *file) {
    int arrayLen = fileLength(file, 1);

    // Creates array for storing banking data
    float billData[arrayLen][5];
    rewind(file);

    // Stores banking data from file to array
    for(int index = 0; index < arrayLen; index++) {
        fscanf(file, "%4f %2f %f %f %f\n", &year, &month, &gas, &electricity, &cost);
        billData[index][0] = year;
        billData[index][1] = month;
        billData[index][2] = gas;
        billData[index][3] = electricity;
        billData[index][4] = cost;
    }
    // Puts data through sorting array
    bubbleSort(billData, arrayLen);

    // Prints sorted data back into the file
    wipeData(file);    //clean out the file before writing into the file
    for(int index = 0; index < arrayLen; index++) {
        fprintf(file, "%4.0f %2.0f %.2f %.2f %.2f\n", billData[index][0], billData[index][1], billData[index][2],
                                                    billData[index][3], billData[index][4]);
    }
}

// Counts number of lines in a file
int fileLength(FILE *file, int status) {
    int lineCount = 0;
    rewind(file);

    char c;
    // Scans each line of the file and adds 1 to count for each line
    for(c = getc(file); c != EOF; c = getc(file))
        if(c == '\n')
            lineCount += 1;

    return lineCount;
}


/*
2003  2 58.00 91.00 72.39
2005  6 99.00 58.00 105.07
2007  8 69.00 42.00 73.53
2009  5 25.00 68.00 36.67
2022  5 45.00 68.00 55.67
2022  6 45.00 68.00 55.67
2022  7 45.00 68.00 55.67
2022  8 45.00 68.00 55.67
2022 10 45.00 68.00 55.67
2022 12 89.00 36.00 91.39
*/

void login(char* fileName) {
    char username[15], password[15];
    FILE *account;
    FILE *credentials;

    status = loginCommands();

    while (status != 0) {
        // Want to login
        if(status == 1) {
            system("cls");
            // Header for Login
            printf("=================================\n");
            printf("%24s","ACCOUNT LOGIN\n");
            printf("=================================\n");

            printf("Enter Username: ");
            scanf("%s", username);
            printf("Enter Password: ");
            scanf("%s", password);
            
            // Create variable for text file named after user
            sprintf(fileName, "%s.txt", username);

            // If user doesn't exist
            if ((account = fopen(fileName, "r")) == NULL) {
                printf("\nUser does not exist\n");
                printf("Enter any key to continue: ");
                scanf("%s", &pause);
                
                status = loginCommands();
            }
            // If user exists
            else {
                // Opens file to store usernames/passwords
                credentials = fopen("LoginInfo.txt", "a+");
                credLen = fileLength(credentials, 2);

                rewind(credentials);
                // Searches through credential file
                for(int index = 0; index < credLen; index++) {
                    fscanf(credentials, "%s %s\n", user, pass);

                    // If username and password matches
                    if((strcmp(user, username) == 0) && (strcmp(pass, password) == 0)) {
                        printf("\nSuccessful login!\n");
                        fclose(credentials);

                        printf("Enter any key to continue: ");
                        scanf("%s", &pause);
                        return;
                    }
                }
                // If password is incorrect  
                printf("\nIncorrect password\n");
                printf("Enter any key to continue: ");
                scanf("%s", &pause);

                status = loginCommands();
            }
        }
        // New User
        else if(status == 2) {
            system("cls");
            // Header for Login
            printf("=================================\n");
            printf("%26s","ACCOUNT CREATION\n");
            printf("=================================\n");

            // Asks user for credentials
            printf("Enter Username: ");
            scanf("%s", username);
            printf("Enter Password: ");
            scanf("%s", password);

            // Checks if account doesn't exists
            sprintf(fileName, "%s.txt", username);
            if ((account = fopen(fileName, "r")) == NULL){
                // Creates account
                account = fopen(fileName, "a+");
                fclose(account);

                // Writes username and password to credential file
                credentials = fopen("LoginInfo.txt", "a+");
                fprintf(credentials, "%s %s\n", username, password);
                fclose(credentials);

                printf("\nAccount successfully created!\n");
                printf("Enter any key to continue: ");
                scanf("%s", &pause);
                
                status = loginCommands();                
            }
            // Account already exists
            else {
                puts("User already exists\n");
                printf("Enter any key to continue: ");
                scanf("%s", &pause);
                status = loginCommands();   
            }
        }
        // Invalid command
        else {
            printf("Invalid Command\n\n");
            printf("Enter any key to continue: ");
            scanf("%s", &pause);
                
            status = loginCommands(); 
        }
    }
}


int loginCommands() {
    system("cls");
    // Header for Login
    printf("=================================\n");
    printf("%27s","ENERGY BILLING LOGIN\n");
    printf("=================================\n");
    printf("%24s", ":Main Menu:\n\n");

    // Prompts user for command
    printf("Enter 1 to login.\n"
        "Enter 2 to create account.\n:");
    scanf("%d", &status);
    printf("\n");  

    return status;  
}