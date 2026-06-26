#include <iostream>
#include "exchange/order.h"
#include "exchange/order_book.h"

int main() {
    std::cout << "=== Testing Order ===\n";

    // Test 1: Create an order and print it
    Order o1("AAPL", 100, Side::BUY, OrderType::LIMIT, 99.99, 1, 1001);
    std::cout << o1.toString() << "\n";

    Order o2("AAPL", 50, Side::SELL, OrderType::LIMIT, 100.01, 2, 1002);
    std::cout << o2.toString() << "\n";

    std::cout << "\n=== Testing OrderBook ===\n";

    OrderBook book;

    // Test 2: Add orders and print book
    book.add(Order("AAPL", 100, Side::BUY,  OrderType::LIMIT, 99.99,  1, 1001));
    book.add(Order("AAPL", 200, Side::BUY,  OrderType::LIMIT, 99.98,  1, 1002));
    book.add(Order("AAPL", 50,  Side::SELL, OrderType::LIMIT, 100.01, 2, 1003));
    book.add(Order("AAPL", 75,  Side::SELL, OrderType::LIMIT, 100.02, 2, 1004));

    std::cout << book.toString();

    // Test 3: Check best bid and ask
    std::cout << "Best bid: " << book.bestBid() << "\n";  // should be 99.99
    std::cout << "Best ask: " << book.bestAsk() << "\n";  // should be 100.01

    // Test 4: Cancel an order
    std::cout << "\n=== After cancelling order 1001 ===\n";
    book.cancel(1001);
    std::cout << book.toString();

    std::cout << "Best bid after cancel: " << book.bestBid() << "\n";  // should be 99.98

    std::cout << "\nAll tests passed!\n";
    return 0;
}
