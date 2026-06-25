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


// =========================== GAME PACKET ========================

// Server - > Client
struct PackagedPlayerInfo { 
    Vector<PlayerNetworkData> player_packet;
    int health;
    int kills;
    int deaths;
    int current_map = 0;
};


// =========================== VOTING PACKET ======================


// Client -> Server
struct VoteData{
    uint32_t  player_id;
    int map_number = 0;
};


// Server -> Client
struct PlayerVotingData {
    Vector<VoteData> player_vote_packet;
    bool victory = false;
    int voting_time_left = 0;
};



// Server's memory aboutp the player
struct PlayerInfo {
    // Game State data
    int peer_id;
    PlayerNetworkData latest_packet;
    
    int health;
    int kills;
    int deaths;

    bool dead = false;
    int death_time = 0;

    float last_update_time;

    // Vote State Data
    VoteData vote_data;
};


struct MapData {
    MapData(String nm, String file_nm) {
        name = nm;
        file_nm = file_nm;
    }
    MapData() {}
    String name;
    String file_name;
    int votes = 0;
};

class Server : public Node {
    GDCLASS(Server, Node);

private:
    enum PacketTypeID : uint8_t {
    PACKET_GAME_UPDATE = 1,  
    PACKET_VOTE        = 2,
    };

    enum GameState {inGame, Voting};
    GameState current_game_state = inGame;

    int map_start_time = 0;
    int map_time = 15000;

    int voting_time = 15000;
    int voting_start_time = 0;
    int current_map = 0;
    int current_map_votes = 0;
    HashMap<int, MapData> maps;

    Ref<WebSocketMultiplayerPeer> peer;
    HashMap<int, PlayerInfo> players;  // player_id -> PlayerInfo
    float broadcast_timer;
    int respawn_time = 3000;
    
    void receieve_packet();

    void process_game_packet(int sender_id, const PackedByteArray &data);
    void process_voting_packet(int sender_id, const PackedByteArray &data);

    void send_game_packets();
    void send_voting_packets();


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
    
    // Total size: count (4 bytes) + array elements + health, kills, deaths, current_map (4 ints)
    bytes.resize(sizeof(uint32_t) + count * sizeof(PlayerNetworkData) + 4 * sizeof(int));
    uint8_t *w = bytes.ptrw();
    
    // Write count
    memcpy(w, &count, sizeof(uint32_t));
    w += sizeof(uint32_t);
    
    // Write each PlayerNetworkData
    for (int i = 0; i < (int)count; i++) {
        memcpy(w, &info.player_packet[i], sizeof(PlayerNetworkData));
        w += sizeof(PlayerNetworkData);
    }
    
    // Write health, kills, deaths, current_map (advance pointer each time)
    memcpy(w, &info.health, sizeof(int));
    w += sizeof(int);
    memcpy(w, &info.kills, sizeof(int));
    w += sizeof(int);
    memcpy(w, &info.deaths, sizeof(int));
    w += sizeof(int);
    memcpy(w, &info.current_map, sizeof(int));
    
    return bytes;
}

inline PackagedPlayerInfo deserialize_packaged_player_info(const PackedByteArray &bytes) {
    PackagedPlayerInfo info;
    // Minimum size: count (4 bytes) + 4 ints
    if (bytes.size() < sizeof(uint32_t) + 4 * sizeof(int)) return info;
    
    const uint8_t *r = bytes.ptr();
    uint32_t count;
    memcpy(&count, r, sizeof(uint32_t));
    r += sizeof(uint32_t);
    
    // Verify total size matches expected
    size_t expected_size = sizeof(uint32_t) + count * sizeof(PlayerNetworkData) + 4 * sizeof(int);
    if (bytes.size() != expected_size) return info;
    
    // Resize vector and copy elements
    info.player_packet.resize(count);
    for (uint32_t i = 0; i < count; i++) {
        memcpy(&info.player_packet.write[i], r, sizeof(PlayerNetworkData));
        r += sizeof(PlayerNetworkData);
    }
    
    // Read health, kills, deaths, current_map (advance pointer each time)
    memcpy(&info.health, r, sizeof(int));
    r += sizeof(int);
    memcpy(&info.kills, r, sizeof(int));
    r += sizeof(int);
    memcpy(&info.deaths, r, sizeof(int));
    r += sizeof(int);
    memcpy(&info.current_map, r, sizeof(int));
    
    return info;
}

// ========== VoteData (Client -> Server) ==========
inline PackedByteArray serialize_vote_data(const VoteData &data) {
    PackedByteArray bytes;
    // Total size: player_id (4 bytes) + map_number (4 bytes)
    bytes.resize(sizeof(uint32_t) + sizeof(int));
    uint8_t *w = bytes.ptrw();
    
    memcpy(w, &data.player_id, sizeof(uint32_t));
    w += sizeof(uint32_t);
    memcpy(w, &data.map_number, sizeof(int));
    
    return bytes;
}

inline VoteData deserialize_vote_data(const PackedByteArray &bytes) {
    VoteData data;
    if (bytes.size() < sizeof(uint32_t) + sizeof(int)) {
        return data; // not enough data
    }
    
    const uint8_t *r = bytes.ptr();
    memcpy(&data.player_id, r, sizeof(uint32_t));
    r += sizeof(uint32_t);
    memcpy(&data.map_number, r, sizeof(int));
    
    return data;
}

// ========== PlayerVotingData (Server -> Client) ==========
inline PackedByteArray serialize_player_voting_data(const PlayerVotingData &data) {
    PackedByteArray bytes;
    uint32_t count = data.player_vote_packet.size();
    
    // Total size: count (4 bytes) + (count * sizeof(VoteData))
    // sizeof(VoteData) = 8 bytes now (uint32_t + int)
    bytes.resize(sizeof(uint32_t) + count * sizeof(VoteData));
    uint8_t *w = bytes.ptrw();
    
    // Write count
    memcpy(w, &count, sizeof(uint32_t));
    w += sizeof(uint32_t);
    
    // Write each VoteData
    for (int i = 0; i < (int)count; i++) {
        memcpy(w, &data.player_vote_packet[i], sizeof(VoteData));
        w += sizeof(VoteData);
    }
    
    return bytes;
}

inline PlayerVotingData deserialize_player_voting_data(const PackedByteArray &bytes) {
    PlayerVotingData data;
    
    if (bytes.size() < sizeof(uint32_t)) return data;
    
    const uint8_t *r = bytes.ptr();
    uint32_t count;
    memcpy(&count, r, sizeof(uint32_t));
    r += sizeof(uint32_t);
    
    // Verify total size matches expected
    size_t expected_size = sizeof(uint32_t) + count * sizeof(VoteData);
    if (bytes.size() != expected_size) return data;
    
    data.player_vote_packet.resize(count);
    for (uint32_t i = 0; i < count; i++) {
        memcpy(&data.player_vote_packet.write[i], r, sizeof(VoteData));
        r += sizeof(VoteData);
    }
    
    return data;
}