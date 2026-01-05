#include <gtest/gtest.h>
#include "engine/events/Event.hpp"

TEST(EventTest, DefaultConstructedNotHandled) {
    Event event;
    EXPECT_FALSE(event.isHandled());
}

TEST(EventTest, SetIsHandled) {
    Event event;
    event.setIsHandled();
    EXPECT_TRUE(event.isHandled());
}

TEST(EventTest, VirtualDestructor) {
    struct TestEvent : public Event {
        int value = 42;
    };
    
    Event* event = new TestEvent();
    delete event;
    SUCCEED();
}

TEST(EventTest, DerivedEventPreservesHandledFlag) {
    struct TestEvent : public Event {
        int data = 100;
    };
    
    TestEvent event;
    EXPECT_FALSE(event.isHandled());
    
    event.setIsHandled();
    EXPECT_TRUE(event.isHandled());
    EXPECT_EQ(event.data, 100);
}