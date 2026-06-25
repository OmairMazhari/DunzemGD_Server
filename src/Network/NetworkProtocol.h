// ========================= SHARED - NetworkProtocol.h =========================
#pragma once

#include <godot_cpp/variant/packed_byte_array.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <cstring>
#include <godot_cpp/templates/hash_map.hpp>

using namespace godot;

// Bit flags for inputs (16 possible inputs in 2 bytes)
enum InputFlags : uint16_t {
    INPUT_W       = 1 << 0,  // 1
    INPUT_A       = 1 << 1,  // 2
    INPUT_S       = 1 << 2,  // 4
    INPUT_D       = 1 << 3,  // 8
    INPUT_SPACE   = 1 << 4,  // 16
    INPUT_1       = 1 << 5,  // 32
    INPUT_2       = 1 << 6,  // 64
    INPUT_3       = 1 << 7,  // 128
    INPUT_SHIFT   = 1 << 8,  // 256
    INPUT_MOUSE1  = 1 << 9,  // 512
    INPUT_MOUSE2  = 1 << 10, // 1024
};

// Packet types
enum PacketType : uint8_t {
    TYPE_PLAYER_UPDATE = 1,  // Regular position/input update
    TYPE_PLAYER_STATE  = 2,  // Full player state broadcast
};

// Information packet that we need to render other player in our version of the world

#pragma pack(push, 1)
struct PlayerNetworkData {
    // char player_name[16] = "";    // 16 bytes – enough for 15 chars + null terminator
    uint8_t  packet_type;    // 1 byte - TYPE_PLAYER_UPDATE or TYPE_PLAYER_STATE
    uint32_t  player_id;      // 4 byte - Which player this data belongs to
    uint16_t input_flags;    // 2 bytes - All W,A,S,D,Space,1,2,3,Shift,Mouse1,Mouse2
    float    pos_x;          // 4 bytes - X position
    float    pos_y;          // 4 bytes - Y position
    float    pos_z;          // 4 bytes - Z position
    int16_t  mouse_offset_x; // 2 bytes - Mouse X offset (-32768 to 32767)
    int16_t  mouse_offset_y; // 2 bytes - Mouse Y offset
    float player_y_rotation;
    float camera_rotation;

    uint32_t health = 0;
    uint32_t kills = 0;
    uint32_t deaths= 0;
    // TOTAL: 20 bytes per player
};
#pragma pack(pop)

//  Sent by us to the server to inform the neccesary things we need the server to be aware about
#pragma pack(push, 1)


// Client -> Server
struct SelfNetworkData {
    PlayerNetworkData playerNetworkdata; 

    HashMap<int, int> tick_combat_report;
    // TOTAL: 20 bytes per player
};



#pragma pack(pop)

// Helper functions
inline PackedByteArray serialize_player_data(const PlayerNetworkData &data) {
    PackedByteArray bytes;
    bytes.resize(sizeof(PlayerNetworkData));
    memcpy(bytes.ptrw(), &data, sizeof(PlayerNetworkData));
    return bytes;
}

inline PlayerNetworkData deserialize_player_data(const PackedByteArray &bytes) {
    PlayerNetworkData data;
    if (bytes.size() >= sizeof(PlayerNetworkData)) {
        memcpy(&data, bytes.ptr(), sizeof(PlayerNetworkData));
    }
    return data;
}

inline PackedByteArray serialize_self_network_data(const SelfNetworkData &data) {
    PackedByteArray bytes;
    
    // PlayerNetworkData is fixed size (23 bytes as earlier)
    size_t fixed_size = sizeof(PlayerNetworkData);
    
    uint32_t map_size = data.tick_combat_report.size();
    bytes.resize(fixed_size + sizeof(uint32_t) + map_size * (sizeof(int) * 2));
    uint8_t *w = bytes.ptrw();
    
    // Write the entire PlayerNetworkData as raw bytes
    memcpy(w, &data.playerNetworkdata, sizeof(PlayerNetworkData));
    w += sizeof(PlayerNetworkData);
    
    // Write map size
    memcpy(w, &map_size, sizeof(uint32_t));
    w += sizeof(uint32_t);
    
    // Write each key-value pair
    for (const KeyValue<int, int>& kv : data.tick_combat_report) {
        memcpy(w, &kv.key, sizeof(int));
        w += sizeof(int);
        memcpy(w, &kv.value, sizeof(int));
        w += sizeof(int);
    }
    
    return bytes;
}

inline SelfNetworkData deserialize_self_network_data(const PackedByteArray &bytes) {
    SelfNetworkData result;
    result.tick_combat_report.clear();
    
    size_t fixed_size = sizeof(PlayerNetworkData);
    if (bytes.size() < fixed_size + sizeof(uint32_t)) {
        return result; // Not enough data
    }
    
    const uint8_t *r = bytes.ptr();
    
    // Read PlayerNetworkData
    memcpy(&result.playerNetworkdata, r, sizeof(PlayerNetworkData));
    r += sizeof(PlayerNetworkData);
    
    // Read map size
    uint32_t map_size;
    memcpy(&map_size, r, sizeof(uint32_t));
    r += sizeof(uint32_t);
    
    // Verify total size matches expected
    size_t expected = fixed_size + sizeof(uint32_t) + map_size * (sizeof(int) * 2);
    if (bytes.size() != expected) {
        return result; // Size mismatch, return empty
    }
    
    // Read map entries
    for (uint32_t i = 0; i < map_size; ++i) {
        int key, value;
        memcpy(&key, r, sizeof(int));
        r += sizeof(int);
        memcpy(&value, r, sizeof(int));
        r += sizeof(int);
        result.tick_combat_report[key] = value;
    }
    
    return result;
}

