#include <gtest/gtest.h>
#include "engine/events/EventHandler.hpp"
#include "engine/events/Event.hpp"

struct TestEvent : public Event {
    int value;
    std::string message;
    
    TestEvent(int v, const std::string& m) : value(v), message(m) {}
};

TEST(EventHandlerTest, InterfaceGetId) {
    struct MockHandler : public EventHandlerInterface {
        MockHandler(int id) : EventHandlerInterface(id) {}
        void call(Event&) override {}
    };
    
    MockHandler handler(42);
    EXPECT_EQ(handler.getId(), 42);
}

TEST(EventHandlerTest, TemplateHandlerConstruction) {
    int callCount = 0;
    int capturedValue = 0;
    std::string capturedMessage;
    
    Subscriber<TestEvent> subscriber = [&](TestEvent& e) {
        callCount++;
        capturedValue = e.value;
        capturedMessage = e.message;
    };
    
    EventHandler<TestEvent> handler(subscriber, 100);
    EXPECT_EQ(handler.getId(), 100);
    
    TestEvent event(42, "Hello");
    handler.exec(event);
    
    EXPECT_EQ(callCount, 1);
    EXPECT_EQ(capturedValue, 42);
    EXPECT_EQ(capturedMessage, "Hello");
}

TEST(EventHandlerTest, StaticCastSafety) {
    bool called = false;
    Subscriber<TestEvent> subscriber = [&](TestEvent& e) {
        called = true;
        EXPECT_EQ(e.value, 999);
    };
    
    EventHandler<TestEvent> handler(subscriber, 1);
    
    TestEvent event(999, "Test");
    handler.exec(event);
    EXPECT_TRUE(called);
}

TEST(EventHandlerTest, DerivedEventConcept) {
    EventHandler<TestEvent> handler(Subscriber<TestEvent>{}, 1);
    SUCCEED();
}