function ret = calculate_velocity(old_value, A_without_gravity, old_A_without_gravity, freq_sample)
    newVx = (old_value(1) + ((A_without_gravity(1) + old_A_without_gravity(1)) / (2 * freq_sample)));
    newVy = (old_value(2) + ((A_without_gravity(2) + old_A_without_gravity(2)) / (2 * freq_sample)));
    newVz = (old_value(3) + ((A_without_gravity(3) + old_A_without_gravity(3)) / (2 * freq_sample)));

    ret = [newVx, newVy, newVz];
end