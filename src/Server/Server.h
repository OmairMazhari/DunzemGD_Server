// ========================= SERVER - Server.h =========================
#pragma once

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/multiplayer_api.hpp>
#include <godot_cpp/classes/web_socket_multiplayer_peer.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

class Server : public Node {
    GDCLASS(Server, Node);

private:
    Ref<WebSocketMultiplayerPeer> peer;

protected:
    static void _bind_methods();
    
public:
    Server();
    ~Server();

    void _ready() override;
    void _process(double delta) override;

    void _on_peer_connected(int id);
    void receive_data(PackedByteArray bytes);

    struct MyData {
        String message;
        int number;
    };

    PackedByteArray serialize(const MyData &data);
    MyData deserialize(const PackedByteArray &bytes);
};