// ========================= SHARED - NetworkProtocol.h =========================
#pragma once

#include <godot_cpp/variant/packed_byte_array.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <cstring>

using namespace godot;

// Bit flags for inputs (16 possible inputs in 2 bytes)
enum InputFlags : uint16_t {
    INPUT_W       = 1 << 0,
    INPUT_A       = 1 << 1,
    INPUT_S       = 1 << 2,
    INPUT_D       = 1 << 3,
    INPUT_SPACE   = 1 << 4,
    INPUT_1       = 1 << 5,
    INPUT_2       = 1 << 6,
    INPUT_3       = 1 << 7,
    INPUT_SHIFT   = 1 << 8,
    INPUT_MOUSE1  = 1 << 9,
    INPUT_MOUSE2  = 1 << 10,
};

enum PacketType : uint8_t {
    TYPE_PLAYER_UPDATE = 1,
    TYPE_PLAYER_STATE  = 2,
};

#pragma pack(push, 1)
struct PlayerNetworkData {
    uint8_t  packet_type;
    uint32_t player_id;
    uint16_t input_flags;
    float    pos_x;
    float    pos_y;
    float    pos_z;
    int16_t  mouse_offset_x;
    int16_t  mouse_offset_y;
    // TOTAL: 23 bytes
};
#pragma pack(pop)


// ========== SERIALIZATION FUNCTIONS ==========
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
