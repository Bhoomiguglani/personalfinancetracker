#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int isValidDate(int day, int month, int year) {
    if (year < 1900 || year > 2100) return 0;   // Year check
    if (month < 1 || month > 12) return 0;      // Month check

    // Month ke hisaab se days check
    if (month == 1 || month == 3 || month == 5 || month == 7 || 
        month == 8 || month == 10 || month == 12) {
        if (day < 1 || day > 31) return 0;
    } 
    else if (month == 4 || month == 6 || month == 9 || month == 11) {
        if (day < 1 || day > 30) return 0;
    } 
    else if (month == 2) {//leap year
        if ((year % 400 == 0) || (year % 4 == 0 && year % 100 != 0)) {
            if (day < 1 || day > 29) return 0;
        } else {
            if (day < 1 || day > 28) return 0;
        }
    }

    return 1;
}

void viewstransactions() {
    char type[10];
    float amount;
    char category[30];
    char date[11];
    float totalincome = 0.0;    
    float totalexpense = 0.0;  

    FILE *fp = fopen("finance.txt", "r");
    if (fp == NULL) {
        printf("Abhi tak koi transaction nahi hai.\n");
        return;
    }

    printf("\n--- All Transactions ---\n");
    printf("\n%-10s %-10s %-15s %-12s\n", "Type", "Amount", "Category", "Date");
    printf("-------------------------------------------------------------\n");
    while (fscanf(fp, "%s %f %s %s", type, &amount, category, date) != EOF) {
        printf("%-10s %-10.2f %-15s %-12s\n", type, amount, category, date);
        if (strcasecmp(type, "Income") == 0)
            totalincome += amount;
        else if (strcasecmp(type, "Expense") == 0)
            totalexpense += amount;
    }

    printf("\nTotal Income: %.2f\n", totalincome);
    printf("Total Expense: %.2f\n", totalexpense);
    printf("Balance: %.2f\n", totalincome - totalexpense);

    if (totalexpense > totalincome) {
        printf("\nWarning: Aapka kharcha income se zyada hai!\n");
    }

    fclose(fp);
}

void addtransaction() {
    char type[10];     
    float amount;
    char category[30];
    char date[11];

    FILE *fp = fopen("finance.txt", "a");
    if (fp == NULL) {
        printf("Error: file nahi bani.\n");
        return;
    }

    printf("Transaction type (Income/Expense): ");
    scanf("%s", type);
    while(getchar() != '\n'); // clear buffer

    int day, month, year;

    if (strcasecmp(type, "Income") == 0) {
        strcpy(type, "Income");

        printf("Amount: ");
        scanf("%f", &amount);
        if(amount <= 0){
            printf("Invalid Amount! Hamesha 0 se bada hona chahiye\n");
            fclose(fp);
            while(getchar() != '\n'); 
            return;
        }
        while(getchar() != '\n');

        printf("Category (Salary/Bonus/others): ");   
        scanf("%s", category);
        while(getchar() != '\n');

        printf("Date(dd/mm/yyyy): ");
        scanf("%s", date);
        sscanf(date, "%d/%d/%d", &day, &month, &year);
        if (!isValidDate(day, month, year)) {
            printf("Invalid date! Transaction add nahi hua.\n");
            fclose(fp);
            return;
        }
    } 
    else if (strcasecmp(type, "Expense") == 0) {
        strcpy(type, "Expense");

        printf("Amount: ");
        scanf("%f", &amount);
        while(getchar() != '\n');

        printf("Category (Food/Travel/Rent/other): ");
        scanf("%s", category);
        while(getchar() != '\n');

        printf("Date (dd/mm/yyyy): ");
        scanf("%s", date);
        sscanf(date, "%d/%d/%d", &day, &month, &year);
        if (!isValidDate(day, month, year)) {
            printf("Invalid date! Transaction add nahi hua.\n");
            fclose(fp);
            return;
        }
    } 
    else {
        printf("Galat type! Sirf Income/Expense likho.\n");
        fclose(fp);
        return;
    }

    fprintf(fp, "%s %.2f %s %s\n", type, amount, category, date);
    printf("Transaction added successfully!\n");

    fclose(fp);
}

void searchtransactions() {
    int option;
    char searchDate[11];
    char type[10];
    float amount;
    char category[30];
    char date[11];
    int found = 0;
    char searchCategory[30];

    printf("\nSearch option:\n");
    printf("1. Search by date\n");
    printf("2. Search by category\n");
    printf("Enter your choice: ");
    scanf("%d",&option);
    while(getchar() != '\n');

    FILE *fp = fopen("finance.txt", "r");
    if (fp == NULL) {
        printf("Abhi tak koi transaction nahi hai.\n");
        return;
    }

    if(option == 1){
        printf("Search date (dd/mm/yyyy): ");
        scanf("%s", searchDate);

        printf("\n--- Transactions on %s ---\n", searchDate);
        while (fscanf(fp, "%s %f %s %s", type, &amount, category, date) != EOF) {
            if (strcmp(date, searchDate) == 0) {
                printf("Type: %s | Amount: %.2f | Category: %s | Date: %s\n", 
                        type, amount, category, date);
                found = 1;
            }
        }
    }
    else if(option == 2){
        printf("Search category: ");
        scanf("%s",searchCategory);

        printf("\n--- Transactions in category '%s' ---\n",searchCategory);
        while(fscanf(fp,"%s %f %s %s",type, &amount,category,date) != EOF){
            if(strcasecmp(category,searchCategory) == 0){  
                printf("Type: %s | Amount: %.2f | Category: %s | Date: %s\n",
                        type, amount, category, date);
                found = 1;
            }
        }
    }
    else {
        printf("Invalid search option!\n");
        fclose(fp);
        return;
    }

    if(!found){
        if(option == 1) printf("Is date pe koi transaction nahi mila.\n");
        else if(option == 2) printf("Is category me koi transaction nahi mila.\n");
    }

    fclose(fp);
}

int main() {
    int choice;
    int transactionCount = 0;

    while (1) {
        printf("\n--- Personal Finance Tracker ---\n");
        printf("1. View Transactions\n");
        printf("2. Add Transaction\n");
        printf("3. Search Transactions\n"); 
        printf("4. Exit\n"); 
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input! Number likho.\n");
            while(getchar() != '\n');
            continue;
        }

        switch(choice) {
            case 1:
                viewstransactions();
                break; 

            case 2:
                addtransaction();
                transactionCount++;
                if(transactionCount == 2) {
                    printf("\nTransactions add ho chuki hain.\n");
                }
                break;  // BREAK added to avoid fall-through

            case 3:
                searchtransactions();
                break;  // BREAK added

            case 4:
                printf("Exit... bye! bye\n");
                exit(0);

            default:
                printf("Invalid choice! Try again.\n");
        }
    }
    return 0;
}
