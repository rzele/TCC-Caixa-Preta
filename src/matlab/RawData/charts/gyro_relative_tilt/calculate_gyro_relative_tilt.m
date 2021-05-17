function ret = calculate_gyro_relative_tilt(old_tilt, G, old_G, freq_sample)
    period = 1/freq_sample;
    newRoll  = integrate(old_tilt(1), G(1), old_G(1), period);
    newPitch = integrate(old_tilt(2), G(2), old_G(2), period);
    newYaw   = integrate(old_tilt(3), G(3), old_G(3), period);

    ret = [newRoll, newPitch, newYaw];
end