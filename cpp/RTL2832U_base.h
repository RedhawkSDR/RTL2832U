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
#ifndef RTL2832U_IMPL_BASE_H
#define RTL2832U_IMPL_BASE_H

#include <boost/thread.hpp>
#include <frontend/frontend.h>
#include <ossie/ThreadedComponent.h>

#include <frontend/frontend.h>
#include <bulkio/bulkio.h>
#include "struct_props.h"

#define BOOL_VALUE_HERE 0
#define DOUBLE_VALUE_HERE 0

class RTL2832U_base : public frontend::FrontendTunerDevice<frontend_tuner_status_struct_struct>, public virtual frontend::digital_tuner_delegation, public virtual frontend::rfinfo_delegation, protected ThreadedComponent
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
        bool update_available_devices;
        std::string group_id;
        bool digital_agc_enable;
        short frequency_correction;
        target_device_struct target_device;
        current_device_struct current_device;
        std::vector<rtl_device_struct_struct> available_devices;

        // Ports
        frontend::InRFInfoPort *RFInfo_in;
        frontend::InDigitalTunerPort *DigitalTuner_in;
        bulkio::OutOctetPort *dataOctet_out;
        bulkio::OutFloatPort *dataFloat_out;

        std::map<std::string, std::string> listeners;

        virtual void setNumChannels(size_t num);

    private:
        void construct();
};
#endif // RTL2832U_IMPL_BASE_H
