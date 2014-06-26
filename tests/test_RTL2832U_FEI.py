#!/usr/bin/env python

# This file is protected by Copyright. Please refer to the COPYRIGHT file
# distributed with this source distribution.
#
# This file is part of RTL2832U Device.
#
# RTL2832U Device is free software: you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by the
# Free Software Foundation, either version 3 of the License, or (at your
# option) any later version.
#
# RTL2832U Device is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
# for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see http://www.gnu.org/licenses/.

import sys, os
script_dir = os.path.dirname(os.path.abspath(__file__))
project_dir = os.path.abspath(os.path.join(script_dir, '..'))
lib_dir = os.path.join(script_dir, 'fei_base')
sys.path.append(lib_dir)
import frontend_tuner_unit_test_base as fe

''' TODO:
  1)  set the desired DEBUG_LEVEL (typical values include 0, 1, 2, 3, 4 and 5)
  2)  set DUT correctly to specify the tuner chip paired with the RTL2832U
  3)  set IMPL_ID to the implementation that should be tested.
  4)  Optional: set dut_execparams if it is necessary to specify a particular
      RTL dongle. Default behavior is to target the device found at index 0.
  5)  Optional: set dut_capabilities to reflect the hardware capabilities of
      the dut if different from what is provided below.
'''

DEBUG_LEVEL = 3

DUT='RTL|R820T'
#DUT='RTL|E4000'
#DUT='RTL|FC0013'
#DUT='RTL|FC0012'
#DUT='RTL|R828D'
#DUT='RTL|FC2580'

IMPL_ID='cpp'
#IMPL_ID='cpp_arm'

if 'RTL' in DUT:
    dut_name = 'RTL2832U'
    dut_execparams = {'target_device':{'target::name':'',
                                       'target::serial':'',
                                       'target::vendor':'',
                                       'target::product':'',
                                       'target::index':0},
                      'group_id':'FEI_UNIT_TESTING'
                     }
    dut_capabilities = {'RX_DIGITIZER':{'COMPLEX': True,
                                        'CF_MAX': 1.1e9,
                                        'CF_MIN':  22e6,
                                        'SR_MAX': 2.56e6, #'SR_MAX': 2.8e6,
                                        'SR_MIN': 28.126e3, #'SR_MIN': 256e3,
                                        'GAIN_MIN': -9.9,
                                        'GAIN_MAX': 19.7}}
    if 'R82' in DUT: # R820T and R828D
        dut_capabilities['RX_DIGITIZER']['CF_MAX'] = 1.766e9
        dut_capabilities['RX_DIGITIZER']['CF_MIN'] =    24e6
        dut_capabilities['RX_DIGITIZER']['GAIN_MIN'] = 0.0
        dut_capabilities['RX_DIGITIZER']['GAIN_MAX'] = 49.6
    if 'E4000' in DUT:
        dut_capabilities['RX_DIGITIZER']['CF_MAX'] = 2.2e9 # gap from 1100 to 1250 MHz
        dut_capabilities['RX_DIGITIZER']['CF_MIN'] =  52e6
        dut_capabilities['RX_DIGITIZER']['GAIN_MIN'] = -1.0
        dut_capabilities['RX_DIGITIZER']['GAIN_MAX'] = 49.0
    if 'FC0013' in DUT:
        dut_capabilities['RX_DIGITIZER']['CF_MAX'] = 1.1e9
        dut_capabilities['RX_DIGITIZER']['CF_MIN'] =  22e6
        dut_capabilities['RX_DIGITIZER']['GAIN_MIN'] = -9.9
        dut_capabilities['RX_DIGITIZER']['GAIN_MAX'] = 19.7
    if 'FC0012' in DUT:
        dut_capabilities['RX_DIGITIZER']['CF_MAX'] = 948.6e6
        dut_capabilities['RX_DIGITIZER']['CF_MIN'] =    22e6
        dut_capabilities['RX_DIGITIZER']['GAIN_MIN'] = -9.9 # TODO - unknown
        dut_capabilities['RX_DIGITIZER']['GAIN_MAX'] = 19.7 # TODO - unknown
    if 'FC2580' in DUT:
        dut_capabilities['RX_DIGITIZER']['CF_MAX'] = 924e6 # gap from 308 to 438 MHz
        dut_capabilities['RX_DIGITIZER']['CF_MIN'] = 146e6
        dut_capabilities['RX_DIGITIZER']['GAIN_MIN'] = -9.9 # TODO - unknown
        dut_capabilities['RX_DIGITIZER']['GAIN_MAX'] = 19.7 # TODO - unknown
    # BW is equal to SR since complex samples
    dut_capabilities['RX_DIGITIZER']['BW_MIN'] = dut_capabilities['RX_DIGITIZER']['SR_MIN']
    dut_capabilities['RX_DIGITIZER']['BW_MAX'] = dut_capabilities['RX_DIGITIZER']['SR_MAX']


#******* DO NOT MODIFY BELOW **********#
DEVICE_INFO = {}
DEVICE_INFO[dut_name] = dut_capabilities
DEVICE_INFO[dut_name]['SPD'] = os.path.join(project_dir, 'RTL2832U.spd.xml')
DEVICE_INFO[dut_name]['execparams'] = dut_execparams
#******* DO NOT MODIFY ABOVE **********#



class FrontendTunerTests(fe.FrontendTunerTests):
    
    def __init__(self,*args,**kwargs):
        import ossie.utils.testing
        super(FrontendTunerTests,self).__init__(*args,**kwargs)
        fe.set_debug_level(DEBUG_LEVEL)
        fe.set_device_info(DEVICE_INFO[dut_name])
        fe.set_impl_id(IMPL_ID)
    
    # Use functions below to add pre-/post-launch commands if your device has special startup requirements
    @classmethod
    def devicePreLaunch(self):
        pass
    @classmethod
    def devicePostLaunch(self):
        pass
    
    # Use functions below to add pre-/post-release commands if your device has special shutdown requirements
    @classmethod
    def devicePreRelease(self):
        pass
    @classmethod
    def devicePostRelease(self):
        pass
    
    
if __name__ == '__main__':
    fe.set_debug_level(DEBUG_LEVEL)
    fe.set_device_info(DEVICE_INFO[dut_name])
    fe.set_impl_id(IMPL_ID)
    
    # run using nose
    import nose
    nose.main(defaultTest=__name__)
