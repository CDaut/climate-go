package de.cdaut.climategoapp.sensor;

import android.Manifest;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.le.BluetoothLeScanner;

import android.content.pm.PackageManager;
import android.os.Handler;
import android.os.Looper;


import androidx.core.content.ContextCompat;

import java.util.ArrayList;
import java.util.List;

import de.cdaut.climategoapp.DebugFragment;
import de.cdaut.climategoapp.R;

public final class SensorDebugUtils {
    private static boolean scanning = false;
    private static final long SCAN_PERIOD_MS = 10000;

    private SensorDebugUtils() {
        throw new IllegalAccessError("Utility Class");
    }

    /**
     * Method to read a List of Data from the Sensor
     *
     * @return a List of Strings containing the sensor data
     */
    public static List<String> readSensorData() {
        ArrayList<String> data = new ArrayList<>();
        data.add("Test");
        data.add("a");
        data.add("b");
        return data;
    }

    public static void scanForLeDevices(DebugFragment rootFragment) {
        //get the bluetooth adapter responsible for scanning
        BluetoothLeScanner bluetoothLeScanner = BluetoothAdapter
                .getDefaultAdapter()
                .getBluetoothLeScanner();

        //check if the required location permissions have been granted
        //request the permissions if they have not been granted
        if (ContextCompat.checkSelfPermission(rootFragment.getContext(),
                Manifest.permission.ACCESS_COARSE_LOCATION
        ) != PackageManager.PERMISSION_GRANTED) {
            rootFragment.getLocAccessRequester().launch(new String[]{
                    Manifest.permission.ACCESS_COARSE_LOCATION,
                    Manifest.permission.ACCESS_FINE_LOCATION
            });
        }

        //if the permissions were denied, display a message
        if (ContextCompat.checkSelfPermission(rootFragment.getContext(),
                Manifest.permission.ACCESS_COARSE_LOCATION
        ) != PackageManager.PERMISSION_GRANTED) {
            rootFragment.getDebugTextView().setText(R.string.enable_loc_note);
            return;
        }

        //instantiate custom scan callback that can also set the right device
        CustomScanCallback leScanCallback = new CustomScanCallback(rootFragment, bluetoothLeScanner);

        //check if the scan is already running
        if (!scanning) {
            // Stops scanning after a predefined scan period.
            new Handler(Looper.getMainLooper())
                    .postDelayed(() -> {
                        scanning = false;
                        bluetoothLeScanner.stopScan(leScanCallback);
                    }, SCAN_PERIOD_MS);

            scanning = true;
            bluetoothLeScanner.startScan(leScanCallback);
        } else {
            scanning = false;
            bluetoothLeScanner.stopScan(leScanCallback);
        }
    }
}
