#include <connection_manager.hpp>

ConnectionManager::ConnectionManager() {}

ConnectionManager::~ConnectionManager() {}

void ConnectionManager::add_connection(std::unique_ptr<Connection> conn) {
    connections_[conn->get_fd()] = std::move(conn);
}

void ConnectionManager::remove_connect(int fd) {
    connections_.erase(fd);
}
