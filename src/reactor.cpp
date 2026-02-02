#include <reactor.hpp>
#include <acceptor.hpp>
#include <connection.hpp>
#include <iostream>
#include <unistd.h>

Reactor::Reactor() {
    kq_ = kqueue();
    if (kq_ < 0) perror("kqueue");
    changes_.reserve(1024);
}

Reactor::~Reactor() {
    close(kq_);
}

void Reactor::add_fd(int fd, bool read, bool write, Handler* handler) {
    if (read) enable_read(fd);
    if (write) enable_write(fd);
    handlers_[fd] = handler;
}

void Reactor::enable_read(int fd) {
    struct kevent ev;
    EV_SET(&ev, fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, nullptr);
    changes_.push_back(ev);
    kevent(kq_, &changes_.back(), 1, nullptr, 0, nullptr);
}

void Reactor::disable_read(int fd) {
    struct kevent ev;
    EV_SET(&ev, fd, EVFILT_READ, EV_DISABLE, 0, 0, nullptr);
    changes_.push_back(ev);
    kevent(kq_, &changes_.back(), 1, nullptr, 0, nullptr);
}

void Reactor::enable_write(int fd) {
    struct kevent ev;
    EV_SET(&ev, fd, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, nullptr);
    changes_.push_back(ev);
    kevent(kq_, &changes_.back(), 1, nullptr, 0, nullptr);
}

void Reactor::disable_write(int fd) {
    struct kevent ev;
    EV_SET(&ev, fd, EVFILT_WRITE, EV_DISABLE, 0, 0, nullptr);
    changes_.push_back(ev);
    kevent(kq_, &changes_.back(), 1, nullptr, 0, nullptr);
}

void Reactor::run() {
    std::vector<struct kevent> events(1024);

    while (true) {
        if (!changes_.empty()) {
            kevent(kq_, changes_.data(), changes_.size(), nullptr, 0, nullptr);
            changes_.clear();
        }

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
