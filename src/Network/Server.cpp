// ========================= SERVER - Server.cpp (WITH FULL PRINTING) =========================
#include "Server.h"
#include <godot_cpp/classes/time.hpp>
#include <godot_cpp/classes/web_socket_peer.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

Server::Server() : broadcast_timer(0) {}
Server::~Server() {}

void Server::_bind_methods() {
    ClassDB::bind_method(D_METHOD("_on_peer_connected", "id"), &Server::_on_peer_connected);
    ClassDB::bind_method(D_METHOD("_on_peer_disconnected", "id"), &Server::_on_peer_disconnected);
}

void Server::_ready() {
    UtilityFunctions::print("Game Server starting...");
    
    peer.instantiate();
    
    Error err = peer->create_server(8080);
    if (err != OK) {
        UtilityFunctions::print("Failed to start server on port 8080!");
        return;
    }
    
    // Connect to peer signals directly
    peer->connect("peer_connected", Callable(this, "_on_peer_connected"));
    peer->connect("peer_disconnected", Callable(this, "_on_peer_disconnected"));
    
    UtilityFunctions::print("Game Server started on port 8080");
}

void Server::_process(double delta) {
    if (!peer.is_valid()) return;
    
    // Poll the peer directly
    peer->poll();
    
    // Process all incoming raw packets
    while (peer->get_available_packet_count() > 0) {
        int sender_id = peer->get_packet_peer();
        PackedByteArray data = peer->get_packet();
        process_raw_packet(sender_id, data);
    }
    
    // Broadcast all player states 20 times per second
    broadcast_timer += delta;
    if (broadcast_timer >= 0.05f) {
        broadcast_timer = 0;
        broadcast_player_states();
    }
}

void Server::process_raw_packet(int sender_id, const PackedByteArray &data) {
    // Check packet size against our expected size
    if (data.size() != sizeof(PlayerNetworkData)) {
        UtilityFunctions::print("[ERROR] Wrong packet size from player ", sender_id, 
                               ". Expected: ", sizeof(PlayerNetworkData), 
                               " Got: ", data.size());
        return;
    }
    
    PlayerNetworkData packet = deserialize_player_data(data);
    
    // Security check: ensure packet claims to be from the actual sender
    // if (packet.player_id != sender_id) {
    //     UtilityFunctions::print("[SECURITY] Player ", sender_id, 
    //                            " tried to impersonate player ", packet.player_id);
    //     return;
    // }
    
    // Update or create player info
    if (!players.has(sender_id)) {
        players[sender_id] = PlayerInfo();
        players[sender_id].peer_id = sender_id;
    }
    
    PlayerNetworkData& info = players[sender_id].latest_packet;
    info.pos_x = packet.pos_x;
    info.pos_y = packet.pos_y;
    info.pos_z = packet.pos_z;
    info.input_flags = packet.input_flags;
    info.mouse_offset_x = packet.mouse_offset_x;
    info.mouse_offset_y = packet.mouse_offset_y;
    info.player_id = sender_id;
    players[sender_id].last_update_time = Time::get_singleton()->get_ticks_msec() / 1000.0f;
    
    // ========== PRINT ALL INCOMING DATA EVERY FRAME ==========
    UtilityFunctions::print("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
    UtilityFunctions::print("[INCOMING] From Player ID: ", sender_id);
    UtilityFunctions::print("  ├─ Packet Type: ", packet.packet_type);
    UtilityFunctions::print("  ├─ Position: (", packet.pos_x, ", ", packet.pos_y, ", ", packet.pos_z, ")");
    
    // Print input flags with readable names
    UtilityFunctions::print("  ├─ Inputs: ", packet.input_flags, " (", 
                           (packet.input_flags & INPUT_W ? "W " : ""),
                           (packet.input_flags & INPUT_A ? "A " : ""),
                           (packet.input_flags & INPUT_S ? "S " : ""),
                           (packet.input_flags & INPUT_D ? "D " : ""),
                           (packet.input_flags & INPUT_SPACE ? "SPACE " : ""),
                           (packet.input_flags & INPUT_1 ? "1 " : ""),
                           (packet.input_flags & INPUT_2 ? "2 " : ""),
                           (packet.input_flags & INPUT_3 ? "3 " : ""),
                           (packet.input_flags & INPUT_SHIFT ? "SHIFT " : ""),
                           (packet.input_flags & INPUT_MOUSE1 ? "MOUSE1 " : ""),
                           (packet.input_flags & INPUT_MOUSE2 ? "MOUSE2 " : ""),
                           ")");
    
    UtilityFunctions::print("  └─ Mouse Offset: (", packet.mouse_offset_x, ", ", packet.mouse_offset_y, ")");
    UtilityFunctions::print("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
    // ============================================================
}

void Server::broadcast_player_states() {
    if (players.size() == 0) return;
    
    // Print broadcast info
    UtilityFunctions::print("\n[ BROADCAST ] Sending states to ", players.size(), " clients");
    
    // For each connected client
    for (const KeyValue<int, PlayerInfo>& kv : players) {
        int target_client_id = kv.key;
        
        UtilityFunctions::print("  └─ Sending to Client ", target_client_id, ":");
        
        PackagedPlayerInfo info;

        for (const KeyValue<int, PlayerInfo>& other : players) {
            int other_player_id = other.key;
            //  Don't store your own data
            if (other_player_id == target_client_id) {
                continue;
            }
            info.player_packet.push_back(players[other_player_id].latest_packet);
        }

        PackedByteArray bytes = serialize_packaged_player_info(info);
    
        // Get the peer for the specific client and send directly
        Ref<WebSocketPeer> client_peer = peer->get_peer(target_client_id);
        if (client_peer.is_valid()) {
            client_peer->put_packet(bytes);
        }

        // // Send ALL other players' data to this client
        // for (const KeyValue<int, PlayerInfo>& other : players) {
        //     int other_player_id = other.key;
            
        //     // // Skip sending the client their own data
        //     // if (other_player_id == target_client_id) {
        //     //     UtilityFunctions::print("       ├─ (SKIP) Own data for player ", other_player_id);
        //     //     continue;
        //     // }
            
        //     UtilityFunctions::print("       ├─ Sending player ", other_player_id, 
        //                            " at (", other.value.latest_packet.pos_x, ", ", 
        //                            other.value.latest_packet.pos_y, ", ", other.value.latest_packet.pos_z, ")");
            
        //     send_player_state_to_client(target_client_id, other_player_id);
        // }
        UtilityFunctions::print("       └─ Done");
    }
}

void Server::send_packaged_player_info_to_client(int target_client_id) {
    

    

};
void Server::send_player_state_to_client(int target_client_id, int player_id) {
    if (!players.has(player_id)) return;
    
    PlayerInfo& info = players[player_id];
 
    PackedByteArray bytes = serialize_player_data(info.latest_packet);
    
    // Get the peer for the specific client and send directly
    Ref<WebSocketPeer> client_peer = peer->get_peer(target_client_id);
    if (client_peer.is_valid()) {
        client_peer->put_packet(bytes);
    }
}

void Server::_on_peer_connected(int id) {
    UtilityFunctions::print("\n═══════════════════════════════════════════");
    UtilityFunctions::print("[CONNECTION] Player ", id, " connected to server");
    UtilityFunctions::print("═══════════════════════════════════════════\n");
    
    // Initialize player info
    PlayerInfo new_player;
    new_player.peer_id = id;
    new_player.latest_packet.pos_x= 0.0f;
    new_player.latest_packet.pos_y = 0.0f;
    new_player.latest_packet.pos_z = 0.0f;
    new_player.latest_packet.input_flags = 0;
    new_player.latest_packet.mouse_offset_x = 0;
    new_player.latest_packet.mouse_offset_y = 0;

    players[id] = new_player;
    players[id].last_update_time = Time::get_singleton()->get_ticks_msec() / 1000.0f;
    
    // Print current player list
    UtilityFunctions::print("Active players (", players.size(), " total):");
    for (const KeyValue<int, PlayerInfo>& kv : players) {
        UtilityFunctions::print("  └─ Player ID: ", kv.key);
    }
}

void Server::_on_peer_disconnected(int id) {
    UtilityFunctions::print("\n═══════════════════════════════════════════");
    UtilityFunctions::print("[DISCONNECTION] Player ", id, " disconnected");
    UtilityFunctions::print("═══════════════════════════════════════════\n");
    
    if (players.has(id)) {
        players.erase(id);
    }
    
    // Print remaining players
    UtilityFunctions::print("Remaining players (", players.size(), " total):");
    for (const KeyValue<int, PlayerInfo>& kv : players) {
        UtilityFunctions::print("  └─ Player ID: ", kv.key);
    }
}
