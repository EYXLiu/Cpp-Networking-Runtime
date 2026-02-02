#ifndef BUFFER_POOL_HPP
#define BUFFER_POOL_HPP

#include <cstdint>
#include <cstddef>
#include <vector>
#include <stack>
#include <memory>

struct Buffer { 
    uint8_t* data;
    size_t size;
    size_t used;
};

class BufferPool {
public:
    BufferPool(size_t chunk_size, size_t num_chunks);
    Buffer acquire();
    void release(Buffer buf);
private:
    std::vector<std::unique_ptr<uint8_t[]>> chunks_;
    std::stack<Buffer> free_buffers_;
};

#endif
