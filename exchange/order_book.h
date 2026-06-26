#pragma once

#include <algorithm>
#include <map>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "exchange/order.h"
#include "common/mem_pool.h"

class OrderBook {
public:
    // Constructor — pre-allocate order slots using MemPool
    explicit OrderBook(std::size_t pool_size = 1000)
        : pool_(pool_size) {
    }

    // ADD — place a new order into the book
    void add(const Order& order) {
        // Allocate a slot from the memory pool
        Order* o = pool_.allocate(
            order.getTicker(),
            order.getSize(),
            order.getSide(),
            order.getType(),
            order.getPrice(),
            order.getClientId(),
            order.getOrderId()
        );

        if (!o) return;  // pool is full, cannot add

        // Save pointer by order_id for fast cancel lookup
        orders_by_id_[order.getOrderId()] = o;

        // Add to price-sorted map (bids or asks)
        if (order.getSide() == Side::BUY) {
            bids_[order.getPrice()].push_back(o);  // buys: highest price = best
        } else {
            asks_[order.getPrice()].push_back(o);  // sells: lowest price = best
        }
    }

    // CANCEL — remove an order from the book by order_id
    void cancel(int order_id) {
        // Find the order pointer
        auto it = orders_by_id_.find(order_id);
        if (it == orders_by_id_.end()) return;  // not found

        Order* o = it->second;

        // Remove from bids or asks price level
        if (o->getSide() == Side::BUY) {
            removeFromLevel(bids_, o->getPrice(), o);
        } else {
            removeFromLevel(asks_, o->getPrice(), o);
        }

        // Remove from id lookup map
        orders_by_id_.erase(it);

        // Return memory slot back to pool
        pool_.deallocate(o);
    }

    // Best bid = highest price a buyer will pay
    double bestBid() const {
        if (bids_.empty()) return 0.0;
        return bids_.rbegin()->first;
    }

    // Best ask = lowest price a seller will accept
    double bestAsk() const {
        if (asks_.empty()) return 0.0;
        return asks_.begin()->first;
    }

    // TOSTRING — print the full order book
    std::string toString() const {
        std::ostringstream oss;
        oss << "=== ORDER BOOK ===\n";

        // Print asks from highest to lowest
        oss << "  ASKS:\n";
        for (auto it = asks_.rbegin(); it != asks_.rend(); ++it) {
            for (const Order* o : it->second) {
                oss << "    " << it->first << "  size=" << o->getSize() << "\n";
            }
        }

        // Print mid price
        if (!bids_.empty() && !asks_.empty()) {
            double mid = (bestBid() + bestAsk()) / 2.0;
            oss << "  --- mid " << mid << " ---\n";
        }

        // Print bids from highest to lowest
        oss << "  BIDS:\n";
        for (auto it = bids_.rbegin(); it != bids_.rend(); ++it) {
            for (const Order* o : it->second) {
                oss << "    " << it->first << "  size=" << o->getSize() << "\n";
            }
        }

        oss << "==================\n";
        return oss.str();
    }

private:
    // Helper — remove a specific order pointer from a price level
    void removeFromLevel(std::map<double, std::vector<Order*>>& side,
                         double price, Order* o) {
        auto level_it = side.find(price);
        if (level_it == side.end()) return;

        auto& vec = level_it->second;
        vec.erase(std::remove(vec.begin(), vec.end(), o), vec.end());

        // If no orders left at this price, remove the price level
        if (vec.empty()) side.erase(level_it);
    }

    MemPool<Order> pool_;  // pre-allocated memory for orders

    // std::map keeps prices sorted automatically
    // bids_: buy orders sorted by price (highest = best bid)
    // asks_: sell orders sorted by price (lowest = best ask)
    std::map<double, std::vector<Order*>> bids_;
    std::map<double, std::vector<Order*>> asks_;

    // Fast lookup by order_id for cancellations
    std::unordered_map<int, Order*> orders_by_id_;
};
