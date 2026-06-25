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
    
    maps[0] = MapData("TDM_MAP","res://Maps/TDM_map.tscn");
    maps[1] = MapData("INDUSTRY","res://Maps/Industrial_map.tscn");
    maps[2] = MapData("BIG_TOWN","res://Maps/LittleTown.tscn");

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

    // Code for setting the current state
    if (Time::get_singleton()->get_ticks_msec() - map_start_time > map_time && current_game_state == GameState::inGame) { 
        current_game_state = GameState::Voting;
        voting_start_time = Time::get_singleton()->get_ticks_msec();
        UtilityFunctions::print("State changed to Voting");
    } else if (Time::get_singleton()->get_ticks_msec() - voting_start_time > voting_time && current_game_state == GameState::Voting) { 
        current_game_state = GameState::inGame;
        map_start_time = Time::get_singleton()->get_ticks_msec();
        UtilityFunctions::print("State changed to Game: Starting map with most votes: ", current_map);
    }

    UtilityFunctions::print("Time left ", Time::get_singleton()->get_ticks_msec() - map_start_time);


    if (!peer.is_valid()) return;
    
    // Poll the peer directly
    peer->poll();
    
    if (current_game_state == GameState::inGame) { 
        /// InGame processing
        // Process all incoming raw packets
        while (peer->get_available_packet_count() > 0) {
            int sender_id = peer->get_packet_peer();
            PackedByteArray data = peer->get_packet();

            uint8_t packet_type = data[0];   // first byte
            // Remove the type byte for further processing
            PackedByteArray payload;
            payload.resize(data.size() - 1);
            memcpy(payload.ptrw(), data.ptr() + 1, data.size() - 1);
            UtilityFunctions::print("Packet type iss ", packet_type);

            process_game_packet(sender_id, payload);
        }

        // Broadcast all player states 20 times per second
        broadcast_timer += delta;
        if (broadcast_timer >= 0.05f) {
            broadcast_timer = 0;
            send_game_packets();  
        }

         

        // Game Logic
        for (const KeyValue<int, PlayerInfo>& kv : players) {
            if (kv.value.dead) { 
                if(Time::get_singleton()->get_ticks_msec() - kv.value.death_time >= respawn_time) {
                players[kv.key].dead = false;
                players[kv.key].health = 100;
                UtilityFunctions::print("Player : ", kv.key, " respawned.");
                }
            } 
        }
            

    } else if (current_game_state == GameState::Voting) {
        // Voting processing
        UtilityFunctions::print("Voting Phase Server");
        // Process all incoming raw packets
        while (peer->get_available_packet_count() > 0) {
            int sender_id = peer->get_packet_peer();
            PackedByteArray data = peer->get_packet();

            uint8_t packet_type = data[0];   // first byte
            // Remove the type byte for further processing
            PackedByteArray payload;
            payload.resize(data.size() - 1);
            memcpy(payload.ptrw(), data.ptr() + 1, data.size() - 1);
            UtilityFunctions::print("Packet type iss ", packet_type);
            
            // CHECK: only process voting packet and screw the late arriving game packets if so happens to be the case
            if(packet_type == PacketTypeID::PACKET_VOTE) {
                process_voting_packet(sender_id, payload);
            }  
        }
        // Tally the votes
        for (const KeyValue<int, PlayerInfo>& kv : players) {
            if(players[kv.key].vote_data.map_number != -1) {
                maps[players[kv.key].vote_data.map_number].votes = maps[players[kv.key].vote_data.map_number].votes + 1;  
            }
        }
        // Decide the top map based on the curret no of votes
        int top_map_votes = 0;
        for(const KeyValue<int, MapData>& kv : maps) {
            UtilityFunctions::print("MAP NO:", kv.key, " Map Name: ", kv.value.name, " Map votes: ", kv.value.votes);
            if(maps[kv.key].votes > top_map_votes) {
                // Set current map to current top map 
                current_map = kv.key;
                top_map_votes = maps[kv.key].votes;
            }
            // Reset votes for next round of deciding 
            maps[kv.key].votes = 0;
        }
        


        broadcast_timer += delta;
        if (broadcast_timer >= 0.05f) {
            broadcast_timer = 0;
            send_voting_packets();
        }   
    }  
 
} 



void Server::process_game_packet(int sender_id, const PackedByteArray &data) {
    // Check packet size against our expected size
    // if (data.size() != sizeof(SelfNetworkData)) {
    //     UtilityFunctions::print("[ERROR] Wrong packet size from player ", sender_id, 
    //                            ". Expected: ", sizeof(SelfNetworkData), 
    //                            " Got: ", data.size());
    //     return;
    // }

    if (data.size() == 4) return;
    
    SelfNetworkData packet = deserialize_self_network_data(data);

    // Store player's render informatoin sent by player
    // FLAGGED FOR SIMPLICITY
    PlayerNetworkData& info = players[sender_id].latest_packet;

    // memcpy(info.player_name, packet.playerNetworkdata.player_name, sizeof(info.player_name));

    info.pos_x = packet.playerNetworkdata.pos_x;
    info.pos_y = packet.playerNetworkdata.pos_y;
    info.pos_z = packet.playerNetworkdata.pos_z;
    info.input_flags = packet.playerNetworkdata.input_flags;
    info.mouse_offset_x = packet.playerNetworkdata.mouse_offset_x;
    info.mouse_offset_y = packet.playerNetworkdata.mouse_offset_y;
    info.player_id = sender_id;
    info.camera_rotation = packet.playerNetworkdata.camera_rotation;
    info.player_y_rotation = packet.playerNetworkdata.player_y_rotation;

    // Now store player's information recorded by us that will be useful to send to other players
    info.health = players[sender_id].health;
    info.kills = players[sender_id].kills;
    info.deaths = players[sender_id].deaths;
     
    for (const KeyValue<int, int>& kv : packet.tick_combat_report) {
        // Apply damage to other players (trusting the client)
        players[kv.key].health -= kv.value;  
        // On death
        if (players[kv.key].health <= 0 && !players[kv.key].dead) { 
            players[kv.key].health = 0;
            players[kv.key].dead = true;
            players[kv.key].death_time = Time::get_singleton()->get_ticks_msec();
            UtilityFunctions::print("Player : ", kv.key, " died.");
            players[kv.key].deaths += 1;
            
            // Register kill for the player
            players[sender_id].kills += 1;
        }
    }
    players[sender_id].last_update_time = Time::get_singleton()->get_ticks_msec() / 1000.0f;
    
}

void Server::process_voting_packet(int sender_id, const PackedByteArray & data) {
     if (data.size() == 4) return;

    VoteData voteData = deserialize_vote_data(data);
    UtilityFunctions::print("Player : ", sender_id, " voted for map; ", voteData.map_number);
    players[sender_id].vote_data.map_number = voteData.map_number;

}

void Server::send_game_packets() {
    UtilityFunctions::print("sending game packet");
    if (players.size() == 0) return;
    
    // Print broadcast info
    UtilityFunctions::print("\n[ BROADCAST ] Sending states to ", players.size(), " clients");
    
    // For each connected client
    for (const KeyValue<int, PlayerInfo>& kv : players) {
        int target_client_id = kv.key;
        
        UtilityFunctions::print("  └─ Sending to Client ", target_client_id, ":");
        
        PackagedPlayerInfo packagedInfo;

        // Initialize the vector store other player's information
        for (const KeyValue<int, PlayerInfo>& other : players) {
            int other_player_id = other.key;
            //  Don't store your own data
            if (other_player_id == target_client_id) {
                continue;
            }
            packagedInfo.player_packet.push_back(players[other_player_id].latest_packet);
        }

        // Iniitalize self data 
        packagedInfo.health = players[target_client_id].health;
        UtilityFunctions::print("Player ID: ", target_client_id, " has kills : ",  players[target_client_id].kills);
        packagedInfo.kills = players[target_client_id].kills;
        packagedInfo.deaths = players[target_client_id].deaths;

        UtilityFunctions::print("Current map in game packet is : ", current_map);
        // Initialize map data
        packagedInfo.current_map = current_map;

        PackedByteArray bytes = serialize_packaged_player_info(packagedInfo);
    
        // Get the peer for the specific client and send directly
        Ref<WebSocketPeer> client_peer = peer->get_peer(target_client_id);
        if (client_peer.is_valid()) {
            // Prepend the data with info about what type of packet this is
            PackedByteArray payload = serialize_packaged_player_info(packagedInfo);
            PackedByteArray packet;
            packet.resize(payload.size() + 1);
            uint8_t* w = packet.ptrw();
            w[0] = PACKET_GAME_UPDATE;              
            memcpy(w + 1, payload.ptr(), payload.size());
            // Send the data
            client_peer->put_packet(packet);
        }
    }
}



void Server::send_voting_packets() {
    UtilityFunctions::print("sending voting packet");
 // For each connected client
    for (const KeyValue<int, PlayerInfo>& kv : players) {
        int target_client_id = kv.key;
        
        UtilityFunctions::print("  └─ Sending to Client ", target_client_id, ":");
        
        PlayerVotingData playerVotingData;

        // Initialize the vector store other player's information
        for (const KeyValue<int, PlayerInfo>& other : players) {
            int other_player_id = other.key;
            //  Don't store your own data
            if (other_player_id == target_client_id) {
                continue;
            }
            // Store voteData
            VoteData vote_data;

            vote_data.map_number = players[kv.key].vote_data.map_number;
            vote_data.player_id = players[kv.key].vote_data.player_id;

            vote_data.map_number = 1;
            vote_data.player_id = 23;
            playerVotingData.player_vote_packet.push_back(vote_data);



            // playerVotingData.player_vote_packet.push_back(players[other_player_id].vote_data);
            
        }

        // PackedByteArray bytes = serialize_player_voting_data(playerVotingData);
    
        // Get the peer for the specific client and send directly
        Ref<WebSocketPeer> client_peer = peer->get_peer(target_client_id);
        if (client_peer.is_valid()) {
            // Prepend the data with info about what type of packet this is
            PackedByteArray payload = serialize_player_voting_data(playerVotingData);
            PackedByteArray packet;
            packet.resize(payload.size() + 1);
            uint8_t* w = packet.ptrw();
            w[0] = PACKET_VOTE;              
            memcpy(w + 1, payload.ptr(), payload.size());
            // Send the data
            client_peer->put_packet(packet);
        }
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
    PlayerInfo new_player_info;
    new_player_info.peer_id = id;
    new_player_info.latest_packet.pos_x= 0.0f;
    new_player_info.latest_packet.pos_y = 0.0f;
    new_player_info.latest_packet.pos_z = 0.0f;
    new_player_info.latest_packet.input_flags = 0;
    new_player_info.latest_packet.mouse_offset_x = 0;
    new_player_info.latest_packet.mouse_offset_y = 0;
    new_player_info.health = 100;

    players[id] = new_player_info;
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
