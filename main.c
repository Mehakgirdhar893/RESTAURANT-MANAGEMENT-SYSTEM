#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h> // for sleep()
// restaurant management system
struct panel
{
    char uid[10];
    char pwd[10];
};

struct Menu
{
    char pname[20];
    int price;
    char category[20];
    int quantity;
};

struct record
{
    char name[20];
    int phone;
    char address[30];
};

// ADMIN REGISTER

void registerAdmin()
{
    struct panel person;
    FILE *ptr;
    ptr = fopen("restaurant.txt", "w");

    fflush(stdin);
    printf("Enter the uid: ");
    gets(person.uid);
    fflush(stdin);
    printf("Enter the password: ");
    gets(person.pwd);

    fwrite(&person, sizeof(person), 1, ptr);
    fclose(ptr);

    printf("\nHey %s, now you are the admin!", person.uid);
}


// CHECK IF CUSTOMER EXISTS

int check(int pno)
{
    struct record customer;
    FILE *ptr;
    int found = 0;

    ptr = fopen("rest.txt", "r");
    if (ptr == NULL)
        return 0;

    while (fread(&customer, sizeof(customer), 1, ptr))
    {
        if (customer.phone == pno)
        {
            found = 1;
            break;
        }
    }
    fclose(ptr);

    return found;
}


// REGISTER NEW CUSTOMER

void createNewCustomer()
{
    struct record customer;
    int k;
    FILE *ptr;
    ptr = fopen("rest.txt", "a");

    fflush(stdin);
    printf("Enter your name: ");
    gets(customer.name);
    fflush(stdin);
    printf("Enter the phone: ");
    scanf("%d", &customer.phone);
    fflush(stdin);
    printf("Enter the address: ");
    gets(customer.address);

    k = check(customer.phone);

    if (k == 1)
    {
        printf("\nAlready exists, try with a new number!");
    }
    else
    {
        fwrite(&customer, sizeof(customer), 1, ptr);
        printf("\nHey %s, you are registered successfully!", customer.name);
    }

    fclose(ptr);
}


// OLD CUSTOMER LOGIN

void oldCustomer()
{
    int pno, k;
    printf("Enter the phone: ");
    scanf("%d", &pno);
    k = check(pno);

    if (k == 0)
    {
        printf("\nNot registered. Please register first........");
        createNewCustomer();
    }
    else
    {
        printf("\nWelcome back, valued customer!");
    }
}


// ADMIN VERIFICATION WITH 3 ATTEMPTS

int verifiedAdmin()
{
    struct panel customer;
    char x[10], y[10];
    int k1, k2;
    int attempts = 0;

    while (attempts < 3)
    {
        FILE *ptr = fopen("restaurant.txt", "r");
        if (ptr == NULL)
        {
            printf("\nNo admin registered yet. Register first.");
            return 0;
        }

        fread(&customer, sizeof(customer), 1, ptr);
        fclose(ptr);

        fflush(stdin);
        printf("\nEnter user id: ");
        gets(x);
        fflush(stdin);
        printf("Enter user password: ");
        gets(y);

        k1 = strcmp(customer.uid, x);
        k2 = strcmp(customer.pwd, y);

        if (k1 == 0 && k2 == 0)
        {
            printf(" Verified successfully!\n");
            return 1;
        }
        else
        {
            printf("Wrong credentials. Try again.\n");
            attempts++;
        }

        if (attempts == 3)
        {
            printf(" You have reached your limit. Try again after 30 seconds.\n");
            int sec;
            for (sec = 30; sec >= 1; sec--)
            {
                printf("\rPlease wait %d seconds...", sec);
                fflush(stdout);
                sleep(1);
            }
            printf("\nNow you can try again!");
            attempts = 0; // reset attempts
        }
    }
    return 0;
}


// REGISTER PRODUCTS

void registerProduct()
{
    struct Menu product;
    FILE *ptr;
    int i = 1;

    ptr = fopen("product.txt", "a");

    while (i)
    {
        fflush(stdin);
        printf("\nEnter the product name: ");
        gets(product.pname);

        printf("Enter the product price: ");
        scanf("%d", &product.price);

        fflush(stdin);
        printf("Enter the product category: ");
        gets(product.category);

        printf("Enter the quantity available: ");
        scanf("%d", &product.quantity);

        fwrite(&product, sizeof(product), 1, ptr);
        printf(" %s registered successfully!\n", product.pname);

        printf("\nPress 1 for more product, else 0: ");
        scanf("%d", &i);
    }
    fclose(ptr);
}


// DISPLAY PRODUCTS

void displayProducts()
{
    struct Menu product;
    FILE *ptr;
    ptr = fopen("product.txt", "r");

    if (ptr == NULL)
    {
        printf("\nNo products available.");
        return;
    }

    printf("\n--- Product List ---\n");
    printf("\nName \tPrice \tCategory \tQty \tTotal \n");
    printf("\n-----------------------------------------------\n");

    while (fread(&product, sizeof(product), 1, ptr))
    {
        int total = product.price * product.quantity;
        printf("%-15s %-8d -15%s %-8d %-8d\n", product.pname, product.price, product.category, product.quantity, total);
    }

    fclose(ptr);
}


void placeOrder()
{
    struct Menu product;
    FILE *ptr;
    char searchName[20];
    int quantity, found = 0, total = 0;

    ptr = fopen("product.txt", "r");
    if (ptr == NULL)
    {
        printf("\nNo products available to order!");
        return;
    }

    printf("\nEnter product name to order: ");
    fflush(stdin);
    gets(searchName);

    while (fread(&product, sizeof(product), 1, ptr))
    {
        if (strcmp(product.pname, searchName) == 0)
        {
            found = 1;
            printf("\nProduct found: %s", product.pname );
            printf("\nCategory: %s", product.category);
            printf("\nPrice per unit: %d", product.price);
            printf("\nAvailable quantity: %d", product.quantity);

            printf("\nEnter quantity to order: ");
            scanf("%d", &quantity);

            if (quantity > product.quantity)
            {
                printf("Not enough stock! Only %d left.\n", product.quantity);
            }
            else
            {
                total = quantity * product.price;
                printf(" \n Order placed successfully!\n");
                printf("You ordered %d x %s = Total: %d\n", quantity, product.pname, total);
            }
            break;
        }
    }
    fclose(ptr);

    if (!found)
        printf("\n Product not found.");
}



// MAIN

int main()

{
    int type, count = 0, choice;
    int x, i = 1;

    while (i)
    {
    again:
        printf("\nPress 1: for Admin");
        printf("\nPress 2: for Customer");
        printf("\nEnter type: ");
        scanf("%d", &type);

        if (type == 1)
        {
            x = verifiedAdmin();
            if (x == 1)
            {
                printf("\n--- Welcome Admin ---");
                printf("1. Register Product\n");
                printf("2. Display Products\n");
                printf("Enter your choice: ");
                scanf("%d", &choice);

                if (choice == 1)
                    registerProduct();
                else if (choice == 2)
                {
                	displayProducts();
                	if(x==1)
					{
						printf("\n_ _ _Welcome Admin_ _ _");
					}
				}    
                else
                    printf("Invalid choice!\n");
            }
        }
        else if (type == 2)
        {
            printf("\n1. New Customer");
            printf("\n2. Old Customer");
            printf("\nEnter your choice: ");
            scanf("%d", &choice);

            if (choice == 1)
                createNewCustomer();
            else if (choice == 2)
               {
				oldCustomer();
				displayProducts();
				placeOrder();
			   }
            else
                printf("Wrong choice!\n");
        }
        else
        {
            printf("\nTry again...");
            goto again;
        }

        printf("\nPress 1 for more, else 0: ");
        scanf("%d", &i);
        while (getchar() != '\n');
    }

    printf("\nExiting system... Goodbye see u again :) !");
    return 0;
}
