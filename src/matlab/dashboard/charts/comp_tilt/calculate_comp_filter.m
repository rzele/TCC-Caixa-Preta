function ret = calculate_comp_filter(old_value, gyro_tilt, old_gyro_tilt, tilt, mu)
    newRoll  = weightedMeanAngle(sumAngle(old_value(1), subAngle(gyro_tilt(1), old_gyro_tilt(1))), tilt(1), mu);
    newPitch = weightedMeanAngle(sumAngle(old_value(2), subAngle(gyro_tilt(2), old_gyro_tilt(2))), tilt(2), mu);
    newYaw   = weightedMeanAngle(sumAngle(old_value(3), subAngle(gyro_tilt(3), old_gyro_tilt(3))), tilt(3), mu);

    ret = [newRoll, newPitch, newYaw];
end