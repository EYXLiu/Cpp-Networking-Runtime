#ifndef REACTOR_HPP
#define REACTOR_HPP

#include <unordered_map>
#include <sys/event.h>
#include <sys/time.h>
#include <functional>
#include <handler.hpp>


class Reactor {
public:
    Reactor();
    ~Reactor();

    void add_fd(int fd, bool read, bool write, Handler* handler);
    void remove_fd(int fd);
    void enable_read(int fd);
    void disable_read(int fd);
    void enable_write(int fd);
    void disable_write(int fd);
    void run();

    void schedule_close(int fd);
    void process_closes();
private:
    int kq_;
    std::unordered_map<int, Handler*> handlers_;

    std::mutex close_mutex_;
    std::vector<int> fds_to_close_;
};

#endif 
