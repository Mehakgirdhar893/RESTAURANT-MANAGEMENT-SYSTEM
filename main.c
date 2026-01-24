#include <stdio.h>
#include <string.h>

/* ================= STRUCTURES ================= */

struct Admin
{
    char id[20];
    char pass[20];
};

struct Menu
{
    int id;
    char name[30];
    int price;
};

struct Customer
{
    int cid;
    char name[30];
    char phone[15];
};

struct Order
{
    int cid;
    int food_id;
    int qty;
};

/* ================= ADMIN FUNCTIONS ================= */

/*void registerAdmin()
{
    struct Admin a;
    FILE *fp = fopen("admin.txt", "wb");

    if (fp == NULL)
    {
        printf("File error!\n");
        return;
    }

    printf("\nCreate Admin ID: ");
    scanf("%s", a.id);

    printf("Create Password: ");
    scanf("%s", a.pass);

    fwrite(&a, sizeof(a), 1, fp);
    fclose(fp);

    printf("\nAdmin registered successfully!\n");
}*/

int adminLogin()
{
    struct Admin a, temp;
    FILE *fp = fopen("admin.txt", "rb");

    if (fp == NULL)
    {
        printf("\nNo admin found. Please register first.\n");
        return 0;
    }

    printf("\nAdmin ID: ");
    scanf("%s", temp.id);

    printf("Password: ");
    scanf("%s", temp.pass);

    fread(&a, sizeof(a), 1, fp);
    fclose(fp);

    if (strcmp(a.id, temp.id) == 0 && strcmp(a.pass, temp.pass) == 0)
    {
        printf("\nLogin successful!\n");
        return 1;
    }

    printf("\nInvalid ID or Password!\n");
    return 0;
}

/* ================= MENU FUNCTIONS ================= */

void addFood()
{
    struct Menu m;
    FILE *fp = fopen("menu.txt", "ab");

    printf("\nFood ID: ");
    scanf("%d", &m.id);

    printf("Food Name: ");
    scanf("%s", m.name);

    printf("Price: ");
    scanf("%d", &m.price);

    fwrite(&m, sizeof(m), 1, fp);
    fclose(fp);

    printf("\nFood item added successfully!\n");
}

void viewMenu()
{
    struct Menu m;
    FILE *fp = fopen("menu.txt", "rb");

    if (fp == NULL)
    {
        printf("\nMenu is empty.\n");
        return;
    }

    printf("\n------ MENU ------\n");
    printf("ID\tName\t\tPrice\n");

    while (fread(&m, sizeof(m), 1, fp))
    {
        printf("%d\t%-10s\t%d\n", m.id, m.name, m.price);
    }

    fclose(fp);
}

/* ================= CUSTOMER FUNCTIONS ================= */

void addCustomer()
{
    struct Customer c;
    FILE *fp = fopen("customer.txt", "ab");

    printf("\nCustomer ID: ");
    scanf("%d", &c.cid);

    printf("Customer Name: ");
    scanf("%s", c.name);

    printf("Phone Number: ");
    scanf("%s", c.phone);

    fwrite(&c, sizeof(c), 1, fp);
    fclose(fp);

    printf("\nCustomer added successfully!\n");
}

int isCustomerExists(int cid)
{
    struct Customer c;
    FILE *fp = fopen("customer.txt", "rb");
    if (fp == NULL) return 0;

    while (fread(&c, sizeof(c), 1, fp))
    {
        if (c.cid == cid)
        {
            fclose(fp);
            return 1;  // Customer exists
        }
    }
    fclose(fp);
    return 0; // Customer not found
}

/* ================= ORDER & BILL ================= */

int getFoodPrice(int food_id)
{
    struct Menu m;
    FILE *fp = fopen("menu.txt", "rb");

    if (fp == NULL)
        return 0;

    while (fread(&m, sizeof(m), 1, fp))
    {
        if (m.id == food_id)
        {
            fclose(fp);
            return m.price;
        }
    }

    fclose(fp);
    return 0;
}

void placeOrder()
{
    struct Order o;
    int total = 0, price;
    int customerType, cid;

    printf("\nAre you a new or existing customer?");
    printf("\n1. New Customer");
    printf("\n2. Existing Customer");
    printf("\nChoice: ");
    scanf("%d", &customerType);

    if (customerType == 1)
    {
        addCustomer();  // Adds new customer
        printf("Enter your Customer ID to continue order: ");
        scanf("%d", &cid);
        if (!isCustomerExists(cid))
        {
            printf("Customer ID not found. Order cancelled.\n");
            return;
        }
        o.cid = cid;
    }
    else if (customerType == 2)
    {
        printf("Enter your Customer ID: ");
        scanf("%d", &cid);
        if (!isCustomerExists(cid))
        {
            printf("Customer not found! Please register first.\n");
            return;
        }
        o.cid = cid;
    }
    else
    {
        printf("Invalid choice!\n");
        return;
    }

    FILE *fp = fopen("order.txt", "ab");

    viewMenu();

    while (1)
    {
        printf("\nEnter Food ID (0 to finish): ");
        scanf("%d", &o.food_id);
        if (o.food_id == 0)
            break;

        printf("Quantity: ");
        scanf("%d", &o.qty);

        price = getFoodPrice(o.food_id);
        if (price == 0)
        {
            printf("Invalid food ID!\n");
            continue;
        }

        total += price * o.qty;
        fwrite(&o, sizeof(o), 1, fp);
    }

    fclose(fp);

    printf("\n================ BILL =================");
    printf("\nTotal Amount: %d", total);
    printf("\nThank you! Visit again.\n");
}

/* ================= MAIN FUNCTION ================= */

int main()
{
    int choice, adminChoice;

    while (1)
    {
        printf("\n\n===== RESTAURANT MANAGEMENT SYSTEM =====");
        //printf("\n0. Admin Register");
        printf("\n1. Admin Login");
        printf("\n2. Add Customer");
        printf("\n3. View Menu");
        printf("\n4. Place Order");
        printf("\n5. Exit");

        printf("\nEnter choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 0:
            //registerAdmin();
            break;

        case 1:
            if (adminLogin())
            {
                do
                {
                    printf("\n--- ADMIN PANEL ---");
                    printf("\n1. Add Food Item");
                    printf("\n2. View Menu");
                    printf("\n0. Logout");
                    printf("\nChoice: ");
                    scanf("%d", &adminChoice);

                    if (adminChoice == 1)
                        addFood();
                    else if (adminChoice == 2)
                        viewMenu();

                } while (adminChoice != 0);
            }
            break;

        case 2:
            addCustomer();
            break;

        case 3:
            viewMenu();
            break;

        case 4:
            placeOrder();
            break;

        case 5:
            printf("\nExiting system...\n");
            return 0;

        default:
            printf("\nInvalid choice!\n");
        }
    }
}
