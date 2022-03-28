package de.cdaut.climategoapp;

import static android.app.Activity.RESULT_OK;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.Intent;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;
import android.widget.Toast;


import androidx.activity.result.ActivityResultLauncher;
import androidx.activity.result.contract.ActivityResultContracts;
import androidx.annotation.NonNull;
import androidx.fragment.app.Fragment;

import de.cdaut.climategoapp.databinding.FragmentDebugBinding;
import de.cdaut.climategoapp.sensor.SensorDebugUtils;

public class DebugFragment extends Fragment {

    private FragmentDebugBinding binding;
    private BluetoothDevice sensor;

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

    public void setSensor(BluetoothDevice sensor) {
        this.sensor = sensor;
        this.binding.debugView.setText(sensor.getName() + " " + sensor.getAddress());
    }

    public TextView getDebugTextView() {
        return this.binding.debugView;
    }
}
