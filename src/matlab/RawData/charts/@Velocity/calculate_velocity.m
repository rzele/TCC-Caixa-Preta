function ret = calculate_velocity(obj, old_value, A_without_gravity, old_A_without_gravity, freq_sample)
    period = 1/freq_sample;
    newVx = integrate(old_value(1), A_without_gravity(1), old_A_without_gravity(1), period);
    newVy = integrate(old_value(2), A_without_gravity(2), old_A_without_gravity(2), period);
    newVz = integrate(old_value(3), A_without_gravity(3), old_A_without_gravity(3), period);

    ret = [newVx, newVy, newVz];
end