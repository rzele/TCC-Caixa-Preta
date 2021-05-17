function ret = calculate_position(old_value, velocity, old_velocity, freq_sample)
    period = 1/freq_sample;
    newPx = integrate(old_value(1), velocity(1), old_velocity(1), period);
    newPy = integrate(old_value(2), velocity(2), old_velocity(2), period);
    newPz = integrate(old_value(3), velocity(3), old_velocity(3), period);

    ret = [newPx, newPy, newPz];
end