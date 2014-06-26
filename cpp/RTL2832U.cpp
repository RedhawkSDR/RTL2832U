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

/**************************************************************************

    This is the device code. This file contains the child class where
    custom functionality can be added to the device. Custom
    functionality to the base class can be extended here. Access to
    the ports can also be done from this class

**************************************************************************/

#include "RTL2832U.h"

PREPARE_LOGGING(RTL2832U_i)

RTL2832U_i::RTL2832U_i(char *devMgr_ior, char *id, char *lbl, char *sftwrPrfl) :
    RTL2832U_base(devMgr_ior, id, lbl, sftwrPrfl)
{
    construct();
}

RTL2832U_i::RTL2832U_i(char *devMgr_ior, char *id, char *lbl, char *sftwrPrfl, char *compDev) :
    RTL2832U_base(devMgr_ior, id, lbl, sftwrPrfl, compDev)
{
    construct();
}

RTL2832U_i::RTL2832U_i(char *devMgr_ior, char *id, char *lbl, char *sftwrPrfl, CF::Properties capacities) :
    RTL2832U_base(devMgr_ior, id, lbl, sftwrPrfl, capacities)
{
    construct();
}

RTL2832U_i::RTL2832U_i(char *devMgr_ior, char *id, char *lbl, char *sftwrPrfl, CF::Properties capacities, char *compDev) :
    RTL2832U_base(devMgr_ior, id, lbl, sftwrPrfl, capacities, compDev)
{
    construct();
}

RTL2832U_i::~RTL2832U_i()
{
}

/***********************************************************************************************

    Basic functionality:

        The service function is called by the serviceThread object (of type ProcessThread).
        This call happens immediately after the previous call if the return value for
        the previous call was NORMAL.
        If the return value for the previous call was NOOP, then the serviceThread waits
        an amount of time defined in the serviceThread's constructor.
        
    SRI:
        To create a StreamSRI object, use the following code:
                std::string stream_id = "testStream";
                BULKIO::StreamSRI sri = bulkio::sri::create(stream_id);

    Time:
        To create a PrecisionUTCTime object, use the following code:
                BULKIO::PrecisionUTCTime tstamp = bulkio::time::utils::now();

        
    Ports:

        Data is passed to the serviceFunction through the getPacket call (BULKIO only).
        The dataTransfer class is a port-specific class, so each port implementing the
        BULKIO interface will have its own type-specific dataTransfer.

        The argument to the getPacket function is a floating point number that specifies
        the time to wait in seconds. A zero value is non-blocking. A negative value
        is blocking.  Constants have been defined for these values, bulkio::Const::BLOCKING and
        bulkio::Const::NON_BLOCKING.

        Each received dataTransfer is owned by serviceFunction and *MUST* be
        explicitly deallocated.

        To send data using a BULKIO interface, a convenience interface has been added 
        that takes a std::vector as the data input

        NOTE: If you have a BULKIO dataSDDS or dataVITA49  port, you must manually call 
              "port->updateStats()" to update the port statistics when appropriate.

        Example:
            // this example assumes that the device has two ports:
            //  A provides (input) port of type bulkio::InShortPort called short_in
            //  A uses (output) port of type bulkio::OutFloatPort called float_out
            // The mapping between the port and the class is found
            // in the device base class header file

            bulkio::InShortPort::dataTransfer *tmp = short_in->getPacket(bulkio::Const::BLOCKING);
            if (not tmp) { // No data is available
                return NOOP;
            }

            std::vector<float> outputData;
            outputData.resize(tmp->dataBuffer.size());
            for (unsigned int i=0; i<tmp->dataBuffer.size(); i++) {
                outputData[i] = (float)tmp->dataBuffer[i];
            }

            // NOTE: You must make at least one valid pushSRI call
            if (tmp->sriChanged) {
                float_out->pushSRI(tmp->SRI);
            }
            float_out->pushPacket(outputData, tmp->T, tmp->EOS, tmp->streamID);

            delete tmp; // IMPORTANT: MUST RELEASE THE RECEIVED DATA BLOCK
            return NORMAL;

        If working with complex data (i.e., the "mode" on the SRI is set to
        true), the std::vector passed from/to BulkIO can be typecast to/from
        std::vector< std::complex<dataType> >.  For example, for short data:

            bulkio::InShortPort::dataTransfer *tmp = myInput->getPacket(bulkio::Const::BLOCKING);
            std::vector<std::complex<short> >* intermediate = (std::vector<std::complex<short> >*) &(tmp->dataBuffer);
            // do work here
            std::vector<short>* output = (std::vector<short>*) intermediate;
            myOutput->pushPacket(*output, tmp->T, tmp->EOS, tmp->streamID);

        Interactions with non-BULKIO ports are left up to the device developer's discretion

    Properties:
        
        Properties are accessed directly as member variables. For example, if the
        property name is "baudRate", it may be accessed within member functions as
        "baudRate". Unnamed properties are given a generated name of the form
        "prop_n", where "n" is the ordinal number of the property in the PRF file.
        Property types are mapped to the nearest C++ type, (e.g. "string" becomes
        "std::string"). All generated properties are declared in the base class
        (RTL2832U_base).
    
        Simple sequence properties are mapped to "std::vector" of the simple type.
        Struct properties, if used, are mapped to C++ structs defined in the
        generated file "struct_props.h". Field names are taken from the name in
        the properties file; if no name is given, a generated name of the form
        "field_n" is used, where "n" is the ordinal number of the field.
        
        Example:
            // This example makes use of the following Properties:
            //  - A float value called scaleValue
            //  - A boolean called scaleInput
              
            if (scaleInput) {
                dataOut[i] = dataIn[i] * scaleValue;
            } else {
                dataOut[i] = dataIn[i];
            }
            
        Callback methods can be associated with a property so that the methods are
        called each time the property value changes.  This is done by calling 
        addPropertyChangeListener(<property name>, this, &RTL2832U_i::<callback method>)
        in the constructor.

        Callback methods should take two arguments, both const pointers to the value
        type (e.g., "const float *"), and return void.

        Example:
            // This example makes use of the following Properties:
            //  - A float value called scaleValue
            
        //Add to RTL2832U.cpp
        RTL2832U_i::RTL2832U_i(const char *uuid, const char *label) :
            RTL2832U_base(uuid, label)
        {
            addPropertyChangeListener("scaleValue", this, &RTL2832U_i::scaleChanged);
        }

        void RTL2832U_i::scaleChanged(const float *oldValue, const float *newValue)
        {
            std::cout << "scaleValue changed from" << *oldValue << " to " << *newValue
                      << std::endl;
        }
            
        //Add to RTL2832U.h
        void scaleChanged(const float* oldValue, const float* newValue);
        
        
************************************************************************************************/
int RTL2832U_i::serviceFunction()
{
    //LOG_TRACE(RTL2832U_i,__PRETTY_FUNCTION__);
    if (rtl_device_ptr == NULL)
        return NOOP;

    if (frontend_tuner_status.size() <= 0)
        return NOOP;

    bool rx_data = false;

    exclusive_lock tuner_lock(*rtl_tuner.lock);

    //Check to make sure channel is allocated
    if (getControlAllocationId(0).empty()){
        return NOOP;
    }

    //Check to see if channel output is enabled
    if (!frontend_tuner_status[0].enabled){
        return NOOP;
    }

    long num_samps = rtlReceive(1.0); // 1 second timeout
    // if the buffer is full OR (overflow occurred and buffer isn't empty), push buffer out as is and move to next buffer
    if(rtl_tuner.buffer_size >= rtl_tuner.buffer_capacity || (num_samps < 0 && rtl_tuner.buffer_size > 0) ){
        rx_data = true;

        // Note: divide buffer_size by 2 because it takes two elements to represent a single complex sample
        LOG_DEBUG(RTL2832U_i,"serviceFunction|pushing buffer of " << rtl_tuner.buffer_size/2 << " samples");

        // get stream id (creates one if not already created for this tuner)
        std::string stream_id = getStreamId();

        // Send updated SRI
        if (rtl_tuner.update_sri){
            BULKIO::StreamSRI sri = create(stream_id, frontend_tuner_status[0]);
            sri.mode = 1; // complex
            dataFloat_out->pushSRI(sri);
            dataOctet_out->pushSRI(sri);
            rtl_tuner.update_sri = false;
        }

        // Pushing Data
        // handle partial packet (b/c overflow occured)
        if(rtl_tuner.buffer_size < rtl_tuner.buffer_capacity){
            rtl_tuner.float_output_buffer.resize(rtl_tuner.buffer_size);
            rtl_tuner.octet_output_buffer.resize(rtl_tuner.buffer_size);
        }
        dataFloat_out->pushPacket(rtl_tuner.float_output_buffer, rtl_tuner.output_buffer_time, false, stream_id);
        dataOctet_out->pushPacket(rtl_tuner.octet_output_buffer, rtl_tuner.output_buffer_time, false, stream_id);
        // restore buffer size if necessary
        if(rtl_tuner.buffer_size < rtl_tuner.buffer_capacity){
            rtl_tuner.float_output_buffer.resize(rtl_tuner.buffer_capacity);
            rtl_tuner.octet_output_buffer.resize(rtl_tuner.buffer_capacity);
        }
        rtl_tuner.buffer_size = 0;

    } else if(num_samps != 0){ // either received data or overflow occurred, either way data is available
        rx_data = true;
    }

    if(rx_data)
        return NORMAL;
    return NOOP;
}

void RTL2832U_i::initialize() throw (CF::LifeCycle::InitializeError, CORBA::SystemException)
{
    RTL2832U_base::initialize();

    // try to find an available RTL
    { // scope for prop_lock
        exclusive_lock lock(prop_lock);
        try{
            initRtl();
        } catch(CF::PropertySet::InvalidConfiguration& e) {
            LOG_INFO(RTL2832U_i,"No available RTL devices found upon startup")
        }
    }

    /** As of the REDHAWK 1.8.3 release, device are not started automatically by the node. Therefore
     *  the device must start itself. */
    start();

}

void RTL2832U_i::construct()
{
    /***********************************************************************************
     this function is invoked in the constructor
    ***********************************************************************************/
    LOG_TRACE(RTL2832U_i,__PRETTY_FUNCTION__);
    rtl_device_ptr = NULL;

    // set some default values that should get overwritten by correct values
    group_id = "RTL_GROUP_ID_NOT_SET";
    digital_agc_enable = false;

    target_device.index = -1;
    target_device.name.clear();
    target_device.product.clear();
    target_device.serial.clear();
    target_device.vendor.clear();

    // initialize rf info packets w/ very large ranges
    rfinfo_pkt.rf_flow_id = "RTL_FLOW_ID_NOT_SET";
    rfinfo_pkt.rf_center_freq = 50e9; // 50 GHz
    rfinfo_pkt.rf_bandwidth = 100e9; // 100 GHz, makes range 0 Hz to 100 GHz
    rfinfo_pkt.if_center_freq = 0; // 0 Hz, no up/down converter

    //USRP_UHD_base::construct();
    /***********************************************************************************
     this function is invoked in the constructor
    ***********************************************************************************/

    addPropertyChangeListener("target_device", this, &RTL2832U_i::targetDeviceChanged);
    addPropertyChangeListener("group_id", this, &RTL2832U_i::groupIdChanged);
    addPropertyChangeListener("digital_agc_enable", this, &RTL2832U_i::rtl2832uAgcEnableChanged);
    addPropertyChangeListener("update_available_devices", this, &RTL2832U_i::updateAvailableDevicesChanged);
    addPropertyChangeListener("frequency_correction", this, &RTL2832U_i::frequencyCorrectionChanged);

    if(update_available_devices){
        update_available_devices = false;
        updateAvailableDevices();
    }
}

/*************************************************************
Functions supporting tuning allocation
*************************************************************/
void RTL2832U_i::deviceEnable(frontend_tuner_status_struct_struct &fts, size_t tuner_id){
    /************************************************************
    modify fts, which corresponds to this->frontend_tuner_status[tuner_id]
    Make sure to set the 'enabled' member of fts to indicate that tuner as enabled
    ************************************************************/
    LOG_TRACE(RTL2832U_i,__PRETTY_FUNCTION__ << " tuner_id=" << tuner_id);
    // Start Streaming Now
    rtlEnable(); // modifies fts.enabled appropriately
}
void RTL2832U_i::deviceDisable(frontend_tuner_status_struct_struct &fts, size_t tuner_id){
    /************************************************************
    modify fts, which corresponds to this->frontend_tuner_status[tuner_id]
    Make sure to reset the 'enabled' member of fts to indicate that tuner as disabled
    ************************************************************/
    LOG_TRACE(RTL2832U_i,__PRETTY_FUNCTION__ << " tuner_id=" << tuner_id);
    // Stop Streaming Now
    rtlDisable(); //modifies fts.enabled appropriately
}
bool RTL2832U_i::deviceSetTuning(const frontend::frontend_tuner_allocation_struct &request, frontend_tuner_status_struct_struct &fts, size_t tuner_id){
    /************************************************************
    modify fts, which corresponds to this->frontend_tuner_status[tuner_id]
    return true if the tuning succeeded, and false if it failed
    ************************************************************/
    LOG_TRACE(RTL2832U_i,__PRETTY_FUNCTION__ << " tuner_id=" << tuner_id);

    double if_offset = 0.0;

    { // scope for prop_lock
        exclusive_lock lock(prop_lock);

        // check request against RTL specs and analog input
        const bool complex = true; // RTL operates using complex data
        // Note: since samples are complex, assume BW == SR (rather than BW == SR/2 for Real samples)
        try {
            if( !frontend::validateRequestVsDevice(request, rfinfo_pkt, complex, rtl_capabilities.center_frequency_min, rtl_capabilities.center_frequency_max,
                    rtl_capabilities.sample_rate_max /* bw=sr since data is complex */, rtl_capabilities.sample_rate_max) ){
                throw FRONTEND::BadParameterException("Invalid request -- falls outside of analog input or device capabilities");
            }
        } catch(FRONTEND::BadParameterException& e){
            LOG_INFO(RTL2832U_i,"deviceSetTuning|BadParameterException - " << e.msg);
            throw;
        }

        // calculate if_offset according to rx rfinfo packet
        if(frontend::floatingPointCompare(rfinfo_pkt.if_center_freq,0) > 0){
            if_offset = rfinfo_pkt.rf_center_freq-rfinfo_pkt.if_center_freq;
        }
    } // end scope for prop_lock

    // device only supports setting sample rate, which is equal to the usable bandwidth since samples are complex
    // set sample rate to lowest value that fulfills both sample rate and bandwidth requests
    double bw_sr = std::max(request.bandwidth,request.sample_rate);

    // account for RFInfo_pkt that specifies RF and IF frequencies
    // since request is always in RF, and USRP may be operating in IF
    // adjust requested center frequency according to rx rfinfo packet

    LOG_DEBUG(RTL2832U_i, std::fixed << "request: freq="<<request.center_frequency<<"  bw="<<request.bandwidth<<"  sr="<<request.sample_rate<<"  bw_sr="<<bw_sr);

    exclusive_lock tuner_lock(*rtl_tuner.lock);

    // configure hw
    rtl_device_ptr->setFreq(request.center_frequency-if_offset);
    rtl_device_ptr->setRate(bw_sr);

    // update frontend_tuner_status with actual hw values
    fts.center_frequency = rtl_device_ptr->getFreq()+if_offset;
    fts.sample_rate = rtl_device_ptr->getRate();
    // can't get or set device bandwidth, but usable bandwidth is equal to sample rate since samples are complex
    fts.bandwidth = fts.sample_rate;

    LOG_DEBUG(RTL2832U_i, std::fixed << "result: freq="<<fts.center_frequency<<"  bw="<<fts.bandwidth<<"  sr="<<fts.sample_rate);

    // creates a stream id if not already created for this tuner
    std::string stream_id = getStreamId();

    // enable multi-out capability for this stream/allocation/connection
    //matchAllocationIdToStreamId(request.allocation_id, stream_id);
    matchAllocationIdToStreamId(request.allocation_id, stream_id, "dataFloat_out");
    matchAllocationIdToStreamId(request.allocation_id, stream_id, "dataOctet_out");

    rtl_tuner.update_sri = true;

    return true;
}
bool RTL2832U_i::deviceDeleteTuning(frontend_tuner_status_struct_struct &fts, size_t tuner_id) {
    /************************************************************
    modify fts, which corresponds to this->frontend_tuner_status[tuner_id]
    return true if the tune deletion succeeded, and false if it failed
    ************************************************************/
    LOG_TRACE(RTL2832U_i,__PRETTY_FUNCTION__ << " tuner_id=" << tuner_id);

    exclusive_lock tuner_lock(*rtl_tuner.lock);

    std::string stream_id = getStreamId();
    if(rtl_tuner.update_sri){
        BULKIO::StreamSRI sri = create(stream_id, frontend_tuner_status[0]);
        sri.mode = 1; // complex
        //printSRI(&sri); // DEBUG
        dataFloat_out->pushSRI(sri);
        dataOctet_out->pushSRI(sri);
        rtl_tuner.update_sri = false;
    }

    rtl_tuner.float_output_buffer.resize(rtl_tuner.buffer_size);
    rtl_tuner.octet_output_buffer.resize(rtl_tuner.buffer_size);
    LOG_DEBUG(RTL2832U_i,"deviceDeleteTuning|pushing EOS with remaining samples. buffer_size=" << rtl_tuner.buffer_size );
    dataFloat_out->pushPacket(rtl_tuner.float_output_buffer, rtl_tuner.output_buffer_time, true, stream_id);
    dataOctet_out->pushPacket(rtl_tuner.octet_output_buffer, rtl_tuner.output_buffer_time, true, stream_id);
    rtl_tuner.buffer_size = 0;
    rtl_tuner.float_output_buffer.resize(rtl_tuner.buffer_capacity);
    rtl_tuner.octet_output_buffer.resize(rtl_tuner.buffer_capacity);

    rtl_tuner.reset();
    fts.center_frequency = 0.0;
    fts.sample_rate = 0.0;
    fts.bandwidth = 0.0;
    //fts.gain = 0.0; // this doesn't need to be reset since it's not part of allocation
    fts.stream_id.clear();
    return true;
}

/*************************************************************
Functions servicing the tuner control port
*************************************************************/
std::string RTL2832U_i::getTunerType(const std::string& allocation_id) {
    long idx = getTunerMapping(allocation_id);
    if (idx < 0) throw FRONTEND::FrontendException("Invalid allocation id");
    return frontend_tuner_status[idx].tuner_type;
}

bool RTL2832U_i::getTunerDeviceControl(const std::string& allocation_id) {
    long idx = getTunerMapping(allocation_id);
    if (idx < 0) throw FRONTEND::FrontendException("Invalid allocation id");
    if (getControlAllocationId(idx) == allocation_id)
        return true;
    return false;
}

std::string RTL2832U_i::getTunerGroupId(const std::string& allocation_id) {
    long idx = getTunerMapping(allocation_id);
    if (idx < 0) throw FRONTEND::FrontendException("Invalid allocation id");
    return frontend_tuner_status[idx].group_id;
}

std::string RTL2832U_i::getTunerRfFlowId(const std::string& allocation_id) {
    long idx = getTunerMapping(allocation_id);
    if (idx < 0) throw FRONTEND::FrontendException("Invalid allocation id");
    return frontend_tuner_status[idx].rf_flow_id;
}

void RTL2832U_i::setTunerCenterFrequency(const std::string& allocation_id, double freq) {
    long idx = getTunerMapping(allocation_id);
    if (idx < 0) throw FRONTEND::FrontendException("Invalid allocation id");
    if(allocation_id != getControlAllocationId(idx))
        throw FRONTEND::FrontendException(("ID "+allocation_id+" does not have authorization to modify the tuner").c_str());
    // set hardware to new value. Raise an exception if it's not possible
    try {
        if (frontend::floatingPointCompare(freq,rtl_capabilities.center_frequency_min)<0 ||
                frontend::floatingPointCompare(freq,rtl_capabilities.center_frequency_max)>0){
            std::ostringstream msg;
            msg << "setTunerCenterFrequency|Invalid center frequency (" << freq <<"). Center frequency must be between "
                << rtl_capabilities.center_frequency_min << " and " << rtl_capabilities.center_frequency_max << " Hz";
            LOG_WARN(RTL2832U_i,msg.str() );
            throw FRONTEND::BadParameterException(msg.str().c_str());
        }

        // set hw with new value
        rtl_device_ptr->setFreq(freq);

        // update status from hw
        frontend_tuner_status[idx].center_frequency = rtl_device_ptr->getFreq();
        rtl_tuner.update_sri = true;

    } catch (std::exception& e) {
        std::ostringstream msg;
        msg << "setTunerCenterFrequency|Exception: " << e.what();
        LOG_WARN(RTL2832U_i,msg.str() );
        throw FRONTEND::FrontendException(msg.str().c_str());
    }
}

double RTL2832U_i::getTunerCenterFrequency(const std::string& allocation_id) {
    long idx = getTunerMapping(allocation_id);
    if (idx < 0) throw FRONTEND::FrontendException("Invalid allocation id");
    frontend_tuner_status[idx].center_frequency = rtl_device_ptr->getFreq();
    return frontend_tuner_status[idx].center_frequency;
}

void RTL2832U_i::setTunerBandwidth(const std::string& allocation_id, double bw) {
    // Cannot set bandwidth of tuner, it is auto-selected by RTL device
    throw FRONTEND::NotSupportedException("setTunerBandwidth not supported");
}

double RTL2832U_i::getTunerBandwidth(const std::string& allocation_id) {
    long idx = getTunerMapping(allocation_id);
    if (idx < 0) throw FRONTEND::FrontendException("Invalid allocation id");
    // usable bandwidth is the sample rate (BW == SR since samples are complex)
    frontend_tuner_status[idx].bandwidth = frontend_tuner_status[idx].sample_rate = rtl_device_ptr->getRate();
    return frontend_tuner_status[idx].bandwidth;
}

void RTL2832U_i::setTunerAgcEnable(const std::string& allocation_id, bool enable)
{
    long idx = getTunerMapping(allocation_id);
    if (idx < 0) throw FRONTEND::FrontendException("Invalid allocation id");
    if(allocation_id != getControlAllocationId(idx))
        throw FRONTEND::FrontendException(("ID "+allocation_id+" does not have authorization to modify the tuner").c_str());
    rtl_device_ptr->setGainMode(enable);
    frontend_tuner_status[idx].agc = enable;
    frontend_tuner_status[idx].gain = rtl_device_ptr->getGain();
}

bool RTL2832U_i::getTunerAgcEnable(const std::string& allocation_id)
{
    long idx = getTunerMapping(allocation_id);
    if (idx < 0) throw FRONTEND::FrontendException("Invalid allocation id");
    return frontend_tuner_status[idx].agc;
}

void RTL2832U_i::setTunerGain(const std::string& allocation_id, float gain)
{
    long idx = getTunerMapping(allocation_id);
    if (idx < 0) throw FRONTEND::FrontendException("Invalid allocation id");
    if(allocation_id != getControlAllocationId(idx))
        throw FRONTEND::FrontendException(("ID "+allocation_id+" does not have authorization to modify the tuner").c_str());
    // set hardware to new value. Raise an exception if it's not possible
    try {

        // check if tuner AGC is enabled before adjusting manually
        if(frontend_tuner_status[idx].agc){
            throw FRONTEND::FrontendException("Gain mode set to auto; must set mode to manual before manually setting gain.");
        }

        // Instead of throwing a BadParameterException here, we'll get as close as possible
        if (frontend::floatingPointCompare(gain,rtl_capabilities.gain_min)<0){
            std::ostringstream msg;
            msg << "setTunerGain|Invalid request (" << gain <<" dB), setting to minimum gain. Gain must be set between " << rtl_capabilities.gain_min << " and " << rtl_capabilities.gain_max << " dB";
            LOG_WARN(RTL2832U_i,msg.str() );
            //throw FRONTEND::BadParameterException(msg.str().c_str());
            gain = rtl_capabilities.gain_min;
        } else if (frontend::floatingPointCompare(gain,rtl_capabilities.gain_max)>0){
            std::ostringstream msg;
            msg << "setTunerGain|Invalid request (" << gain <<" dB), setting to maximum gain. Gain must be set between " << rtl_capabilities.gain_min << " and " << rtl_capabilities.gain_max << " dB";
            LOG_WARN(RTL2832U_i,msg.str() );
            //throw FRONTEND::BadParameterException(msg.str().c_str());
            gain = rtl_capabilities.gain_max;
        }

        // set hw with new value
        rtl_device_ptr->setGain(gain);

        // update status from hw
        frontend_tuner_status[idx].gain = rtl_device_ptr->getGain();
        rtl_tuner.update_sri = true;

    } catch (std::exception& e) {
        std::ostringstream msg;
        msg << "setTunerGain|Exception: " << e.what();
        LOG_WARN(RTL2832U_i,msg.str() );
        throw FRONTEND::FrontendException(msg.str().c_str());
    }
}

float RTL2832U_i::getTunerGain(const std::string& allocation_id)
{
    long idx = getTunerMapping(allocation_id);
    if (idx < 0) throw FRONTEND::FrontendException("Invalid allocation id");
    // since tuner AGC could be enabled and changing gain, let's get it straight from the source
    frontend_tuner_status[idx].gain = rtl_device_ptr->getGain();
    return frontend_tuner_status[idx].gain;
}

void RTL2832U_i::setTunerReferenceSource(const std::string& allocation_id, long source)
{
    throw FRONTEND::NotSupportedException("setTunerReferenceSource not supported");
}

long RTL2832U_i::getTunerReferenceSource(const std::string& allocation_id)
{
    throw FRONTEND::NotSupportedException("getTunerReferenceSource not supported");
}

void RTL2832U_i::setTunerEnable(const std::string& allocation_id, bool enable) {
    long idx = getTunerMapping(allocation_id);
    if (idx < 0) throw FRONTEND::FrontendException("Invalid allocation id");
    if(allocation_id != getControlAllocationId(idx))
        throw FRONTEND::FrontendException(("ID "+allocation_id+" does not have authorization to modify the tuner").c_str());
    // set hardware to new value. Raise an exception if it's not possible
    if(enable)
        rtlEnable();
    else
        rtlDisable();
}

bool RTL2832U_i::getTunerEnable(const std::string& allocation_id) {
    long idx = getTunerMapping(allocation_id);
    if (idx < 0) throw FRONTEND::FrontendException("Invalid allocation id");
    return frontend_tuner_status[idx].enabled;
}

void RTL2832U_i::setTunerOutputSampleRate(const std::string& allocation_id, double sr) {
    long idx = getTunerMapping(allocation_id);
    if (idx < 0) throw FRONTEND::FrontendException("Invalid allocation id");
    if(allocation_id != getControlAllocationId(idx))
        throw FRONTEND::FrontendException(("ID "+allocation_id+" does not have authorization to modify the tuner").c_str());
    // set hardware to new value. Raise an exception if it's not possible
    try {

        if (frontend::floatingPointCompare(sr,rtl_capabilities.sample_rate_min)<0 ||
                frontend::floatingPointCompare(sr,rtl_capabilities.sample_rate_max)>0){
            std::ostringstream msg;
            msg << "setTunerOutputSampleRate|Invalid sample rate (" << sr <<"). Sample rate must be between "
                << rtl_capabilities.sample_rate_min << " and " << rtl_capabilities.sample_rate_max << " sps.";
            LOG_WARN(RTL2832U_i,msg.str() );
            throw FRONTEND::BadParameterException(msg.str().c_str());
        }

        // set hw with new value
        rtl_device_ptr->setRate(sr);

        // update status from hw
        frontend_tuner_status[idx].sample_rate = rtl_device_ptr->getRate();
        // update usable bandwidth as well (BW == SR since samples are complex)
        frontend_tuner_status[idx].bandwidth = frontend_tuner_status[idx].sample_rate;
        rtl_tuner.update_sri = true;

    } catch (std::exception& e) {
        std::ostringstream msg;
        msg << "setTunerOutputSampleRate|Exception: " << e.what();
        LOG_WARN(RTL2832U_i,msg.str() );
        throw FRONTEND::FrontendException(msg.str().c_str());
    }
}

double RTL2832U_i::getTunerOutputSampleRate(const std::string& allocation_id){
    long idx = getTunerMapping(allocation_id);
    if (idx < 0) throw FRONTEND::FrontendException("Invalid allocation id");
    // usable bandwidth is the sample rate (BW == SR since samples are complex)
    frontend_tuner_status[idx].sample_rate = frontend_tuner_status[idx].bandwidth = rtl_device_ptr->getRate();
    return frontend_tuner_status[idx].sample_rate;
}

/*************************************************************
Functions servicing the RFInfo port(s)
- port_name is the port over which the call was received
*************************************************************/
std::string RTL2832U_i::get_rf_flow_id(const std::string& port_name)
{
    LOG_TRACE(RTL2832U_i,__PRETTY_FUNCTION__ << " port_name=" << port_name);

    if( port_name == "RFInfo_in"){
        exclusive_lock lock(prop_lock);
        return rfinfo_pkt.rf_flow_id;
    } else {
        LOG_WARN(RTL2832U_i, "get_rf_flow_id|Unknown port name: " << port_name);
        return std::string("none");
    }
}

void RTL2832U_i::set_rf_flow_id(const std::string& port_name, const std::string& id)
{
    LOG_TRACE(RTL2832U_i,__PRETTY_FUNCTION__ << " port_name=" << port_name << " id=" << id);

    if( port_name == "RFInfo_in"){
        exclusive_lock lock(prop_lock);
        rfinfo_pkt.rf_flow_id = id;
        frontend_tuner_status[0].rf_flow_id = id;
        rtl_tuner.update_sri = true;
    } else {
        LOG_WARN(RTL2832U_i, "set_rf_flow_id|Unknown port name: " << port_name);
    }
}

frontend::RFInfoPkt RTL2832U_i::get_rfinfo_pkt(const std::string& port_name)
{
    LOG_TRACE(RTL2832U_i,__PRETTY_FUNCTION__ << " port_name=" << port_name);

    frontend::RFInfoPkt pkt;
    if( port_name != "RFInfo_in"){
        LOG_WARN(RTL2832U_i, "get_rfinfo_pkt|Unknown port name: " << port_name);
        return pkt;
    }
    exclusive_lock lock(prop_lock);
    pkt.rf_flow_id = rfinfo_pkt.rf_flow_id;
    pkt.rf_center_freq = rfinfo_pkt.rf_center_freq;
    pkt.rf_bandwidth = rfinfo_pkt.rf_bandwidth;
    pkt.if_center_freq = rfinfo_pkt.if_center_freq;
    pkt.spectrum_inverted = rfinfo_pkt.spectrum_inverted;
    pkt.sensor.collector = rfinfo_pkt.sensor.collector;
    pkt.sensor.mission = rfinfo_pkt.sensor.mission;
    pkt.sensor.rx = rfinfo_pkt.sensor.rx;
    pkt.sensor.antenna.description = rfinfo_pkt.sensor.antenna.description;
    pkt.sensor.antenna.name = rfinfo_pkt.sensor.antenna.name;
    pkt.sensor.antenna.size = rfinfo_pkt.sensor.antenna.size;
    pkt.sensor.antenna.type = rfinfo_pkt.sensor.antenna.type;
    pkt.sensor.feed.name = rfinfo_pkt.sensor.feed.name;
    pkt.sensor.feed.polarization = rfinfo_pkt.sensor.feed.polarization;
    pkt.sensor.feed.freq_range.max_val = rfinfo_pkt.sensor.feed.freq_range.max_val;
    pkt.sensor.feed.freq_range.min_val = rfinfo_pkt.sensor.feed.freq_range.min_val;
    pkt.sensor.feed.freq_range.values.resize(rfinfo_pkt.sensor.feed.freq_range.values.size());
    for (unsigned int i=0; i<rfinfo_pkt.sensor.feed.freq_range.values.size(); i++) {
        pkt.sensor.feed.freq_range.values[i] = rfinfo_pkt.sensor.feed.freq_range.values[i];
    }
    return pkt;
}

void RTL2832U_i::set_rfinfo_pkt(const std::string& port_name, const frontend::RFInfoPkt &pkt)
{
    LOG_DEBUG(RTL2832U_i, "set_rfinfo_pkt|port_name=" << port_name << " pkt.rf_flow_id=" << pkt.rf_flow_id);
    LOG_DEBUG(RTL2832U_i, "set_rfinfo_pkt|rf_center_freq=" << pkt.rf_center_freq );
    LOG_DEBUG(RTL2832U_i, "set_rfinfo_pkt|rf_bandwidth=" << pkt.rf_bandwidth );
    LOG_DEBUG(RTL2832U_i, "set_rfinfo_pkt|if_center_freq=" << pkt.if_center_freq );

    if( port_name == "RFInfo_in"){
        exclusive_lock lock(prop_lock);
        rfinfo_pkt.rf_flow_id = pkt.rf_flow_id;
        rfinfo_pkt.rf_center_freq = pkt.rf_center_freq;
        rfinfo_pkt.rf_bandwidth = pkt.rf_bandwidth;
        rfinfo_pkt.if_center_freq = pkt.if_center_freq;
        rfinfo_pkt.spectrum_inverted = pkt.spectrum_inverted;
        rfinfo_pkt.sensor.collector = pkt.sensor.collector;
        rfinfo_pkt.sensor.mission = pkt.sensor.mission;
        rfinfo_pkt.sensor.rx = pkt.sensor.rx;
        rfinfo_pkt.sensor.antenna.description = pkt.sensor.antenna.description;
        rfinfo_pkt.sensor.antenna.name = pkt.sensor.antenna.name;
        rfinfo_pkt.sensor.antenna.size = pkt.sensor.antenna.size;
        rfinfo_pkt.sensor.antenna.type = pkt.sensor.antenna.type;
        rfinfo_pkt.sensor.feed.name = pkt.sensor.feed.name;
        rfinfo_pkt.sensor.feed.polarization = pkt.sensor.feed.polarization;
        rfinfo_pkt.sensor.feed.freq_range.max_val = pkt.sensor.feed.freq_range.max_val;
        rfinfo_pkt.sensor.feed.freq_range.min_val = pkt.sensor.feed.freq_range.min_val;
        rfinfo_pkt.sensor.feed.freq_range.values.resize(pkt.sensor.feed.freq_range.values.size());
        for (unsigned int i=0; i<pkt.sensor.feed.freq_range.values.size(); i++) {
            rfinfo_pkt.sensor.feed.freq_range.values[i] = pkt.sensor.feed.freq_range.values[i];
        }
        frontend_tuner_status[0].rf_flow_id = pkt.rf_flow_id;
        rtl_tuner.update_sri = true;
    } else {
        LOG_WARN(RTL2832U_i, "set_rfinfo_pkt|Unknown port name: " + port_name);
    }
}

/////////////////////////
// Developer additions //
/////////////////////////

/*************************************************************
Configure callbacks
*************************************************************/
void RTL2832U_i::updateAvailableDevicesChanged(const bool* old_value, const bool* new_value){
    LOG_TRACE(RTL2832U_i,__PRETTY_FUNCTION__);

    exclusive_lock lock(prop_lock);

    if (update_available_devices){
        updateAvailableDevices();
    }
    update_available_devices = false;
}
void RTL2832U_i::groupIdChanged(const std::string* old_value, const std::string* new_value){
    LOG_TRACE(RTL2832U_i,__PRETTY_FUNCTION__);
    exclusive_lock lock(prop_lock);
    if(frontend_tuner_status.size() > 0){
        frontend_tuner_status[0].group_id = *new_value;
    }
}
void RTL2832U_i::rtl2832uAgcEnableChanged(const bool* old_value, const bool* new_value){
    LOG_TRACE(RTL2832U_i,__PRETTY_FUNCTION__);
    exclusive_lock lock(prop_lock);
    if(rtl_device_ptr != NULL)
        rtl_device_ptr->setAgcMode(digital_agc_enable);
}
void RTL2832U_i::targetDeviceChanged(const target_device_struct* old_value, const target_device_struct* new_value){
    LOG_TRACE(RTL2832U_i,__PRETTY_FUNCTION__);

    if(started()){
        LOG_DEBUG(RTL2832U_i, "targetDeviceChanged|device has been started, must stop before initialization");
        stop();
    } else {
        LOG_DEBUG(RTL2832U_i, "targetDeviceChanged|device has not been started, continue with initialization");
    }

    { // scope for prop_lock
        exclusive_lock lock(prop_lock);

        try{
            initRtl();
        }catch(...){
            LOG_WARN(RTL2832U_i,"Caught exception when initializing rtl device. Waiting 1 second and trying again");
            sleep(1);
            initRtl();
        }
    } // end scope for prop_lock

    if(!started()){
        LOG_DEBUG(RTL2832U_i, "targetDeviceChanged|device is not started, must start device after initialization");
        start();
    } else {
        LOG_DEBUG(RTL2832U_i, "targetDeviceChanged|device was already started after initialization, not calling start again");
    }
}

void RTL2832U_i::frequencyCorrectionChanged(const short* old_value, const short* new_value){
    LOG_TRACE(RTL2832U_i,__PRETTY_FUNCTION__);
    exclusive_lock lock(prop_lock);
    if(rtl_device_ptr != NULL){
        rtl_device_ptr->setFreqCorrection(frequency_correction);
        frequency_correction = rtl_device_ptr->getFreqCorrection();
    } else {
        frequency_correction = *old_value;
    }
}

/*************************************************************
Helper functions
*************************************************************/
std::string RTL2832U_i::getStreamId(){
    if (frontend_tuner_status[0].stream_id.empty()){
        std::ostringstream id;
        id<<"tuner_freq_"<<long(frontend_tuner_status[0].center_frequency)<<"_Hz_"<<frontend::uuidGenerator();
        frontend_tuner_status[0].stream_id = id.str();
        rtl_tuner.update_sri = true;
    }
    return frontend_tuner_status[0].stream_id;
}

/*************************************************************
Functions that interface with RTL device/driver
*************************************************************/

/* call acquire prop_lock prior to calling this function */
void RTL2832U_i::updateAvailableDevices(){
    LOG_TRACE(RTL2832U_i,__PRETTY_FUNCTION__);
    available_devices.clear();

    uint32_t num_devices = rtlsdr_get_device_count();

    if (num_devices == 0) {
        LOG_WARN(RTL2832U_i, "No rtl devices found\n");
    }

    for (uint32_t i = 0; i < num_devices; i++) {
        rtl_device_struct_struct avail_dev;
        char vendor[256], product[256], serial[256];
        rtlsdr_get_device_usb_strings(i, vendor, product, serial);

        avail_dev.index = i;
        avail_dev.name.assign(rtlsdr_get_device_name(i));
        avail_dev.vendor.assign(vendor);
        avail_dev.product.assign(product);
        avail_dev.serial.assign(serial);

        available_devices.push_back(avail_dev);
    }
}

/* call acquire prop_lock prior to calling this function */
void RTL2832U_i::initRtl() throw (CF::PropertySet::InvalidConfiguration) {
    LOG_TRACE(RTL2832U_i,__PRETTY_FUNCTION__);
    try {
        // clear current configuration
        if(rtl_device_ptr != NULL){
            delete rtl_device_ptr;
            rtl_device_ptr = NULL;
        }
        setNumChannels(0);
        current_device.index = -1;
        current_device.name.clear();
        current_device.product.clear();
        current_device.vendor.clear();
        current_device.serial.clear();
        rtl_tuner.reset();
        digital_agc_enable = false;

        // update available devices
        updateAvailableDevices();

        // find target. check index first, and if specified, ignore all else
        size_t rtl_chan_num;
        if( target_device.index >= 0){ // index specified
            if (target_device.index < (short)available_devices.size()){
                rtl_chan_num = (size_t)target_device.index;

                try {
                    rtl_device_ptr = new RtlDevice(rtl_chan_num);
                } catch(...){
                    LOG_ERROR(RTL2832U_i,"Unable to create RtlDevice instance using channel number " << rtl_chan_num);
                    rtl_device_ptr = NULL;
                    throw CF::PropertySet::InvalidConfiguration();
                }
                if (rtl_device_ptr->get() == NULL){
                    LOG_ERROR(RTL2832U_i,"Unable to create RtlDevice instance using channel number " << rtl_chan_num);
                    delete rtl_device_ptr;
                    rtl_device_ptr = NULL;
                    throw CF::PropertySet::InvalidConfiguration();
                }
            } else {
                LOG_ERROR(RTL2832U_i,"InvalidConfiguration: Specified index is out of range");
                throw CF::PropertySet::InvalidConfiguration();
            }
        } else { // index not specified, let's try to find a matching device
            for (rtl_chan_num = 0; rtl_chan_num < available_devices.size(); rtl_chan_num++) {
                // if specified, values must match.
                if( (target_device.name.empty() || target_device.name == available_devices[rtl_chan_num].name) &&
                    (target_device.serial.empty() || target_device.serial == available_devices[rtl_chan_num].serial) &&
                    (target_device.vendor.empty() || target_device.vendor == available_devices[rtl_chan_num].vendor) &&
                    (target_device.product.empty() || target_device.product == available_devices[rtl_chan_num].product) ){
                    // found a match, let's try to connect
                    try {
                        rtl_device_ptr = new RtlDevice(rtl_chan_num);
                    } catch(...){
                        LOG_INFO(RTL2832U_i,"Unable to create RtlDevice instance using channel number " << rtl_chan_num << ", searching for another.");
                        rtl_device_ptr = NULL;
                    }
                    if (rtl_device_ptr->get() == NULL){
                        LOG_INFO(RTL2832U_i,"Unable to create RtlDevice instance using channel number " << rtl_chan_num << ", searching for another.");
                        delete rtl_device_ptr;
                        rtl_device_ptr = NULL;
                    }
                    // if rtl_device_ptr isn't NULL, break out to prevent incrementing rtl_chan_num
                    if (rtl_device_ptr != NULL)
                    	break;
                }
            }
            if (rtl_device_ptr == NULL){
                LOG_ERROR(RTL2832U_i,"Could not find matching rtl device");
                throw CF::PropertySet::InvalidConfiguration();
            }
        }

        // update current device property
        current_device.index = rtl_chan_num;
        current_device.name = available_devices[rtl_chan_num].name;
        current_device.product = available_devices[rtl_chan_num].product;
        current_device.vendor = available_devices[rtl_chan_num].vendor;
        current_device.serial = available_devices[rtl_chan_num].serial;

        // read RTL device capabilities
        rtl_capabilities.center_frequency_max = rtl_device_ptr->getFreqRange().stop();
        rtl_capabilities.center_frequency_min = rtl_device_ptr->getFreqRange().start();
        rtl_capabilities.gain_max = rtl_device_ptr->getGainRange().stop();
        rtl_capabilities.gain_min = rtl_device_ptr->getGainRange().start();
        rtl_capabilities.sample_rate_max = rtl_device_ptr->getRateRange().stop();
        rtl_capabilities.sample_rate_min = rtl_device_ptr->getRateRange().start();

        // get_freq will throw an exception if set_freq has not already been called
        double setFreq = (rtl_capabilities.center_frequency_min + rtl_capabilities.center_frequency_max) / 2;
        rtl_device_ptr->setFreq(setFreq);

        // set RTL2832 AGC mode according to property value
        rtl_device_ptr->setAgcMode(digital_agc_enable);

        // start with tuner gain mode set to auto
        rtl_device_ptr->setGainMode(true);

        // get current frequency correction value
        frequency_correction = rtl_device_ptr->getFreqCorrection();


        // Initialize status vector
        // this device only has a single tuner
        setNumChannels(1);

        //Initialize Data Members
        char tmp[128];
        if(rtl_tuner.lock == NULL)
            rtl_tuner.lock = new boost::mutex;
        rtl_tuner.reset();

        frontend_tuner_status[0].agc = true; // this is the tuner gain mode, not RTL2832U AGC mode
        frontend_tuner_status[0].allocation_id_csv = "";
        frontend_tuner_status[0].tuner_type = "RX_DIGITIZER";
        frontend_tuner_status[0].center_frequency = rtl_device_ptr->getFreq();
        frontend_tuner_status[0].sample_rate = rtl_device_ptr->getRate();
        // set bandwidth to the sample rate (usable BW == SR since samples are complex)
        frontend_tuner_status[0].bandwidth = frontend_tuner_status[0].sample_rate;
        frontend_tuner_status[0].rf_flow_id = rfinfo_pkt.rf_flow_id;
        frontend_tuner_status[0].gain = rtl_device_ptr->getGain();
        frontend_tuner_status[0].group_id = group_id;
        frontend_tuner_status[0].tuner_number = rtl_chan_num;
        frontend_tuner_status[0].enabled = false;
        frontend_tuner_status[0].complex = true;
        frontend_tuner_status[0].stream_id.clear();

        sprintf(tmp,"%.2f-%.2f",rtl_capabilities.center_frequency_min,rtl_capabilities.center_frequency_max);
        frontend_tuner_status[0].available_frequency = std::string(tmp);
        sprintf(tmp,"%.2f-%.2f",rtl_capabilities.gain_min,rtl_capabilities.gain_max);
        frontend_tuner_status[0].available_gain = std::string(tmp);
        sprintf(tmp,"%.2f-%.2f",rtl_capabilities.sample_rate_min,rtl_capabilities.sample_rate_max);
        frontend_tuner_status[0].available_sample_rate = std::string(tmp);

    } catch (...) {
        LOG_ERROR(RTL2832U_i,"rtl device could not be initialized");
        throw CF::PropertySet::InvalidConfiguration();
    }
}

/* acquire tuner's lock prior to calling this function */
long RTL2832U_i::rtlReceive(double timeout){
    LOG_TRACE(RTL2832U_i,__PRETTY_FUNCTION__);

    // calc num samps to rx based on timeout, sr, and buffer size
    // Note: divide by 2 b/c two elements in buffer represent a single complex sample
    size_t samps_to_rx = size_t((rtl_tuner.buffer_capacity-rtl_tuner.buffer_size) / 2);
    if( timeout > 0 ){
        samps_to_rx = std::min(samps_to_rx, size_t(timeout*frontend_tuner_status[0].sample_rate));
    }

    BULKIO::PrecisionUTCTime time;

    // RTL device recv function asks for max length (num elements) rather than max samples,
    // so multiply the samps_to_rx by 2 (2 elements = 1 sample)
    size_t max_length = 2*samps_to_rx;
    // RTL device recv function also returns num elements, so divide by 2 to get num samps
    size_t num_elements = rtl_device_ptr->recv(&rtl_tuner.float_output_buffer.front(), &rtl_tuner.octet_output_buffer.front(), max_length);
    time = bulkio::time::utils::now();
    size_t num_samps = num_elements/2;

    LOG_TRACE(RTL2832U_i, "rtlReceive|num_samps=" << num_samps);
    // Note: multiply by 2 b/c two elements in buffer represent a single complex sample
    rtl_tuner.buffer_size += (num_samps*2);

    if(num_samps == 0)
        return 0;

    LOG_DEBUG(RTL2832U_i, "rtlReceive|received data.  num_samps=" << num_samps
            << "  buffer_size=" << rtl_tuner.buffer_size << "  buffer_capacity=" << rtl_tuner.buffer_capacity );

    // if first samples in buffer, update timestamps
    // Note: multiply by 2 b/c two elements in buffer represent a single complex sample
    if(num_samps*2 == rtl_tuner.buffer_size){
        rtl_tuner.output_buffer_time = time;
    }

    return num_samps;
}

bool RTL2832U_i::rtlEnable(){
    LOG_TRACE(RTL2832U_i,__PRETTY_FUNCTION__);

    {
        exclusive_lock tuner_lock(*rtl_tuner.lock);

        // clear buffers
        rtl_tuner.buffer_size = 0;
    }

    // enable hardware
    rtl_device_ptr->issueStreamCmd(RtlDevice::STREAM_MODE_START_CONTINUOUS);

    // update status structure
    frontend_tuner_status[0].enabled = true;

    // get stream id (creates one if not already created for this tuner)
    std::string stream_id = getStreamId();
    LOG_DEBUG(RTL2832U_i, "rtlEnable|started stream_id=" << stream_id);

    // set flag to update sri before next pushPacket
    rtl_tuner.update_sri = true;

    return true;
}

bool RTL2832U_i::rtlDisable(){
    LOG_TRACE(RTL2832U_i,__PRETTY_FUNCTION__);

    // disable hardware
    rtl_device_ptr->issueStreamCmd(RtlDevice::STREAM_MODE_STOP_CONTINUOUS);

    // update status structure
    frontend_tuner_status[0].enabled = false;

    return true;
}
