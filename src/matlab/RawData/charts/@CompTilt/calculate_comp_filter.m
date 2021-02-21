function ret = calculate_comp_filter(obj, old_value, gyro_tilt, old_gyro_tilt, tilt, mu)
    newRoll  = weightedMeanAngle(old_value(1) + (gyro_tilt(1) - old_gyro_tilt(1)), tilt(1), mu);
    newPitch = weightedMeanAngle(old_value(2) + (gyro_tilt(2) - old_gyro_tilt(2)), tilt(2), mu);
    newYaw   = weightedMeanAngle(old_value(3) + (gyro_tilt(3) - old_gyro_tilt(3)), tilt(3), mu);

    ret = [newRoll, newPitch, newYaw];
end