#include <gtest/gtest.h>
#include "engine/events/EventConnection.hpp"

TEST(EventConnectionTest, DefaultConstructor) {
    EventConnection conn;
    EXPECT_FALSE(conn.isConnected());
    EXPECT_FALSE(static_cast<bool>(conn));
}

TEST(EventConnectionTest, MoveConstructor) {
    EventConnection conn1{1, typeid(int)};
    EXPECT_TRUE(conn1.isConnected());
    
    EventConnection conn2 = std::move(conn1);
    EXPECT_TRUE(conn2.isConnected());
    EXPECT_FALSE(conn1.isConnected());  
}

TEST(EventConnectionTest, MoveAssignment) {
    EventConnection conn1{1, typeid(int)};
    EventConnection conn2{2, typeid(double)};
    
    conn2 = std::move(conn1);
    EXPECT_TRUE(conn2.isConnected());
    EXPECT_FALSE(conn1.isConnected());
}
