#!/usr/bin/env python
#
# This file is protected by Copyright. Please refer to the COPYRIGHT file
# distributed with this source distribution.
#
# This file is part of REDHAWK.
#
# REDHAWK is free software: you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by the
# Free Software Foundation, either version 3 of the License, or (at your
# option) any later version.
#
# REDHAWK is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
# for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see http://www.gnu.org/licenses/.

import os, sys, copy
script_dir = os.path.dirname(os.path.abspath(__file__))
lib_dir = os.path.join(script_dir, 'fei_base')
sys.path.append(lib_dir)
import frontend_tuner_unit_test_base as fe
sdr_devices = os.path.join(os.environ.get('SDRROOT','/var/redhawk/sdr'),'dev/devices')
dut_config = {}

''' TODO:
  1)  set DUT to the key of one of the device configurations defined in the
      device_config dictionary below.
  2)  override spd, parent_dir, namespace, name, execparams, configure,
      or properties if necessary.
  3)  If 'custom', modify configuration to have accurate capabilities for the
      device under test.
  4)  Set DEBUG_LEVEL, DUT_*, or MCAST_* if desired and applicable.
  5)  Advanced: Override functions with device specific behavior.

  NOTE: When testing the USRP_UHD it is possible for the test to freeze due to
        saturating the network connection to the USRP hardware. This is
        is especially true when testing using a virtual machine. To avoid this,
        set the `sr_limit` key in the dut_config struct to the maximum sample
        rate the test should attempt. This must be a valid sample rate for the
        DUT.
'''

#******* MODIFY CONFIG BELOW **********#
DUT = 'RTL'

# Optional:
DEBUG_LEVEL = 3    # typical values include 0, 1, 2, 3, 4 and 5
DUT_INDEX = 1
DUT_IP_ADDR = None
DUT_PORT = None
DUT_IFACE = None
MCAST_GROUP = None
MCAST_PORT = None
MCAST_VLAN = None
MCAST_IFACE = None

# Your custom device
dut_config['custom'] = {
    # parent_dir/namespace/name are only used when spd = None
    'spd'         : '../my_FEI_scanner.spd.xml',       # path to SPD file for asset.
    'parent_dir'  : None,       # path to parent directory
    'namespace'   : None,       # leave as None if asset is not namespaced
    'name'        : 'my_FEI_scanner', # exact project name, without namespace
    'impl_id'     : None,       # None for all (or only), 'cpp', 'python', 'java', etc.
    'execparams'  : {},         # {'prop_name': value}
    'configure'   : {},         # {'prop_name': value}
    'properties'  : {},         # {'prop_name': value}
    'capabilities': [           # entry for each tuner; single entry if all tuners are the same
        {                       # include entry for each tuner type (i.e. RX, TX, RX_DIGITIZER, etc.)
            'RX_DIGITIZER': {
                'COMPLEX' : True,
                'CF'      : [24e3, 1.766e9],
                'BW'      : [28.126e3, 2.56e3],
                'SR'      : [28.126e3, 2.56e3],
                'GAIN'    : [0.0, 49.6]
            },
            'RX_SCANNER_DIGITIZER': {
                'COMPLEX' : True,
                'CF'      : [24e3, 1.766e9],
                'BW'      : [28.126e3, 2.56e6],
                'SR'      : [28.126e3, 2.56e6],
                'GAIN'    : [0.0, 49.6],
                'CONTROL_LIMIT' : 10e3,
            }
        }
    ]
}

#******* MODIFY CONFIG ABOVE **********#

########################################
# Available Pre-defined Configurations #
########################################

# rh.FmRdsSimulator
dut_config['FmRdsSim'] = {
    'spd'         : None,
    'parent_dir'  : None,
    'namespace'   : 'rh',
    'name'        : 'FmRdsSimulator',
    'impl_id'     : 'cpp',
    'execparams'  : {},
    'configure'   : {},
    'properties'  : {},
    'capabilities': [
        {
            'RX_DIGITIZER': {
                'COMPLEX' : True,
                'CF'      : [88e6, 108e6],
                'BW'      : [2.28e6, 2.28e6],
                'SR'      : [2.28e3, 2.28e6],
                'GAIN'    : [-100.0, 100.0]
            }
        }
    ]
}

# rh.USRP_UHD by IP w/o SDDS
dut_config['USRP'] = {
    'spd'         : None,
    'parent_dir'  : None,
    'namespace'   : 'rh',
    'name'        : 'USRP_UHD',
    'impl_id'     : 'cpp',
    'execparams'  : {},
    'configure'   : {
        'target_device': {
            'target::name'      : '',
            'target::serial'    : '',
            'target::ip_address': DUT_IP_ADDR or '',
            'target::type'      : ''
        },
        'device_group_id_global':'FEI_UNIT_TESTING'
    },
    'properties'  : {},
    'capabilities': [],  # populate using query of device props
    'sr_limit'    : 25e6 # prevent network saturation on GigE link
}

# rh.USRP_UHD by IP w/ SDDS
dut_config['USRP|SDDS'] = copy.deepcopy(dut_config['USRP'])
dut_config['USRP|SDDS']['configure']['sdds_network_settings'] = [
    {
        'sdds_network_settings::interface' : MCAST_IFACE or 'lo',
        'sdds_network_settings::ip_address': MCAST_GROUP or '127.0.0.1',
        'sdds_network_settings::port'      : MCAST_PORT or 29495,
        'sdds_network_settings::vlan'      : MCAST_VLAN or 0
    }
]

# rh.USRP_UHD for USRP N2xx w/o SDDS
dut_config['USRP|usrp2'] = copy.deepcopy(dut_config['USRP'])
dut_config['USRP|usrp2']['configure']['target_device']['target::type'] = 'usrp2'

# rh.USRP_UHD for USRP N2xx w/ SDDS
dut_config['USRP|usrp2|SDDS'] = copy.deepcopy(dut_config['USRP|SDDS'])
dut_config['USRP|usrp2|SDDS']['configure']['target_device']['target::type'] = 'usrp2'

# rh.USRP_UHD for USRP X3xx w/o SDDS
dut_config['USRP|x300'] = copy.deepcopy(dut_config['USRP'])
dut_config['USRP|x300']['configure']['target_device']['target::type'] = 'x300'

# rh.USRP_UHD for USRP X3xx w/ SDDS
dut_config['USRP|x300|SDDS'] = copy.deepcopy(dut_config['USRP|SDDS'])
dut_config['USRP|x300|SDDS']['configure']['target_device']['target::type'] = 'x300'

# rh.USRP_UHD for USRP B2xx w/o SDDS
dut_config['USRP|b200'] = copy.deepcopy(dut_config['USRP'])
dut_config['USRP|b200']['configure']['target_device']['target::type'] = 'b200'
dut_config['USRP|b200']['configure']['target_device']['target::ip_address'] = ''

# rh.USRP_UHD for USRP B2xx w/ SDDS
dut_config['USRP|b200|SDDS'] = copy.deepcopy(dut_config['USRP|SDDS'])
dut_config['USRP|b200|SDDS']['configure']['target_device']['target::type'] = 'b200'
dut_config['USRP|b200|SDDS']['configure']['target_device']['target::ip_address'] = ''

# rh.RTL2832U with Rafael Micro R820T or R828D tuner chip
dut_config['RTL'] = {
    'spd'         : "../RTL2832U.spd.xml",
    'parent_dir'  : None,
    'namespace'   : 'rh',
    'name'        : 'RTL2832U',
    'impl_id'     : 'cpp',
    'execparams'  : {},
    'configure'   : {
        'group_id': 'FEI_UNIT_TESTING',
        'target_device': {
            'target::name'      : '',
            'target::serial'    : '',
            'target::index'     : DUT_INDEX,
            'target::vendor'    : '',
            'target::product'   : ''
        },
    },
    'properties'  : {},
    'capabilities': [
        {
            'RX_DIGITIZER': {
                'COMPLEX' : True,
                'CF'      : [24e6, 1.766e9],
                'BW'      : [28.126e3, 2.56e6],
                'SR'      : [28.126e3, 2.56e6],
                'GAIN'    : [0.0, 49.6]
            }
        }
    ]
}
dut_config['RTL']['capabilities'][0]['RX_SCANNER_DIGITIZER'] = copy.copy(dut_config['RTL']['capabilities'][0]['RX_DIGITIZER'])

# rh.RTL2832U with Elonics E4000 tuner chip
dut_config['RTL|E4000'] = copy.deepcopy(dut_config['RTL'])
dut_config['RTL|E4000']['capabilities'][0]['RX_DIGITIZER']['CF'] = [52e6, 1.1e9, 1.25e9, 2.2e9] # gap from 1100 to 1250 MHz
dut_config['RTL|E4000']['capabilities'][0]['RX_DIGITIZER']['GAIN'] = [-1.0, 49.0]
dut_config['RTL|E4000']['capabilities'][0]['RX_SCANNER_DIGITIZER'] = copy.copy(dut_config['RTL|E4000']['capabilities'][0]['RX_DIGITIZER'])



# rh.RTL2832U with FC0013 tuner chip
dut_config['RTL|FC13'] = copy.deepcopy(dut_config['RTL'])
dut_config['RTL|FC13']['capabilities'][0]['RX_DIGITIZER']['CF'] = [22e6, 1.1e9]
dut_config['RTL|FC13']['capabilities'][0]['RX_DIGITIZER']['GAIN'] = [-9.9, 19.7]
dut_config['RTL|FC13']['capabilities'][0]['RX_SCANNER_DIGITIZER'] = copy.copy(dut_config['RTL|FC13']['capabilities'][0]['RX_DIGITIZER'])


# rh.RTL2832U with FC0012 tuner chip
dut_config['RTL|FC13']['capabilities'][0]['RX_DIGITIZER']['CF'] = [22e6, 948.6e6]

# rh.RTL2832U with FC2580 tuner chip
dut_config['RTL|FC2580'] = copy.deepcopy(dut_config['RTL|FC13'])
dut_config['RTL|FC2580']['capabilities'][0]['RX_DIGITIZER']['CF'] = [146e6, 308e6, 438e6, 924e6] # gap from 308 to 438 MHz
dut_config['RTL|FC2580']['capabilities'][0]['RX_SCANNER_DIGITIZER'] = copy.copy(dut_config['RTL|FC2580']['capabilities'][0]['RX_DIGITIZER'])


# rh.MSDD
dut_config['MSDD'] = {
    'spd'         : None,
    'parent_dir'  : None,
    'namespace'   : 'rh',
    'name'        : 'MSDD',
    'impl_id'     : 'python',
    'execparams'  : {},
    'configure'   : {
        'msdd_configuration': {
            'msdd_configuration::msdd_ip_address': DUT_IP_ADDR or '192.168.100.250',
            'msdd_configuration::msdd_port':       '%s'%(DUT_PORT) if DUT_PORT else '23'
        },
        'msdd_output_configuration': [
            {
                'msdd_output_configuration::tuner_number'    : 0,
                'msdd_output_configuration::protocol'        : 'UDP_SDDS',
                'msdd_output_configuration::ip_address'      : MCAST_GROUP or '234.0.0.100',
                'msdd_output_configuration::port'            : MCAST_PORT or 0,
                'msdd_output_configuration::vlan'            : MCAST_VLAN or 0,
                'msdd_output_configuration::enabled'         : True,
                'msdd_output_configuration::timestamp_offset': 0,
                'msdd_output_configuration::endianess'       : 1,
                'msdd_output_configuration::mfp_flush'       : 63,
                'msdd_output_configuration::vlan_enable'     : False                        
            }
        ]
    },
    'properties'  : {},
    'capabilities': [
        {
            'RX_DIGITIZER': {
                'COMPLEX' : True,
                'CF'      : [30e6, 6e9],
                'BW'      : [20e6, 20e6],
                'SR'      : [25e6, 25e6],
                'GAIN'    : [-48.0, 12.0]
            }
        }
    ]
}

# rh.MSDD_RX_Device
dut_config['MSDD|Dreamin'] = {
    'spd'         : None,
    'parent_dir'  : None,
    'namespace'   : 'rh',
    'name'        : 'MSDD_RX_Device_v2',
    'impl_id'     : 'DCE:3f0ac7cb-1601-456d-97fa-2dcc5be684ee',
    'execparams'  : {},
    'configure'   : {
        'NetworkConfiguration': {
             'status_destination_address'               : MCAST_GROUP or '234.0.0.100',
             'master_destination_address'               : MCAST_GROUP or '234.0.0.100',
             'DCE:d91f918d-5cdf-44d4-a2e1-6f4e5f3128bf' : MCAST_PORT or 8887,
             'master_destination_vlan'                  : MCAST_VLAN or 0,
             'msdd_address'                             : DUT_IP_ADDR or '192.168.100.250',
             'msdd_interface'                           : DUT_IFACE or 'em2',
             'msdd_port'                                : DUT_PORT or 8267
         }
    },
    'properties'  : {},
    'capabilities': [
        {
            'RX_DIGITIZER': {
                'COMPLEX' : True,
                'CF'      : [30e6, 3e9],
                'BW'      : [20e6, 20e6],
                'SR'      : [25e6, 25e6],
                'GAIN'    : [-60.0, 0.0]
            }
        }
    ]
}

# TODO - add configs for MSDD3000 vs MSDD6000

########################################
#    END Pre-defined Configurations    #
########################################

########################################
# BEGIN Custom Override Functions      #
########################################

# If special logic must be used when generating tuner requests, modify the
# function below. and add 'tuner_gen' key to the dut_config struct with the
# function as the associated value. Add as many custom functions as needed for
# all target devices in dut_config.
def customGenerateTunerRequest(idx=0):
    #Pick a random set for CF,BW,SR and return
    value = {}
    value['ALLOC_ID'] = str(fe.uuid.uuid4())
    value['TYPE'] = 'RX_DIGITIZER'
    value['BW_TOLERANCE'] = 100.0
    value['SR_TOLERANCE'] = 100.0
    value['RF_FLOW_ID'] = ''
    value['GROUP_ID'] = ''
    value['CONTROL'] = True

    value['CF'] = fe.getValueInRange(fe.DEVICE_INFO['capabilities'][idx]['RX_DIGITIZER']['CF'])
    sr_subrange = fe.DEVICE_INFO['capabilities'][idx]['RX_DIGITIZER']['SR']
    if 'sr_limit' in fe.DEVICE_INFO and fe.DEVICE_INFO['sr_limit'] > 0:
        sr_subrange = fe.getSubranges([0,fe.DEVICE_INFO['sr_limit']], fe.DEVICE_INFO['capabilities'][idx]['RX_DIGITIZER']['SR'])
    value['SR'] = fe.getValueInRange(sr_subrange)
    # Usable BW is typically equal to SR if complex samples, otherwise half of SR
    BW_MULT = 1.0 if fe.DEVICE_INFO['capabilities'][idx]['RX_DIGITIZER']['COMPLEX'] else 0.5
    value['BW'] = 0.8*value['SR']*BW_MULT # Try 80% of SR
    if fe.isValueInRange(value['BW'], fe.DEVICE_INFO['capabilities'][idx]['RX_DIGITIZER']['BW']):
        # success! all done, return value
        return value

    # Can't use 80% of SR as BW, try to find a BW value within SR tolerances
    bw_min = value['SR']*BW_MULT
    bw_max = value['SR']*(1.0+(value['SR_TOLERANCE']/100.0))*BW_MULT
    tolerance_range = [bw_min,bw_max]
    bw_subrange = fe.getSubranges(tolerance_range, fe.DEVICE_INFO['capabilities'][idx]['RX_DIGITIZER']['BW'])
    if len(bw_subrange) > 0:
        # success! get BW value and return
        value['BW'] = fe.getValueInRange(bw_subrange)
        return value

    # last resort
    value['BW'] = fe.getValueInRange(fe.DEVICE_INFO['capabilities'][idx]['RX_DIGITIZER']['BW'])
    return value
#dut_config['custom']['tuner_gen']=customGenerateTunerRequest # TODO - uncomment this line to use custom
                                                              #        function for custom DUT

########################################
#   END Custom Override Functions      #
########################################

# Find SPD if not specified
if not dut_config[DUT]['spd']:
    if dut_config[DUT]['parent_dir']:
        ns = ''
        if dut_config[DUT]['namespace']:
            ns = dut_config[DUT]['namespace']+'.'
        dut_config[DUT]['spd'] = os.path.join(dut_config[DUT]['parent_dir'], ns+dut_config[DUT]['name'], dut_config[DUT]['name']+'.spd.xml')
    else: # Assume it must be in SDR
        dut_config[DUT]['spd'] = os.path.join(sdr_devices, dut_config[DUT]['namespace'] or '', dut_config[DUT]['name'], dut_config[DUT]['name']+'.spd.xml')

# TODO - update to use `properties` instead of `configure` and `execparam`

class FrontendTunerTests(fe.FrontendTunerTests):
    
    def __init__(self,*args,**kwargs):
        import ossie.utils.testing
        super(FrontendTunerTests,self).__init__(*args,**kwargs)
        fe.set_debug_level(DEBUG_LEVEL)
        fe.set_device_info(dut_config[DUT])

    # Use functions below to add pre-/post-launch commands if your device has special startup requirements
    @classmethod
    def devicePreLaunch(cls):
        pass

    @classmethod
    def devicePostLaunch(cls):
        if 'USRP' == DUT[:4] and len(dut_config[DUT]['capabilities'])==0:
            device_channels = cls._query( ('device_channels',) )['device_channels']
            if DEBUG_LEVEL >= 4:
                from pprint import pprint as pp
                print 'device channel: '
                pp(device_channels)

            if len(device_channels) == 0:
                print 'ERROR - Unable to populate device capabilities struct with data from USRP device.'
                #raise Exception('Unable to populate device capabilities struct with data from USRP device.')

            for chan in device_channels:
                if chan['device_channels::tuner_type'] != 'RX_DIGITIZER':
                    continue
                chan_capabilities = {
                    chan['device_channels::tuner_type']: {
                        'COMPLEX' : True,
                        'CF'      : [chan['device_channels::freq_min'], chan['device_channels::freq_max']],
                        'BW'      : [chan['device_channels::bandwidth_min'], chan['device_channels::bandwidth_max']],
                        'SR'      : [chan['device_channels::rate_min'], chan['device_channels::rate_max']],
                        'GAIN'    : [chan['device_channels::gain_min'], chan['device_channels::gain_max']]
                    }
                }
                dut_config[DUT]['capabilities'].append(chan_capabilities)

    # Use functions below to add pre-/post-release commands if your device has special shutdown requirements
    @classmethod
    def devicePreRelease(self):
        pass
    @classmethod
    def devicePostRelease(self):
        pass

if __name__ == '__main__':
    fe.set_debug_level(DEBUG_LEVEL)
    fe.set_device_info(dut_config[DUT])
    
    # run using nose
    import nose
    nose.main(defaultTest=__name__)
