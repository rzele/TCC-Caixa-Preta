function ret = calculate_gyro_absolute_tilt(rel_tilt, old_rel_tilt, old_abs_tilt)
    delta_tilt = rel_tilt - old_rel_tilt;
    delta_rot = ang2rotZYX(delta_tilt(1), delta_tilt(2), delta_tilt(3));
    old_rot = ang2rotZYX(old_abs_tilt(1), old_abs_tilt(2), old_abs_tilt(3));
    new_rot = old_rot * delta_rot;
    ret = rot2angZYX(new_rot);
end