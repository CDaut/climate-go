package de.cdaut.climategoapp.sensor;

import android.annotation.SuppressLint;
import android.bluetooth.le.BluetoothLeScanner;
import android.bluetooth.le.ScanCallback;
import android.bluetooth.le.ScanResult;
import android.util.Log;

import de.cdaut.climategoapp.DebugFragment;
import de.cdaut.climategoapp.R;

/**
 * Custom Scan Callback Class to be able to connect to the Debug Fragment and pass Data
 */
public class CustomScanCallback extends ScanCallback {
    private final DebugFragment rootFragment;
    private final BluetoothLeScanner bleScanner;

    public CustomScanCallback(DebugFragment rootFragment, BluetoothLeScanner bleScanner) {
        this.rootFragment = rootFragment;
        this.bleScanner = bleScanner;
    }

    //TODO: Find out what permission to check and check them properly
    @SuppressLint("MissingPermission")
    @Override
    public void onScanResult(int callbackType, ScanResult result) {
        super.onScanResult(callbackType, result);
        String deviceName = result.getDevice().getName();

        //check if the sensor has been found
        if (deviceName != null
                && deviceName.equals(rootFragment.getString(R.string.sensor_name))) {
            //set the sensor and stop scanning
            this.rootFragment.setSensor(result.getDevice());
            bleScanner.stopScan(this);
            Log.d("BLE", "Sensor with mac " +
                    result.getDevice().getAddress() +
                    " found and scan terminated."
            );
        }
    }

    @Override
    public void onScanFailed(int errorCode) {
        super.onScanFailed(errorCode);
        Log.e("BLUETOOTH", "BLE scanning returned Error code " + errorCode);
    }
}
