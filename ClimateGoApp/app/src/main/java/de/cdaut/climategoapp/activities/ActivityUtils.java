package de.cdaut.climategoapp.activities;

import android.content.Intent;
import android.view.MenuItem;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.drawerlayout.widget.DrawerLayout;

import de.cdaut.climategoapp.R;

public final class ActivityUtils {
    private ActivityUtils() {
        throw new IllegalAccessError("Utility Class");
    }

    public static boolean handleNavBarItemClicks(@NonNull MenuItem item,
                                                 AppCompatActivity rootClass,
                                                 DrawerLayout drawer) {

        Class<? extends AppCompatActivity> targetActivityClass;

        switch (item.getItemId()) {
            case R.id.nav_main:
                targetActivityClass = HomeActivity.class;
                break;
            case R.id.nav_debug:
                targetActivityClass = DebugActivity.class;
                break;
            default:
                throw new IllegalAccessError("Illegal activity met in nav drawer");
        }

        if (targetActivityClass.equals(rootClass.getClass())) {
            drawer.closeDrawers();
            return false;
        }

        Intent targetIntent = new Intent(rootClass, targetActivityClass);
        rootClass.startActivity(targetIntent);

        return false;
    }
}
