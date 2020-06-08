#include "init_service.hpp"

#include <goblin-engineer/components/http.hpp>
#include <goblin-engineer/components/root_manager.hpp>
#include <iostream>

#include <services/storage/storage.hpp>
#include <services/zmq-hub/zmq_hub.hpp>

using goblin_engineer::components::make_manager_service;
using goblin_engineer::components::make_service;
using goblin_engineer::components::root_manager;
using namespace goblin_engineer::components;

using actor_zeta::link;

class controller_t final : public goblin_engineer::abstract_service {
public:
    template<class Manager>
    controller_t(actor_zeta::intrusive_ptr<Manager> ptr)
        : abstract_service(ptr, "controller") {
        add_handler("dispatcher", &controller_t::dispatcher);
    }

    void dispatcher(services::zmq_buffer_t&buffer) {
        auto msg = std::move( static_cast<actor_zeta::context&>(*this).current_message());
    }
};

void init_service(goblin_engineer::components::root_manager& env, rocketjoe::configuration& cfg, components::log_t& log) {
    auto zmq_hub = make_manager_service<services::zmq_hub_t>(env, log, std::move(std::make_unique<zmq::context_t>()));
    auto controller = make_service<controller_t>(zmq_hub);
    services::make_listener_zmq_socket(zmq_hub, services::make_url("tcp", "*", 9999), zmq::socket_type::router, controller);
    auto storage = make_service<services::storage_hub>(zmq_hub);
    link(controller, storage);
/*
    http::router router_;

    router_.http_get(
        R"(/my/super/url)", [](http::query_context& ctx) {
            std::cerr << ctx.response().body().c_str() << std::endl;
            ctx.response().body() = ctx.request().body();
            ctx.write();
        });

    auto http = make_manager_service<http::server>(env, cfg.port_http_);
    make_service<http::http_dispatcher>(http, router_);
    */
}
