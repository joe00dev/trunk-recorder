#ifndef OPENMHZ_UPLOADER_H
#define OPENMHZ_UPLOADER_H



class Uploader;

#include "../../uploaders/uploader.h"

class OpenmhzUploader : public Uploader {
public:
  int upload(struct call_data_t *call);
};


#include <time.h>
#include <vector>
#include <websocketpp/client.hpp>
#include <websocketpp/config/asio_no_tls_client.hpp>

#include <gr_blocks/decoder_wrapper.h>
#include "../../call_concluder/call_concluder.h
#include "../plugin-common.h"


// This header pulls in the WebSocket++ abstracted thread support that will
// select between boost::thread and std::thread based on how the build system
// is configured.
#include <websocketpp/common/thread.hpp>

typedef websocketpp::client<websocketpp::config::asio_client> client;
//typedef websocketpp::retry_client_endpoint<websocketpp::retry_config<websocketpp::config::asio_client>> client;
typedef websocketpp::lib::lock_guard<websocketpp::lib::mutex> scoped_lock;

class stat_socket {
public:
  stat_socket();
  void send_stat(std::string val);
  void poll_one();
  void telemetry_loop();
  void on_fail(websocketpp::connection_hdl);
  void on_close(websocketpp::connection_hdl);
  void on_open(websocketpp::connection_hdl);
  void on_message(websocketpp::connection_hdl, client::message_ptr msg);
  void open_stat();
  bool is_open();
  bool config_sent();
  void send_calls_active(std::vector<Call *> calls);
  void send_config(std::vector<Source *> sources, std::vector<System *> systems);
  void send_sys_rates(std::vector<System *> systems, float timeDiff);
  void send_call_start(Call *call);
  void send_call_end(Call_Data call_info);
  void send_recorder(Recorder *recorder);
  void initialize(Config *config, void (*callback)(void*), void* context);
  void send_recorders(std::vector<Recorder *> recorders);
  void send_systems(std::vector<System *> systems);
  void send_system(System *systems);
  void send_signal(long unitId, const char *signaling_type, gr::blocks::SignalType sig_type, Call *call, System *system, Recorder *recorder);

private:
  void reopen_stat();
  client m_client;
  websocketpp::connection_hdl m_hdl;
  websocketpp::lib::mutex m_lock;
  int retry_attempt;
  time_t reconnect_time;
  bool m_reconnect;
  bool m_open;
  bool m_done;
  bool m_config_sent;
  Config *m_config;
  void* m_context;
  void (*m_callback)(void*);
  void send_object(boost::property_tree::ptree data, std::string name, std::string type);
};

typedef struct stat_plugin_t stat_plugin_t;

struct stat_plugin_t {
  std::vector<Source *> sources;
  std::vector<System *> systems;
  std::vector<Call *> calls;
  Config* config;
};

#endif
