#ifndef ACCEPTOR_HPP
#define ACCEPTOR_HPP

#include <handler.hpp>
#include <reactor.hpp>
#include <buffer_pool.hpp>
#include <connection.hpp>
#include <unordered_map>

class Acceptor : public Handler {
public:
    Acceptor(int port, Reactor& reactor, BufferPool& pool);
    ~Acceptor();

    void on_readable() override;
    void on_writeable() override;
    int get_fd() override;

    void connection_remove(int fd);
private:
    int fd_;
    Reactor& reactor_;
    BufferPool& pool_;
    std::unordered_map<int, std::unique_ptr<Connection>> conn_mgr_;
};

#endif 
