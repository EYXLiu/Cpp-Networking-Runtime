#include <connection.hpp>
#include <unistd.h>
#include <iostream>

Connection::Connection(int fd, BufferPool& pool) : fd_(fd), pool_(pool) {
    try {
        read_buf_ = pool_.acquire();
    } catch (std::runtime_error e) {
        std::cout << "Failed to acquire buffer" << std::endl;
        close(fd_);
    }
}

Connection::~Connection() {
    pool_.release(read_buf_);
    close(fd_);
}

void Connection::on_readable() {
    ssize_t n = read(fd_, read_buf_.data, read_buf_.size);
    if (n <= 0) {
        std::cout << "Client closed fd=" << fd_ << std::endl;
        pool_.release(read_buf_);
        close(fd_);
        return;
    }
    write_buf_size_ = n;
    on_writeable();
}

void Connection::on_writeable() {
    if (write_buf_size_ == 0) return;
    write(fd_, read_buf_.data, write_buf_size_);
    write_buf_size_ = 0;
}

int Connection::get_fd() {
    return fd_;
}
