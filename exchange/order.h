#pragma once

#include <string>
#include <format>

// Side: is this a buy or sell order?
enum class Side { BUY, SELL };

// OrderType: limit waits in book, market executes immediately
enum class OrderType { LIMIT, MARKET };

class Order {
public:
    // Constructor — creates an order with all required fields
    Order(std::string ticker,
          int         size,
          Side        side,
          OrderType   type,
          double      price,
          int         client_id,
          int         order_id)
        : ticker_(ticker)
        , size_(size)
        , side_(side)
        , type_(type)
        , price_(price)
        , client_id_(client_id)
        , order_id_(order_id) {
    }

    // Getters — read-only access to fields
    std::string getTicker()   const { return ticker_; }
    int         getSize()     const { return size_; }
    Side        getSide()     const { return side_; }
    OrderType   getType()     const { return type_; }
    double      getPrice()    const { return price_; }
    int         getClientId() const { return client_id_; }
    int         getOrderId()  const { return order_id_; }

    // Setter — size changes during partial fills
    void setSize(int size) { size_ = size; }

    // toString — like Python's __str__, prints order info
    std::string toString() const {
        return std::format(
            "Order[id={}, ticker={}, side={}, type={}, price={:.2f}, size={}, client={}]",
            order_id_,
            ticker_,
            side_ == Side::BUY ? "BUY" : "SELL",
            type_ == OrderType::LIMIT ? "LIMIT" : "MARKET",
            price_,
            size_,
            client_id_
        );
    }

private:
    std::string ticker_;     // stock symbol e.g. "AAPL"
    int         size_;       // number of shares e.g. 100
    Side        side_;       // BUY or SELL
    OrderType   type_;       // LIMIT or MARKET
    double      price_;      // price e.g. 100.00
    int         client_id_;  // who placed the order e.g. 1234
    int         order_id_;   // unique order number e.g. 567890
};
