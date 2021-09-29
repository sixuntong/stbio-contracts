#pragma once

#include <eosio/eosio.hpp>
#include <eosio/time.hpp>

namespace tracecontract
{
    using eosio::microseconds;
    using eosio::time_point;
    using eosio::name;
    using eosio::const_mem_fun;
    using eosio::permission_level;
    using eosio::action;
    using eosio::require_auth;
    using eosio::check;
    using eosio::datastream;
    using eosio::indexed_by;
    using eosio::unsigned_int;
    using eosio::checksum256;


    // 商品基础信息表结构
    struct [[eosio::table, eosio::contract("eosio.trace")]] base_data {
      uint64_t id;                              // id （自增）
      std::string udid;                         // 识别码
      eosio::checksum256 udid_hash;             // 识别码hash
      std::string brand;                        // 品牌
      std::string product_name;                 // 商品名称
      std::string factory_name;                 // 厂名
      std::string factory_address;              // 厂址
      std::string factory_number;               // 厂家联系方式
      std::string production_license_number;    // 生产许可证编号
      std::string json_data;                    // 商品属性(json格式)

      uint64_t primary_key()const { return id; }
      eosio::checksum256 second_key() const { return udid_hash; }

    };
    // 商品基础信息表  
    typedef eosio::multi_index< "basedata"_n, base_data, eosio::indexed_by<"bysubkey"_n, 
      eosio::const_mem_fun<base_data, eosio::checksum256, &base_data::second_key>>> base_data_table;


    // 单次识别信息表结构
    struct per_identify_data {
      int32_t identify_count;           // 识别次数
      std::string identify_ip;          // 识别ip
      std::string identify_location;    // 识别位置
      std::string identify_time;        // 识别时间
      uint8_t is_transregional;            // 是否跨地区
      std::string buy_area;             // 购买区域
      std::string longitude;            // 识别位置经度
      std::string latitude;             // 识别位置纬度
    };

    // 商品识别信息溯源表结构
    struct [[eosio::table, eosio::contract("eosio.trace")]] identify_data {
      uint64_t id;                              // 自增
      std::string udid;                         // 识别码
      eosio::checksum256 udid_hash;             // 识别码hash
      uint8_t identify_total_count;             // 识别总次数 
      std::vector<per_identify_data> per_identify_datas;       //单次识别信息

      uint64_t primary_key()const { return id; }
      eosio::checksum256 second_key() const { return udid_hash; }

    };
    // 商品识别信息溯源表
    typedef eosio::multi_index< "identifydata"_n, identify_data, eosio::indexed_by<"bysubkey"_n, 
      eosio::const_mem_fun<identify_data, eosio::checksum256, &identify_data::second_key>>> identify_data_table; 

   class [[eosio::contract("eosio.trace")]] trace_contract : public eosio::contract
   {
      public:
        using eosio::contract::contract;

        ACTION addbasedata(std::string& udid, std::string& brand, std::string& product_name, std::string& factory_name, std::string& factory_address, std::string& factory_number, std::string& production_license_number, std::string& json_data); 
        ACTION addidfydata(std::string& udid, std::string& identify_ip, std::string& identify_location, std::string& identify_time, uint8_t& is_transregional, std::string& buy_area, std::string& longitude, std::string& latitude);
        ACTION rmdata(std::string& udid);
             
   };
}

