#include "DCommandEvent.h"
#include "dispatcher.h"
#include <memory>

DCommandEvent::DCommandEvent(const dpp::slashcommand_t &event) : _event(event) {}

const dpp::slashcommand_t DCommandEvent::getEvent() {
    return _event;
}

template<> 
const std::string DCommandEvent::getArgument(std::string name) {
    std::string val = std::get<std::string>(_event.get_parameter(name));
    
    return val;
}

template<> 
const int64_t DCommandEvent::getArgument(std::string name) {
    int64_t val = std::get<int64_t>(_event.get_parameter(name));
    
    return val;
}

template<> 
const double DCommandEvent::getArgument(std::string name) {
    double val = std::get<double>(_event.get_parameter(name));
    
    return val;
}