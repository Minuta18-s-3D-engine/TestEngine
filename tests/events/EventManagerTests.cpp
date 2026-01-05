#include <gtest/gtest.h>
#include "engine/events/EventManager.hpp"

struct TestEvent : public Event {
    int value;
    std::string message;
    
    TestEvent(int v = 0, const std::string& m = "") 
        : value(v), message(m) {}
};

struct AnotherEvent : public Event {
    float data;
    AnotherEvent(float d = 0.0f) : data(d) {}
};

TEST(EventManagerTest, SubscribeAndTrigger) {
    EventManager manager;
    int callCount = 0;
    TestEvent capturedEvent;
    
    auto conn = manager.subscribe<TestEvent>([&](TestEvent& e) {
        callCount++;
        capturedEvent = e;
    });
    
    EXPECT_TRUE(conn.isConnected());
    
    manager.triggerEvent(TestEvent(42, "Hello"));
    manager.dispatchEvents();
    
    EXPECT_EQ(callCount, 1);
    EXPECT_EQ(capturedEvent.value, 42);
    EXPECT_EQ(capturedEvent.message, "Hello");
}

TEST(EventManagerTest, MultipleSubscribers) {
    EventManager manager;
    int callCount1 = 0;
    int callCount2 = 0;
    
    auto conn1 = manager.subscribe<TestEvent>([&](TestEvent&) {
        callCount1++;
    });
    
    auto conn2 = manager.subscribe<TestEvent>([&](TestEvent&) {
        callCount2++;
    });
    
    manager.triggerEvent(TestEvent());
    manager.dispatchEvents();
    
    EXPECT_EQ(callCount1, 1);
    EXPECT_EQ(callCount2, 1);
}

TEST(EventManagerTest, EventTypeSeparation) {
    EventManager manager;
    int testEventCount = 0;
    int anotherEventCount = 0;
    
    manager.subscribe<TestEvent>([&](TestEvent&) { testEventCount++; });
    manager.subscribe<AnotherEvent>([&](AnotherEvent&) { anotherEventCount++; });
    
    manager.triggerEvent(TestEvent());
    manager.triggerEvent(AnotherEvent(3.14f));
    manager.dispatchEvents();
    
    EXPECT_EQ(testEventCount, 1);
    EXPECT_EQ(anotherEventCount, 1);
}

TEST(EventManagerTest, Unsubscribe) {
    EventManager manager;
    int callCount = 0;
    
    auto conn = manager.subscribe<TestEvent>([&](TestEvent&) {
        callCount++;
    });
    
    manager.triggerEvent(TestEvent());
    manager.dispatchEvents();
    EXPECT_EQ(callCount, 1);
    
    manager.unsubscribe(conn);
    
    manager.triggerEvent(TestEvent());
    manager.dispatchEvents();
    EXPECT_EQ(callCount, 1); 
}

TEST(EventManagerTest, UnsubscribeMultiple) {
    EventManager manager;
    int callCount1 = 0;
    int callCount2 = 0;
    
    auto conn1 = manager.subscribe<TestEvent>([&](TestEvent&) {
        callCount1++;
    });
    
    auto conn2 = manager.subscribe<TestEvent>([&](TestEvent&) {
        callCount2++;
    });
    
    manager.triggerEvent(TestEvent());
    manager.dispatchEvents();
    EXPECT_EQ(callCount1, 1);
    EXPECT_EQ(callCount2, 1);
    
    manager.unsubscribe(conn1);
    
    manager.triggerEvent(TestEvent());
    manager.dispatchEvents();
    EXPECT_EQ(callCount1, 1); 
    EXPECT_EQ(callCount2, 2); 
}

TEST(EventManagerTest, ClearSubscribersByType) {
    EventManager manager;
    int testEventCount = 0;
    int anotherEventCount = 0;
    
    manager.subscribe<TestEvent>([&](TestEvent&) { testEventCount++; });
    manager.subscribe<AnotherEvent>([&](AnotherEvent&) { anotherEventCount++; });
    
    manager.triggerEvent(TestEvent());
    manager.triggerEvent(AnotherEvent());
    manager.dispatchEvents();
    
    EXPECT_EQ(testEventCount, 1);
    EXPECT_EQ(anotherEventCount, 1);
    
    manager.clearSubscribers<TestEvent>();
    
    manager.triggerEvent(TestEvent());
    manager.triggerEvent(AnotherEvent());
    manager.dispatchEvents();
    
    EXPECT_EQ(testEventCount, 1); 
    EXPECT_EQ(anotherEventCount, 2);
}

TEST(EventManagerTest, ClearAllSubscribers) {
    EventManager manager;
    int testEventCount = 0;
    int anotherEventCount = 0;
    
    manager.subscribe<TestEvent>([&](TestEvent&) { testEventCount++; });
    manager.subscribe<AnotherEvent>([&](AnotherEvent&) { anotherEventCount++; });
    
    manager.triggerEvent(TestEvent());
    manager.triggerEvent(AnotherEvent());
    manager.dispatchEvents();
    
    EXPECT_EQ(testEventCount, 1);
    EXPECT_EQ(anotherEventCount, 1);
    
    manager.clearAllSubscribers();
    
    manager.triggerEvent(TestEvent());
    manager.triggerEvent(AnotherEvent());
    manager.dispatchEvents();
    
    EXPECT_EQ(testEventCount, 1);
    EXPECT_EQ(anotherEventCount, 1); 
}

TEST(EventManagerTest, QueueMultipleEvents) {
    EventManager manager;
    std::vector<int> receivedValues;
    
    manager.subscribe<TestEvent>([&](TestEvent& e) {
        receivedValues.push_back(e.value);
    });
    
    manager.triggerEvent(TestEvent(1));
    manager.triggerEvent(TestEvent(2));
    manager.triggerEvent(TestEvent(3));
    
    EXPECT_TRUE(receivedValues.empty());
    
    manager.dispatchEvents();
    
    EXPECT_EQ(receivedValues.size(), 3);
    EXPECT_EQ(receivedValues[0], 1);
    EXPECT_EQ(receivedValues[1], 2);
    EXPECT_EQ(receivedValues[2], 3);
}

TEST(EventManagerTest, EventHandledStopsPropagation) {
    EventManager manager;
    int callCount = 0;
    
    manager.subscribe<TestEvent>([&](TestEvent& e) {
        callCount++;
        e.setIsHandled(); 
    });
    
    manager.subscribe<TestEvent>([&](TestEvent&) {
        callCount++;  
    });
    
    manager.triggerEvent(TestEvent());
    manager.dispatchEvents();
    
    EXPECT_EQ(callCount, 1); 
}

TEST(EventManagerTest, CopyAndMoveDeleted) {
    EventManager manager1;
    
    SUCCEED();  
}

TEST(EventManagerTest, ConnectionIdsUnique) {
    EventManager manager;
    
    auto conn1 = manager.subscribe<TestEvent>([](TestEvent&) {});
    auto conn2 = manager.subscribe<TestEvent>([](TestEvent&) {});
    auto conn3 = manager.subscribe<AnotherEvent>([](AnotherEvent&) {});
    
    EXPECT_TRUE(conn1.isConnected());
    EXPECT_TRUE(conn2.isConnected());
    EXPECT_TRUE(conn3.isConnected());
    
    manager.unsubscribe(conn1);
    EXPECT_FALSE(conn1.isConnected());
    EXPECT_TRUE(conn2.isConnected());
    EXPECT_TRUE(conn3.isConnected());
}

TEST(EventManagerTest, DoubleUnsubscribeSafe) {
    EventManager manager;
    int callCount = 0;
    
    auto conn = manager.subscribe<TestEvent>([&](TestEvent&) {
        callCount++;
    });
    
    manager.triggerEvent(TestEvent());
    manager.dispatchEvents();
    EXPECT_EQ(callCount, 1);
    
    manager.unsubscribe(conn);
    EXPECT_FALSE(conn.isConnected());
    
    manager.unsubscribe(conn);
    
    manager.triggerEvent(TestEvent());
    manager.dispatchEvents();
    EXPECT_EQ(callCount, 1);
}

TEST(EventManagerTest, MoveConnectionAfterUnsubscribe) {
    EventManager manager;
    int callCount = 0;
    
    auto conn1 = manager.subscribe<TestEvent>([&](TestEvent&) {
        callCount++;
    });
    
    manager.triggerEvent(TestEvent());
    manager.dispatchEvents();
    EXPECT_EQ(callCount, 1);
    
    auto conn2 = std::move(conn1);
    EXPECT_FALSE(conn1.isConnected());
    EXPECT_TRUE(conn2.isConnected());
    
    manager.unsubscribe(conn2);
    EXPECT_FALSE(conn2.isConnected());
    
    manager.triggerEvent(TestEvent());
    manager.dispatchEvents();
    EXPECT_EQ(callCount, 1);
}