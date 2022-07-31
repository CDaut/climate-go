package de.cdaut.climategoapp;

import static android.app.Activity.RESULT_OK;

import android.Manifest;
import android.annotation.SuppressLint;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothGatt;
import android.bluetooth.BluetoothGattCallback;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;
import android.widget.Toast;


import androidx.activity.result.ActivityResultLauncher;
import androidx.activity.result.contract.ActivityResultContracts;
import androidx.annotation.NonNull;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;
import androidx.fragment.app.Fragment;

import de.cdaut.climategoapp.databinding.FragmentDebugBinding;
import de.cdaut.climategoapp.sensor.SensorDebugUtils;

public class DebugFragment extends Fragment {

    private FragmentDebugBinding binding;
    private BluetoothDevice sensor;
    private BluetoothGatt sensorConn;
    private boolean newDataRequested = false;

    //this is the Launcher that will make the request to
    //enable Bluetooth and handle the result callback
    //launch this to start get a sensor
    private final ActivityResultLauncher<Intent> mBtEnable = registerForActivityResult(
            new ActivityResultContracts.StartActivityForResult(),
            result -> {
                if (result.getResultCode() != RESULT_OK) {
                    //make an error toast if the user denys enabling bluetooth
                    Toast.makeText(
                            getActivity().getApplicationContext(),
                            R.string.bt_required,
                            Toast.LENGTH_LONG
                    ).show();
                } else {
                    SensorDebugUtils.scanForLeDevices(this);
                }
            }
    );

    private final ActivityResultLauncher<String[]> mLocAccess = registerForActivityResult(
            new ActivityResultContracts.RequestMultiplePermissions(),
            result -> {
                if (result.containsValue(false)) {
                    //make an error toast if the user denys enabling bluetooth
                    Toast.makeText(
                            getActivity().getApplicationContext(),
                            R.string.loc_required,
                            Toast.LENGTH_LONG
                    ).show();
                } else {
                    SensorDebugUtils.scanForLeDevices(this);
                }
            }
    );

    private final ActivityResultLauncher<String> mBtConnect = registerForActivityResult(
            new ActivityResultContracts.RequestPermission(),
            result -> {
                if (Boolean.FALSE.equals(result)) {
                    //make an error toast if the user denys connection permissions
                    Toast.makeText(
                            getActivity().getApplicationContext(),
                            R.string.bt_connect_required,
                            Toast.LENGTH_LONG
                    ).show();
                } else {
                    this.connectToSensor();
                }
            }
    );

    private final BluetoothGattCallback btCallback = new BluetoothGattCallback() {
        @Override
        public void onConnectionStateChange(BluetoothGatt gatt, int status, int newState) {
            super.onConnectionStateChange(gatt, status, newState);
            Log.d("BLE", "New state: " + newState);
        }
    };

    @Override
    public View onCreateView(
            @NonNull LayoutInflater inflater,
            ViewGroup container,
            Bundle savedInstanceState
    ) {

        binding = FragmentDebugBinding.inflate(inflater, container, false);

        this.binding.buttonTest.setOnClickListener(view -> {
            //make request to enable bluetooth
            mBtEnable.launch(new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE));
            //request new data so when new sensor is set, data will be queried
            this.newDataRequested = true;
        });

        return binding.getRoot();

    }

    @Override
    public void onDestroyView() {
        super.onDestroyView();
        binding = null;
    }

    public ActivityResultLauncher<String[]> getLocAccessRequester() {
        return mLocAccess;
    }

    public BluetoothDevice getSensor() {
        return sensor;
    }

    @SuppressLint("MissingPermission")
    public void setSensor(@NonNull BluetoothDevice sensor) {
        this.sensor = sensor;
        this.binding.debugView.setText(sensor.getName() + " " + sensor.getAddress());
        this.sensorUpdated();
    }

    public TextView getDebugTextView() {
        return this.binding.debugView;
    }

    private void connectToSensor() {
        //connect to the sensor
        if (ContextCompat.checkSelfPermission(this.getContext(), Manifest.permission.BLUETOOTH_CONNECT) != PackageManager.PERMISSION_GRANTED) {
            this.mBtConnect.launch(Manifest.permission.BLUETOOTH_CONNECT);
            return;
        }
        this.sensorConn = this.sensor.connectGatt(this.getContext(), false, btCallback);
        this.sensorConn.connect();
    }

    /**
     * This Method will be called every time the sensor is updated
     */
    private void sensorUpdated() {
        if (this.sensorConn == null) this.connectToSensor();

        //only pull data if new data has been requested
        if (this.newDataRequested) {
            //
        }
    }
}
