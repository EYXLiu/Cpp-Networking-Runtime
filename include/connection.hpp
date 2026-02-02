#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <handler.hpp>
#include <buffer_pool.hpp>
#include <reactor.hpp>

class Connection : public Handler {
public:
    Connection(int fd, BufferPool& pool, Reactor& reactor);
    ~Connection();

    void on_readable() override;
    void on_writeable() override;
    int get_fd() override;
private:
    int fd_;
    Buffer read_buf_;
    Buffer write_buf_;
    size_t write_size_;
    size_t write_offset_;
    BufferPool& pool_;
    Reactor& reactor_;

    void handle_backpressure();
};

#endif 
