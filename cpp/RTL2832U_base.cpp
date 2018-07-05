/*
 * This file is protected by Copyright. Please refer to the COPYRIGHT file
 * distributed with this source distribution.
 *
 * This file is part of RTL2832U Device.
 *
 * RTL2832U Device is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * RTL2832U Device is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see http://www.gnu.org/licenses/.
 */
#include "RTL2832U_base.h"

/*******************************************************************************************

    AUTO-GENERATED CODE. DO NOT MODIFY

    The following class functions are for the base class for the device class. To
    customize any of these functions, do not modify them here. Instead, overload them
    on the child class

******************************************************************************************/

RTL2832U_base::RTL2832U_base(char *devMgr_ior, char *id, char *lbl, char *sftwrPrfl) :
    frontend::FrontendScanningTunerDevice<frontend_tuner_status_struct_struct>(devMgr_ior, id, lbl, sftwrPrfl),
    ThreadedComponent()
{
    construct();
}

RTL2832U_base::RTL2832U_base(char *devMgr_ior, char *id, char *lbl, char *sftwrPrfl, char *compDev) :
    frontend::FrontendScanningTunerDevice<frontend_tuner_status_struct_struct>(devMgr_ior, id, lbl, sftwrPrfl, compDev),
    ThreadedComponent()
{
    construct();
}

RTL2832U_base::RTL2832U_base(char *devMgr_ior, char *id, char *lbl, char *sftwrPrfl, CF::Properties capacities) :
    frontend::FrontendScanningTunerDevice<frontend_tuner_status_struct_struct>(devMgr_ior, id, lbl, sftwrPrfl, capacities),
    ThreadedComponent()
{
    construct();
}

RTL2832U_base::RTL2832U_base(char *devMgr_ior, char *id, char *lbl, char *sftwrPrfl, CF::Properties capacities, char *compDev) :
    frontend::FrontendScanningTunerDevice<frontend_tuner_status_struct_struct>(devMgr_ior, id, lbl, sftwrPrfl, capacities, compDev),
    ThreadedComponent()
{
    construct();
}

RTL2832U_base::~RTL2832U_base()
{
    RFInfo_in->_remove_ref();
    RFInfo_in = 0;
    DigitalTuner_in->_remove_ref();
    DigitalTuner_in = 0;
    dataOctet_out->_remove_ref();
    dataOctet_out = 0;
    dataFloat_out->_remove_ref();
    dataFloat_out = 0;
}

void RTL2832U_base::construct()
{
    loadProperties();

    RFInfo_in = new frontend::InRFInfoPort("RFInfo_in", this);
    addPort("RFInfo_in", RFInfo_in);
    DigitalTuner_in = new frontend::InDigitalScanningTunerPort("DigitalTuner_in", this);
    addPort("DigitalTuner_in", DigitalTuner_in);
    dataOctet_out = new bulkio::OutOctetPort("dataOctet_out");
    addPort("dataOctet_out", dataOctet_out);
    dataFloat_out = new bulkio::OutFloatPort("dataFloat_out");
    addPort("dataFloat_out", dataFloat_out);

}

/*******************************************************************************************
    Framework-level functions
    These functions are generally called by the framework to perform housekeeping.
*******************************************************************************************/
void RTL2832U_base::start() throw (CORBA::SystemException, CF::Resource::StartError)
{
    frontend::FrontendScanningTunerDevice<frontend_tuner_status_struct_struct>::start();
    ThreadedComponent::startThread();
}

void RTL2832U_base::stop() throw (CORBA::SystemException, CF::Resource::StopError)
{
    frontend::FrontendScanningTunerDevice<frontend_tuner_status_struct_struct>::stop();
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

    frontend::FrontendScanningTunerDevice<frontend_tuner_status_struct_struct>::releaseObject();
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
                "property");

    addProperty(group_id,
                "",
                "group_id",
                "group_id",
                "readwrite",
                "",
                "external",
                "property");

    addProperty(digital_agc_enable,
                false,
                "digital_agc_enable",
                "digital_agc_enable",
                "readwrite",
                "",
                "external",
                "property");

    addProperty(frequency_correction,
                0,
                "frequency_correction",
                "frequency_correction",
                "readwrite",
                "ppm",
                "external",
                "property");

    frontend_listener_allocation = frontend::frontend_listener_allocation_struct();
    frontend_tuner_allocation = frontend::frontend_tuner_allocation_struct();
    addProperty(target_device,
                target_device_struct(),
                "target_device",
                "target_device",
                "readwrite",
                "",
                "external",
                "property");

    addProperty(current_device,
                current_device_struct(),
                "current_device",
                "current_device",
                "readonly",
                "",
                "external",
                "property");

    frontend_scanner_allocation = frontend::frontend_scanner_allocation_struct();
    addProperty(available_devices,
                "available_devices",
                "available_devices",
                "readonly",
                "",
                "external",
                "property");

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
    // find control allocation_id
    std::string existing_alloc_id = allocation_id;
    std::map<std::string,std::string>::iterator existing_listener;
    while ((existing_listener=listeners.find(existing_alloc_id)) != listeners.end())
        existing_alloc_id = existing_listener->second;
    listeners[listen_alloc_id] = existing_alloc_id;

}

void RTL2832U_base::removeListener(const std::string& listen_alloc_id)
{
    if (listeners.find(listen_alloc_id) != listeners.end()) {
        listeners.erase(listen_alloc_id);
    }
}
void RTL2832U_base::removeAllocationIdRouting(const size_t tuner_id) {
}

