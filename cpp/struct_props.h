#ifndef STRUCTPROPS_H
#define STRUCTPROPS_H

/*******************************************************************************************

    AUTO-GENERATED CODE. DO NOT MODIFY

*******************************************************************************************/

#include <ossie/CorbaUtils.h>
#include <CF/cf.h>
#include <ossie/PropertyMap.h>

#include <frontend/fe_tuner_struct_props.h>

struct target_device_struct {
    target_device_struct ()
    {
        name = "";
        serial = "";
        index = -1;
        vendor = "";
        product = "";
    }

    static std::string getId() {
        return std::string("target_device");
    }

    static const char* getFormat() {
        return "sshss";
    }

    std::string name;
    std::string serial;
    short index;
    std::string vendor;
    std::string product;
};

inline bool operator>>= (const CORBA::Any& a, target_device_struct& s) {
    CF::Properties* temp;
    if (!(a >>= temp)) return false;
    const redhawk::PropertyMap& props = redhawk::PropertyMap::cast(*temp);
    if (props.contains("target::name")) {
        if (!(props["target::name"] >>= s.name)) return false;
    }
    if (props.contains("target::serial")) {
        if (!(props["target::serial"] >>= s.serial)) return false;
    }
    if (props.contains("target::index")) {
        if (!(props["target::index"] >>= s.index)) return false;
    }
    if (props.contains("target::vendor")) {
        if (!(props["target::vendor"] >>= s.vendor)) return false;
    }
    if (props.contains("target::product")) {
        if (!(props["target::product"] >>= s.product)) return false;
    }
    return true;
}

inline void operator<<= (CORBA::Any& a, const target_device_struct& s) {
    redhawk::PropertyMap props;
 
    props["target::name"] = s.name;
 
    props["target::serial"] = s.serial;
 
    props["target::index"] = s.index;
 
    props["target::vendor"] = s.vendor;
 
    props["target::product"] = s.product;
    a <<= props;
}

inline bool operator== (const target_device_struct& s1, const target_device_struct& s2) {
    if (s1.name!=s2.name)
        return false;
    if (s1.serial!=s2.serial)
        return false;
    if (s1.index!=s2.index)
        return false;
    if (s1.vendor!=s2.vendor)
        return false;
    if (s1.product!=s2.product)
        return false;
    return true;
}

inline bool operator!= (const target_device_struct& s1, const target_device_struct& s2) {
    return !(s1==s2);
}

struct current_device_struct {
    current_device_struct ()
    {
    }

    static std::string getId() {
        return std::string("current_device");
    }

    static const char* getFormat() {
        return "ssssH";
    }

    std::string name;
    std::string vendor;
    std::string product;
    std::string serial;
    unsigned short index;
};

inline bool operator>>= (const CORBA::Any& a, current_device_struct& s) {
    CF::Properties* temp;
    if (!(a >>= temp)) return false;
    const redhawk::PropertyMap& props = redhawk::PropertyMap::cast(*temp);
    if (props.contains("current::rtl::name")) {
        if (!(props["current::rtl::name"] >>= s.name)) return false;
    }
    if (props.contains("current::rtl::vendor")) {
        if (!(props["current::rtl::vendor"] >>= s.vendor)) return false;
    }
    if (props.contains("current::rtl::product")) {
        if (!(props["current::rtl::product"] >>= s.product)) return false;
    }
    if (props.contains("current::rtl::serial")) {
        if (!(props["current::rtl::serial"] >>= s.serial)) return false;
    }
    if (props.contains("current::rtl::index")) {
        if (!(props["current::rtl::index"] >>= s.index)) return false;
    }
    return true;
}

inline void operator<<= (CORBA::Any& a, const current_device_struct& s) {
    redhawk::PropertyMap props;
 
    props["current::rtl::name"] = s.name;
 
    props["current::rtl::vendor"] = s.vendor;
 
    props["current::rtl::product"] = s.product;
 
    props["current::rtl::serial"] = s.serial;
 
    props["current::rtl::index"] = s.index;
    a <<= props;
}

inline bool operator== (const current_device_struct& s1, const current_device_struct& s2) {
    if (s1.name!=s2.name)
        return false;
    if (s1.vendor!=s2.vendor)
        return false;
    if (s1.product!=s2.product)
        return false;
    if (s1.serial!=s2.serial)
        return false;
    if (s1.index!=s2.index)
        return false;
    return true;
}

inline bool operator!= (const current_device_struct& s1, const current_device_struct& s2) {
    return !(s1==s2);
}

struct frontend_tuner_status_struct_struct : public frontend::default_frontend_tuner_status_struct_struct {
    frontend_tuner_status_struct_struct () : frontend::default_frontend_tuner_status_struct_struct()
    {
    }

    static std::string getId() {
        return std::string("FRONTEND::tuner_status_struct");
    }

    static const char* getFormat() {
        return "bssssddbbdssdbsbhs";
    }

    bool agc;
    std::string available_frequency;
    std::string available_gain;
    std::string available_sample_rate;
    bool complex;
    double gain;
    bool scan_mode_enabled;
    std::string stream_id;
    bool supports_scan;
    short tuner_number;
};

inline bool operator>>= (const CORBA::Any& a, frontend_tuner_status_struct_struct& s) {
    CF::Properties* temp;
    if (!(a >>= temp)) return false;
    const redhawk::PropertyMap& props = redhawk::PropertyMap::cast(*temp);
    if (props.contains("FRONTEND::tuner_status::agc")) {
        if (!(props["FRONTEND::tuner_status::agc"] >>= s.agc)) return false;
    }
    if (props.contains("FRONTEND::tuner_status::allocation_id_csv")) {
        if (!(props["FRONTEND::tuner_status::allocation_id_csv"] >>= s.allocation_id_csv)) return false;
    }
    if (props.contains("FRONTEND::tuner_status::available_frequency")) {
        if (!(props["FRONTEND::tuner_status::available_frequency"] >>= s.available_frequency)) return false;
    }
    if (props.contains("FRONTEND::tuner_status::available_gain")) {
        if (!(props["FRONTEND::tuner_status::available_gain"] >>= s.available_gain)) return false;
    }
    if (props.contains("FRONTEND::tuner_status::available_sample_rate")) {
        if (!(props["FRONTEND::tuner_status::available_sample_rate"] >>= s.available_sample_rate)) return false;
    }
    if (props.contains("FRONTEND::tuner_status::bandwidth")) {
        if (!(props["FRONTEND::tuner_status::bandwidth"] >>= s.bandwidth)) return false;
    }
    if (props.contains("FRONTEND::tuner_status::center_frequency")) {
        if (!(props["FRONTEND::tuner_status::center_frequency"] >>= s.center_frequency)) return false;
    }
    if (props.contains("FRONTEND::tuner_status::complex")) {
        if (!(props["FRONTEND::tuner_status::complex"] >>= s.complex)) return false;
    }
    if (props.contains("FRONTEND::tuner_status::enabled")) {
        if (!(props["FRONTEND::tuner_status::enabled"] >>= s.enabled)) return false;
    }
    if (props.contains("FRONTEND::tuner_status::gain")) {
        if (!(props["FRONTEND::tuner_status::gain"] >>= s.gain)) return false;
    }
    if (props.contains("FRONTEND::tuner_status::group_id")) {
        if (!(props["FRONTEND::tuner_status::group_id"] >>= s.group_id)) return false;
    }
    if (props.contains("FRONTEND::tuner_status::rf_flow_id")) {
        if (!(props["FRONTEND::tuner_status::rf_flow_id"] >>= s.rf_flow_id)) return false;
    }
    if (props.contains("FRONTEND::tuner_status::sample_rate")) {
        if (!(props["FRONTEND::tuner_status::sample_rate"] >>= s.sample_rate)) return false;
    }
    if (props.contains("FRONTEND::tuner_status::scan_mode_enabled")) {
        if (!(props["FRONTEND::tuner_status::scan_mode_enabled"] >>= s.scan_mode_enabled)) return false;
    }
    if (props.contains("FRONTEND::tuner_status::stream_id")) {
        if (!(props["FRONTEND::tuner_status::stream_id"] >>= s.stream_id)) return false;
    }
    if (props.contains("FRONTEND::tuner_status::supports_scan")) {
        if (!(props["FRONTEND::tuner_status::supports_scan"] >>= s.supports_scan)) return false;
    }
    if (props.contains("FRONTEND::tuner_status::tuner_number")) {
        if (!(props["FRONTEND::tuner_status::tuner_number"] >>= s.tuner_number)) return false;
    }
    if (props.contains("FRONTEND::tuner_status::tuner_type")) {
        if (!(props["FRONTEND::tuner_status::tuner_type"] >>= s.tuner_type)) return false;
    }
    return true;
}

inline void operator<<= (CORBA::Any& a, const frontend_tuner_status_struct_struct& s) {
    redhawk::PropertyMap props;
 
    props["FRONTEND::tuner_status::agc"] = s.agc;
 
    props["FRONTEND::tuner_status::allocation_id_csv"] = s.allocation_id_csv;
 
    props["FRONTEND::tuner_status::available_frequency"] = s.available_frequency;
 
    props["FRONTEND::tuner_status::available_gain"] = s.available_gain;
 
    props["FRONTEND::tuner_status::available_sample_rate"] = s.available_sample_rate;
 
    props["FRONTEND::tuner_status::bandwidth"] = s.bandwidth;
 
    props["FRONTEND::tuner_status::center_frequency"] = s.center_frequency;
 
    props["FRONTEND::tuner_status::complex"] = s.complex;
 
    props["FRONTEND::tuner_status::enabled"] = s.enabled;
 
    props["FRONTEND::tuner_status::gain"] = s.gain;
 
    props["FRONTEND::tuner_status::group_id"] = s.group_id;
 
    props["FRONTEND::tuner_status::rf_flow_id"] = s.rf_flow_id;
 
    props["FRONTEND::tuner_status::sample_rate"] = s.sample_rate;
 
    props["FRONTEND::tuner_status::scan_mode_enabled"] = s.scan_mode_enabled;
 
    props["FRONTEND::tuner_status::stream_id"] = s.stream_id;
 
    props["FRONTEND::tuner_status::supports_scan"] = s.supports_scan;
 
    props["FRONTEND::tuner_status::tuner_number"] = s.tuner_number;
 
    props["FRONTEND::tuner_status::tuner_type"] = s.tuner_type;
    a <<= props;
}

inline bool operator== (const frontend_tuner_status_struct_struct& s1, const frontend_tuner_status_struct_struct& s2) {
    if (s1.agc!=s2.agc)
        return false;
    if (s1.allocation_id_csv!=s2.allocation_id_csv)
        return false;
    if (s1.available_frequency!=s2.available_frequency)
        return false;
    if (s1.available_gain!=s2.available_gain)
        return false;
    if (s1.available_sample_rate!=s2.available_sample_rate)
        return false;
    if (s1.bandwidth!=s2.bandwidth)
        return false;
    if (s1.center_frequency!=s2.center_frequency)
        return false;
    if (s1.complex!=s2.complex)
        return false;
    if (s1.enabled!=s2.enabled)
        return false;
    if (s1.gain!=s2.gain)
        return false;
    if (s1.group_id!=s2.group_id)
        return false;
    if (s1.rf_flow_id!=s2.rf_flow_id)
        return false;
    if (s1.sample_rate!=s2.sample_rate)
        return false;
    if (s1.scan_mode_enabled!=s2.scan_mode_enabled)
        return false;
    if (s1.stream_id!=s2.stream_id)
        return false;
    if (s1.supports_scan!=s2.supports_scan)
        return false;
    if (s1.tuner_number!=s2.tuner_number)
        return false;
    if (s1.tuner_type!=s2.tuner_type)
        return false;
    return true;
}

inline bool operator!= (const frontend_tuner_status_struct_struct& s1, const frontend_tuner_status_struct_struct& s2) {
    return !(s1==s2);
}

struct rtl_device_struct_struct {
    rtl_device_struct_struct ()
    {
    }

    static std::string getId() {
        return std::string("available_devices::rtl_device_struct");
    }

    static const char* getFormat() {
        return "ssssH";
    }

    std::string name;
    std::string vendor;
    std::string product;
    std::string serial;
    unsigned short index;
};

inline bool operator>>= (const CORBA::Any& a, rtl_device_struct_struct& s) {
    CF::Properties* temp;
    if (!(a >>= temp)) return false;
    const redhawk::PropertyMap& props = redhawk::PropertyMap::cast(*temp);
    if (props.contains("available_devices::rtl::name")) {
        if (!(props["available_devices::rtl::name"] >>= s.name)) return false;
    }
    if (props.contains("available_devices::rtl::vendor")) {
        if (!(props["available_devices::rtl::vendor"] >>= s.vendor)) return false;
    }
    if (props.contains("available_devices::rtl::product")) {
        if (!(props["available_devices::rtl::product"] >>= s.product)) return false;
    }
    if (props.contains("available_devices::rtl::serial")) {
        if (!(props["available_devices::rtl::serial"] >>= s.serial)) return false;
    }
    if (props.contains("available_devices::rtl::index")) {
        if (!(props["available_devices::rtl::index"] >>= s.index)) return false;
    }
    return true;
}

inline void operator<<= (CORBA::Any& a, const rtl_device_struct_struct& s) {
    redhawk::PropertyMap props;
 
    props["available_devices::rtl::name"] = s.name;
 
    props["available_devices::rtl::vendor"] = s.vendor;
 
    props["available_devices::rtl::product"] = s.product;
 
    props["available_devices::rtl::serial"] = s.serial;
 
    props["available_devices::rtl::index"] = s.index;
    a <<= props;
}

inline bool operator== (const rtl_device_struct_struct& s1, const rtl_device_struct_struct& s2) {
    if (s1.name!=s2.name)
        return false;
    if (s1.vendor!=s2.vendor)
        return false;
    if (s1.product!=s2.product)
        return false;
    if (s1.serial!=s2.serial)
        return false;
    if (s1.index!=s2.index)
        return false;
    return true;
}

inline bool operator!= (const rtl_device_struct_struct& s1, const rtl_device_struct_struct& s2) {
    return !(s1==s2);
}

#endif // STRUCTPROPS_H
