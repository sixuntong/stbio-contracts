#include "eosio.trace/eosio.trace.hpp"
#include <eosio/crypto.hpp>

using namespace tracecontract;
using namespace eosio;


static constexpr eosio::name bts_auth_name{"bts.auth"_n};                      

void trace_contract::addbasedata(std::string& udid, std::string& brand, std::string& product_name, std::string& factory_name, std::string& factory_address, std::string& factory_number, std::string& production_license_number, std::string& json_data) {
    require_auth( get_self() );
    
    eosio::checksum256 udid_hash = eosio::sha256(const_cast<char*>(udid.data()), udid.size());
    base_data_table bdt(get_self(), get_self().value);
    auto sub_bdt_index = bdt.get_index<"bysubkey"_n>();
    auto itr = sub_bdt_index.find(udid_hash);

    if(itr == sub_bdt_index.end()){
        bdt.emplace( _self, [&]( auto& b ) {
                b.id = bdt.available_primary_key();
                b.udid = udid;
                b.udid_hash = udid_hash;
                b.brand = brand;
                b.product_name = product_name;
                b.factory_name = factory_name;
                b.factory_address = factory_address;
                b.factory_number = factory_number;
                b.production_license_number = production_license_number;
                b.json_data = json_data;
            });  
    }
    else{
        sub_bdt_index.modify( itr, _self, [&]( auto& b ) {
                b.udid = udid;
                b.udid_hash = udid_hash;
                b.brand = brand;
                b.product_name = product_name;
                b.factory_name = factory_name;
                b.factory_address = factory_address;
                b.factory_number = factory_number;
                b.production_license_number = production_license_number;
                b.json_data = json_data;
        });
    }
}

void trace_contract::addidfydata(std::string& udid, std::string& identify_ip, std::string& identify_location, std::string& identify_time, uint8_t& is_transregional, std::string& buy_area, std::string& longitude, std::string& latitude) {
    require_auth( get_self() );
                        
    eosio::checksum256 udid_hash = eosio::sha256(const_cast<char*>(udid.data()), udid.size());
    base_data_table bdt(get_self(), get_self().value);
    auto sub_bdt_index = bdt.get_index<"bysubkey"_n>();
    auto itr_bdt = sub_bdt_index.find(udid_hash);
    check( itr_bdt != sub_bdt_index.end(), "The udid is not exist in the base-data table!");

    identify_data_table idt(get_self(), get_self().value);
    auto sub_idt_index = idt.get_index<"bysubkey"_n>();
    auto itr_idt = sub_idt_index.find(udid_hash);

    per_identify_data pid; 
    
    pid.identify_ip = identify_ip;
    pid.identify_location = identify_location;
    pid.identify_time = identify_time;
    pid.is_transregional = is_transregional;
    pid.buy_area = buy_area;
    pid.longitude = longitude;
    pid.latitude = latitude;

    if(itr_idt == sub_idt_index.end()) {
        pid.identify_count = 1;
        idt.emplace( _self, [&]( auto& i ) {
            i.id = idt.available_primary_key();
            i.udid = udid;
            i.udid_hash = udid_hash;
            i.identify_total_count = 1;
            i.per_identify_datas.push_back(pid);
        });
    }
    else{
        sub_idt_index.modify( itr_idt, _self, [&]( auto& i ) {
            pid.identify_count = i.per_identify_datas.size() + 1;
            i.identify_total_count = pid.identify_count;
            i.per_identify_datas.push_back(pid);
        });        

    }

}

void trace_contract::rmdata(std::string& udid) {
    require_auth(bts_auth_name);
    
    eosio::checksum256 udid_hash = eosio::sha256(const_cast<char*>(udid.data()), udid.size());
    base_data_table bdt(get_self(), get_self().value);
    auto sub_bdt_index = bdt.get_index<"bysubkey"_n>();
    auto itr_bdt_index = sub_bdt_index.find(udid_hash);
    check( itr_bdt_index != sub_bdt_index.end(), "The udid is not exist in the base-data table!");
    auto id_bdt = itr_bdt_index->id;
    auto itr_bdt = bdt.find(id_bdt);
    bdt.erase(itr_bdt) ;

    identify_data_table idt(get_self(), get_self().value);
    auto sub_idt_index = idt.get_index<"bysubkey"_n>();
    auto itr_idt_index = sub_idt_index.find(udid_hash);
    check( itr_idt_index != sub_idt_index.end(), "The udid is not exist in the identified-data table!");
    auto id_idt = itr_idt_index->id;
    auto itr_idt = idt.find(id_idt);
    idt.erase(itr_idt) ;
}
