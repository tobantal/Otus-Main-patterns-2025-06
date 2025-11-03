#include "AuthApplication.hpp"

int main() {
    try {
        AuthApplication app;
        app.run("0.0.0.0", 8095);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}