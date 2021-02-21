function ret = calculate_gyro_absolute_tilt(obj, relative_tilt)
    rot = ang2rotZYX(relative_tilt(1), relative_tilt(2), relative_tilt(3));
    ret = rot2angZYX(rot);
end