#include "RTL2832U_base.h"

/*******************************************************************************************

    AUTO-GENERATED CODE. DO NOT MODIFY

    The following class functions are for the base class for the device class. To
    customize any of these functions, do not modify them here. Instead, overload them
    on the child class

******************************************************************************************/

RTL2832U_base::RTL2832U_base(char *devMgr_ior, char *id, char *lbl, char *sftwrPrfl) :
    frontend::FrontendTunerDevice<frontend_tuner_status_struct_struct>(devMgr_ior, id, lbl, sftwrPrfl),
    ThreadedComponent()
{
    construct();
}

RTL2832U_base::RTL2832U_base(char *devMgr_ior, char *id, char *lbl, char *sftwrPrfl, char *compDev) :
    frontend::FrontendTunerDevice<frontend_tuner_status_struct_struct>(devMgr_ior, id, lbl, sftwrPrfl, compDev),
    ThreadedComponent()
{
    construct();
}

RTL2832U_base::RTL2832U_base(char *devMgr_ior, char *id, char *lbl, char *sftwrPrfl, CF::Properties capacities) :
    frontend::FrontendTunerDevice<frontend_tuner_status_struct_struct>(devMgr_ior, id, lbl, sftwrPrfl, capacities),
    ThreadedComponent()
{
    construct();
}

RTL2832U_base::RTL2832U_base(char *devMgr_ior, char *id, char *lbl, char *sftwrPrfl, CF::Properties capacities, char *compDev) :
    frontend::FrontendTunerDevice<frontend_tuner_status_struct_struct>(devMgr_ior, id, lbl, sftwrPrfl, capacities, compDev),
    ThreadedComponent()
{
    construct();
}

RTL2832U_base::~RTL2832U_base()
{
    delete RFInfo_in;
    RFInfo_in = 0;
    delete DigitalTuner_in;
    DigitalTuner_in = 0;
    delete dataOctet_out;
    dataOctet_out = 0;
    delete dataFloat_out;
    dataFloat_out = 0;
}

void RTL2832U_base::construct()
{
    loadProperties();

    RFInfo_in = new frontend::InRFInfoPort("RFInfo_in", this);
    addPort("RFInfo_in", RFInfo_in);
    DigitalTuner_in = new frontend::InDigitalTunerPort("DigitalTuner_in", this);
    addPort("DigitalTuner_in", DigitalTuner_in);
    dataOctet_out = new bulkio::OutOctetPort("dataOctet_out");
    addPort("dataOctet_out", dataOctet_out);
    dataFloat_out = new bulkio::OutFloatPort("dataFloat_out");
    addPort("dataFloat_out", dataFloat_out);

    this->addPropertyChangeListener("connectionTable", this, &RTL2832U_base::connectionTableChanged);

}

/*******************************************************************************************
    Framework-level functions
    These functions are generally called by the framework to perform housekeeping.
*******************************************************************************************/
void RTL2832U_base::start() throw (CORBA::SystemException, CF::Resource::StartError)
{
    frontend::FrontendTunerDevice<frontend_tuner_status_struct_struct>::start();
    ThreadedComponent::startThread();
}

void RTL2832U_base::stop() throw (CORBA::SystemException, CF::Resource::StopError)
{
    frontend::FrontendTunerDevice<frontend_tuner_status_struct_struct>::stop();
    if (!ThreadedComponent::stopThread()) {
        throw CF::Resource::StopError(CF::CF_NOTSET, "Processing thread did not die");
    }
}

void RTL2832U_base::releaseObject() throw (CORBA::SystemException, CF::LifeCycle::ReleaseError)
{
    // This function clears the device running condition so main shuts down everything
    try {
        stop();
    } catch (CF::Resource::StopError& ex) {
        // TODO - this should probably be logged instead of ignored
    }

    frontend::FrontendTunerDevice<frontend_tuner_status_struct_struct>::releaseObject();
}

void RTL2832U_base::connectionTableChanged(const std::vector<connection_descriptor_struct>* oldValue, const std::vector<connection_descriptor_struct>* newValue)
{
    dataOctet_out->updateConnectionFilter(*newValue);
    dataFloat_out->updateConnectionFilter(*newValue);
}

void RTL2832U_base::loadProperties()
{
    device_kind = "FRONTEND::TUNER";
    device_model = "RTL2832U";
    addProperty(update_available_devices,
                false,
                "update_available_devices",
                "update_available_devices",
                "readwrite",
                "",
                "external",
                "configure");

    addProperty(group_id,
                "",
                "group_id",
                "group_id",
                "readwrite",
                "",
                "external",
                "execparam,configure");

    addProperty(RTL2832U_agc_enable,
                false,
                "RTL2832U_agc_enable",
                "RTL2832U_agc_enable",
                "readwrite",
                "",
                "external",
                "configure");

    frontend_listener_allocation = frontend::frontend_listener_allocation_struct();
    frontend_tuner_allocation = frontend::frontend_tuner_allocation_struct();
    addProperty(target_device,
                target_device_struct(),
                "target_device",
                "target_device",
                "readwrite",
                "",
                "external",
                "configure");

    addProperty(connectionTable,
                "connectionTable",
                "",
                "readonly",
                "",
                "external",
                "configure");

    addProperty(available_devices,
                "available_devices",
                "available_devices",
                "readonly",
                "",
                "external",
                "configure");

}

/* This sets the number of entries in the frontend_tuner_status struct sequence property
 *  * as well as the tuner_allocation_ids vector. Call this function during initialization
 *   */
void RTL2832U_base::setNumChannels(size_t num)
{
    frontend_tuner_status.clear();
    frontend_tuner_status.resize(num);
    tuner_allocation_ids.clear();
    tuner_allocation_ids.resize(num);
}

void RTL2832U_base::frontendTunerStatusChanged(const std::vector<frontend_tuner_status_struct_struct>* oldValue, const std::vector<frontend_tuner_status_struct_struct>* newValue)
{
    this->tuner_allocation_ids.resize(this->frontend_tuner_status.size());
}

CF::Properties* RTL2832U_base::getTunerStatus(const std::string& allocation_id)
{
    CF::Properties* tmpVal = new CF::Properties();
    long tuner_id = getTunerMapping(allocation_id);
    if (tuner_id < 0)
        throw FRONTEND::FrontendException(("ERROR: ID: " + std::string(allocation_id) + " IS NOT ASSOCIATED WITH ANY TUNER!").c_str());
    CORBA::Any prop;
    prop <<= *(static_cast<frontend_tuner_status_struct_struct*>(&this->frontend_tuner_status[tuner_id]));
    prop >>= tmpVal;

    CF::Properties_var tmp = new CF::Properties(*tmpVal);
    return tmp._retn();
}

void RTL2832U_base::assignListener(const std::string& listen_alloc_id, const std::string& allocation_id)
{
    listeners[listen_alloc_id] = allocation_id;
    std::vector<connection_descriptor_struct> old_table = this->connectionTable;
    for (std::map<std::string, std::string>::iterator listener=listeners.begin();listener!=listeners.end();listener++) {
        std::vector<std::string> streamids;
        std::vector<std::string> port_names;
        for (std::vector<connection_descriptor_struct>::iterator entry=this->connectionTable.begin();entry!=this->connectionTable.end();entry++) {
            if (entry->connection_id == listener->second) {
                streamids.push_back(entry->stream_id);
                port_names.push_back(entry->port_name);
            }
        }
        for (unsigned int i=0; i<streamids.size(); i++) {
            bool foundEntry = false;
            for (std::vector<connection_descriptor_struct>::iterator entry=this->connectionTable.begin();entry!=this->connectionTable.end();entry++) {
                if ((entry->stream_id == streamids[i]) and (entry->connection_id == listen_alloc_id) and (entry->port_name == port_names[i])) {
                    foundEntry = true;
                    break;
                }
            }
            if (!foundEntry) {
                connection_descriptor_struct tmp;
                tmp.stream_id = streamids[i];
                tmp.connection_id = listen_alloc_id;
                tmp.port_name = port_names[i];
                this->connectionTable.push_back(tmp);
            }
        }
    }
    this->connectionTableChanged(&old_table, &this->connectionTable);
}

void RTL2832U_base::removeListener(const std::string& listen_alloc_id)
{
    if (listeners.find(listen_alloc_id) != listeners.end()) {
        listeners.erase(listen_alloc_id);
    }
    std::vector<connection_descriptor_struct> old_table = this->connectionTable;
    std::vector<connection_descriptor_struct>::iterator entry = this->connectionTable.begin();
    while (entry != this->connectionTable.end()) {
        if (entry->connection_id == listen_alloc_id) {
            entry = this->connectionTable.erase(entry);
        } else {
            entry++;
        }
    }
    ExtendedCF::UsesConnectionSequence_var tmp;
    // Check to see if port "dataOctet_out" is on connectionTable (old or new)
    tmp = this->dataOctet_out->connections();
    for (unsigned int i=0; i<this->dataOctet_out->connections()->length(); i++) {
        std::string connection_id = ossie::corba::returnString(tmp[i].connectionId);
        if (connection_id == listen_alloc_id) {
            this->dataOctet_out->disconnectPort(connection_id.c_str());
        }
    }
    // Check to see if port "dataFloat_out" is on connectionTable (old or new)
    tmp = this->dataFloat_out->connections();
    for (unsigned int i=0; i<this->dataFloat_out->connections()->length(); i++) {
        std::string connection_id = ossie::corba::returnString(tmp[i].connectionId);
        if (connection_id == listen_alloc_id) {
            this->dataFloat_out->disconnectPort(connection_id.c_str());
        }
    }
    this->connectionTableChanged(&old_table, &this->connectionTable);
}

void RTL2832U_base::removeAllocationIdRouting(const size_t tuner_id) {
    std::string allocation_id = getControlAllocationId(tuner_id);
    std::vector<connection_descriptor_struct> old_table = this->connectionTable;
    std::vector<connection_descriptor_struct>::iterator itr = this->connectionTable.begin();
    while (itr != this->connectionTable.end()) {
        if (itr->connection_id == allocation_id) {
            itr = this->connectionTable.erase(itr);
            continue;
        }
        itr++;
    }
    for (std::map<std::string, std::string>::iterator listener=listeners.begin();listener!=listeners.end();listener++) {
        if (listener->second == allocation_id) {
            std::vector<connection_descriptor_struct>::iterator itr = this->connectionTable.begin();
            while (itr != this->connectionTable.end()) {
                if (itr->connection_id == listener->first) {
                    itr = this->connectionTable.erase(itr);
                    continue;
                }
                itr++;
            }
        }
    }
    this->connectionTableChanged(&old_table, &this->connectionTable);
}

void RTL2832U_base::removeStreamIdRouting(const std::string stream_id, const std::string allocation_id) {
    std::vector<connection_descriptor_struct> old_table = this->connectionTable;
    std::vector<connection_descriptor_struct>::iterator itr = this->connectionTable.begin();
    while (itr != this->connectionTable.end()) {
        if (allocation_id == "") {
            if (itr->stream_id == stream_id) {
                itr = this->connectionTable.erase(itr);
                continue;
            }
        } else {
            if ((itr->stream_id == stream_id) and (itr->connection_id == allocation_id)) {
                itr = this->connectionTable.erase(itr);
                continue;
            }
        }
        itr++;
    }
    for (std::map<std::string, std::string>::iterator listener=listeners.begin();listener!=listeners.end();listener++) {
        if (listener->second == allocation_id) {
            std::vector<connection_descriptor_struct>::iterator itr = this->connectionTable.begin();
            while (itr != this->connectionTable.end()) {
                if ((itr->connection_id == listener->first) and (itr->stream_id == stream_id)) {
                    itr = this->connectionTable.erase(itr);
                    continue;
                }
                itr++;
            }
        }
    }
    this->connectionTableChanged(&old_table, &this->connectionTable);
}

void RTL2832U_base::matchAllocationIdToStreamId(const std::string allocation_id, const std::string stream_id, const std::string port_name) {
    if (port_name != "") {
        for (std::vector<connection_descriptor_struct>::iterator prop_itr = this->connectionTable.begin(); prop_itr!=this->connectionTable.end(); prop_itr++) {
            if ((*prop_itr).port_name != port_name)
                continue;
            if ((*prop_itr).stream_id != stream_id)
                continue;
            if ((*prop_itr).connection_id != allocation_id)
                continue;
            // all three match. This is a repeat
            return;
        }
        std::vector<connection_descriptor_struct> old_table = this->connectionTable;
        connection_descriptor_struct tmp;
        tmp.connection_id = allocation_id;
        tmp.port_name = port_name;
        tmp.stream_id = stream_id;
        this->connectionTable.push_back(tmp);
        this->connectionTableChanged(&old_table, &this->connectionTable);
        return;
    }
    std::vector<connection_descriptor_struct> old_table = this->connectionTable;
    connection_descriptor_struct tmp;
    tmp.connection_id = allocation_id;
    tmp.port_name = "dataOctet_out";
    tmp.stream_id = stream_id;
    this->connectionTable.push_back(tmp);
    tmp.connection_id = allocation_id;
    tmp.port_name = "dataFloat_out";
    tmp.stream_id = stream_id;
    this->connectionTable.push_back(tmp);
    this->connectionTableChanged(&old_table, &this->connectionTable);
}

