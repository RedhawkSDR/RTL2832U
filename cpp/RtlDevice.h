/*
 * This file is protected by Copyright. Please refer to the COPYRIGHT file
 * distributed with this source distribution.
 *
 * This file is part of RTL_R820T Device.
 *
 * RTL_R820T Device is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * RTL_R820T Device is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see http://www.gnu.org/licenses/.
 */

#ifndef RTLDEVICE_H_
#define RTLDEVICE_H_

#include <rtl-sdr.h>
#include <boost/thread.hpp>
#include <string>
#include <vector>

#include "ossie/debug.h"

class RtlDevice;

//////////////////////////////
////////// DEFINES ///////////
//////////////////////////////

// sample rate limitations
#define RTL_MIN_SR 28.126e3   // Minimum tested sample rate of the RTL2832U w/o drops
#define RTL_MAX_SR 2.56e6     // Maximum tested sample rate of the RTL2832U w/o drops
//#define RTL_MAX_SR 3.2e6    // Maximum theoretical sample rate of the RTL2832U
//#define RTL_MAX_SR 2.88e6   // Possible maximum sample rate of the RTL2832U depending on host interface, not achieved in testing

// center frequency limitations
// See: http://sdr.osmocom.org/trac/wiki/rtl-sdr
#define E4000_MIN_CF     52e6
#define E4000_MAX_CF   2200e6
#define FC0012_MIN_CF    22e6
#define FC0012_MAX_CF 948.6e6
#define FC0013_MIN_CF    22e6
#define FC0013_MAX_CF  1100e6
#define FC2580_MIN_CF   146e6
#define FC2580_MAX_CF   924e6
#define R82XX_MIN_CF     24e6
#define R82XX_MAX_CF   1766e6
#define FM_BROADCAST_MIN_CF 87.7e6
#define FM_BROADCAST_MAX_CF  108e6


//////////////////////////////
////////// STRUCTS ///////////
//////////////////////////////

// A struct to hold a range defined by a minimum and maximum double value.
// (i.e. start and stop values)
struct DoubleRange
{
    public:
        DoubleRange() { m_start = 0; m_stop = 0; }
        DoubleRange(double start, double stop) { m_start = start; m_stop = stop; }

        double start() { return m_start; }
        double stop() { return m_stop; }

        void setStart(double start) { m_start = start; }
        void setStop(double stop) { m_stop = stop; }

    private:
        double m_start;
        double m_stop;
};

// A struct to hold the device's maximum and minimum carrier frequency
// values in Hz
typedef DoubleRange FrequencyRange;

// A struct to hold the device's maximum and minimum sample rate
// values in Hz
typedef DoubleRange RateRange;

// A struct to hold the device's maximum and minimum gain values in dB
typedef DoubleRange GainRange;


//////////////////////////////
//////// MAIN CLASS //////////
//////////////////////////////

// The class used to encapsulate the functionality of an RTL Dongle
class RtlDevice
{
    ENABLE_LOGGING

    public:
        RtlDevice(uint32_t channelNumber);
        virtual ~RtlDevice();

        // The currently supported streaming modes
        enum stream_cmd_t { STREAM_MODE_START_CONTINUOUS = 0, STREAM_MODE_STOP_CONTINUOUS = 1 };

    public:
        uint32_t recv(float *floatOutputBuffer, unsigned char *octetOutputBuffer, uint32_t maxLength);
        void issueStreamCmd(stream_cmd_t cmd);

    // Getters and Setters
    public:
        rtlsdr_dev_t * get() { return m_device; }

        // sets AGC mode of RTL2832U chip
        void setAgcMode(bool enable);

        // sets gain mode (auto or manual) of the tuner chip
        void setGainMode(bool enable);
        // sets gain value of the tuner chip. Requires that gain mode be set to manual.
        void setGain(double gain);
        // gets current gain setting of the tuner chip
        double getGain();

        FrequencyRange getFreqRange();

        RateRange getRateRange();

        GainRange getGainRange();

        void setFreq(double freq);
        double getFreq();

        void setRate(double rate);
        double getRate();

        std::vector<double> getClockRates();

    private:
        // Device related data
        uint32_t m_channelNumber;
        rtlsdr_dev_t *m_device;
        char m_vendor[256];
        char m_product[256];
        char m_serial[256];
        FrequencyRange m_frequencyRange;
        RateRange m_rateRange;
        GainRange m_gainRange;

        // Buffer related data
        static const uint32_t m_BUFFER_LEN = 65536;
        static const uint32_t m_NUM_BUFFERS = 32;
        uint32_t m_curRead;
        uint32_t m_curWrite;
        uint32_t m_curIndex;
        uint8_t **m_buffer;

        // Thread related data and functions
        boost::thread *m_dataRecvThread;
        boost::mutex m_readLock;
        boost::condition_variable m_dataCondition;
        bool m_startWriting;

        void threadFunction();
        static void rtlCallback(unsigned char *buf, uint32_t len, void *ctx);
};

#endif /* RTLDEVICE_H_ */
