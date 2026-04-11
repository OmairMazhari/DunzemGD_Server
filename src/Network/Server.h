// ========================= SERVER - Server.h =========================
#pragma once

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/multiplayer_api.hpp>
#include <godot_cpp/classes/web_socket_multiplayer_peer.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/templates/hash_map.hpp>
#include "NetworkProtocol.h"

using namespace godot;

struct PlayerInfo {
    int peer_id;
    Vector3 position;
    uint16_t input_flags;
    Vector2 mouse_offset;
    float last_update_time;
};

class Server : public Node {
    GDCLASS(Server, Node);

private:
    Ref<WebSocketMultiplayerPeer> peer;
    HashMap<int, PlayerInfo> players;  // player_id -> PlayerInfo
    float broadcast_timer;
    
    void process_raw_packet(int sender_id, const PackedByteArray &data);
    void broadcast_player_states();
    void send_player_state_to_client(int target_client_id, int player_id);
    
protected:
    static void _bind_methods();
    
public:
    Server();
    ~Server();
    
    void _ready() override;
    void _process(double delta) override;
    
    void _on_peer_connected(int id);
    void _on_peer_disconnected(int id);
};