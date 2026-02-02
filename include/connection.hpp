#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <handler.hpp>
#include <buffer_pool.hpp>

class Connection : public Handler {
public:
    Connection(int fd, BufferPool& pool);
    ~Connection();

    void on_readable() override;
    void on_writeable() override;
    int get_fd() override;
private:
    int fd_;
    Buffer read_buf_;
    int write_buf_size_;
    BufferPool& pool_;
};

#endif 
