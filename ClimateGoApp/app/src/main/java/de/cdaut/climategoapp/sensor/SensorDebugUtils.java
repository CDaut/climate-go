package de.cdaut.climategoapp.sensor;

import java.util.ArrayList;
import java.util.List;

public final class SensorDebugUtils {
    private SensorDebugUtils() {
        throw new IllegalAccessError("Utility Class");
    }

    public static List<String> readSensorData() {
        ArrayList<String> data = new ArrayList<>();
        data.add("Test");
        data.add("a");
        data.add("b");
        return data;
    }
}
