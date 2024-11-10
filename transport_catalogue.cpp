//
// Created by ilya_ on 05.11.2024.
//

#include "transport_catalogue.hpp"
#include <stdexcept>
#include <unordered_set>
#include "geo.hpp"


Stop::Stop(std::string name, double latitude, double longitude): name(std::move(name)), latitude(latitude), longitude(longitude) {}

Bus::Bus(std::string name, std::vector<const Stop *> stops, bool is_round_trip): name(std::move(name)), stops(std::move(stops)), is_round_trip(is_round_trip) {}

TransportCatalogue::TransportCatalogue() = default;

TransportCatalogue::~TransportCatalogue() = default;

void TransportCatalogue::addStop(Stop stop) {
    auto it = stop_name_to_stop_.find(stop.name);
    if (it != stop_name_to_stop_.end()) {
        it->second->latitude = stop.latitude;
        it->second->longitude = stop.longitude;
        return;
    }
    stops_.push_back(std::move(stop));
    Stop& added_stop = stops_.back();
    stop_name_to_stop_.emplace(added_stop.name, &added_stop);
}


void TransportCatalogue::addBus(Bus bus) {
    buses_.push_back(std::move(bus));
    Bus& added_bus = buses_.back();
    bus_name_to_bus_.emplace(added_bus.name, &added_bus);
}

const Stop* TransportCatalogue::findStop(std::string_view stop_name) const {
    auto it = stop_name_to_stop_.find(stop_name);
    return it != stop_name_to_stop_.end() ? it->second : nullptr;
}

const Bus* TransportCatalogue::findBus(std::string_view bus_name) const {
    auto it = bus_name_to_bus_.find(bus_name);
    return it != bus_name_to_bus_.end() ? it->second : nullptr;
}

Stop *TransportCatalogue::add_to_stops_deque(const Stop &stop) {
    stops_.push_back(stop);

    return &(stops_.back());
}

Bus *TransportCatalogue::add_to_buses_deque(const Bus &bus) {
    buses_.push_back(bus);

    return &buses_.back();
}

void TransportCatalogue::add_to_stop_name_to_stop(std::string_view stop_name,  Stop* stop) {
    stop_name_to_stop_[stop_name] = stop;
}

void TransportCatalogue::add_to_bus_name_to_bus(std::string_view bus_name, Bus* bus) {
    bus_name_to_bus_[bus_name] = bus;
}


std::optional<RouteInfo> TransportCatalogue::getRouteInfo(std::string_view bus_name) const {
    const Bus* bus = findBus(bus_name);
    if (!bus) {
        return std::nullopt;
    }

    size_t stop_count = bus->stops.size();
    std::unordered_set<const Stop*> unique_stops(bus->stops.begin(), bus->stops.end());
    size_t unique_stop_count = unique_stops.size();

    double route_length = 0.0;
    for (size_t i = 1; i < bus->stops.size(); ++i) {
        route_length += ComputeDistance(
            {bus->stops[i - 1]->latitude, bus->stops[i - 1]->longitude},
            {bus->stops[i]->latitude, bus->stops[i]->longitude}
        );
    }

    if (!bus->is_round_trip) {
        for (size_t i = bus->stops.size() - 1; i > 0; --i) {
            route_length += ComputeDistance(
                {bus->stops[i]->latitude, bus->stops[i]->longitude},
                {bus->stops[i - 1]->latitude, bus->stops[i - 1]->longitude}
            );
        }
        stop_count = stop_count * 2 - 1;
    }

    return RouteInfo{stop_count, unique_stop_count, route_length};
}
