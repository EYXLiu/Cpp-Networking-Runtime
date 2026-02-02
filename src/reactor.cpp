#include <reactor.hpp>
#include <acceptor.hpp>
#include <connection.hpp>
#include <iostream>
#include <unistd.h>

Reactor::Reactor() {
    kq_ = kqueue();
    if (kq_ < 0) perror("kqueue");
}

Reactor::~Reactor() {
    close(kq_);
}

void Reactor::add_fd(int fd, bool read, bool write, Handler* handler) {
    if (read) enable_read(fd);
    if (write) enable_write(fd);
    handlers_[fd] = handler;
}

void Reactor::remove_fd(int fd) {
    struct kevent ev;
    EV_SET(&ev, fd, EVFILT_READ, EV_DELETE, 0, 0, nullptr);
    kevent(kq_, &ev, 1, nullptr, 0, nullptr);
    EV_SET(&ev, fd, EVFILT_WRITE, EV_DELETE, 0, 0, nullptr);
    kevent(kq_, &ev, 1, nullptr, 0, nullptr);

    handlers_.erase(fd);
    close(fd);
}

void Reactor::enable_read(int fd) {
    struct kevent ev;
    EV_SET(&ev, fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, nullptr);
    kevent(kq_, &ev, 1, nullptr, 0, nullptr);
}

void Reactor::disable_read(int fd) {
    struct kevent ev;
    EV_SET(&ev, fd, EVFILT_READ, EV_DISABLE, 0, 0, nullptr);
    kevent(kq_, &ev, 1, nullptr, 0, nullptr);
}

void Reactor::enable_write(int fd) {
    struct kevent ev;
    EV_SET(&ev, fd, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, nullptr);
    kevent(kq_, &ev, 1, nullptr, 0, nullptr);
}

void Reactor::disable_write(int fd) {
    struct kevent ev;
    EV_SET(&ev, fd, EVFILT_WRITE, EV_DISABLE, 0, 0, nullptr);
    kevent(kq_, &ev, 1, nullptr, 0, nullptr);
}

void Reactor::run() {
    std::vector<struct kevent> events(1024);

    while (true) {
        process_closes();

        size_t nev = kevent(kq_, nullptr, 0, events.data(), events.size(), nullptr);
        if (nev < 0) {
            perror("kevent");
            continue;
        }

        for (size_t i = 0; i < nev; i++) {
            int fd = events[i].ident;
            Handler* handler = handlers_[fd];

            if (events[i].filter == EVFILT_READ) handler->on_readable();
            if (events[i].filter == EVFILT_WRITE) handler->on_writeable();
        }

        if (nev == events.size()) events.resize(events.size() * 2);
        else if (events.size() != 1024 && nev < events.size() / 2) events.resize(events.size() / 2);
    }
}

void Reactor::schedule_close(int fd) {
    std::lock_guard<std::mutex> lock(close_mutex_);
    fds_to_close_.push_back(fd);
}

void Reactor::process_closes() {
    std::lock_guard<std::mutex> lock(close_mutex_);
    for (int fd : fds_to_close_) {
        remove_fd(fd);
    }
    fds_to_close_.clear();
}
