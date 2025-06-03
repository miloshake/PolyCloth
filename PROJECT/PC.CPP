#include <stdio.h>
#include <string.h>
#include <conio.h>

// File paths
#define USER_FILE "C:data1/userinfo.txt"
#define CART_FILE "C:data1/cart.txt"
#define ORDERS_FILE "C:data1/orders.txt"
#define SUBS_FILE "C:data1/subs.txt"
#define ADMIN_FILE "C:data1/admin_cr.txt"
#define PRODUCTS_FILE "C:data1/products.txt"

// Simplified structures
struct User {
    char name[50];
    char email[50];
    char password[50];
    char address[100];
    char phone[20];
};

struct Product {
    int id;
    char name[50];
    float price;
    int category; // 1=Shirts, 2=Hoodies, 3=Jackets
};

struct CartItem {
    char name[50];
    float price;
};

struct Subscription {
    char email[50];
    int type; // 1=None, 2=Monthly, 3=Annual
};

// Global variables
struct User currentUser;
int isLoggedIn = 0;

// ===== UTILITY FUNCTIONS =====
void clearScreen() { clrscr(); }
void pauseScreen() { printf("\nPress any key..."); getch(); }
void clearBuffer() { while(getchar() != '\n'); }

int getCartFileNumber(const char* email) {
    int hash = 0;
    int i;
    for(i = 0; email[i] != '\0'; i++) {
        hash += email[i];
    }
    return (hash % 100) + 1; // Returns 1-100
}

// Add this new function to get user-specific cart file
void getUserCartFile(char* cartPath, const char* email) {
    int fileNum = getCartFileNumber(email);
    sprintf(cartPath, "C:data1/c%d.txt", fileNum);
}

int getValidChoice(int min, int max) {
    int choice;
    printf("\nEnter your choice: ");
    while(scanf("%d", &choice) != 1 || choice < min || choice > max) {
        printf("Invalid choice. Enter %d-%d: ", min, max);
        clearBuffer();
    }
    clearBuffer();
    return choice;
}

// ===== SUBSCRIPTION FUNCTIONS =====
const char* getSubscriptionName(int type) {
    switch(type) {
	case 1: return "None";
        case 2: return "Monthly Premium";
        case 3: return "Annual Premium";
        default: return "Unknown";
    }
}

float getSubscriptionDiscount(int type) {
    switch(type) {
        case 1: return 0.0;    // No discount
        case 2: return 0.10;   // 10% discount
        case 3: return 0.20;   // 20% discount
        default: return 0.0;
    }
}

int getUserSubscription(const char* email) {
    FILE* file = fopen(SUBS_FILE, "r");
    if(!file) return 1; // Default to "None"
    
    char fileEmail[50];
    int subType;
    
    while(fscanf(file, "%49s %d", fileEmail, &subType) == 2) {
        if(strcmp(email, fileEmail) == 0) {
            fclose(file);
            return subType;
        }
    }
    
    fclose(file);
    return 1; // Default to "None"
}

void saveUserSubscription(const char* email, int newType) {
    // Read existing subscriptions
    struct Subscription subs[100];
    int count = 0;
    int userFound = 0;
    int i; // Declare i at the beginning
    FILE* file;
    
    file = fopen(SUBS_FILE, "r");
    if(file) {
        while(count < 100 && fscanf(file, "%49s %d", subs[count].email, &subs[count].type) == 2) {
            if(strcmp(email, subs[count].email) == 0) {
                subs[count].type = newType;
                userFound = 1;
            }
            count++;
        }
        fclose(file);
    }
    
    // If user not found, add new subscription
    if(!userFound && count < 100) {
        strcpy(subs[count].email, email);
        subs[count].type = newType;
        count++;
    }
    
    // Write all subscriptions back
    file = fopen(SUBS_FILE, "w");
    if(file) {
        for(i = 0; i < count; i++) {
            fprintf(file, "%s %d\n", subs[i].email, subs[i].type);
        }
        fclose(file);
    }
}

void manageSubscription() {
    int currentSub = getUserSubscription(currentUser.email);
    
    printf("\n=== Subscription Management ===\n");
    printf("Current Plan: %s\n", getSubscriptionName(currentSub));
    
    if(currentSub == 2) {
        printf("Benefits: 10%% discount on all purchases\n");
    } else if(currentSub == 3) {
        printf("Benefits: 20%% discount on all purchases\n");
    }
    
    printf("\nAvailable Plans:\n");
    printf("1. None (No benefits)\n");
    printf("2. Monthly Premium - 10%% discount (PHP 99/month)\n");
    printf("3. Annual Premium - 20%% discount (PHP 999/year)\n");
    printf("0. Go back\n");
    
    printf("Select new plan: ");
    int choice = getValidChoice(0, 3);
    
    if(choice == 0) return;
    
    if(choice == currentSub) {
        printf("You already have this subscription!\n");
        pauseScreen();
        return;
    }
    
    // Confirm subscription change
    printf("\nConfirm subscription to %s? (1-Yes, 0-No): ", getSubscriptionName(choice));
    int confirm = getValidChoice(0, 1);
    
    if(confirm == 1) {
        saveUserSubscription(currentUser.email, choice);
        printf("Subscription updated successfully!\n");
        
        // Log subscription change
        FILE* ordersFile = fopen(ORDERS_FILE, "a");
        if(ordersFile) {
            fprintf(ordersFile, "Subscription Change for: %s (%s)\n", 
                    currentUser.name, currentUser.email);
            fprintf(ordersFile, "New Plan: %s\n", getSubscriptionName(choice));
            fprintf(ordersFile, "Status: Activated\n---\n");
            fclose(ordersFile);
        }
    }
    
    pauseScreen();
}

void viewAllSubscriptions() {
    printf("\n=== All User Subscriptions ===\n");
    FILE* file = fopen(SUBS_FILE, "r");
    if(!file) {
        printf("No subscriptions found.\n");
        pauseScreen();
        return;
    }
    
    char email[50];
    int subType;
    int count = 1;
    
    while(fscanf(file, "%49s %d", email, &subType) == 2) {
        printf("%d. %s - %s\n", count++, email, getSubscriptionName(subType));
    }
    
    if(count == 1) {
        printf("No subscriptions found.\n");
    }
    
    fclose(file);
    pauseScreen();
}

// ===== FILE HELPER FUNCTIONS =====
int fileExists(const char* filename) {
    FILE* file = fopen(filename, "r");
    if(file) {
        fclose(file);
        return 1;
    }
    return 0;
}

void createDefaultAdmin() {
    FILE* file = fopen(ADMIN_FILE, "w");
    if(file) {
        fprintf(file, "admin,admin123\n");
        fclose(file);
        printf("Default admin created (admin/admin123)\n");
    }
}

// ===== USER MANAGEMENT =====
void registerUser() {
    struct User newUser;
    
    printf("\n=== User Registration ===\n");
    printf("Name: "); 
    scanf("%49s", newUser.name);
    clearBuffer(); // Clear buffer after name input
    
    printf("Email: "); 
    scanf("%49s", newUser.email);
    clearBuffer(); // Clear buffer after email input
    
    printf("Password: "); 
    scanf("%49s", newUser.password);
    clearBuffer(); // Clear buffer after password input
    
    printf("Address: "); 
    scanf("%99s", newUser.address);
    clearBuffer(); // Clear buffer after address input
    
    printf("Phone: "); 
    scanf("%19s", newUser.phone);
    clearBuffer(); // Clear buffer after phone input
    
    // Check if user already exists
    FILE* file = fopen(USER_FILE, "r");
    if(file) {
        char existingEmail[50];
        while(fscanf(file, "%*s %49s %*s %*s %*s", existingEmail) == 1) {
            if(strcmp(existingEmail, newUser.email) == 0) {
                printf("User already exists!\n");
                fclose(file);
                pauseScreen();
                return;
            }
        }
        fclose(file);
    }
    
    // Save new user
    file = fopen(USER_FILE, "a");
    if(file) {
        fprintf(file, "%s %s %s %s %s\n", 
                newUser.name, newUser.email, newUser.password, 
                newUser.address, newUser.phone);
        fclose(file);
        printf("Registration successful!\n");
        
        // Initialize subscription to "None"
        saveUserSubscription(newUser.email, 1);
    } else {
        printf("Error saving user data!\n");
    }
    pauseScreen();
}


int loginUser() {
    char email[50], password[50];
    
    printf("\n=== User Login ===\n");
    printf("Email: "); 
    scanf("%49s", email);
    clearBuffer();
    
    printf("Password: "); 
    scanf("%49s", password);
    clearBuffer();
    
    FILE* file = fopen(USER_FILE, "r");
    if(!file) {
        printf("No users found. Please register first.\n");
        pauseScreen();
        return 0;
    }
    
    struct User user;
    while(fscanf(file, "%49s %49s %49s %99s %19s", 
                 user.name, user.email, user.password, 
                 user.address, user.phone) == 5) {
        
        if(strcmp(email, user.email) == 0 && strcmp(password, user.password) == 0) {
            currentUser = user;
            isLoggedIn = 1;
            printf("Welcome, %s!\n", user.name);
            
            // Show current subscription
            int subType = getUserSubscription(user.email);
            printf("Current Plan: %s\n", getSubscriptionName(subType));
            
            fclose(file);
            pauseScreen();
            return 1;
        }
    }
    
    fclose(file);
    printf("Invalid credentials!\n");
    pauseScreen();
    return 0;
}


// ===== ADMIN FUNCTIONS =====
int adminLogin() {
    if(!fileExists(ADMIN_FILE)) {
        createDefaultAdmin();
    }
    
    char username[50], password[50];
    printf("\n=== Admin Login ===\n");
    printf("Username: "); scanf("%49s", username);
    printf("Password: "); scanf("%49s", password);
    clearBuffer();
    
    FILE* file = fopen(ADMIN_FILE, "r");
    if(file) {
        char adminUser[50], adminPass[50];
        if(fscanf(file, "%49[^,],%49s", adminUser, adminPass) == 2) {
            fclose(file);
            if(strcmp(username, adminUser) == 0 && strcmp(password, adminPass) == 0) {
                printf("Admin login successful!\n");
                pauseScreen();
                return 1;
            }
        }
    }
    
    printf("Invalid admin credentials!\n");
    pauseScreen();
    return 0;
}

void viewAllUsers() {
    printf("\n=== All Users ===\n");
    FILE* file = fopen(USER_FILE, "r");
    if(!file) {
        printf("No users found.\n");
        pauseScreen();
        return;
    }
    
    struct User user;
    int count = 1;
    while(fscanf(file, "%49s %49s %49s %99s %19s", 
                 user.name, user.email, user.password, 
                 user.address, user.phone) == 5) {
        int subType = getUserSubscription(user.email);
        printf("%d. %s (%s) - %s [%s]\n", 
               count++, user.name, user.email, user.phone, getSubscriptionName(subType));
    }
    
    fclose(file);
    pauseScreen();
}

// ===== PRODUCT MANAGEMENT =====
void loadProducts(struct Product products[], int* count) {
    *count = 0;
    FILE* file = fopen(PRODUCTS_FILE, "r");
    if(!file) return;
    
    while(*count < 50 && 
          fscanf(file, "%d %49s %f %d", 
                 &products[*count].id, products[*count].name, 
                 &products[*count].price, &products[*count].category) == 4) {
        (*count)++;
    }
    fclose(file);
}

void saveProducts(struct Product products[], int count) {
    FILE* file;
    int i; // Declare i at the beginning
    
    file = fopen(PRODUCTS_FILE, "w");
    if(!file) return;
    
    for(i = 0; i < count; i++) {
        fprintf(file, "%d %s %.2f %d\n", 
                products[i].id, products[i].name, 
                products[i].price, products[i].category);
    }
    fclose(file);
}

void addProduct() {
    struct Product products[50];
    int count;
    int i; // Declare i at the beginning
    loadProducts(products, &count);
    
    if(count >= 50) {
        printf("Product catalog is full!\n");
        pauseScreen();
        return;
    }
    
    struct Product newProduct;
    newProduct.id = count + 1; // Simple ID assignment
    
    printf("\n=== Add Product ===\n");
    printf("Name: "); 
    scanf("%49s", newProduct.name);
    clearBuffer(); // Add buffer clear after name
    
    // Check for duplicate product names
    for(i = 0; i < count; i++) {
        if(strcmp(products[i].name, newProduct.name) == 0) {
            printf("Error: A product with the name '%s' already exists!\n", newProduct.name);
            printf("Please choose a different name.\n");
            pauseScreen();
            return;
        }
    }
    
    printf("Price: "); 
    scanf("%f", &newProduct.price);
    clearBuffer(); // Add buffer clear after price
    
    printf("Category (1-Shirts, 2-Hoodies, 3-Jackets): ");
    newProduct.category = getValidChoice(1, 3);
    
    products[count] = newProduct;
    saveProducts(products, count + 1);
    
    printf("Product added successfully!\n");
    pauseScreen();
}

void deleteProduct() {
    struct Product products[50];
    int count;
    int i; // Declare i at the beginning
    int choice, productIndex, confirm;
    const char* category;
    
    loadProducts(products, &count);

    if(count == 0) {
        printf("No products available to delete.\n");
        pauseScreen();
        return;
    }

    printf("\n=== Delete Product ===\n");
    printf("Current Products:\n");
    for(i = 0; i < count; i++) {
        category = (products[i].category == 1) ? "Shirts" :
                   (products[i].category == 2) ? "Hoodies" : "Jackets";
        printf("%d. %s - PHP %.2f (%s)\n",
               i + 1, products[i].name, products[i].price, category);
    }

    printf("\nSelect product to delete (1-%d, 0 to cancel): ", count);
    choice = getValidChoice(0, count);

    if(choice == 0) {
        printf("Delete operation cancelled.\n");
        pauseScreen();
        return;
    }

    productIndex = choice - 1;

    // Show product details and confirm deletion
    printf("\nProduct to delete:\n");
    printf("Name: %s\n", products[productIndex].name);
    printf("Price: PHP %.2f\n", products[productIndex].price);
    category = (products[productIndex].category == 1) ? "Shirts" :
               (products[productIndex].category == 2) ? "Hoodies" : "Jackets";
    printf("Category: %s\n", category);

    printf("\nAre you sure you want to delete this product? (1-Yes, 0-No): ");
    confirm = getValidChoice(0, 1);

    if(confirm == 1) {
        // Remove the product by shifting all elements after it
        for(i = productIndex; i < count - 1; i++) {
            products[i] = products[i + 1];
        }

        // Update product IDs to maintain sequence
        for(i = 0; i < count - 1; i++) {
            products[i].id = i + 1;
        }
        
        // Save the updated product list
        saveProducts(products, count - 1);
        
        printf("Product deleted successfully!\n");
    } else {
        printf("Delete operation cancelled.\n");
    }
    
    pauseScreen();
}

void viewProducts() {
    struct Product products[50];
    int count;
    int i; // Declare i at the beginning
    const char* category;
    
    loadProducts(products, &count);
    
    printf("\n=== Products ===\n");
    if(count == 0) {
        printf("No products available.\n");
    } else {
        for(i = 0; i < count; i++) {
            category = (products[i].category == 1) ? "Shirts" : 
                       (products[i].category == 2) ? "Hoodies" : "Jackets";
            printf("%d. %s - PHP %.2f (%s) [ID: %d]\n", 
                   i + 1, products[i].name, products[i].price, category, products[i].id);
        }
        printf("\nTotal products: %d\n", count);
    }
    pauseScreen();
}

// ===== SHOPPING CART =====
void addToCart(const char* name, float price) {
    char cartPath[100];
    getUserCartFile(cartPath, currentUser.email);
    
    FILE* file = fopen(cartPath, "a");
    if(file) {
        fprintf(file, "%s %.2f\n", name, price);
        fclose(file);
        printf("Added %s to cart!\n", name);
    } else {
        printf("Error: Could not open cart file\n");
    }
}

void viewCart() {
    char cartPath[100];
    getUserCartFile(cartPath, currentUser.email);
    
    printf("\n=== Your Cart ===\n");
    FILE* file = fopen(cartPath, "r");
    if(!file) {
        printf("Cart is empty.\n");
        return;
    }
    
    char name[50];
    float price, total = 0;
    int count = 1;
    int itemsRead = 0;
    
    while(fscanf(file, "%49s %f", name, &price) == 2) {
        printf("%d. %s - PHP %.2f\n", count++, name, price);
        total += price;
        itemsRead++;
    }
    
    if(itemsRead == 0) {
        printf("Cart is empty.\n");
        fclose(file);
        return;
    }
    
    // Apply subscription discount
    int subType = getUserSubscription(currentUser.email);
    float discount = getSubscriptionDiscount(subType);
    float discountAmount = total * discount;
    float finalTotal = total - discountAmount;
    
    printf("----------------------------\n");
    printf("Subtotal: PHP %.2f\n", total);
    if(discount > 0) {
        printf("Subscription Discount (%.0f%%): -PHP %.2f\n", discount * 100, discountAmount);
        printf("Final Total: PHP %.2f\n", finalTotal);
    } else {
        printf("Total: PHP %.2f\n", total);
    }
    
    fclose(file);
}

void clearCart() {
    char cartPath[100];
    getUserCartFile(cartPath, currentUser.email);
    
    FILE* file = fopen(cartPath, "w");
    if(file) {
        fclose(file);
        printf("Cart cleared.\n");
    }
}

// ===== SHOPPING FUNCTIONS =====
void shopByCategory(int category) {
    struct Product products[50];
    int count;
    int categoryProducts[50];
    int categoryCount = 0;
    int i, choice, productIndex; // Declare all variables at the beginning
    const char* categoryName;
    
    loadProducts(products, &count);
    
    categoryName = (category == 1) ? "Shirts" : 
                   (category == 2) ? "Hoodies" : "Jackets";
    
    printf("\n=== %s ===\n", categoryName);
    
    // Show products in category
    for(i = 0; i < count; i++) {
        if(products[i].category == category) {
            printf("%d. %s - PHP %.2f\n", 
                   categoryCount + 1, products[i].name, products[i].price);
            categoryProducts[categoryCount] = i;
            categoryCount++;
        }
    }
    
    if(categoryCount == 0) {
        printf("No products in this category.\n");
        pauseScreen();
        return;
    }
    
    printf("Select product (1-%d, 0 to go back)", categoryCount);
    choice = getValidChoice(0, categoryCount);
    
    if(choice > 0) {
        productIndex = categoryProducts[choice - 1];
        addToCart(products[productIndex].name, products[productIndex].price);
    }
    
    pauseScreen();
}

void checkout() {
    char cartPath[100];
    getUserCartFile(cartPath, currentUser.email);
    
    viewCart();
    
    printf("\nProceed with checkout? (1-Yes, 0-No): ");
    int choice = getValidChoice(0, 1);
    
    if(choice == 1) {
        // Calculate total with discount
        FILE* cartFile = fopen(cartPath, "r");
        float total = 0;
        char name[50];
        float price;
        
        if(cartFile) {
            while(fscanf(cartFile, "%49s %f", name, &price) == 2) {
                total += price;
            }
            fclose(cartFile);
        }
        
        int subType = getUserSubscription(currentUser.email);
        float discount = getSubscriptionDiscount(subType);
        float discountAmount = total * discount;
        float finalTotal = total - discountAmount;
        
        // Record order
        FILE* ordersFile = fopen(ORDERS_FILE, "a");
        if(ordersFile) {
            fprintf(ordersFile, "Order for: %s (%s)\n", currentUser.name, currentUser.email);
            
            // Copy cart to orders
            cartFile = fopen(cartPath, "r");
            if(cartFile) {
                while(fscanf(cartFile, "%49s %f", name, &price) == 2) {
                    fprintf(ordersFile, "- %s: PHP %.2f\n", name, price);
                }
                fclose(cartFile);
            }
            
            fprintf(ordersFile, "Subtotal: PHP %.2f\n", total);
            if(discount > 0) {
                fprintf(ordersFile, "Subscription: %s (%.0f%% discount)\n", 
                        getSubscriptionName(subType), discount * 100);
                fprintf(ordersFile, "Discount: -PHP %.2f\n", discountAmount);
            }
            fprintf(ordersFile, "Final Total: PHP %.2f\n", finalTotal);
            fprintf(ordersFile, "Status: Confirmed\n---\n");
            fclose(ordersFile);
        }
        
        clearCart();
        printf("Order placed successfully!\n");
        if(discountAmount > 0) {
            printf("You saved PHP %.2f with your subscription!\n", discountAmount);
        }
    }
    
    pauseScreen();
}

void adminMenu() {
    int choice;
    do {
        clearScreen();
        printf("\n=== Admin Dashboard ===\n");
        printf("1. View All Users\n");
        printf("2. View Products\n");
        printf("3. Add Product\n");
        printf("4. Delete Product\n");
        printf("5. View All Subscriptions\n");
        printf("0. Exit\n");
        
        choice = getValidChoice(0, 5);
        
        switch(choice) {
            case 1: viewAllUsers(); break;
            case 2: viewProducts(); break;
            case 3: addProduct(); break;
            case 4: deleteProduct(); break;
            case 5: viewAllSubscriptions(); break;
        }
    } while(choice != 0);
}

void removeFromCart() {
    char cartPath[100];
    getUserCartFile(cartPath, currentUser.email);
    
    printf("\n=== Remove from Cart ===\n");
    FILE* file = fopen(cartPath, "r");
    if(!file) {
        printf("Cart is empty.\n");
        pauseScreen();
        return;
    }
    
    // Read all cart items
    struct CartItem items[50];
    int count = 0;
    
    while(count < 50 && fscanf(file, "%49s %f", items[count].name, &items[count].price) == 2) {
        count++;
    }
    fclose(file);
    
    if(count == 0) {
        printf("Cart is empty.\n");
        pauseScreen();
        return;
    }
    
    // Display cart items with numbers
    printf("Current cart items:\n");
    int i;
    for(i = 0; i < count; i++) {
        printf("%d. %s - PHP %.2f\n", i + 1, items[i].name, items[i].price);
    }
    
    printf("\nSelect item to remove (1-%d, 0 to cancel): ", count);
    int choice = getValidChoice(0, count);
    
    if(choice == 0) {
        printf("Remove operation cancelled.\n");
        pauseScreen();
        return;
    }
    
    int itemIndex = choice - 1;
    
    // Confirm removal
    printf("\nRemove '%s' from cart? (1-Yes, 0-No): ", items[itemIndex].name);
    int confirm = getValidChoice(0, 1);
    
    if(confirm == 1) {
        // Rewrite cart file without the selected item
        file = fopen(cartPath, "w");
        if(file) {
            for(i = 0; i < count; i++) {
                if(i != itemIndex) {
                    fprintf(file, "%s %.2f\n", items[i].name, items[i].price);
                }
            }
            fclose(file);
            printf("Item removed successfully!\n");
        } else {
            printf("Error: Could not update cart file\n");
        }
    } else {
        printf("Remove operation cancelled.\n");
    }
    
    pauseScreen();
}

void userMenu() {
    int choice;
    do {
        clearScreen();
        printf("\n=== PolyCloth Shop ===\n");
        printf("Welcome, %s!\n", currentUser.name);
        
        int subType = getUserSubscription(currentUser.email);
        printf("Current Plan: %s", getSubscriptionName(subType));
        if(subType > 1) {
            printf(" (%.0f%% discount)", getSubscriptionDiscount(subType) * 100);
        }
        printf("\n\n");
        
        printf("1. Shop Shirts\n");
        printf("2. Shop Hoodies\n");
        printf("3. Shop Jackets\n");
        printf("4. View Cart\n");
        printf("5. Remove from Cart\n");
        printf("6. Checkout\n");
        printf("7. Manage Subscription\n");
        printf("0. Logout\n");
        
        choice = getValidChoice(0, 7);
        
        switch(choice) {
            case 1: shopByCategory(1); break;
            case 2: shopByCategory(2); break;
            case 3: shopByCategory(3); break;
            case 4: viewCart(); pauseScreen(); break;
            case 5: removeFromCart(); break;
            case 6: checkout(); break;
            case 7: manageSubscription(); break;
        }
    } while(choice != 0);
    
    isLoggedIn = 0;
}

void mainMenu() {
    int choice;
    do {
        clearScreen();
        printf("\n=== PolyCloth ===\n");
        printf("1. User Login\n");
        printf("2. User Register\n");
        printf("3. Admin Login\n");
        printf("0. Exit\n");
        
        choice = getValidChoice(0, 3);
        
        switch(choice) {
            case 1: 
                if(loginUser()) {
                    userMenu();
                }
                break;
            case 2: registerUser(); break;
            case 3: 
                if(adminLogin()) {
                    adminMenu();
                }
                break;
        }
    } while(choice != 0);
}

// ===== MAIN FUNCTION =====
void main() {
    clearScreen();
    printf("\n========================================\n");
    printf("     Welcome to PolyCloth!              \n");
    printf("========================================\n");
    pauseScreen();
    
    mainMenu();
    
    printf("\nThank you for using PolyCloth!\n");
    pauseScreen();
    getch();
}