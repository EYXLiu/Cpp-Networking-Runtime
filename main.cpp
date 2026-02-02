#include <reactor.hpp>
#include <buffer_pool.hpp>
#include <acceptor.hpp>
#include <connection_manager.hpp>
#include <thread>
#include <iostream>

int main() {
    int num_reactors = std::thread::hardware_concurrency();
    std::vector<std::unique_ptr<Reactor>> reactors;
    std::vector<Reactor*> reactor_ptrs;
    std::vector<std::thread> reactor_threads;

    if (num_reactors == 0) num_reactors = 2;

    for (int i = 0; i < num_reactors; i++) {
        auto r = std::make_unique<Reactor>();
        reactor_ptrs.push_back(r.get());
        reactors.push_back(std::move(r));
        reactor_threads.emplace_back([i, &reactors]() {
            reactors[i]->run();
        });
    }

    BufferPool buffer_pool(128, 4096);
    ConnectionManager conn_mgr;
    Acceptor acceptor(8080, reactor_ptrs, buffer_pool, conn_mgr);


    reactors[0]->add_fd(acceptor.get_fd(), true, false, &acceptor);

    for (auto& t : reactor_threads) t.join();

    return 0;
}
