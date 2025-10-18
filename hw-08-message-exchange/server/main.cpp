#include "http_server/httplib.h"
#include <iostream>

int main() {
    httplib::Server svr;

    // GET endpoint, возвращающий строку
    svr.Get("/hello", [](const httplib::Request& req, httplib::Response& res) {
        std::cout << "[DEBUG] GET request received on /hello" << std::endl;
        res.set_content("Hello World!", "text/plain");
    });

    // GET endpoint, возвращающий JSON
    svr.Get("/hello-json", [](const httplib::Request& req, httplib::Response& res) {
        std::cout << "[DEBUG] GET request received on /hello-json" << std::endl;
        res.set_content("{\"message\": \"Hello World!\"}", "application/json");
    });

    std::cout << "Server is running on http://localhost:8080" << std::endl;
    std::cout << "Try: http://localhost:8080/hello" << std::endl;
    std::cout << "Try: http://localhost:8080/hello-json" << std::endl;
    
    svr.listen("0.0.0.0", 8080);
    
    return 0;
}
