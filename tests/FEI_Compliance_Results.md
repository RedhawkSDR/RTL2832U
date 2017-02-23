# REDHAWK FEI Compliance Test Results

## Execution

To run [test\_RTL2832U\_FEI.py](test_RTL2832U_FEI.py), execute the following command from the `RTL2832U/tests` directory:

```
python test_RTL2832U_FEI.py
```

The test may take several minutes to perform the 283 checks when successful. It is common that fewer checks are made when unexpected failures occur, which prevent all checks from being made.

## Results

### Summary Report

```
Report Statistics:
   Checks that returned "FAIL" .................. 6
   Checks that returned "WARN" .................. 1
   Checks that returned "info" .................. 2
   Checks that returned "no" .................... 10
   Checks that returned "ok" .................... 229
   Checks with silent results ................... 38
   Total checks made ............................ 286
```

* `FAIL` indicates the test failed. It may be acceptable to fail a test depending on the device/design. See below.
* `WARN` CAN be fine, and may just be informational. See below.
* `info` is fine, just informational for developer to confirm the intended results. See below.
* `no` is fine, just informational for developer to confirm the intended results. Indicates an optional field was not used.
* `ok` is good, and indicates a check passed.
* `silent results` are checks that passed but do not appear anywhere in the output unless they fail.

### `info` Details

The two `info` checks are reporting that it's impossible to test full multi-out port capability with a single channel. The features that cannot be tested are not necessary for a single channel anyway.

```
dataFloat_out: Cannot fully test multiport because only single
     RX_DIGITIZER tuner capability..........................................info
dataOctet_out: Cannot fully test multiport because only single
     RX_DIGITIZER tuner capability..........................................info
```

### `WARN` Details

The single `WARN` check is reporting that an unknown field was found in the tuner status, which is permitted. The reason is it a warning is to call extra attention in case the unknown (user defined) property could be modified to one of the many pre-defined optional fields also reported in the test.
```
tuner_status has UNKNOWN field FRONTEND::tuner_status::stream_id............WARN
```

### `FAIL` Details

There are 6 checks that report `FAIL` with the RTL, and this is known.

#### Multi-out port checks

The RTL does not implement a multi-out port, which is acceptable since the RTL is a single channel device, but is considered bad practice nonetheless. It was done intentionally in order to make the RTL device as easy to use for a beginner as possible, without knowing all necessary steps to make use of a device with a multi-out port. This of course means the multi-out tests will fail, and since there are two ports, they'll each fail twice, once for each port. These failures are all expected.

```
dataFloat_out: Did not receive data from tuner allocation with wrong
     alloc_id (multiport test)..............................................FAIL
dataFloat_out: Did not receive correct SRI from tuner allocation with
     wrong alloc_id (multiport test)........................................FAIL
dataOctet_out: Did not receive data from tuner allocation with wrong
     alloc_id (multiport test)..............................................FAIL
dataOctet_out: Did not receive correct SRI from tuner allocation with
     wrong alloc_id (multiport test)........................................FAIL
```

#### `setTunerGain` Errors

Additionally, the `setTunerGain` function was implemented to be very forgiving for the same reasons as above (easy for beginner). Instead of producing an exception that indicates an invalid gain value was requested, the RTL simply sets the gain to be the nearest valid value (i.e. value greater than valid range is replaces with max valid value). Because of this, tests would fail that check for invalid gain values to produce an exception, and this is expected.

Reported Error:
```
ERROR: RX_DIG 3.23 Verify digital tuner port setTunerGain function out of bounds retune
```
Failed check:
```
Out-of-bounds setting of gain produces BadParameterException (produces
     FrontendException instead).............................................FAIL
```

You will notice that the test actually failed because the wrong exception was produced. The invalid out-of-bounds gain value is replaced with a valid value before being set, but the valid value fails to be set because the `setTunerGain` function also fails for the reason described next.

#### Auto-gain Control

Lastly, the RTL has an auto-gain feature which is enabled by default. When enabled, attempts to manually set the gain will fail, which causes the `setTunerGain` tests with valid values to fail as well. This is expected (see the test output indicated that gain mode is auto).

Reported Error:
```
2015-10-15 17:13:38 INFO  RtlDevice:312 - Tuner gain mode set to auto
ERROR: RX_DIG 3.19 Verify digital tuner port setTunerGain function in-bounds retune
```
Failed check:
```
In-bounds setting of gain - set to minimum gain (0.0) produces
     FrontendException......................................................FAIL
```

