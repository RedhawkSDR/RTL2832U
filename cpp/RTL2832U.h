#ifndef RTL2832U_IMPL_H
#define RTL2832U_IMPL_H

#include "RTL2832U_base.h"

#include "RtlDevice.h"

/** Device Individual Tuner. This structure contains stream specific data for channel/tuner to include:
 *      - Data buffer
 *      - Additional stream metadata (stream_id & timestamps)
 */
struct rtlTunerStruct {
    rtlTunerStruct(){
        lock = NULL;

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
    std::string stream_id;
    bool update_sri;
    boost::mutex *lock; // protects this structure as well as interfacing with RTL device/drivers

    void reset(){
        buffer_size = 0;
        bulkio::sri::zeroTime(output_buffer_time);
        stream_id.clear();
        update_sri = false;
    };
    ~rtlTunerStruct(){
        if (lock != NULL)
            delete lock;
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

class RTL2832U_i : public RTL2832U_base
{
    ENABLE_LOGGING
    public:
        RTL2832U_i(char *devMgr_ior, char *id, char *lbl, char *sftwrPrfl);
        RTL2832U_i(char *devMgr_ior, char *id, char *lbl, char *sftwrPrfl, char *compDev);
        RTL2832U_i(char *devMgr_ior, char *id, char *lbl, char *sftwrPrfl, CF::Properties capacities);
        RTL2832U_i(char *devMgr_ior, char *id, char *lbl, char *sftwrPrfl, CF::Properties capacities, char *compDev);
        ~RTL2832U_i();
        int serviceFunction();

    protected:
        std::string getTunerType(const std::string& allocation_id);
        bool getTunerDeviceControl(const std::string& allocation_id);
        std::string getTunerGroupId(const std::string& allocation_id);
        std::string getTunerRfFlowId(const std::string& allocation_id);
        double getTunerCenterFrequency(const std::string& allocation_id);
        void setTunerCenterFrequency(const std::string& allocation_id, double freq);
        double getTunerBandwidth(const std::string& allocation_id);
        void setTunerBandwidth(const std::string& allocation_id, double bw);
        bool getTunerAgcEnable(const std::string& allocation_id);
        void setTunerAgcEnable(const std::string& allocation_id, bool enable);
        float getTunerGain(const std::string& allocation_id);
        void setTunerGain(const std::string& allocation_id, float gain);
        long getTunerReferenceSource(const std::string& allocation_id);
        void setTunerReferenceSource(const std::string& allocation_id, long source);
        bool getTunerEnable(const std::string& allocation_id);
        void setTunerEnable(const std::string& allocation_id, bool enable);
        double getTunerOutputSampleRate(const std::string& allocation_id);
        void setTunerOutputSampleRate(const std::string& allocation_id, double sr);
        std::string get_rf_flow_id(const std::string& port_name);
        void set_rf_flow_id(const std::string& port_name, const std::string& id);
        frontend::RFInfoPkt get_rfinfo_pkt(const std::string& port_name);
        void set_rfinfo_pkt(const std::string& port_name, const frontend::RFInfoPkt& pkt);

    private:
        ////////////////////////////////////////
        // Required device specific functions // -- to be implemented by device developer
        ////////////////////////////////////////

        // these are pure virtual, must be implemented here
        void deviceEnable(frontend_tuner_status_struct_struct &fts, size_t tuner_id);
        void deviceDisable(frontend_tuner_status_struct_struct &fts, size_t tuner_id);
        bool deviceSetTuning(const frontend::frontend_tuner_allocation_struct &request, frontend_tuner_status_struct_struct &fts, size_t tuner_id);
        bool deviceDeleteTuning(frontend_tuner_status_struct_struct &fts, size_t tuner_id);

        /////////////////////////
        // Developer additions //
        /////////////////////////

        // Ensures access to properties is thread safe
        boost::mutex prop_lock;

        // keep track of RFInfoPkt from RFInfo_in port
        frontend::RFInfoPkt rfinfo_pkt;

        // run-time rtl tuner tracking struct
        // data buffer/stream_id/timestamps
        // protected by rtl_tuner.lock
        rtlTunerStruct rtl_tuner;

        // capabilities of target rtl device
        rtlCapabilitiesStruct rtl_capabilities;

        // configure callbacks
        void updateAvailableDevicesChanged(const bool* old_value, const bool* new_value);
        void groupIdChanged(const std::string* old_value, const std::string* new_value);
        void rtl2832uAgcEnableChanged(const std::string* old_value, const std::string* new_value);
        void targetDeviceChanged(const target_device_struct* old_value, const target_device_struct* new_value);

        // helper functions
        std::string getStreamId();

        // interface with RTL device/driver
        void updateAvailableDevices();
        void initRtl() throw (CF::PropertySet::InvalidConfiguration);
        long rtlReceive(double timeout = 0.0);
        bool rtlEnable();
        bool rtlDisable();

        // RTL device-specific
        RtlDevice *rtl_device_ptr;

        void construct();
};

#endif // RTL2832U_IMPL_H
