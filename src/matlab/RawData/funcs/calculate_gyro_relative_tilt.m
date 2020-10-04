function ret = calculate_gyro_relative_tilt(old_tilt, G, old_G, freq_sample)
    newRoll  = (old_tilt(1) + ((G(1) + old_G(1)) / (2 * freq_sample)));
    newPitch = (old_tilt(2) + ((G(2) + old_G(2)) / (2 * freq_sample)));
    newYaw   = (old_tilt(3) + ((G(3) + old_G(3)) / (2 * freq_sample)));

    ret = [newRoll, newPitch, newYaw];
end