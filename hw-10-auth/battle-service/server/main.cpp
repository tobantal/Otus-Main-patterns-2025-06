#include "BattleApplication.hpp"

int main() {
    try {
        BattleApplication app;
        app.run("0.0.0.0", 8090);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}