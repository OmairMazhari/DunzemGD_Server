// ========================= SHARED - NetworkProtocol.h =========================
#pragma once

#include <godot_cpp/variant/packed_byte_array.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <cstring>

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

// Player data structure for network transfer
#pragma pack(push, 1)
struct PlayerNetworkData {
    uint8_t  packet_type;    // 1 byte - TYPE_PLAYER_UPDATE or TYPE_PLAYER_STATE
    uint32_t  player_id;      // 4 byte - Which player this data belongs to
    uint16_t input_flags;    // 2 bytes - All W,A,S,D,Space,1,2,3,Shift,Mouse1,Mouse2
    float    pos_x;          // 4 bytes - X position
    float    pos_y;          // 4 bytes - Y position
    float    pos_z;          // 4 bytes - Z position
    int16_t  mouse_offset_x; // 2 bytes - Mouse X offset (-32768 to 32767)
    int16_t  mouse_offset_y; // 2 bytes - Mouse Y offset
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