#include <zmq.hpp>
#include <iostream>
#include <cassert>

struct cbhe_eid_t {
    uint64_t nodeId;
    uint64_t serviceId;
};

struct CommonHdr {
    uint16_t type;
    uint16_t flags;
};

struct RouteUpdateHdr {
    CommonHdr base;
    uint8_t unused3;
    uint8_t unused4;
    cbhe_eid_t nextHopEid;
    cbhe_eid_t finalDestEid;
    uint64_t route[20];
};

int main() {
    zmq::message_t message;
    zmq::context_t ctx;
    zmq::socket_t sock(ctx, zmq::socket_type::sub);
    
    try {
        sock.connect("tcp://*:10210");
        //sock.set(zmq::sockopt::subscribe, "");
        sock.setsockopt(ZMQ_SUBSCRIBE, "", std::strlen(""));
    } catch (const zmq::error_t & ex) {
        std::cerr << ex.what() << std::endl;
    }

    zmq::pollitem_t items[] = {{sock.handle(), 0, ZMQ_POLLIN, 0}};

    std::cout << "socket connected to tcp://*:10210" << std::endl;
    std::cout << "waiting for a message..." << std::endl;

    while (true) {
        int rc = zmq::poll(&items[0], 1, -1);
        assert(rc >= 0);

        if (items[0].revents & ZMQ_POLLIN) { 
            std::cout << "event from router" << std::endl;

            if (!sock.recv(message, zmq::recv_flags::none)) {
                std::cout << "no event for router" << std::endl;
                return 1;
            }

            if (message.size() < sizeof(CommonHdr)) {
                std::cout << "unknown" << std::endl;
                return 1;
            }

            CommonHdr *common = (CommonHdr *)message.data();
            if (common->type == 0xFC07) {
                std::cout << "route update" << std::endl;
                RouteUpdateHdr *update = (RouteUpdateHdr *)message.data();
                uint64_t node = update->nextHopEid.nodeId;
                std::cout << node << std::endl;
                return 0;
            }

           std::cout << "failed to do anything with event" << std::endl;
        }
    }

    return 0;
}
