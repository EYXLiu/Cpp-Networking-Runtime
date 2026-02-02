#include <acceptor.hpp>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>

Acceptor::Acceptor(int port, std::vector<Reactor*>& reactor, BufferPool& pool, ConnectionManager& conn_mgr) : fd_(-1), reactors_(reactor), next_reactor_(0), pool_(pool), conn_mgr_(conn_mgr) {
    fd_ = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    bind(fd_, (sockaddr*)&addr, sizeof(addr));
    listen(fd_, SOMAXCONN);
    fcntl(fd_, F_SETFL, fcntl(fd_, F_GETFL, 0) | O_NONBLOCK);
}

Acceptor::~Acceptor() {
    if (fd_ >= 0) close(fd_);
}

void Acceptor::on_readable() {
    while (true) {
        sockaddr_in client_addr;
        socklen_t addrlen = sizeof(client_addr);
        int client_fd = accept(fd_, (sockaddr*)&client_addr, &addrlen);
        if (client_fd < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) break;
            perror("accept");
            continue;
        }

        fcntl(client_fd, F_SETFL, fcntl(client_fd, F_GETFL, 0) | O_NONBLOCK);

        Reactor& reactor = *reactors_[next_reactor_];
        next_reactor_ = (next_reactor_ + 1) % reactors_.size();

        auto conn = std::make_unique<Connection>(client_fd, pool_, reactor);
        reactor.add_fd(client_fd, true, true, conn.get());

        conn_mgr_.add_connection(std::move(conn));
    }
}

void Acceptor::on_writeable() {}

int Acceptor::get_fd() {
    return fd_;
}
