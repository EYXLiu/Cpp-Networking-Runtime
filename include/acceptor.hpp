#ifndef ACCEPTOR_HPP
#define ACCEPTOR_HPP

#include <handler.hpp>
#include <reactor.hpp>
#include <buffer_pool.hpp>
#include <connection_manager.hpp>

class Acceptor : public Handler {
public:
    Acceptor(int port, Reactor& reactor, BufferPool& pool, ConnectionManager& conn_mgr);
    ~Acceptor();

    void on_readable() override;
    void on_writeable() override;
    int get_fd() override;
private:
    int fd_;
    Reactor& reactor_;
    BufferPool& pool_;
    ConnectionManager& conn_mgr_;
};

#endif 
