#include <buffer_pool.hpp>

BufferPool::BufferPool(size_t chunk_size, size_t num_chunks) {
    for (size_t i = 0; i < num_chunks; i++) {
        auto mem = std::make_unique<uint8_t[]>(chunk_size);
        free_buffers_.push({mem.get(), chunk_size, 0});
        chunks_.push_back(std::move(mem));
    }
}

Buffer BufferPool::acquire() {
    if (free_buffers_.empty()) throw std::runtime_error("No free buffers");
    Buffer buf = free_buffers_.top();
    free_buffers_.pop();
    return buf;
}

void BufferPool::release(Buffer buf) {
    free_buffers_.push(buf);
}
