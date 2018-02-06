#ifndef RTL2832U_BASE_IMPL_BASE_H
#define RTL2832U_BASE_IMPL_BASE_H

#include <boost/thread.hpp>
#include <frontend/frontend.h>
#include <ossie/ThreadedComponent.h>

#include <frontend/frontend.h>
#include <bulkio/bulkio.h>
#include "struct_props.h"

#define BOOL_VALUE_HERE 0

class RTL2832U_base : public frontend::FrontendScanningTunerDevice<frontend_tuner_status_struct_struct>, public virtual frontend::digital_scanning_tuner_delegation, public virtual frontend::rfinfo_delegation, protected ThreadedComponent
{
    public:
        RTL2832U_base(char *devMgr_ior, char *id, char *lbl, char *sftwrPrfl);
        RTL2832U_base(char *devMgr_ior, char *id, char *lbl, char *sftwrPrfl, char *compDev);
        RTL2832U_base(char *devMgr_ior, char *id, char *lbl, char *sftwrPrfl, CF::Properties capacities);
        RTL2832U_base(char *devMgr_ior, char *id, char *lbl, char *sftwrPrfl, CF::Properties capacities, char *compDev);
        ~RTL2832U_base();

        void start() throw (CF::Resource::StartError, CORBA::SystemException);

        void stop() throw (CF::Resource::StopError, CORBA::SystemException);

        void releaseObject() throw (CF::LifeCycle::ReleaseError, CORBA::SystemException);

        void loadProperties();
        void removeAllocationIdRouting(const size_t tuner_id);

        virtual CF::Properties* getTunerStatus(const std::string& allocation_id);
        virtual void assignListener(const std::string& listen_alloc_id, const std::string& allocation_id);
        virtual void removeListener(const std::string& listen_alloc_id);
        void frontendTunerStatusChanged(const std::vector<frontend_tuner_status_struct_struct>* oldValue, const std::vector<frontend_tuner_status_struct_struct>* newValue);

    protected:
        // Member variables exposed as properties
        /// Property: update_available_devices
        bool update_available_devices;
        /// Property: group_id
        std::string group_id;
        /// Property: digital_agc_enable
        bool digital_agc_enable;
        /// Property: frequency_correction
        short frequency_correction;
        /// Property: target_device
        target_device_struct target_device;
        /// Property: current_device
        current_device_struct current_device;
        /// Property: available_devices
        std::vector<rtl_device_struct_struct> available_devices;

        // Ports
        /// Port: RFInfo_in
        frontend::InRFInfoPort *RFInfo_in;
        /// Port: DigitalTuner_in
        frontend::InDigitalScanningTunerPort *DigitalTuner_in;
        /// Port: dataOctet_out
        bulkio::OutOctetPort *dataOctet_out;
        /// Port: dataFloat_out
        bulkio::OutFloatPort *dataFloat_out;

        std::map<std::string, std::string> listeners;

    private:
        void construct();
};
#endif // RTL2832U_BASE_IMPL_BASE_H
