# PolyCloth - C-based E-commerce System

A console-based clothing e-commerce application developed in C using Turbo C++ environment. This system provides user registration, shopping cart functionality, subscription management, and administrative features.

## Features

### User Features
- **User Registration & Login** - Secure account creation and authentication
- **Product Browsing** - Browse products by categories (Shirts, Hoodies, Jackets)
- **Shopping Cart** - Add/remove items and view cart contents
- **Subscription Plans** - Premium subscription with discount benefits
- **Order Management** - Checkout and order history tracking

### Admin Features
- **User Management** - View all registered users
- **Product Management** - Add, delete, and view products
- **Subscription Monitoring** - View all user subscriptions
- **Administrative Dashboard** - Centralized admin controls

### Subscription Plans
1. **None** - Standard pricing, no discounts
2. **Monthly Premium** - 10% discount on all purchases (PHP 99/month)
3. **Annual Premium** - 20% discount on all purchases (PHP 999/year)

## System Requirements

- Turbo C++ 3.0 or compatible C compiler
- DOS/Windows environment
- Minimum 640KB RAM
- Console/terminal support

## File Structure

```
PROJECT/
├── PC.CPP              # Main application file
├── PC.BAK              # Backup file
└── C:data1/            # Data directory
    ├── userinfo.txt    # User account data
    ├── cart.txt        # Shopping cart data
    ├── orders.txt      # Order history
    ├── subs.txt        # Subscription data
    ├── admin_cr.txt    # Admin credentials
    ├── products.txt    # Product catalog
    └── c[1-100].txt    # User-specific cart files
```

## Installation & Setup

1. **Prepare Environment**
   ```bash
   # Ensure Turbo C++ is installed
   # Create data directory structure
   mkdir C:\data1
   ```

2. **Compile the Application**
   ```bash
   # In Turbo C++ IDE
   # Open PC.CPP
   # Press Ctrl+F9 to compile and run
   ```

3. **Default Admin Credentials**
   - Username: `admin`
   - Password: `admin123`
   
4. **Current Admin Credentials**
   - Username: `poly_cloth`
   - Password: `clothing_421`
   
**Note**: You could change the admin credentials in the data1 folder!!

## Usage Guide

### For Users

1. **Registration**
   - Select "User Register" from main menu
   - Provide: Name, Email, Password, Address, Phone
   - System automatically creates "None" subscription

2. **Shopping**
   - Login with registered credentials
   - Browse categories: Shirts, Hoodies, Jackets
   - Add items to cart
   - View cart with automatic discount calculation
   - Proceed to checkout

3. **Subscription Management**
   - Access "Manage Subscription" from user menu
   - Choose between None, Monthly Premium, or Annual Premium
   - Changes apply immediately to future purchases

### For Administrators

1. **Admin Login**
   - Select "Admin Login" from main menu
   - Use default credentials or custom admin account

2. **Product Management**
   - Add new products with name, price, and category
   - Delete existing products
   - View complete product catalog

3. **User Monitoring**
   - View all registered users
   - Monitor subscription statuses
   - Track user activity

## Data Management

### File Formats

**User Data (userinfo.txt)**
```
name email password address phone
```

**Product Data (products.txt)**
```
id name price category
```

**Subscription Data (subs.txt)**
```
email subscription_type
```

**Cart Data (c[number].txt)**
```
product_name price
```

### User-Specific Cart System
- Each user gets a unique cart file based on email hash
- Cart files numbered 1-100: `c1.txt`, `c2.txt`, etc.
- Prevents cart data conflicts between users

## Key Functions

### Core Functions
- [`mainMenu()`](PC.cpp) - Main application entry point
- [`loginUser()`](PC.cpp) - User authentication
- [`registerUser()`](PC.cpp) - New user registration
- [`userMenu()`](PC.cpp) - User interface dashboard

### Shopping Functions
- [`shopByCategory()`](PC.cpp) - Browse products by category
- [`addToCart()`](PC.cpp) - Add items to shopping cart
- [`viewCart()`](PC.cpp) - Display cart with pricing
- [`checkout()`](PC.cpp) - Process orders and payments

### Admin Functions
- [`adminLogin()`](PC.cpp) - Admin authentication
- [`adminMenu()`](PC.cpp) - Administrative dashboard
- [`addProduct()`](PC.cpp) - Add new products
- [`deleteProduct()`](PC.cpp) - Remove products

### Subscription Functions
- [`getUserSubscription()`](PC.cpp) - Get user's current plan
- [`saveUserSubscription()`](PC.cpp) - Update subscription
- [`getSubscriptionDiscount()`](PC.cpp) - Calculate discounts
- [`manageSubscription()`](PC.cpp) - Subscription interface

## Security Features

- Password-based authentication for users and admins
- Input validation and buffer protection
- Separate cart files prevent data mixing
- Admin credential file protection

## Limitations

- Text-based file storage (no database)
- Single-session application
- Limited to console interface
- No network/multi-user support
- Basic encryption/security measures

## Troubleshooting

### Common Issues

1. **File Access Errors**
   - Ensure `C:\data1\` directory exists
   - Check file permissions
   - Verify Turbo C++ path settings

2. **Compilation Errors**
   - Use Turbo C++ 3.0 or compatible compiler
   - Check include paths for `<conio.h>`
   - Ensure proper C syntax compliance

3. **Data Corruption**
   - Backup data files regularly
   - Check file format consistency
   - Restart application if data appears corrupted

### Debug Tips
- Use `printf()` statements for debugging
- Check file operations return values
- Verify user input validation

## Future Enhancements

- Database integration (MySQL/SQLite)
- GUI interface using graphics libraries
- Network support for multi-user access
- Enhanced security and encryption
- Product image support
- Payment gateway integration
- Inventory management system

## License

This project is developed for educational purposes. Free to use and modify.

## Contributors

- [Vinze Siega](https://github.com/miloshake) 
- [Kenji Gapuz](github.com/S1drra)
- [Anne Margaret Tablanza](https://github.com/Margaeu)

---

**Note**: This application is designed for educational purposes and demonstrates fundamental C programming concepts including file I/O, structures, and console application development.
