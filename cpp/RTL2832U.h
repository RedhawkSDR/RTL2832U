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

#ifndef RTL2832U_IMPL_H
#define RTL2832U_IMPL_H

#include "RTL2832U_base.h"

#include "RtlDevice.h"


/** struct ticket_lock and class scoped_tuner_lock are used for improved thread synchronization
 *      - Forces FIFO order for locking mutex
 *      - Avoids serviceFunction greedily holding tuner_lock mutex, starving other threads
 */
typedef struct ticket_lock {
    ticket_lock(){
        cond=NULL;
        mutex=NULL;
        queue_head=queue_tail=0;
    }
    boost::condition_variable* cond;
    boost::mutex* mutex;
    size_t queue_head, queue_tail;
} ticket_lock_t;

class scoped_tuner_lock{
    public:
        scoped_tuner_lock(ticket_lock_t& _ticket){
            ticket = &_ticket;

            boost::mutex::scoped_lock lock(*ticket->mutex);
            queue_me = ticket->queue_tail++;
            while (queue_me != ticket->queue_head)
            {
                ticket->cond->wait(lock);
            }
        }
        ~scoped_tuner_lock(){
            boost::mutex::scoped_lock lock(*ticket->mutex);
            ticket->queue_head++;
            ticket->cond->notify_all();
        }
    private:
        ticket_lock_t* ticket;
        size_t queue_me;
};

/** Device Individual Tuner. This structure contains stream specific data for channel/tuner to include:
 *      - Data buffer
 *      - Additional stream metadata (timestamps)
 */
struct rtlTunerStruct {
    rtlTunerStruct(){

        // size buffer within CORBA transfer limits
        // Multiply by some number < 1 to leave some margin for the CORBA header
        // fyi: the bulkio pushPacket call does this same calculation as of 1.10,
        //      so we'll only require a single pushPacket call per buffer
        // Also, since data is complex, ensure number of samples is even
        // Note: use float to determine max buffer size and size both buffers with that num samples
        const size_t max_payload_size    = (size_t) (bulkio::Const::MaxTransferBytes() * .9);
        const size_t max_samples_per_push = size_t((max_payload_size/sizeof(float))/2)*2;

        buffer_capacity = max_samples_per_push;
        float_output_buffer.resize( buffer_capacity );
        octet_output_buffer.resize( buffer_capacity );

        reset();
    }

    std::vector<float> float_output_buffer;
    std::vector<unsigned char> octet_output_buffer;
    size_t buffer_capacity; // num samps buffer can hold
    size_t buffer_size; // num samps in buffer
    BULKIO::PrecisionUTCTime output_buffer_time;
    bool update_sri;
    ticket_lock_t lock; // protects this structure as well as interfacing with RTL device/drivers

    void reset(){
        buffer_size = 0;
        bulkio::sri::zeroTime(output_buffer_time);
        update_sri = false;
    }
};

struct rtlCapabilitiesStruct {
    rtlCapabilitiesStruct(){
        center_frequency_max=0;
        center_frequency_min=0;
        sample_rate_max=0;
        sample_rate_min=0;
        gain_max=0;
        gain_min=0;
    }
    double center_frequency_max;
    double center_frequency_min;
    double sample_rate_max;
    double sample_rate_min;
    double gain_max;
    double gain_min;
};

struct scanSettings {
	std::vector<double> scanFrequencies; // List of Frequencies to scan
	short currentScanIndex; //Current index in scanFrequencies
	double scanTime; //Time to dwell in each scan
	BULKIO::PrecisionUTCTime scan_start_time;
	scanSettings () {
		currentScanIndex = 0;
		scanTime = 0;


	}


};

class RTL2832U_i : public RTL2832U_base
{
    ENABLE_LOGGING
    public:
        RTL2832U_i(char *devMgr_ior, char *id, char *lbl, char *sftwrPrfl);
        RTL2832U_i(char *devMgr_ior, char *id, char *lbl, char *sftwrPrfl, char *compDev);
        RTL2832U_i(char *devMgr_ior, char *id, char *lbl, char *sftwrPrfl, CF::Properties capacities);
        RTL2832U_i(char *devMgr_ior, char *id, char *lbl, char *sftwrPrfl, CF::Properties capacities, char *compDev);
        ~RTL2832U_i();

        /* acquires the rtl_tuner.lock */
        int serviceFunction();

        /* acquires the prop_lock and the rtl_tuner.lock */
       // void initialize() throw (CF::LifeCycle::InitializeError, CORBA::SystemException);

    protected:

        std::string getTunerType(const std::string& allocation_id);

        bool getTunerDeviceControl(const std::string& allocation_id);

        std::string getTunerGroupId(const std::string& allocation_id);

        std::string getTunerRfFlowId(const std::string& allocation_id);

        double getTunerCenterFrequency(const std::string& allocation_id);

        /* acquires the prop_lock and the rtl_tuner.lock */
        void setTunerCenterFrequency(const std::string& allocation_id, double freq);

        double getTunerBandwidth(const std::string& allocation_id);

        void setTunerBandwidth(const std::string& allocation_id, double bw);

        bool getTunerAgcEnable(const std::string& allocation_id);

        /* acquires the rtl_tuner.lock */
        void setTunerAgcEnable(const std::string& allocation_id, bool enable);

        float getTunerGain(const std::string& allocation_id);

        /* acquires the prop_lock and the rtl_tuner.lock */
        void setTunerGain(const std::string& allocation_id, float gain);

        long getTunerReferenceSource(const std::string& allocation_id);

        void setTunerReferenceSource(const std::string& allocation_id, long source);

        bool getTunerEnable(const std::string& allocation_id);

        /* acquires the rtl_tuner.lock */
        void setTunerEnable(const std::string& allocation_id, bool enable);

        double getTunerOutputSampleRate(const std::string& allocation_id);

        /* acquires the prop_lock and the rtl_tuner.lock */
        void setTunerOutputSampleRate(const std::string& allocation_id, double sr);

        /* acquires the prop_lock */
        std::string get_rf_flow_id(const std::string& port_name);

        /* acquires the prop_lock and the rtl_tuner.lock */
        void set_rf_flow_id(const std::string& port_name, const std::string& id);

        /* acquires the prop_lock */
        frontend::RFInfoPkt get_rfinfo_pkt(const std::string& port_name);

        /* acquires the prop_lock and the rtl_tuner.lock */
        void set_rfinfo_pkt(const std::string& port_name, const frontend::RFInfoPkt& pkt);
        frontend::ScanStatus getScanStatus(const std::string& allocation_id);

        void setScanStartTime(const std::string& allocation_id, const BULKIO::PrecisionUTCTime& start_time);

        void setScanStrategy(const std::string& allocation_id, const frontend::ScanStrategy* scan_strategy);

    private:
        ////////////////////////////////////////
        // Required device specific functions // -- to be implemented by device developer
        ////////////////////////////////////////

        // these are pure virtual, must be implemented here

        void deviceEnable(frontend_tuner_status_struct_struct &fts, size_t tuner_id);

        void deviceDisable(frontend_tuner_status_struct_struct &fts, size_t tuner_id);

        bool deviceSetTuning(const frontend::frontend_tuner_allocation_struct &request, frontend_tuner_status_struct_struct &fts, size_t tuner_id);

        bool deviceSetTuningScan(const frontend::frontend_tuner_allocation_struct &request, const frontend::frontend_scanner_allocation_struct &scan_request, frontend_tuner_status_struct_struct &fts, size_t tuner_id);

        bool deviceDeleteTuning(frontend_tuner_status_struct_struct &fts, size_t tuner_id);

        /////////////////////////
        // Developer additions //
        /////////////////////////

        // rtl tuner tracking struct
        // Protected by rtl_tuner.lock
        // rtl_tuner.lock protects:
        //     - rtl_tuner.* (except the lock itself)
        //     - frontend_tuner_status[0].* (except allocation_id_csv, which is managed internally by FrontendTunerDevice class)
        //     - rtl_device_ptr->*
        // Note: Cannot acquire prop_lock while holding rtl_tuner.lock
        //       Must acquire prop_lock before rtl_tuner.lock if both are required
        rtlTunerStruct rtl_tuner;

        // Ensures access to properties is thread safe
        // Protects:
        //     - rfinfo_pkt
        //     - rtl_capabilities
        //     - and properties:
        //         - target_device
        //         - current_device
        //         - available_devices
        //         - update_available_devices
        //         - group_id
        //         - digital_agc_enable
        //         - frequency_correction
        //         - frontend_tuner_status
        // Note: Cannot acquire prop_lock while holding rtl_tuner.lock
        //       Must acquire prop_lock before rtl_tuner.lock if both are required
        boost::mutex prop_lock;

        // keep track of RFInfoPkt from RFInfo_in port
        // - protected by prop_lock
        frontend::RFInfoPkt rfinfo_pkt;

        // capabilities of target rtl device
        // - protected by prop_lock
        rtlCapabilitiesStruct rtl_capabilities;

        scanSettings scan_settings;

        //
        // configure callbacks
        //

        /* acquires the prop_lock */
        void updateAvailableDevicesChanged(const bool* old_value, const bool* new_value);

        /* acquires the prop_lock and the rtl_tuner.lock */
        void groupIdChanged(const std::string* old_value, const std::string* new_value);

        /* acquires the rtl_tuner.lock */
        void rtl2832uAgcEnableChanged(const bool* old_value, const bool* new_value);

        /* acquires the prop_lock and the rtl_tuner.lock */
        void targetDeviceChanged(const target_device_struct* old_value, const target_device_struct* new_value);

        /* acquires the prop_lock and the rtl_tuner.lock */
        void frequencyCorrectionChanged(const short* old_value, const short* new_value);

        //
        // helper functions
        //

        /* acquire rtl_tuner.lock prior to calling this function */
        std::string generateStreamId();

        //
        // interface with RTL device/driver
        //

        /* acquire prop_lock prior to calling this function */
        void updateAvailableDevices();

        /* acquire prop_lock prior to calling this function */
        /* acquires the rtl_tuner.lock */
        void initRtl() throw (CF::PropertySet::InvalidConfiguration);

        /* acquire rtl_tuner.lock prior to calling this function */
        long rtlReceive(double timeout = 0.0);

        /* acquire rtl_tuner.lock prior to calling this function */
        bool rtlEnable();

        /* acquire rtl_tuner.lock prior to calling this function */
        bool rtlDisable();

        // RTL device-specific
        RtlDevice *rtl_device_ptr;

        void construct();
};

#endif // RTL2832U_IMPL_H
