# Smart Vending Machine

A modern Qt-based vending machine application with user and admin interfaces, designed to manage inventory, handle transactions, and provide change.

## Project Overview

This application simulates a real-world vending machine with an intuitive graphical user interface. It features two main modes:

1. **User Mode**: For customers to purchase items
2. **Admin Mode**: For managing stock, handling money, and system maintenance

## Features

### User Features
- View available items with prices and stock information
- Purchase items with different denominations (1, 5, 10, 20, 100 THB)
- Receive change automatically calculated from available coins
- User-friendly dark-themed interface

### Admin Features
- Stock management (add, delete, restock items)
- Money management (refill change, collect money)
- View detailed information about:
  - Current stock levels
  - Change box status
  - Collection box status
- System condition monitoring

## Technical Details

### Database Structure
The application uses SQLite with three main tables:
- `stock_67011755`: Stores item information (name, price, stock)
- `change_box_67011755`: Manages available change (denominations and counts)
- `collection_box_67011755`: Tracks collected money

### Operating Conditions
The system monitors its operational status and will not allow user mode if:
- More than 50% of items are out of stock
- Change box has insufficient coins for transactions
- Collection box is full (reaches 100 coins of any denomination)

## Getting Started

### Prerequisites
- Qt 5.x or higher
- SQLite support

### Running the Application
1. Compile the project using qmake and your preferred compiler
2. Run the executable
3. The database will be automatically initialized on first run

## Usage Guide

### For Users
1. Select "User Mode" from the main screen
2. Choose an item from the available products list
3. Insert coins when prompted
4. Collect your item and any change returned

### For Administrators
1. Select "Admin Mode" from the main screen
2. Use the control panel to:
   - Manage inventory
   - Refill change when needed
   - Collect money from the collection box
   - Monitor system status

## Supported Denominations
- Accepted for payment: 1 THB, 5 THB, 10 THB, 20 THB, 100 THB
- Available for change: 1 THB, 5 THB, 10 THB, 20 THB

## System Design
The application follows a modular architecture with:
- MainWindow class handling the UI and user interactions
- Database class managing all data operations
- Separate logic for inventory, payment, and change calculation

## Development
This project was developed using the Qt framework and C++, with SQLite for persistent storage. The modern UI features a dark theme for improved visibility and user experience.
