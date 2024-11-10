//
// Created by ilya_ on 05.11.2024.
//

#include "transport_catalogue.hpp"
#include <stdexcept>
#include <unordered_set>
#include "geo.hpp"


Stop::Stop(std::string name, double latitude, double longitude): name(std::move(name)), latitude(latitude), longitude(longitude) {}

Bus::Bus(std::string name, std::vector<const Stop *> stops, bool is_round_trip): name(std::move(name)), stops(std::move(stops)), is_round_trip(is_round_trip) {}

size_t StopPairHasher::operator()(const std::pair<const Stop *, const Stop *> &stops) const noexcept {
    return std::hash<const void*>()(stops.first) ^ std::hash<const void*>()(stops.second) << 1;
}

TransportCatalogue::TransportCatalogue() = default;

TransportCatalogue::~TransportCatalogue() = default;

void TransportCatalogue::addStop(Stop stop) {
    stop.name.erase(0, stop.name.find_first_not_of(' '));
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

    for (const Stop* stop : added_bus.stops) {
        stop_to_buses_[stop].insert(&added_bus);
    }
}

const Stop* TransportCatalogue::findStop(std::string_view stop_name) const {
    const auto it = stop_name_to_stop_.find(stop_name);
    return it != stop_name_to_stop_.end() ? it->second : nullptr;
}

const Bus* TransportCatalogue::findBus(std::string_view bus_name) const {
    const auto it = bus_name_to_bus_.find(bus_name);
    return it != bus_name_to_bus_.end() ? it->second : nullptr;
}

Stop *TransportCatalogue::add_to_stops_deque(const Stop &stop) {
    stops_.push_back(stop);

    return &stops_.back();
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


std::optional<RouteInfo> TransportCatalogue::GetRouteInfo(std::string_view bus_name) const {
    const Bus* bus = findBus(bus_name);
    if (!bus) {
        return std::nullopt;
    }

    double geo_length = 0.0;
    double road_length = 0;

    const auto& stops = bus->stops;
    for (size_t i = 1; i < stops.size(); ++i) {
        geo_length += ComputeDistance({stops[i - 1]->latitude, stops[i - 1]->longitude},
                                      {stops[i]->latitude, stops[i]->longitude});

        road_length += getDistance(stops[i - 1], stops[i]);
    }

    if (!bus->is_round_trip) {
        for (size_t i = stops.size() - 1; i > 0; --i) {
            road_length += getDistance(stops[i], stops[i - 1]);
        }
    }

    const double curvature = road_length / geo_length;

    return RouteInfo{stops.size(), std::unordered_set(stops.begin(), stops.end()).size(), road_length, curvature};
}


std::optional<StopInfo> TransportCatalogue::getStopInfo(std::string_view stop_name) const {
    const Stop* stop = findStop(stop_name);
    if (!stop) {
        return std::nullopt;
    }
    auto it = stop_to_buses_.find(stop);
    if (it != stop_to_buses_.end()) {
        return StopInfo{stop->name, it->second};
    }
    return StopInfo{stop->name, {}};
}

void TransportCatalogue::setDistance(const Stop *from, const Stop *to, double distance) {
    distances_[{from, to}] = distance;
}

double TransportCatalogue::getDistance(const Stop *from, const Stop *to) const {
    if (const auto it = distances_.find({from, to}); it != distances_.end()) {
        return it->second;
    }

    if (const auto it = distances_.find({to, from}); it != distances_.end()) {
        return it->second;
    }

    return 0;
}
