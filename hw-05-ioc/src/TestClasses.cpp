#include "TestClasses.hpp"

std::string TestClassA::getName() const {
    return "TestClassA";
}

TestClassB::TestClassB(std::shared_ptr<TestClassA> a) 
    : dependencyA_(std::move(a)) {
}

std::shared_ptr<TestClassA> TestClassB::getDependencyA() const {
    return dependencyA_;
}

std::string TestClassB::getName() const {
    return "TestClassB";
}

std::string TestImplementation::getInterfaceInfo() const {
    return "TestImplementation";
}