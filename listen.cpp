#include <zmq.hpp>
#include <iostream>

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
        //sock.bind("tcp://*:10210");
        sock.set(zmq::sockopt::subscribe, "");
    } catch (const zmq::error_t & ex) {
        std::cerr << ex.what() << std::endl;
    }

    std::cout << "socket connected to tcp://*:10210" << std::endl;
    std::cout << "waiting for a message..." << std::endl;
    //while(!sock.recv(message, zmq::recv_flags::none));
    sock.recv(message, zmq::recv_flags::none);

    if (message.size() < sizeof(CommonHdr)) std::cout << "unknown" << std::endl;
    CommonHdr *common = (CommonHdr *)message.data();
    if (common->type == 0xFC07) {
        RouteUpdateHdr *update = (RouteUpdateHdr *)message.data();
        uint64_t node = update->nextHopEid.nodeId;
        std::cout << node << std::endl;
    }

    return 0;
}
