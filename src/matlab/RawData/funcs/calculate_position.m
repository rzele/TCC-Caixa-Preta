function ret = calculate_position(old_value, velocity, old_velocity, freq_sample)
    newPx = (old_value(1) + ((velocity(1) + old_velocity(1)) / (2 * freq_sample) ));
    newPy = (old_value(2) + ((velocity(2) + old_velocity(2)) / (2 * freq_sample) ));
    newPz = (old_value(3) + ((velocity(3) + old_velocity(3)) / (2 * freq_sample) ));

    ret = [newPx, newPy, newPz];
end