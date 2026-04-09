// ========================= SERVER - Server.cpp =========================
#include "Server.h"
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

Server::Server() {}
Server::~Server() {}

void Server::_bind_methods() {
    ClassDB::bind_method(D_METHOD("_on_peer_connected", "id"), &Server::_on_peer_connected);
    ClassDB::bind_method(D_METHOD("receive_data", "bytes"), &Server::receive_data);
}

void Server::_ready() {
    UtilityFunctions::print("Server running from CPP");

    peer.instantiate();

    Error err = peer->create_server(8080, "*");
    if (err != OK) {
        UtilityFunctions::print("Failed to start server");
        return;
    }

    get_multiplayer()->set_multiplayer_peer(peer);

    // Configure RPC for receive_data (client will call this)
    Dictionary config;
    config["rpc_mode"] = MultiplayerAPI::RPC_MODE_ANY_PEER;
    rpc_config("receive_data", config);

    get_multiplayer()->connect("peer_connected", Callable(this, "_on_peer_connected"));

    UtilityFunctions::print("Server started on port 8080");
}

void Server::_process(double delta) {
    if (get_multiplayer().is_valid()) {
        get_multiplayer()->poll();
    }
}

void Server::_on_peer_connected(int id) {
    UtilityFunctions::print("Client connected: ", id);
}

void Server::receive_data(PackedByteArray bytes) {
    UtilityFunctions::print("Server received data!");
    MyData d = deserialize(bytes);
    
    UtilityFunctions::print("Server got: ", d.message, " ", d.number);
    
    // Echo back to the client that sent the data
    int sender_id = get_multiplayer()->get_remote_sender_id();
    UtilityFunctions::print("Sending back to client: ", sender_id);
    
    Error err = rpc_id(sender_id, "receive_data_back", bytes);
    UtilityFunctions::print("Send back result: ", err);
}

PackedByteArray Server::serialize(const MyData &data) {
    PackedByteArray bytes;

    CharString utf8 = data.message.utf8();
    int len = utf8.length();

    bytes.resize(sizeof(int) + len + sizeof(int));

    uint8_t *w = bytes.ptrw();

    memcpy(w, &len, sizeof(int));
    memcpy(w + sizeof(int), utf8.get_data(), len);
    memcpy(w + sizeof(int) + len, &data.number, sizeof(int));

    return bytes;
}

Server::MyData Server::deserialize(const PackedByteArray &bytes) {
    MyData data;

    const uint8_t *r = bytes.ptr();

    int len;
    memcpy(&len, r, sizeof(int));

    data.message = String::utf8((const char *)(r + sizeof(int)), len);

    memcpy(&data.number, r + sizeof(int) + len, sizeof(int));

    return data;
}