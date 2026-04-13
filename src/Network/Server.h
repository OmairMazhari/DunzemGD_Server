// ========================= SERVER - Server.h =========================
#pragma once

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/multiplayer_api.hpp>
#include <godot_cpp/classes/web_socket_multiplayer_peer.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/templates/hash_map.hpp>
#include "NetworkProtocol.h"

using namespace godot;

// struct PlayerInfoPacket {
//     int peer_id;
//     Vector3 position;
//     uint16_t input_flags;
//     Vector2 mouse_offset;
//     float last_update_time;
// };


struct PackagedPlayerInfo { 
    Vector<PlayerNetworkData> player_packet;
};

struct PlayerInfo {
    PlayerNetworkData latest_packet;
    float last_update_time;

    int peer_id;
    bool dead = false;
    
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
    void send_packaged_player_info_to_client(int target_client_id);
    
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


inline PackedByteArray serialize_packaged_player_info(const PackagedPlayerInfo &info) {
    PackedByteArray bytes;
    uint32_t count = info.player_packet.size();
    
    bytes.resize(sizeof(uint32_t) + count * sizeof(PlayerNetworkData));
    uint8_t *w = bytes.ptrw();
    
    memcpy(w, &count, sizeof(uint32_t));
    w += sizeof(uint32_t);
    
    for (int i = 0; i < (int)count; i++) {
        memcpy(w, &info.player_packet[i], sizeof(PlayerNetworkData));
        w += sizeof(PlayerNetworkData);
    }
    return bytes;
};

inline PackagedPlayerInfo deserialize_packaged_player_info(const PackedByteArray &bytes) {
    PackagedPlayerInfo info;
    if (bytes.size() < sizeof(uint32_t)) return info;
    
    const uint8_t *r = bytes.ptr();
    uint32_t count;
    memcpy(&count, r, sizeof(uint32_t));
    r += sizeof(uint32_t);
    
    if (bytes.size() != sizeof(uint32_t) + count * sizeof(PlayerNetworkData)) {
        return info;
    }
    
    info.player_packet.resize(count);
    for (uint32_t i = 0; i < count; i++) {
        memcpy(&info.player_packet.write[i], r, sizeof(PlayerNetworkData));
        r += sizeof(PlayerNetworkData);
    }
    return info;
}