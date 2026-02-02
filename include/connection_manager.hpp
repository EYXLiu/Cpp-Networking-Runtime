#ifndef CONNECTION_MANAGER_HPP
#define CONNECTION_MANAGER_HPP

#include <connection.hpp>
#include <unordered_map>

class ConnectionManager {
public:
    ConnectionManager();
    ~ConnectionManager();

    void add_connection(std::unique_ptr<Connection> conn);
    void remove_connect(int fd);
private:
    std::unordered_map<int, std::unique_ptr<Connection>> connections_;
};

#endif 
