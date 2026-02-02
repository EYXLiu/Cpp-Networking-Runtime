#include <reactor.hpp>
#include <buffer_pool.hpp>
#include <acceptor.hpp>
#include <connection_manager.hpp>

int main() {
    Reactor reactor;
    BufferPool buffer_pool(128, 4096);
    ConnectionManager conn_mgr;
    Acceptor acceptor(8080, reactor, buffer_pool, conn_mgr);

    reactor.add_fd(acceptor.get_fd(), true, false, &acceptor);
    reactor.run();
}
