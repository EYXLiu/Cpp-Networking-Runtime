#ifndef HANDLER_HPP
#define HANDLER_HPP

class Handler {
public:
    virtual ~Handler() = default;

    virtual void on_readable() = 0;
    virtual void on_writeable() = 0;
    virtual int get_fd() = 0;
};

#endif
