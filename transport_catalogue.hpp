//
// Created by ilya_ on 05.11.2024.
//

#ifndef TRANSPORT_CATALOGUE_HPP
#define TRANSPORT_CATALOGUE_HPP

#include <deque>
#include <string>
#include <functional>
#include <optional>

struct Stop {
    std::string name;
    double latitude{};
    double longitude{};

    Stop(std::string name, double latitude, double longitude);
};

struct Bus {
    std::string name;
    std::vector<const Stop*> stops;
    bool is_round_trip;

    Bus(std::string name, std::vector<const Stop*> stops, bool is_round_trip);
};

struct RouteInfo {
    size_t stop_count;
    size_t unique_stop_count;
    double route_length;
};

class TransportCatalogue {
private:
    std::deque<Stop> stops_;
    std::deque<Bus> buses_;
    std::unordered_map<std::string_view, Stop*> stop_name_to_stop_;
    std::unordered_map<std::string_view, Bus*> bus_name_to_bus_;



public:
    TransportCatalogue();

    ~TransportCatalogue();

    void addStop(Stop stop);

    void addBus(Bus bus);

    const Stop *findStop(std::string_view stop_name) const;

    const Bus *findBus(std::string_view bus_name) const;

    Stop *add_to_stops_deque(const Stop &stop);
    Bus* add_to_buses_deque(const Bus& bus);
    void add_to_stop_name_to_stop(std::string_view stop_name, Stop *stop);
    void add_to_bus_name_to_bus(std::string_view bus_name, Bus *bus);

    std::optional<RouteInfo> getRouteInfo(std::string_view bus_name) const;
};

#endif //TRANSPORT_CATALOGUE_HPP